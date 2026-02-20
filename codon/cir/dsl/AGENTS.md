# CIR DSL Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Domain-specific language support for extending Codon IR with custom types, instructions, and code generation patterns.

## Key Files

- `nodes.h/cpp` - DSL node definitions (CustomType, CustomInstr, CustomFlow, CustomConst)
- `codegen.h` - Code generation builders for DSL extensions (CFBuilder, TypeBuilder, ValueBuilder)

## For AI Agents

**DSL Extension Framework:**
- Allows plugins to extend CIR with domain-specific constructs
- Custom types inherit from `dsl::types::CustomType`
- Custom instructions inherit from `dsl::CustomInstr`
- Custom control flow from `dsl::CustomFlow`
- Custom constants from `dsl::CustomConst`

**DSL Components:**
- **CustomType**: Domain-specific type extensions (e.g., tensor types, GPU types)
- **CustomInstr**: Custom instruction nodes with specialized semantics
- **CustomFlow**: Custom control flow constructs
- **Builders**: Convenience APIs for constructing DSL IR

**Usage Pattern:**
```cpp
// Define custom type
class TensorType : public dsl::types::CustomType {
    // Type implementation
};

// Define custom instruction
class TensorOp : public dsl::CustomInstr {
    // Operation implementation
};

// Register with plugin system
```

**Integration:**
- DSL nodes participate in standard CIR visitor patterns
- Custom code generation via LLVM visitor extension
- Custom optimizations via transform passes
- Used by GPU backend, tensor operations, specialized domains

## Dependencies

- Depends on: `codon/cir/base.h`, `codon/cir/instr.h`, `codon/cir/types/types.h`
- Used by: Plugin system, GPU backend, domain-specific extensions
