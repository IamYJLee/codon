# stdlib/numpy/linalg

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Linear algebra operations including matrix decompositions, solving linear systems, eigenvalues, and matrix operations.

## Key Files
- `__init__.codon` - Linear algebra module interface
- `linalg.codon` - Core linear algebra routines (solve, inv, det, eig, svd)
- `blas.codon` - BLAS (Basic Linear Algebra Subprograms) bindings

## For AI Agents
- BLAS bindings provide low-level matrix operations (GEMM, GEMV, etc.)
- `linalg.codon` implements high-level operations using BLAS/LAPACK
- Supports matrix decompositions: LU, QR, Cholesky, SVD, eigendecomposition
- Solving linear systems: Ax=b via various methods
- Matrix properties: determinant, inverse, norm, rank
- Both CPU and potentially GPU-accelerated paths

## Dependencies
- BLAS library (OpenBLAS, MKL, etc.)
- LAPACK library for advanced operations
- `../ndarray.codon` - Array type
- C/Fortran numerical libraries
