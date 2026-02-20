# stdlib/numpy/lib

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
NumPy library utility functions including set operations and array manipulation.

## Key Files
- `__init__.codon` - Library module interface
- `arraysetops.codon` - Set operations on arrays (unique, intersect, union)
- `stride_tricks.codon` - Stride manipulation for advanced indexing

## For AI Agents
- `arraysetops.codon`: Array-as-set operations (unique values, set intersection/union)
- `stride_tricks.codon`: Manipulate array strides for sliding windows, broadcasting tricks
- Stride tricks enable zero-copy views of data with different shapes

## Dependencies
- `../ndarray.codon` - Array type
- `../sorting.codon` - For unique/set operations
