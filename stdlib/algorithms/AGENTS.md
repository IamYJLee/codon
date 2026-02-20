# stdlib/algorithms

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Sorting and string algorithm implementations for the Codon standard library. Provides multiple sorting algorithms optimized for different use cases and input patterns.

## Key Files
- `heapsort.codon` - Heap sort implementation with key function support
- `quicksort.codon` (qsort.codon) - Quick sort algorithm
- `timsort.codon` - Adaptive merge sort (Python-style Timsort)
- `pdqsort.codon` - Pattern-defeating quicksort (C++ style)
- `insertionsort.codon` - Simple insertion sort for small arrays
- `strings.codon` - String-specific algorithms and utilities

## For AI Agents
- Each sorting algorithm supports custom key functions via `CallableTrait`
- Algorithms operate on `Array[T]` with begin/end indices
- Selection of algorithm typically based on input size and characteristics
- Heapsort: O(n log n) guaranteed, good for worst-case scenarios
- Timsort: Adaptive, excellent for partially sorted data
- Pdqsort: Modern quicksort variant with fallbacks to avoid O(n²)
- Insertion sort: Used for small subarrays (typically < 16 elements)

## Dependencies
- `internal/types/array.codon` - Array type implementation
- `internal/sort.codon` - Sort infrastructure and utilities
