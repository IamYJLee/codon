# Native Code Generation Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Platform-specific native code generation and architecture-specific optimizations for x86, ARM, and AArch64 targets.

## Key Files

- `native.h/cpp` - Native code generation infrastructure and platform abstraction

## Subdirectories

- `targets/` - Architecture-specific code generation (x86, ARM, AArch64)

## For AI Agents

**Native Code Generation:**
- Provides platform-specific optimizations beyond LLVM's default behavior
- Handles architecture-specific calling conventions
- Manages vector instruction sets (SSE, AVX, NEON)
- Platform-specific intrinsics and builtin functions

**Platform Support:**
- x86/x86-64: SSE, AVX, AVX-512 optimizations
- ARM: NEON vectorization
- AArch64: ARM64 optimizations

## Dependencies

- Depends on: `codon/cir/llvm/`, LLVM target-specific libraries
- Used by: LLVM code generation pipeline for platform-specific lowering
