# stdlib/internal

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Core internal runtime support and type system infrastructure. Contains built-in functions, C bindings, garbage collection, GPU support, and fundamental language features. This is the foundation upon which all Codon code runs.

## Key Files
- `__init__.codon` - Internal module initialization
- `builtin.codon` - Built-in functions (print, id, object, etc.)
- `core.codon` - Core runtime infrastructure
- `attributes.codon` - Function attributes (pure, commutative, etc.)
- `gc.codon` - Garbage collector interface and controls
- `gpu.codon` - GPU runtime support and kernel launching
- `c_stubs.codon` - C library bindings and foreign function interface
- `file.codon` - File I/O operations
- `format.codon` - String formatting infrastructure
- `python.codon` - Python interop layer
- `dlopen.codon` - Dynamic library loading
- `khash.codon` - Hash table implementation (used by dict/set)
- `sort.codon` - Sorting infrastructure
- `str.codon` - String manipulation utilities
- `static.codon` - Static/compile-time utilities
- `pynumerics.codon` - Python-compatible numeric operations
- `internal.codon` - Internal utilities and helpers

## Subdirectories
- `types/` - Built-in type implementations (int, str, list, dict, etc.)

## For AI Agents
- This is compiler-generated and internal code - modify with extreme caution
- `builtin.codon` defines fundamental operations like `print()`, `id()`, `object`
- `gc.codon` controls garbage collection behavior
- `gpu.codon` provides `@gpu.kernel` and GPU memory management
- `c_stubs.codon` enables calling C libraries via `_C` namespace
- `khash.codon` is the hash table backbone for dict/set
- Many functions use `@llvm` decorator for direct LLVM IR generation
- Python interop happens through `python.codon` using cpython C API

## Dependencies
- LLVM code generation (many functions emit raw LLVM IR)
- C runtime libraries
- Garbage collector (GC)
- GPU runtime (CUDA/ROCm for GPU support)
