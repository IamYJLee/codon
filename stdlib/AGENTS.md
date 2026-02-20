<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# stdlib - Codon Standard Library

## Purpose

The stdlib directory contains the Codon implementation of Python standard library modules, plus Codon-specific extensions. This includes:

- Core type implementations (int, str, list, dict, etc.)
- Python standard library modules (math, collections, itertools, etc.)
- NumPy compatibility layer for numerical computing
- Internal type system and runtime support
- Algorithm implementations for sorting and searching
- System-level APIs (os, asyncio, etc.)

This is the foundation that Codon programs depend on.

## Key Files

| File | Description |
|------|-------------|
| `math.codon` | Mathematical functions and constants (factorial, sqrt, sin, cos, etc.) |
| `collections.codon` | Data structure: deque with maxlen support |
| `itertools.codon` | Iterator tools (chain, cycle, repeat, combinations, etc.) |
| `functools.codon` | Function tools (reduce, partial, wraps) |
| `heapq.codon` | Heap queue operations |
| `bisect.codon` | Binary search operations |
| `copy.codon` | Object copying (shallow and deep copy) |
| `datetime.codon` | Date and time types and operations |
| `asyncio.codon` | Async/await support |
| `cmath.codon` | Complex number math |
| `gzip.codon` | Gzip compression |
| `bz2.codon` | Bz2 compression |
| `getopt.codon` | Command-line argument parsing |
| `gpu.codon` | GPU-specific functionality |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `algorithms/` | Sorting algorithms: heapsort, insertion sort, quicksort, timsort, pdqsort; string algorithms |
| `codon/` | Codon-specific extensions (static type utilities) |
| `internal/` | Core type system and runtime implementation |
| `internal/types/` | Built-in type implementations (int, float, str, bool, etc.) |
| `internal/types/collections/` | Collection type implementations (list, dict, set, tuple) |
| `numpy/` | NumPy compatibility layer with multi-dimensional array support |
| `numpy/fft/` | Fast Fourier Transform implementations |
| `numpy/lib/` | NumPy utility functions |
| `numpy/linalg/` | Linear algebra operations (using BLAS) |
| `numpy/random/` | Random number generation with multiple RNG algorithms |
| `os/` | OS-level APIs |

## For AI Agents

### Working In This Directory

- **Type Implementations**: Core types are in `internal/types/` and `internal/types/collections/`. These implement Python semantics in Codon.
- **Library Modules**: Standard library modules are at the root level or in subpackages (numpy, os, etc.).
- **Internal Infrastructure**: The `internal/` directory contains runtime support that user code doesn't directly import.
- **NumPy Compatibility**: The `numpy/` directory provides NumPy-like APIs compiled to native code for performance.

### Testing Requirements

- Codon standard library modules are tested as part of the broader Codon compiler test suite
- Type implementations in `internal/types/` have tests in `internal/__init_test__.codon`
- Standard library functions should maintain Python semantic compatibility where feasible
- Numeric operations should match Python behavior for edge cases (inf, nan, zero, negative values)
- Collection operations should support iteration, indexing, and slicing consistently

### Common Patterns

**Type Decorators**:
- `@tuple` - Define tuple/immutable types
- `@dataclass` - Define data classes with fields
- `@__internal__` - Mark types as internal compiler infrastructure
- `@llvm` - Mark functions implemented directly in LLVM IR

**Function Decorators**:
- `@pure` - Mark as pure/side-effect-free
- `@llvm` - Implement function in LLVM IR

**Generic Types**: Use `[T]` syntax for generic type parameters. Example: `Array[T]`, `List[T]`, `Dict[K, V]`

**Error Handling**: Use `raise` statements with Python exception types (ValueError, RuntimeError, etc.)

**Module Structure**: Follow Python module conventions:
- Functions and classes defined at module level
- Import dependencies clearly at the top
- Use copyright header for Exaloop Inc.

## Dependencies

### Internal

- Most modules depend on `internal.static` for compile-time utilities
- Collection types depend on `internal.types.optional` for unwrap and optional handling
- String operations depend on `internal.str` for string manipulation
- Type operations depend on `internal.types` for core type definitions
- RTTI (runtime type information) available via `internal.types.rtti`

### External

- **BLAS/LAPACK**: Linear algebra operations (`numpy/linalg/`) require BLAS libraries
- **FFT**: Pocketfft library for Fourier transforms (`numpy/fft/`)
- **C Standard Library**: Some modules wrap C functions via `internal.c_stubs` and `internal.dlopen`
- **Python C API**: Bridge code in `internal.python` for Python interoperability
- **OS APIs**: System-level functionality via OS-specific modules

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
