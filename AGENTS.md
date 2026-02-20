<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Codon

## Purpose
Codon is a high-performance Python compiler that compiles to native machine code via LLVM. It achieves 10-100x speedups over CPython with native multithreading support (no GIL). The codebase implements a full compiler pipeline: PEG parser → AST → Codon IR (CIR) → LLVM IR → native code. Written primarily in C++20 with the standard library in `.codon` files.

## Key Files

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Main CMake build configuration (C++20, LLVM dependency) |
| `README.md` | Project overview, goals, and quick start guide |
| `LICENSE` | Apache 2.0 license |
| `CONTRIBUTING.md` | Contribution guidelines |
| `CODEOWNERS` | Code ownership mapping |
| `mkdocs.yml` | MkDocs documentation site configuration |
| `.clang-format` | C++ code formatting rules |
| `.clang-tidy` | C++ static analysis configuration |
| `.pre-commit-config.yaml` | Pre-commit hooks configuration |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `codon/` | Core compiler implementation in C++ (see `codon/AGENTS.md`) |
| `stdlib/` | Standard library modules in `.codon` files (see `stdlib/AGENTS.md`) |
| `test/` | Comprehensive test suite organized by component (see `test/AGENTS.md`) |
| `docs/` | MkDocs documentation source files (see `docs/AGENTS.md`) |
| `bench/` | Performance benchmarking suite (see `bench/AGENTS.md`) |
| `cmake/` | CMake build system configuration and templates (see `cmake/AGENTS.md`) |
| `scripts/` | Build, deployment, and utility scripts (see `scripts/AGENTS.md`) |
| `jit/` | Python JIT decorator package (see `jit/AGENTS.md`) |
| `jupyter/` | Jupyter kernel integration (see `jupyter/AGENTS.md`) |
| `.github/` | GitHub Actions CI/CD workflows and Docker build configs |

## For AI Agents

### Working In This Directory
- Build system: CMake with LLVM as primary dependency. See `cmake/deps.cmake` for all external deps.
- C++20 standard required. Clang preferred compiler.
- The PEG grammar at `codon/parser/peg/grammar.peg` is compiled to C++ via the `peg2cpp` utility.
- Standard library `.codon` files are embedded into the binary via CMakeRC.

### Build Commands
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Testing Requirements
- Tests are in `test/` and use the `main.cpp` test runner.
- Run with: `cmake --build build --target test` or CTest.
- Test files use `.codon` extension for parser/stdlib tests and `.cpp` for CIR/core tests.

### Compiler Pipeline
```
Source (.py/.codon) → PEG Parser → AST → Visitors (scoping, typecheck, translate)
→ Codon IR (CIR) → Transform Passes → LLVM IR → Native Code
```

### Common Patterns
- Header/source pairs: `name.h` + `name.cpp` throughout the C++ code.
- Visitor pattern extensively used in parser and CIR.
- LLVM-style RTTI with custom node hierarchy in CIR.
- Plugin/DSL system for extending the compiler.

## Dependencies

### External
- **LLVM** - Backend code generation (primary dependency)
- **fmt** - String formatting
- **peglib** - PEG parser library
- **toml++** - TOML configuration parsing
- **semver** - Semantic versioning
- **zlibng** - Compression library
- **bz2** - BZ2 compression
- **liblzma** - LZMA compression
- **backtrace** - Stack trace support
- **re2** - Regular expression engine
- **openmp** - Parallel programming support

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
