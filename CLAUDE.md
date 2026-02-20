# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What is Codon?

Codon is a high-performance, ahead-of-time Python compiler that compiles Python code to native machine code (10-100x+ speedups). It uses a custom LLVM fork (LLVM 20) and features its own intermediate representation (CIR) that sits between the AST and LLVM IR, enabling Python-specific optimizations impossible at the LLVM level.

## Build Commands

**Prerequisites:** `libgfortran` must be available. Set `CODON_SYSTEM_LIBRARIES` to its parent directory:
```bash
# macOS (brew install gcc):
export CODON_SYSTEM_LIBRARIES=/opt/homebrew/opt/gcc/lib/gcc/current
# Linux:
export CODON_SYSTEM_LIBRARIES=/usr/lib/x86_64-linux-gnu
```

**Build LLVM (one-time):**
```bash
git clone --depth 1 -b codon https://github.com/exaloop/llvm-project
cmake -S llvm-project/llvm -B llvm-project/build \
    -DCMAKE_BUILD_TYPE=Release -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_ENABLE_RTTI=ON -DLLVM_ENABLE_ZLIB=OFF -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_ENABLE_PROJECTS="openmp" -DLLVM_TARGETS_TO_BUILD=all
cmake --build llvm-project/build
cmake --install llvm-project/build --prefix=llvm-project/install
```

**Build Codon:**
```bash
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_DIR=$(llvm-config --cmakedir) \
    -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build --config Release
cmake --install build --prefix=install
```

Produces: `install/bin/codon` executable, `build/codon_test` test binary, and shared libraries (`libcodonc`, `libcodonrt`, `libomp`) in `install/lib/codon/`.

## Testing

**Run all tests:**
```bash
./build/codon_test
```

**Run a specific test suite** (using gtest filter):
```bash
./build/codon_test --gtest_filter='CoreTests*'
./build/codon_test --gtest_filter='TypeTests*'
./build/codon_test --gtest_filter='StdlibTests*'
./build/codon_test --gtest_filter='OptTests*'
./build/codon_test --gtest_filter='NumPyTests*'
./build/codon_test --gtest_filter='GpuTests*'
```

**Run a single test file** (test names derived from file paths with `/` → `_` and `.` → `_`):
```bash
./build/codon_test --gtest_filter='*core_helloworld_codon*'
./build/codon_test --gtest_filter='*stdlib_str_test_codon*'
```

**Writing tests:** Tests are Codon programs in `test/`. New test files must be registered in `test/main.cpp`. Two styles:
- **Preferred (new style):** Use `@test` decorator with `assert` statements
- **Old style:** `print(...)  # EXPECT: expected_output` for output matching

## Code Style

- **C++20** standard, compiled with Clang
- **ClangFormat** with LLVM style, 88 column limit (`.clang-format`)
- **Pre-commit hooks:** clang-format + trailing whitespace (`.pre-commit-config.yaml`)
- **Naming:** PascalCase for types/classes, camelCase for methods/functions
- **Custom RTTI:** LLVM-style with `nodeId()`, `is<T>()`, `as<T>()`, `cast<T>()` — not C++ `typeid`

## Development Workflow

All development happens on the `develop` branch. Releases are version-bumped and merged to `master` with `vX.Y.Z` tags. PRs should target `develop`.

## Architecture Overview

### Compilation Pipeline

```
Source (.codon/.py) → Parser (PEG) → AST → Type Checker → CIR → CIR Passes → LLVM IR → Machine Code
```

### Key Directories

| Directory | Purpose |
|---|---|
| `codon/parser/` | Frontend: PEG parser, AST, type checker, visitors |
| `codon/parser/peg/` | PEG grammar (`grammar.peg`, `openmp.peg`) — compiled to C++ via `peg2cpp` |
| `codon/parser/visitors/typecheck/` | Hindley-Milner type system with monomorphization |
| `codon/cir/` | Codon Intermediate Representation (typed, higher-level than LLVM IR) |
| `codon/cir/transform/` | Optimization passes: cleanup, folding, lowering, parallel, numpy, pythonic |
| `codon/cir/analyze/` | Program analysis: CFG, dominator, reaching defs, capture/escape |
| `codon/cir/llvm/` | LLVM IR lowering, codegen, GPU (PTX) support |
| `codon/compiler/` | Compiler orchestration, JIT support, plugin management |
| `codon/runtime/` | Runtime library (GC, exceptions, I/O, NumPy runtime) |
| `codon/dsl/` | Plugin/DSL system for extending the compiler |
| `stdlib/` | Standard library written in Codon itself (embedded as binary resource) |
| `stdlib/internal/types/` | Core builtin types (list, dict, set, tuple, str, int, etc.) |
| `stdlib/numpy/` | Native NumPy reimplementation with fusion/vectorization |

### Why CIR Exists

LLVM IR loses high-level semantic meaning. CIR retains Python semantics enabling optimizations like:
- Dictionary access pattern fusion (`d[k] = d.get(k, 0) + 1` → single lookup)
- NumPy operator fusion and vectorization
- Python-specific string/list/I/O optimizations

CIR node hierarchy: `Module` → `Type`, `Var`, `Value` (which includes `Const`, `Instr`, `Flow`).

### Type System

Monomorphization-based: generic functions are specialized per argument type combination. The type checker in `codon/parser/visitors/typecheck/` handles resolution across multiple visitor files organized by AST node category (basic, call, class, collections, loops, import).

### Visitor Pattern

Extensively used for both AST and CIR traversal. Custom CIR passes inherit from `ir::transform::Pass` or `ir::transform::OperatorPass` and are registered with the pass manager.

### Build Artifacts from Grammar

PEG grammars (`grammar.peg`, `openmp.peg`) are compiled to `codon_rules.cpp` and `omp_rules.cpp` via the `peg2cpp` utility built as part of the build process.

## Key Environment Variables

| Variable | Purpose |
|---|---|
| `CODON_SYSTEM_LIBRARIES` | Path to libgfortran (required for build) |
| `CODON_PYTHON` | CPython shared library path (for Python interop) |
| `CODON_DEBUG` | Debug logging flags |
| `CODON_DIR` | Codon installation path |

## Dependencies (managed via CPM.cmake)

Major: LLVM 20 (exaloop fork with RTTI + OpenMP), Boehm GC, Google Test, fmt, peglib, RE2, toml++, zlib-ng, libbacktrace, highway (SIMD), fast_float.
