# Runtime Library

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Runtime library support for compiled Codon programs. Provides low-level runtime primitives and built-in functionality.

## Key Files
- `lib.h/cpp` - Core runtime library functions
- `exc.cpp` - Exception handling runtime
- `re.cpp` - Regular expression runtime support

## Subdirectories
- `floatlib/` - Floating-point conversion library (soft-float implementations)
- `numpy/` - NumPy-compatible runtime functions

## For AI Agents
**Purpose**: Provides C/C++ runtime support linked with compiled Codon programs.

**Exception Handling**: Runtime exception support and stack unwinding.

**Numeric Operations**: Extended floating-point conversions, NumPy operations.

**Link-Time**: This code is linked into final Codon executables/libraries.

## Dependencies
- Standard C library
- LLVM runtime support
