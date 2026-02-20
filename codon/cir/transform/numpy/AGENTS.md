# NumPy Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

NumPy array operation optimizations including array expression fusion, indexing optimization, and forward mode automatic differentiation.

## Key Files

- `numpy.h/cpp` - Main NumPy optimization infrastructure
- `expr.cpp` - Array expression fusion and optimization
- `forward.cpp` - Forward mode automatic differentiation for NumPy operations
- `indexing.h/cpp` - Array indexing optimization and bounds check elimination

## For AI Agents

**NumPy Optimizations:**
- **Expression Fusion**: Combines multiple array operations into single kernels
- **Indexing**: Optimizes array access patterns and eliminates redundant bounds checks
- **Forward AD**: Automatic differentiation for gradient computation
- **Vectorization**: Generates efficient SIMD code for array operations

**Example Transformations:**
```python
# Before: Multiple passes over arrays
result = (a + b) * (c - d)

# After fusion: Single pass
result = fused_kernel(a, b, c, d)
```

**Optimization Strategies:**
- Loop fusion for sequential array operations
- Kernel fusion for element-wise operations
- Memory access optimization
- Bounds check elimination with static analysis

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/types/` (array types)
- Used by: Scientific computing and numerical code optimization
