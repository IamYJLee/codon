# stdlib/numpy

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
NumPy-compatible array library providing multidimensional arrays, mathematical operations, linear algebra, FFT, and random number generation. Core numerical computing infrastructure for Codon.

## Key Files
- `__init__.codon` - Main NumPy interface and exports
- `ndarray.codon` - N-dimensional array implementation
- `ndmath.codon` - Mathematical operations on arrays
- `ndgpu.codon` - GPU-accelerated array operations
- `operators.codon` - Array arithmetic operators
- `reductions.codon` - Reduction operations (sum, mean, etc.)
- `ufunc.codon` - Universal functions
- `dtype.codon` - Data type system
- `indexing.codon` - Array indexing and slicing
- `sorting.codon` - Array sorting algorithms
- `statistics.codon` - Statistical functions
- `routines.codon` - Common array routines
- `functional.codon` - Functional programming operations
- `fusion.codon` - Array operation fusion for optimization
- `format.codon` - Array formatting and printing
- `npio.codon` - Array I/O operations
- `interp.codon` - Interpolation functions
- `misc.codon` - Miscellaneous utilities
- `window.codon` - Window functions
- `emath.codon` - Extended math operations
- `zmath.codon` - Complex number math
- `const.codon` - Mathematical constants
- `dragon4.codon` - Float formatting algorithm
- `util.codon` - Utility functions
- `npdatetime.codon` - Datetime support
- `linalg_sym.codon` - Symbolic linear algebra
- `pybridge.codon` - Python NumPy bridge

## Subdirectories
- `fft/` - Fast Fourier Transform implementations
- `lib/` - NumPy library functions
- `linalg/` - Linear algebra operations
- `random/` - Random number generation

## For AI Agents
- Core array type is `ndarray[T]` with shape, strides, and data pointer
- Supports broadcasting following NumPy semantics
- GPU operations via `ndgpu.codon` using CUDA/ROCm
- Operation fusion optimizes chains of array operations
- Universal functions (ufuncs) vectorize operations
- Dtype system supports multiple numeric types (int8-64, float32/64, complex)
- Indexing supports integer, slice, boolean, and fancy indexing
- Many operations have both CPU and GPU implementations
- Memory layout can be C-contiguous or Fortran-contiguous

## Dependencies
- `internal/types/` - Built-in types
- BLAS/LAPACK libraries for linear algebra
- FFTW or similar for FFT operations
- GPU runtime (CUDA/ROCm) for GPU operations
- C math library for mathematical functions
