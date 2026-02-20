# stdlib/internal/types

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Implementations of Codon's built-in types. Defines operators, methods, and semantics for primitive and fundamental composite types.

## Key Files
- `int.codon` - Integer type with arithmetic operators
- `float.codon` - Floating-point type
- `bool.codon` - Boolean type
- `str.codon` - String type with methods
- `byte.codon` - Byte type
- `complex.codon` - Complex number type
- `ptr.codon` - Raw pointer type
- `array.codon` - Fixed-size array type
- `tuple.codon` - Tuple type implementation
- `slice.codon` - Slice type (for indexing)
- `range.codon` - Range type (for iteration)
- `optional.codon` - Optional/None type
- `function.codon` - Function type and decorators
- `generator.codon` - Generator/yield implementation
- `error.codon` - Exception types
- `ellipsis.codon` - Ellipsis type (...)
- `any.codon` - Dynamic any type
- `union.codon` - Union type support
- `type.codon` - Type introspection
- `rtti.codon` - Runtime type information
- `import_.codon` - Import mechanism support
- `intn.codon` - Fixed-width integers (i8, i16, i32, u64, etc.)
- `strbuf.codon` - String buffer for efficient string building

## Subdirectories
- `collections/` - Collection types (list, dict, set, tuple)

## For AI Agents
- Most types use `@extend class` to add methods to built-in types
- Heavy use of `@llvm` decorator for performance-critical operations
- Operators defined via magic methods (`__add__`, `__mul__`, etc.)
- Many functions marked with `@pure`, `@commutative`, `@associative` attributes
- Integer operations handle both signed and unsigned arithmetic
- String operations optimized for performance with C-level implementations
- Pointers provide low-level memory access similar to C
- Generators use coroutine infrastructure for lazy evaluation

## Dependencies
- `internal/attributes.codon` - Function attributes
- `internal/c_stubs.codon` - C library bindings
- `internal/gc.codon` - Memory management
- LLVM IR generation for operators
