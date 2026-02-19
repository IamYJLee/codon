// Copyright (C) 2022-2026 Exaloop Inc. <https://exaloop.io>

#include "metal.h"

#include <algorithm>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "codon/cir/llvm/optimize.h"
#include "codon/util/common.h"

namespace codon {
namespace ir {
namespace {

// ============================================================================
// Section A: CLI Options
// ============================================================================
llvm::cl::opt<std::string> mslOutput("msl",
                                     llvm::cl::desc("Output MSL to specified file"));

// ============================================================================
// Section B: Type Mapping
// ============================================================================

/// Map an LLVM type to its MSL type name string.
/// Returns empty string if the type cannot be mapped.
std::string mslTypeName(llvm::Type *ty) {
  if (ty->isVoidTy())
    return "void";
  if (ty->isFloatTy())
    return "float";
  if (ty->isDoubleTy())
    compilationError("[Metal] double (f64) is not supported on Metal GPUs");
  if (ty->isHalfTy())
    return "half";
  if (auto *intTy = llvm::dyn_cast<llvm::IntegerType>(ty)) {
    switch (intTy->getBitWidth()) {
    case 1:
      return "bool";
    case 8:
      return "char";
    case 16:
      return "short";
    case 32:
      return "int";
    case 64:
      return "long";
    default:
      compilationError("[Metal] unsupported integer width: " +
                       std::to_string(intTy->getBitWidth()));
    }
  }
  if (ty->isPointerTy())
    return "device char*"; // fallback for unresolved pointer types

  compilationError("[Metal] unsupported LLVM type");
  return "";
}

/// Map an LLVM type to its unsigned MSL type name (for bitwise operations).
std::string mslUnsignedTypeName(llvm::Type *ty) {
  if (auto *intTy = llvm::dyn_cast<llvm::IntegerType>(ty)) {
    switch (intTy->getBitWidth()) {
    case 8:
      return "uchar";
    case 16:
      return "ushort";
    case 32:
      return "uint";
    case 64:
      return "ulong";
    default:
      break;
    }
  }
  return mslTypeName(ty);
}

// ============================================================================
// Section C: MSLTranspiler
// ============================================================================

class MSLTranspiler {
  // Maps LLVM Values to MSL variable names
  std::unordered_map<const llvm::Value *, std::string> nameMap;
  int nextTmp = 0;

  // Track which NVVM intrinsics are used (to add Metal built-in params)
  bool usesThreadPositionInThreadgroup = false;
  bool usesThreadgroupPositionInGrid = false;
  bool usesThreadsPerThreadgroup = false;
  bool usesThreadgroupsPerGrid = false;

  // Output stream
  std::ostringstream out;
  int indentLevel = 0;

  // ---- Name Management ----

  std::string getName(const llvm::Value *V) {
    auto it = nameMap.find(V);
    if (it != nameMap.end())
      return it->second;

    // Generate a new name
    std::string name;
    if (V->hasName()) {
      name = cleanName(V->getName().str());
    } else {
      name = "_t" + std::to_string(nextTmp++);
    }
    nameMap[V] = name;
    return name;
  }

  static std::string cleanName(const std::string &name) {
    std::string result;
    result.reserve(name.size());
    bool first = true;
    for (char c : name) {
      bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
      if (!first)
        ok = ok || (c >= '0' && c <= '9');
      result += ok ? c : '_';
      first = false;
    }
    // Avoid MSL keywords
    static const std::unordered_set<std::string> keywords = {
        "kernel",   "device",  "constant", "thread",     "threadgroup",
        "texture",  "sampler", "float",    "half",       "int",
        "uint",     "bool",    "char",     "short",      "long",
        "uchar",    "ushort",  "ulong",    "void",       "return",
        "if",       "else",    "for",      "while",      "do",
        "switch",   "case",    "break",    "continue",   "struct",
        "class",    "using",   "namespace","true",       "false",
    };
    if (keywords.count(result))
      result = "_" + result;
    return result;
  }

  void indent() {
    for (int i = 0; i < indentLevel; ++i)
      out << "    ";
  }

  // ---- Pointee Type Inference ----

  /// Try to infer what type is loaded/stored through a pointer argument.
  /// With opaque pointers (LLVM 15+), we must analyze uses.
  llvm::Type *inferPointeeType(const llvm::Argument *arg) {
    for (const auto &use : arg->uses()) {
      auto *user = use.getUser();
      if (auto *load = llvm::dyn_cast<llvm::LoadInst>(user))
        return load->getType();
      if (auto *store = llvm::dyn_cast<llvm::StoreInst>(user)) {
        if (store->getPointerOperand() == arg)
          return store->getValueOperand()->getType();
      }
      if (auto *gep = llvm::dyn_cast<llvm::GetElementPtrInst>(user)) {
        // Follow GEP users for load/store
        for (const auto &gepUse : gep->uses()) {
          auto *gepUser = gepUse.getUser();
          if (auto *load = llvm::dyn_cast<llvm::LoadInst>(gepUser))
            return load->getType();
          if (auto *store = llvm::dyn_cast<llvm::StoreInst>(gepUser)) {
            if (store->getPointerOperand() == gep)
              return store->getValueOperand()->getType();
          }
        }
      }
      // Follow bitcasts (though rare with opaque ptrs)
      if (auto *bc = llvm::dyn_cast<llvm::BitCastInst>(user)) {
        for (const auto &bcUse : bc->uses()) {
          auto *bcUser = bcUse.getUser();
          if (auto *load = llvm::dyn_cast<llvm::LoadInst>(bcUser))
            return load->getType();
          if (auto *store = llvm::dyn_cast<llvm::StoreInst>(bcUser)) {
            if (store->getPointerOperand() == bc)
              return store->getValueOperand()->getType();
          }
        }
      }
    }
    return nullptr; // couldn't infer
  }

  /// Trace a pointer value back to its originating kernel Argument (if any).
  const llvm::Argument *traceToArg(const llvm::Value *v) {
    while (v) {
      if (auto *arg = llvm::dyn_cast<llvm::Argument>(v))
        return arg;
      if (auto *gep = llvm::dyn_cast<llvm::GetElementPtrInst>(v)) {
        v = gep->getPointerOperand();
        continue;
      }
      if (auto *bc = llvm::dyn_cast<llvm::BitCastInst>(v)) {
        v = bc->getOperand(0);
        continue;
      }
      break;
    }
    return nullptr;
  }

  /// Check if a pointer argument is only read (never stored to).
  bool isReadOnly(const llvm::Argument *arg) {
    for (const auto &use : arg->uses()) {
      auto *user = use.getUser();
      if (auto *store = llvm::dyn_cast<llvm::StoreInst>(user)) {
        if (store->getPointerOperand() == arg)
          return false;
      }
      if (auto *gep = llvm::dyn_cast<llvm::GetElementPtrInst>(user)) {
        for (const auto &gepUse : gep->uses()) {
          if (auto *store = llvm::dyn_cast<llvm::StoreInst>(gepUse.getUser())) {
            if (store->getPointerOperand() == gep)
              return false;
          }
        }
      }
    }
    return true;
  }

  // ---- NVVM Intrinsic Detection ----

  /// Check all instructions in a function for NVVM intrinsic calls
  /// to determine which Metal built-in parameters are needed.
  void detectNVVMIntrinsics(const llvm::Function &F) {
    usesThreadPositionInThreadgroup = false;
    usesThreadgroupPositionInGrid = false;
    usesThreadsPerThreadgroup = false;
    usesThreadgroupsPerGrid = false;

    for (const auto &BB : F) {
      for (const auto &I : BB) {
        if (auto *call = llvm::dyn_cast<llvm::CallInst>(&I)) {
          auto *callee = call->getCalledFunction();
          if (!callee)
            continue;
          llvm::StringRef name = callee->getName();
          if (name.starts_with("llvm.nvvm.read.ptx.sreg.tid."))
            usesThreadPositionInThreadgroup = true;
          else if (name.starts_with("llvm.nvvm.read.ptx.sreg.ctaid."))
            usesThreadgroupPositionInGrid = true;
          else if (name.starts_with("llvm.nvvm.read.ptx.sreg.ntid."))
            usesThreadsPerThreadgroup = true;
          else if (name.starts_with("llvm.nvvm.read.ptx.sreg.nctaid."))
            usesThreadgroupsPerGrid = true;
        }
      }
    }

    // If threadgroup position or threads per threadgroup is used, we might also
    // want thread_position_in_grid for convenience (though we compute it from
    // the others). For now, only add what's actually used.
  }

  // ---- Kernel Signature Generation ----

  void emitKernelSignature(const llvm::Function &F) {
    detectNVVMIntrinsics(F);

    out << "kernel void " << cleanName(F.getName().str()) << "(\n";

    int bufferIdx = 0;
    bool firstParam = true;

    // Emit buffer arguments
    for (const auto &arg : F.args()) {
      if (!firstParam)
        out << ",\n";
      firstParam = false;
      indent();
      out << "    ";

      std::string argName = arg.hasName() ? cleanName(arg.getName().str())
                                          : ("_arg" + std::to_string(arg.getArgNo()));
      nameMap[&arg] = argName;

      if (arg.getType()->isPointerTy()) {
        llvm::Type *pointee = inferPointeeType(&arg);
        std::string ptype = pointee ? mslTypeName(pointee) : "char";
        bool ro = isReadOnly(&arg);
        out << "device " << (ro ? "const " : "") << ptype << "* " << argName
            << " [[buffer(" << bufferIdx++ << ")]]";
      } else {
        // Scalar arguments: pass via constant buffer
        out << "constant " << mslTypeName(arg.getType()) << "& " << argName
            << " [[buffer(" << bufferIdx++ << ")]]";
      }
    }

    // Emit Metal built-in parameters based on NVVM intrinsic usage
    if (usesThreadPositionInThreadgroup) {
      if (!firstParam)
        out << ",\n";
      firstParam = false;
      indent();
      out << "    uint3 _thread_position_in_threadgroup "
             "[[thread_position_in_threadgroup]]";
    }
    if (usesThreadgroupPositionInGrid) {
      if (!firstParam)
        out << ",\n";
      firstParam = false;
      indent();
      out << "    uint3 _threadgroup_position_in_grid "
             "[[threadgroup_position_in_grid]]";
    }
    if (usesThreadsPerThreadgroup) {
      if (!firstParam)
        out << ",\n";
      firstParam = false;
      indent();
      out << "    uint3 _threads_per_threadgroup [[threads_per_threadgroup]]";
    }
    if (usesThreadgroupsPerGrid) {
      if (!firstParam)
        out << ",\n";
      firstParam = false;
      indent();
      out << "    uint3 _threadgroups_per_grid [[threadgroups_per_grid]]";
    }
    out << ")\n";
  }

  // ---- Helper Function Emission ----

  void emitHelperFunction(const llvm::Function &F) {
    // Non-kernel helper functions called from kernels
    llvm::Type *retTy = F.getReturnType();
    out << mslTypeName(retTy) << " " << cleanName(F.getName().str()) << "(";

    bool first = true;
    for (const auto &arg : F.args()) {
      if (!first)
        out << ", ";
      first = false;

      std::string argName = arg.hasName() ? cleanName(arg.getName().str())
                                          : ("_arg" + std::to_string(arg.getArgNo()));
      nameMap[&arg] = argName;

      if (arg.getType()->isPointerTy()) {
        llvm::Type *pointee = inferPointeeType(&arg);
        std::string ptype = pointee ? mslTypeName(pointee) : "char";
        out << "device " << ptype << "* " << argName;
      } else {
        out << mslTypeName(arg.getType()) << " " << argName;
      }
    }

    out << ")\n";
  }

  // ---- Instruction Emission ----

  void emitValue(const llvm::Value *V) {
    if (auto *CI = llvm::dyn_cast<llvm::ConstantInt>(V)) {
      if (CI->getType()->isIntegerTy(1)) {
        out << (CI->isZero() ? "false" : "true");
      } else {
        out << CI->getSExtValue();
      }
      return;
    }
    if (auto *CF = llvm::dyn_cast<llvm::ConstantFP>(V)) {
      llvm::SmallVector<char, 32> buf;
      CF->getValueAPF().toString(buf, /*FormatPrecision=*/8);
      out << std::string(buf.data(), buf.size());
      if (V->getType()->isFloatTy())
        out << "f";
      return;
    }
    if (llvm::isa<llvm::UndefValue>(V) || llvm::isa<llvm::PoisonValue>(V)) {
      // Emit a zero-initialized value for undef/poison
      out << mslTypeName(V->getType()) << "(0)";
      return;
    }
    if (auto *CE = llvm::dyn_cast<llvm::ConstantExpr>(V)) {
      // Handle constant expressions inline
      compilationError("[Metal] constant expressions not supported in MSL transpiler");
      return;
    }
    if (llvm::isa<llvm::ConstantPointerNull>(V)) {
      out << "nullptr";
      return;
    }
    // Regular value - look up name
    out << getName(V);
  }

  void emitBinaryOp(const llvm::BinaryOperator &I) {
    indent();
    std::string ty = mslTypeName(I.getType());
    out << ty << " " << getName(&I) << " = ";

    auto *lhs = I.getOperand(0);
    auto *rhs = I.getOperand(1);

    switch (I.getOpcode()) {
    case llvm::Instruction::FAdd:
    case llvm::Instruction::Add:
      emitValue(lhs);
      out << " + ";
      emitValue(rhs);
      break;
    case llvm::Instruction::FSub:
    case llvm::Instruction::Sub:
      emitValue(lhs);
      out << " - ";
      emitValue(rhs);
      break;
    case llvm::Instruction::FMul:
    case llvm::Instruction::Mul:
      emitValue(lhs);
      out << " * ";
      emitValue(rhs);
      break;
    case llvm::Instruction::FDiv:
    case llvm::Instruction::SDiv:
      emitValue(lhs);
      out << " / ";
      emitValue(rhs);
      break;
    case llvm::Instruction::UDiv:
      out << "(" << mslUnsignedTypeName(I.getType()) << ")(";
      emitValue(lhs);
      out << ") / (" << mslUnsignedTypeName(I.getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::Instruction::FRem:
      out << "metal::fmod(";
      emitValue(lhs);
      out << ", ";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::Instruction::SRem:
      emitValue(lhs);
      out << " % ";
      emitValue(rhs);
      break;
    case llvm::Instruction::URem:
      out << "(" << mslUnsignedTypeName(I.getType()) << ")(";
      emitValue(lhs);
      out << ") % (" << mslUnsignedTypeName(I.getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::Instruction::And:
      emitValue(lhs);
      out << " & ";
      emitValue(rhs);
      break;
    case llvm::Instruction::Or:
      emitValue(lhs);
      out << " | ";
      emitValue(rhs);
      break;
    case llvm::Instruction::Xor:
      emitValue(lhs);
      out << " ^ ";
      emitValue(rhs);
      break;
    case llvm::Instruction::Shl:
      emitValue(lhs);
      out << " << ";
      emitValue(rhs);
      break;
    case llvm::Instruction::AShr:
      emitValue(lhs);
      out << " >> ";
      emitValue(rhs);
      break;
    case llvm::Instruction::LShr:
      out << "(" << mslUnsignedTypeName(I.getType()) << ")(";
      emitValue(lhs);
      out << ") >> ";
      emitValue(rhs);
      break;
    default:
      compilationError("[Metal] unsupported binary operator: " +
                       std::string(I.getOpcodeName()));
    }
    out << ";\n";
  }

  void emitLoad(const llvm::LoadInst &I) {
    indent();
    std::string ty = mslTypeName(I.getType());
    std::string name = getName(&I);
    out << ty << " " << name << " = ";

    auto *ptr = I.getPointerOperand();
    // If loading from an alloca or argument pointer: dereference
    if (llvm::isa<llvm::AllocaInst>(ptr)) {
      emitValue(ptr);
    } else {
      // Array-style access for device pointers: *ptr or ptr[0]
      out << "*";
      emitValue(ptr);
    }
    out << ";\n";
  }

  void emitStore(const llvm::StoreInst &I) {
    indent();
    auto *val = I.getValueOperand();
    auto *ptr = I.getPointerOperand();

    if (llvm::isa<llvm::AllocaInst>(ptr)) {
      emitValue(ptr);
      out << " = ";
      emitValue(val);
    } else {
      out << "*";
      emitValue(ptr);
      out << " = ";
      emitValue(val);
    }
    out << ";\n";
  }

  void emitAlloca(const llvm::AllocaInst &I) {
    indent();
    std::string ty = mslTypeName(I.getAllocatedType());
    std::string name = getName(&I);
    out << ty << " " << name << ";\n";
  }

  void emitReturn(const llvm::ReturnInst &I) {
    indent();
    auto *retVal = I.getReturnValue();
    // Treat void returns and zero-sized returns (e.g., Codon's None = {})
    // as plain "return;" in MSL kernel functions
    if (!retVal || retVal->getType()->isVoidTy()) {
      out << "return;\n";
    } else if (auto *structTy = llvm::dyn_cast<llvm::StructType>(retVal->getType())) {
      if (structTy->getNumElements() == 0) {
        out << "return;\n";
      } else {
        out << "return ";
        emitValue(retVal);
        out << ";\n";
      }
    } else {
      out << "return ";
      emitValue(retVal);
      out << ";\n";
    }
  }

  void emitBranch(const llvm::BranchInst &I) {
    if (I.isUnconditional()) {
      indent();
      out << "goto " << cleanName(I.getSuccessor(0)->getName().str()) << ";\n";
    } else {
      indent();
      out << "if (";
      emitValue(I.getCondition());
      out << ") goto " << cleanName(I.getSuccessor(0)->getName().str()) << ";\n";
      indent();
      out << "else goto " << cleanName(I.getSuccessor(1)->getName().str()) << ";\n";
    }
  }

  void emitPhi(const llvm::PHINode &I) {
    // PHI nodes are handled by emitting assignments at the end of predecessor blocks.
    // Here we just declare the variable.
    indent();
    std::string ty = mslTypeName(I.getType());
    std::string name = getName(&I);
    out << ty << " " << name << ";\n";
  }

  void emitPhiAssignments(const llvm::BasicBlock &BB, const llvm::BasicBlock &succ) {
    // For each PHI in the successor, emit an assignment from the value
    // corresponding to BB as the incoming block.
    // We use temporaries to handle cases where PHIs reference each other.
    std::vector<std::pair<std::string, std::string>> assignments;

    for (const auto &I : succ) {
      auto *phi = llvm::dyn_cast<llvm::PHINode>(&I);
      if (!phi)
        break;

      int idx = phi->getBasicBlockIndex(&BB);
      if (idx < 0)
        continue;

      std::string tmpName = "_phi_tmp_" + std::to_string(nextTmp++);
      indent();
      out << mslTypeName(phi->getType()) << " " << tmpName << " = ";
      emitValue(phi->getIncomingValue(idx));
      out << ";\n";
      assignments.push_back({getName(phi), tmpName});
    }

    for (const auto &[dest, src] : assignments) {
      indent();
      out << dest << " = " << src << ";\n";
    }
  }

  void emitSelect(const llvm::SelectInst &I) {
    indent();
    std::string ty = mslTypeName(I.getType());
    out << ty << " " << getName(&I) << " = ";
    emitValue(I.getCondition());
    out << " ? ";
    emitValue(I.getTrueValue());
    out << " : ";
    emitValue(I.getFalseValue());
    out << ";\n";
  }

  void emitGEP(const llvm::GetElementPtrInst &I) {
    indent();
    // For v1: support simple 1D array GEP (ptr + offset)
    // GEP with single index: element pointer arithmetic
    llvm::Type *srcElemTy = I.getSourceElementType();
    std::string elemTyStr =
        srcElemTy->isIntegerTy(8) ? "char" : mslTypeName(srcElemTy);

    // Propagate const qualifier from originating kernel argument
    bool isConst = false;
    if (auto *arg = traceToArg(I.getPointerOperand()))
      isConst = isReadOnly(arg);
    std::string qualifier =
        std::string("device ") + (isConst ? "const " : "");

    if (I.getNumIndices() == 1) {
      auto *idx = I.getOperand(1);
      std::string name = getName(&I);
      out << qualifier << elemTyStr << "* " << name << " = ";
      emitValue(I.getPointerOperand());
      out << " + ";
      emitValue(idx);
      out << ";\n";
    } else if (I.getNumIndices() == 2) {
      // Common pattern: GEP with [0, idx] for array access
      auto *firstIdx = I.getOperand(1);
      auto *secondIdx = I.getOperand(2);
      std::string name = getName(&I);

      if (auto *CI = llvm::dyn_cast<llvm::ConstantInt>(firstIdx)) {
        if (CI->isZero()) {
          out << qualifier << elemTyStr << "* " << name << " = ";
          emitValue(I.getPointerOperand());
          out << " + ";
          emitValue(secondIdx);
          out << ";\n";
          return;
        }
      }
      compilationError("[Metal] struct GEP not supported in v1; "
                       "only 1D array GEP is supported");
    } else {
      compilationError("[Metal] multi-index GEP not supported in v1; "
                       "only 1D array GEP is supported");
    }
  }

  void emitICmp(const llvm::ICmpInst &I) {
    indent();
    out << "bool " << getName(&I) << " = ";

    auto *lhs = I.getOperand(0);
    auto *rhs = I.getOperand(1);

    switch (I.getPredicate()) {
    case llvm::CmpInst::ICMP_EQ:
      emitValue(lhs);
      out << " == ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_NE:
      emitValue(lhs);
      out << " != ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_SGT:
      emitValue(lhs);
      out << " > ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_SGE:
      emitValue(lhs);
      out << " >= ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_SLT:
      emitValue(lhs);
      out << " < ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_SLE:
      emitValue(lhs);
      out << " <= ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::ICMP_UGT:
      out << "(" << mslUnsignedTypeName(lhs->getType()) << ")(";
      emitValue(lhs);
      out << ") > (" << mslUnsignedTypeName(rhs->getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::CmpInst::ICMP_UGE:
      out << "(" << mslUnsignedTypeName(lhs->getType()) << ")(";
      emitValue(lhs);
      out << ") >= (" << mslUnsignedTypeName(rhs->getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::CmpInst::ICMP_ULT:
      out << "(" << mslUnsignedTypeName(lhs->getType()) << ")(";
      emitValue(lhs);
      out << ") < (" << mslUnsignedTypeName(rhs->getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    case llvm::CmpInst::ICMP_ULE:
      out << "(" << mslUnsignedTypeName(lhs->getType()) << ")(";
      emitValue(lhs);
      out << ") <= (" << mslUnsignedTypeName(rhs->getType()) << ")(";
      emitValue(rhs);
      out << ")";
      break;
    default:
      compilationError("[Metal] unsupported icmp predicate");
    }
    out << ";\n";
  }

  void emitFCmp(const llvm::FCmpInst &I) {
    indent();
    out << "bool " << getName(&I) << " = ";

    auto *lhs = I.getOperand(0);
    auto *rhs = I.getOperand(1);

    switch (I.getPredicate()) {
    case llvm::CmpInst::FCMP_OEQ:
    case llvm::CmpInst::FCMP_UEQ:
      emitValue(lhs);
      out << " == ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_ONE:
    case llvm::CmpInst::FCMP_UNE:
      emitValue(lhs);
      out << " != ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_OGT:
    case llvm::CmpInst::FCMP_UGT:
      emitValue(lhs);
      out << " > ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_OGE:
    case llvm::CmpInst::FCMP_UGE:
      emitValue(lhs);
      out << " >= ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_OLT:
    case llvm::CmpInst::FCMP_ULT:
      emitValue(lhs);
      out << " < ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_OLE:
    case llvm::CmpInst::FCMP_ULE:
      emitValue(lhs);
      out << " <= ";
      emitValue(rhs);
      break;
    case llvm::CmpInst::FCMP_TRUE:
      out << "true";
      break;
    case llvm::CmpInst::FCMP_FALSE:
      out << "false";
      break;
    default:
      compilationError("[Metal] unsupported fcmp predicate");
    }
    out << ";\n";
  }

  /// Try to handle an NVVM intrinsic call. Returns true if handled.
  bool tryEmitNVVMIntrinsic(const llvm::CallInst &I) {
    auto *callee = I.getCalledFunction();
    if (!callee)
      return false;
    llvm::StringRef name = callee->getName();

    struct NVVMMapping {
      llvm::StringRef prefix;
      const char *metalExpr; // use %c for .x/.y/.z component
    };

    static const NVVMMapping mappings[] = {
        {"llvm.nvvm.read.ptx.sreg.tid.", "_thread_position_in_threadgroup.%c"},
        {"llvm.nvvm.read.ptx.sreg.ctaid.", "_threadgroup_position_in_grid.%c"},
        {"llvm.nvvm.read.ptx.sreg.ntid.", "_threads_per_threadgroup.%c"},
        {"llvm.nvvm.read.ptx.sreg.nctaid.", "_threadgroups_per_grid.%c"},
    };

    for (const auto &m : mappings) {
      if (name.starts_with(m.prefix)) {
        char component = name.back(); // 'x', 'y', or 'z'
        indent();
        out << "int " << getName(&I) << " = (int)";
        std::string expr(m.metalExpr);
        size_t pos = expr.find("%c");
        if (pos != std::string::npos)
          expr.replace(pos, 2, std::string(1, component));
        out << expr << ";\n";
        return true;
      }
    }

    // Math intrinsics mapping (v1: basic set)
    struct MathMapping {
      llvm::StringRef llvmName;
      const char *mslName;
    };

    static const MathMapping mathMappings[] = {
        {"llvm.fabs.f32", "metal::abs"},
        {"llvm.fabs.f64", "metal::abs"},
        {"llvm.sqrt.f32", "metal::sqrt"},
        {"llvm.floor.f32", "metal::floor"},
        {"llvm.ceil.f32", "metal::ceil"},
        {"llvm.round.f32", "metal::round"},
        {"llvm.trunc.f32", "metal::trunc"},
        {"llvm.sin.f32", "metal::sin"},
        {"llvm.cos.f32", "metal::cos"},
        {"llvm.exp.f32", "metal::exp"},
        {"llvm.exp2.f32", "metal::exp2"},
        {"llvm.log.f32", "metal::log"},
        {"llvm.log2.f32", "metal::log2"},
        {"llvm.log10.f32", "metal::log10"},
        {"llvm.pow.f32", "metal::pow"},
        {"llvm.minnum.f32", "metal::min"},
        {"llvm.maxnum.f32", "metal::max"},
        {"llvm.fma.f32", "metal::fma"},
        {"llvm.copysign.f32", "metal::copysign"},
        {"llvm.rint.f32", "metal::rint"},
        {"llvm.nearbyint.f32", "metal::rint"},
    };

    for (const auto &m : mathMappings) {
      if (name == m.llvmName) {
        indent();
        out << mslTypeName(I.getType()) << " " << getName(&I) << " = " << m.mslName
            << "(";
        for (unsigned i = 0; i < I.arg_size(); ++i) {
          if (i > 0)
            out << ", ";
          emitValue(I.getArgOperand(i));
        }
        out << ");\n";
        return true;
      }
    }

    return false;
  }

  void emitCall(const llvm::CallInst &I) {
    // Try NVVM intrinsic first
    if (tryEmitNVVMIntrinsic(I))
      return;

    auto *callee = I.getCalledFunction();
    if (!callee) {
      compilationError("[Metal] indirect calls not supported");
      return;
    }

    indent();
    if (!I.getType()->isVoidTy()) {
      out << mslTypeName(I.getType()) << " " << getName(&I) << " = ";
    }
    out << cleanName(callee->getName().str()) << "(";
    for (unsigned i = 0; i < I.arg_size(); ++i) {
      if (i > 0)
        out << ", ";
      emitValue(I.getArgOperand(i));
    }
    out << ");\n";
  }

  void emitCast(const llvm::CastInst &I) {
    indent();
    std::string destTy = mslTypeName(I.getDestTy());
    out << destTy << " " << getName(&I) << " = ";

    switch (I.getOpcode()) {
    case llvm::Instruction::Trunc:
    case llvm::Instruction::ZExt:
    case llvm::Instruction::SExt:
    case llvm::Instruction::FPTrunc:
    case llvm::Instruction::FPExt:
    case llvm::Instruction::FPToSI:
    case llvm::Instruction::FPToUI:
    case llvm::Instruction::SIToFP:
    case llvm::Instruction::UIToFP:
    case llvm::Instruction::BitCast:
    case llvm::Instruction::AddrSpaceCast:
      out << "(" << destTy << ")(";
      emitValue(I.getOperand(0));
      out << ")";
      break;
    case llvm::Instruction::PtrToInt:
      out << "((" << destTy << ")(long)(";
      emitValue(I.getOperand(0));
      out << "))";
      break;
    case llvm::Instruction::IntToPtr:
      out << "((device char*)(long)(";
      emitValue(I.getOperand(0));
      out << "))";
      break;
    default:
      compilationError("[Metal] unsupported cast: " + std::string(I.getOpcodeName()));
    }
    out << ";\n";
  }

  void emitInstruction(const llvm::Instruction &I) {
    // Skip PHI nodes (handled separately in block preamble)
    if (llvm::isa<llvm::PHINode>(I))
      return;

    if (auto *binOp = llvm::dyn_cast<llvm::BinaryOperator>(&I)) {
      emitBinaryOp(*binOp);
    } else if (auto *load = llvm::dyn_cast<llvm::LoadInst>(&I)) {
      emitLoad(*load);
    } else if (auto *store = llvm::dyn_cast<llvm::StoreInst>(&I)) {
      emitStore(*store);
    } else if (auto *alloca = llvm::dyn_cast<llvm::AllocaInst>(&I)) {
      emitAlloca(*alloca);
    } else if (auto *ret = llvm::dyn_cast<llvm::ReturnInst>(&I)) {
      emitReturn(*ret);
    } else if (auto *br = llvm::dyn_cast<llvm::BranchInst>(&I)) {
      if (br->isUnconditional()) {
        emitPhiAssignments(*br->getParent(), *br->getSuccessor(0));
        emitBranch(*br);
      } else {
        // Check if either successor has PHI nodes (PHIs are always first in LLVM BBs)
        bool succ0HasPhi =
            !br->getSuccessor(0)->empty() &&
            llvm::isa<llvm::PHINode>(br->getSuccessor(0)->front());
        bool succ1HasPhi =
            !br->getSuccessor(1)->empty() &&
            llvm::isa<llvm::PHINode>(br->getSuccessor(1)->front());

        if (succ0HasPhi || succ1HasPhi) {
          // Use structured if/else to emit PHI assignments in both paths
          indent();
          out << "if (";
          emitValue(br->getCondition());
          out << ") {\n";
          indentLevel++;
          emitPhiAssignments(*br->getParent(), *br->getSuccessor(0));
          indent();
          out << "goto " << cleanName(br->getSuccessor(0)->getName().str()) << ";\n";
          indentLevel--;
          indent();
          out << "} else {\n";
          indentLevel++;
          emitPhiAssignments(*br->getParent(), *br->getSuccessor(1));
          indent();
          out << "goto " << cleanName(br->getSuccessor(1)->getName().str()) << ";\n";
          indentLevel--;
          indent();
          out << "}\n";
        } else {
          emitBranch(*br);
        }
      }
    } else if (auto *sel = llvm::dyn_cast<llvm::SelectInst>(&I)) {
      emitSelect(*sel);
    } else if (auto *gep = llvm::dyn_cast<llvm::GetElementPtrInst>(&I)) {
      emitGEP(*gep);
    } else if (auto *icmp = llvm::dyn_cast<llvm::ICmpInst>(&I)) {
      emitICmp(*icmp);
    } else if (auto *fcmp = llvm::dyn_cast<llvm::FCmpInst>(&I)) {
      emitFCmp(*fcmp);
    } else if (auto *call = llvm::dyn_cast<llvm::CallInst>(&I)) {
      emitCall(*call);
    } else if (auto *cast = llvm::dyn_cast<llvm::CastInst>(&I)) {
      emitCast(*cast);
    } else if (llvm::isa<llvm::UnreachableInst>(I)) {
      indent();
      out << "// unreachable\n";
    } else if (auto *sw = llvm::dyn_cast<llvm::SwitchInst>(&I)) {
      indent();
      out << "switch (";
      emitValue(sw->getCondition());
      out << ") {\n";
      for (auto &c : sw->cases()) {
        indent();
        out << "case ";
        emitValue(c.getCaseValue());
        out << ": {\n";
        indentLevel++;
        emitPhiAssignments(*sw->getParent(), *c.getCaseSuccessor());
        indent();
        out << "goto " << cleanName(c.getCaseSuccessor()->getName().str()) << ";\n";
        indentLevel--;
        indent();
        out << "}\n";
      }
      indent();
      out << "default: {\n";
      indentLevel++;
      emitPhiAssignments(*sw->getParent(), *sw->getDefaultDest());
      indent();
      out << "goto " << cleanName(sw->getDefaultDest()->getName().str()) << ";\n";
      indentLevel--;
      indent();
      out << "}\n";
      indent();
      out << "}\n";
    } else if (llvm::isa<llvm::FreezeInst>(I)) {
      // freeze is a no-op in MSL, just forward the value
      indent();
      out << mslTypeName(I.getType()) << " " << getName(&I) << " = ";
      emitValue(I.getOperand(0));
      out << ";\n";
    } else {
      compilationError("[Metal] unsupported instruction: " +
                       std::string(I.getOpcodeName()));
    }
  }

  void emitFunction(const llvm::Function &F, bool isKernel) {
    nameMap.clear();
    nextTmp = 0;
    indentLevel = 1;

    if (isKernel) {
      emitKernelSignature(F);
    } else {
      emitHelperFunction(F);
    }
    out << "{\n";

    // Declare PHI variables at top of function
    for (const auto &BB : F) {
      for (const auto &I : BB) {
        if (auto *phi = llvm::dyn_cast<llvm::PHINode>(&I)) {
          emitPhi(*phi);
        }
      }
    }

    // Emit basic blocks
    bool firstBB = true;
    for (const auto &BB : F) {
      if (!firstBB || BB.hasNUsesOrMore(1)) {
        // Emit label if the block is a branch target (or not the entry block)
        std::string label = BB.hasName() ? cleanName(BB.getName().str())
                                         : ("_bb" + std::to_string(nextTmp++));
        if (!BB.hasName()) {
          // Ensure we use this label in nameMap for future references
        }
        out << label << ":\n";
        // Ensure blocks referenced by name have their name stored
        // (cleanName handles the mapping)
      }
      firstBB = false;

      for (const auto &I : BB) {
        emitInstruction(I);
      }
    }

    out << "}\n\n";
  }

public:
  /// Transpile an entire LLVM module to MSL.
  /// @param M The module to transpile (should contain only kernel-related functions)
  /// @param kernelNames Names of the kernel functions
  std::string transpileModule(llvm::Module &M,
                              const std::vector<std::string> &kernelNames) {
    out.str("");
    out.clear();

    // MSL header
    out << "#include <metal_stdlib>\n";
    out << "using namespace metal;\n\n";

    std::unordered_set<std::string> kernelSet(kernelNames.begin(), kernelNames.end());

    // Emit helper functions first (non-kernels with bodies)
    for (const auto &F : M) {
      if (F.isDeclaration())
        continue;
      if (kernelSet.count(F.getName().str()))
        continue;
      emitFunction(F, false);
    }

    // Emit kernel functions
    for (const auto &F : M) {
      if (F.isDeclaration())
        continue;
      if (!kernelSet.count(F.getName().str()))
        continue;
      emitFunction(F, true);
    }

    return out.str();
  }
};

// ============================================================================
// Section D: Module Cleanup (GVExtractor, getRequiredGVs)
// Copied from gpu.cpp as these are internal helpers.
// ============================================================================

class MetalGVExtractor : public llvm::PassInfoMixin<MetalGVExtractor> {
  llvm::SetVector<llvm::GlobalValue *> named;
  bool deleteStuff;
  bool keepConstInit;

public:
  explicit MetalGVExtractor(std::vector<llvm::GlobalValue *> &GVs,
                            bool deleteS = true, bool keepConstInit = false)
      : named(GVs.begin(), GVs.end()), deleteStuff(deleteS),
        keepConstInit(keepConstInit) {}

  static void makeVisible(llvm::GlobalValue &GV, bool del) {
    bool local = GV.hasLocalLinkage();
    if (local || del) {
      GV.setLinkage(llvm::GlobalValue::ExternalLinkage);
      if (local)
        GV.setVisibility(llvm::GlobalValue::HiddenVisibility);
      return;
    }

    if (!GV.hasLinkOnceLinkage()) {
      seqassertn(!GV.isDiscardableIfUnused(), "bad global in extractor");
      return;
    }

    switch (GV.getLinkage()) {
    default:
      seqassertn(false, "unexpected linkage");
    case llvm::GlobalValue::LinkOnceAnyLinkage:
      GV.setLinkage(llvm::GlobalValue::WeakAnyLinkage);
      return;
    case llvm::GlobalValue::LinkOnceODRLinkage:
      GV.setLinkage(llvm::GlobalValue::WeakODRLinkage);
      return;
    }
  }

  llvm::PreservedAnalyses run(llvm::Module &M, llvm::ModuleAnalysisManager &) {
    if (!deleteStuff)
      M.setModuleInlineAsm("");

    for (auto &GV : M.globals()) {
      bool del = deleteStuff == (bool)named.count(&GV) && !GV.isDeclaration() &&
                 (!GV.isConstant() || !keepConstInit);
      if (!del) {
        if (GV.hasAvailableExternallyLinkage())
          continue;
        if (GV.getName() == "llvm.global_ctors")
          continue;
      }

      makeVisible(GV, del);

      if (del) {
        GV.setInitializer(nullptr);
        GV.setComdat(nullptr);
      }
    }

    for (auto &F : M) {
      bool del = deleteStuff == (bool)named.count(&F) && !F.isDeclaration();
      if (!del) {
        if (F.hasAvailableExternallyLinkage())
          continue;
      }

      makeVisible(F, del);

      if (del) {
        F.deleteBody();
        F.setComdat(nullptr);
      }
    }

    for (auto &GA : llvm::make_early_inc_range(M.aliases())) {
      bool del = deleteStuff == (bool)named.count(&GA);
      makeVisible(GA, del);

      if (del) {
        auto *ty = GA.getValueType();
        GA.removeFromParent();
        llvm::Value *decl;
        if (auto *funcTy = llvm::dyn_cast<llvm::FunctionType>(ty)) {
          decl = llvm::Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage,
                                        GA.getAddressSpace(), GA.getName(), &M);

        } else {
          decl = new llvm::GlobalVariable(
              M, ty, false, llvm::GlobalValue::ExternalLinkage, nullptr, GA.getName());
        }
        GA.replaceAllUsesWith(decl);
        delete &GA;
      }
    }

    return llvm::PreservedAnalyses::none();
  }
};

void metalExploreGV(llvm::GlobalValue *G,
                    llvm::SmallPtrSetImpl<llvm::GlobalValue *> &keep) {
  if (keep.contains(G))
    return;

  keep.insert(G);
  if (auto *F = llvm::dyn_cast<llvm::Function>(G)) {
    for (auto I = llvm::inst_begin(F), E = inst_end(F); I != E; ++I) {
      for (auto &U : I->operands()) {
        if (auto *G2 = llvm::dyn_cast<llvm::GlobalValue>(U.get()))
          metalExploreGV(G2, keep);
      }
    }
  }
}

std::vector<llvm::GlobalValue *>
metalGetRequiredGVs(const std::vector<llvm::GlobalValue *> &kernels) {
  llvm::SmallPtrSet<llvm::GlobalValue *, 32> keep;
  for (auto *G : kernels) {
    metalExploreGV(G, keep);
  }
  return std::vector<llvm::GlobalValue *>(keep.begin(), keep.end());
}

// ============================================================================
// Section E: patchMSLVar
// ============================================================================

void patchMSLVar(llvm::Module *M, llvm::GlobalValue *mslVar,
                 const std::string &mslTarget = "__codon_msl__") {
  llvm::SmallVector<llvm::Instruction *, 1> callsToReplace;
  for (auto &F : *M) {
    for (auto &BB : F) {
      for (auto &I : BB) {
        auto *call = llvm::dyn_cast<llvm::CallBase>(&I);
        if (!call)
          continue;

        auto *callee = call->getCalledFunction();
        if (!callee)
          continue;

        if (callee->getName() == mslTarget && call->arg_size() == 0)
          callsToReplace.push_back(call);
      }
    }
  }

  for (auto *call : callsToReplace) {
    if (mslVar) {
      call->replaceAllUsesWith(mslVar);
    } else {
      call->replaceAllUsesWith(
          llvm::ConstantPointerNull::get(llvm::PointerType::get(M->getContext(), 0)));
    }
    call->dropAllReferences();
    call->eraseFromParent();
  }

  if (auto *F = M->getFunction(mslTarget)) {
    seqassertn(F->use_empty(), "some __codon_msl__() calls not replaced in module");
    F->eraseFromParent();
  }
}

// ============================================================================
// Section F: Clean up NVVM intrinsics from host module
// ============================================================================

void metalCleanUpIntrinsics(llvm::Module *M) {
  // Create no-op replacements for any NVVM intrinsics left in the host module
  llvm::SmallVector<llvm::Function *, 16> remove;
  for (auto &F : *M) {
    if (F.getIntrinsicID() != llvm::Intrinsic::not_intrinsic &&
        F.getName().starts_with("llvm.nvvm"))
      remove.push_back(&F);
  }

  for (auto *F : remove) {
    // Create a dummy replacement
    auto dummyName = (".codon.metal.dummy." + F->getName()).str();
    auto *M2 = F->getParent();
    auto &context = M2->getContext();
    auto *dummy = M2->getFunction(dummyName);
    if (!dummy) {
      dummy = llvm::Function::Create(F->getFunctionType(),
                                     llvm::GlobalValue::PrivateLinkage, dummyName, *M2);
      auto *entry = llvm::BasicBlock::Create(context, "entry", dummy);
      llvm::IRBuilder<> B(entry);
      auto *retType = F->getReturnType();
      if (retType->isVoidTy()) {
        B.CreateRetVoid();
      } else {
        B.CreateRet(llvm::UndefValue::get(retType));
      }
    }
    F->replaceAllUsesWith(dummy);
    F->dropAllReferences();
    F->eraseFromParent();
  }
}

} // namespace

// ============================================================================
// Section G: Public API
// ============================================================================

void applyMetalTransformations(llvm::Module *M, const std::string &mslFilename) {
  llvm::LLVMContext &context = M->getContext();

  // Check if __codon_msl__ stub exists; if not, Metal is not being used
  if (!M->getFunction("__codon_msl__"))
    return;

  // Clone the module to preserve the original for host code
  std::unique_ptr<llvm::Module> clone = llvm::CloneModule(*M);

  // Find kernel functions in the clone
  std::vector<llvm::GlobalValue *> kernels;
  std::vector<std::string> kernelNames;
  for (auto &F : *clone) {
    if (!F.hasFnAttribute("kernel"))
      continue;
    kernels.push_back(&F);
    kernelNames.push_back(F.getName().str());
  }

  if (kernels.empty()) {
    patchMSLVar(M, nullptr);
    return;
  }

  // Extract only the kernel functions and their dependencies
  auto keep = metalGetRequiredGVs(kernels);

  {
    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cgam;
    llvm::ModuleAnalysisManager mam;
    llvm::ModulePassManager mpm;
    llvm::PassBuilder pb;

    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.crossRegisterProxies(lam, fam, cgam, mam);

    mpm.addPass(MetalGVExtractor(keep, false));
    mpm.addPass(llvm::GlobalDCEPass());
    mpm.addPass(llvm::StripDeadDebugInfoPass());
    mpm.addPass(llvm::StripDeadPrototypesPass());
    mpm.run(*clone, mam);
  }

  // Remove personality functions and debug info from clone
  for (auto &F : *clone) {
    F.setDoesNotThrow();
    F.setPersonalityFn(nullptr);
    F.removeFnAttr(llvm::Attribute::AttrKind::NoInline);
    F.setWillReturn();
  }
  llvm::StripDebugInfo(*clone);

  // Run O2 optimization on the clone to simplify IR before transpilation
  {
    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cgam;
    llvm::ModuleAnalysisManager mam;
    llvm::PassBuilder pb;

    pb.registerModuleAnalyses(mam);
    pb.registerCGSCCAnalyses(cgam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.crossRegisterProxies(lam, fam, cgam, mam);

    llvm::ModulePassManager mpm =
        pb.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
    mpm.run(*clone, mam);
  }

  // Transpile LLVM IR → MSL
  MSLTranspiler transpiler;
  std::string msl = transpiler.transpileModule(*clone, kernelNames);

  // Clean up NVVM intrinsics from host module
  metalCleanUpIntrinsics(M);

  // Write MSL to file if requested
  std::string outputFile =
      mslOutput.getNumOccurrences() > 0 ? std::string(mslOutput) : mslFilename;
  if (!outputFile.empty()) {
    std::error_code err;
    llvm::ToolOutputFile out(outputFile, err, llvm::sys::fs::OF_Text);
    seqassertn(!err, "Could not open file: {}", err.message());
    llvm::raw_ostream &os = out.os();
    os << msl;
    os.flush();
    out.keep();
  }

  // Embed MSL code as a global variable in the host module
  auto *mslVar = new llvm::GlobalVariable(
      *M, llvm::ArrayType::get(llvm::Type::getInt8Ty(context), msl.length() + 1),
      /*isConstant=*/true, llvm::GlobalValue::PrivateLinkage,
      llvm::ConstantDataArray::getString(context, msl), ".msl");

  mslVar->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
  patchMSLVar(M, mslVar);
}

} // namespace ir
} // namespace codon
