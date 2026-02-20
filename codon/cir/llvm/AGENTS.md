# LLVM Backend Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

LLVM code generation backend for translating Codon IR to LLVM IR. Includes main visitor, GPU support, optimization pipeline, and platform-specific native code generation.

## Key Files

- `llvisitor.h/cpp` - Main LLVM IR generation visitor (LLVMVisitor class)
- `llvm.h` - LLVM integration utilities and type mappings
- `gpu.h/cpp` - GPU code generation support (CUDA, Metal, etc.)
- `optimize.h/cpp` - LLVM optimization pipeline configuration

## Subdirectories

- `native/` - Native platform code generation and architecture-specific optimizations

## For AI Agents

**LLVM Code Generation:**
- `LLVMVisitor` traverses CIR nodes and emits LLVM IR
- Uses visitor pattern to handle all CIR node types
- Manages LLVM context, module, builder state
- Handles coroutines, exceptions, memory management

**GPU Support:**
- GPU kernel generation for parallel constructs
- Platform-specific GPU IR (NVPTX for CUDA, AMDGPU, Metal)
- Memory space management (host vs device memory)

**Optimization Pipeline:**
- Configures LLVM optimization passes
- Platform-specific optimization levels
- Integration with LLVM pass manager

**Key Responsibilities:**
- Type lowering: CIR types → LLVM types
- Value lowering: CIR values → LLVM values
- Instruction lowering: CIR instructions → LLVM instructions
- Function lowering: CIR functions → LLVM functions
- Module lowering: CIR modules → LLVM modules

## Dependencies

- Depends on: `codon/cir/` (all CIR components), LLVM libraries
- Used by: `codon/compiler/` for final code generation
