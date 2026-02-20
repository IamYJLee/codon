# stdlib/internal/types/collections

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Core collection type implementations. Provides Python-compatible list, dict, set, and tuple with optimized internal representations.

## Key Files
- `list.codon` - Dynamic array/list implementation
- `dict.codon` - Hash table dictionary implementation
- `set.codon` - Hash set implementation
- `tuple.codon` - Tuple-specific collection operations

## For AI Agents
- `list.codon`: Dynamic array with amortized O(1) append, uses `Array[T]` internally
- `dict.codon`: Hash table using khash.codon, O(1) average lookup/insert
- `set.codon`: Hash set also based on khash, O(1) membership testing
- All collections support iteration protocols
- Dict/set handle hash collisions via chaining
- List has capacity management with growth factor (typically 1.5x-2x)
- Collections are generic over element type T
- Memory managed by GC, but internal arrays may use manual allocation

## Dependencies
- `internal/khash.codon` - Hash table implementation
- `internal/types/array.codon` - Fixed array backend
- `internal/gc.codon` - Memory management
- Type system for generics
