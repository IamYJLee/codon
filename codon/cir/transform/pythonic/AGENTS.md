# Pythonic Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Python semantics preservation and optimization passes for dict, list, string, generator, and I/O operations to maintain Python behavior while enabling optimizations.

## Key Files

- `dict.h/cpp` - Dictionary operation optimizations and specialization
- `list.h/cpp` - List operation optimizations and specialization
- `str.h/cpp` - String operation optimizations and specialization
- `generator.h/cpp` - Generator and iterator optimizations
- `io.h/cpp` - I/O operation optimizations (print, file operations)

## For AI Agents

**Pythonic Optimizations:**
- **Dict**: Hash table specialization, comprehension optimization
- **List**: Dynamic array optimization, comprehension fusion
- **String**: String interning, concatenation optimization
- **Generator**: Lazy evaluation optimization, generator fusion
- **I/O**: Buffering, format string optimization

**Python Semantics:**
- Preserve Python's dynamic typing behavior where required
- Maintain reference semantics for mutable types
- Preserve iteration order for dicts (Python 3.7+)
- Handle edge cases (empty containers, None values)

**Example Optimizations:**
```python
# List comprehension fusion
[f(x) for x in [g(y) for y in data]]
→ [f(g(y)) for y in data]

# String concatenation
s = ""
for x in items:
    s += str(x)
→ s = "".join(str(x) for x in items)

# Generator fusion
sum(x*2 for x in range(n) if x % 2 == 0)
→ specialized_sum_kernel(n)
```

**Specialization:**
- Type-specialized implementations for common patterns
- Eliminate dynamic dispatch where types are known
- Inline builtin operations

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/types/` (Python types)
- Used by: Python compatibility and performance optimization
