# Architecture Target Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Architecture-specific code generation implementations for x86, ARM, and AArch64 processor families.

## Key Files

- `target.h` - Base target interface and abstractions
- `x86.h/cpp` - x86/x86-64 specific code generation (SSE, AVX, AVX-512)
- `arm.h/cpp` - 32-bit ARM code generation (NEON)
- `aarch64.h/cpp` - 64-bit ARM (ARM64) code generation

## For AI Agents

**Target-Specific Features:**
- x86: SSE/AVX vectorization, x86-64 calling conventions, intrinsics
- ARM: NEON vector extensions, ARM calling conventions
- AArch64: ARM64 optimizations, SVE support (future)

**Common Patterns:**
- Each target implements platform-specific intrinsic lowering
- Vector instruction selection and optimization
- Calling convention handling
- Platform-specific builtin functions

## Dependencies

- Depends on: `codon/cir/llvm/native/`, LLVM target backends
- Used by: Native code generator for architecture dispatch
