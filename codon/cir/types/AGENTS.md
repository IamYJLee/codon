# CIR Types Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Type system implementation for Codon intermediate representation, including primitive types, aggregate types, function types, and type operations.

## Key Files

- `types.h/cpp` - Complete type system implementation for CIR (IntType, FloatType, RecordType, FuncType, etc.)

## For AI Agents

**Type System:**
- **Primitive Types**: int, float, bool, void
- **Aggregate Types**: tuples, records, arrays
- **Function Types**: function signatures with parameter and return types
- **Pointer Types**: references and pointers
- **Generic Types**: type parameters and instantiation
- **Union Types**: sum types for optional values

**Type Operations:**
- Type equality and compatibility checking
- Type unification for generics
- Type lowering to LLVM types
- Type attribute queries (size, alignment)

**Type Hierarchy:**
```
Type (base)
├── IntType (integers of various widths)
├── FloatType (floating point types)
├── BoolType
├── RecordType (structs/classes)
├── RefType (references)
├── FuncType (function signatures)
├── VoidType
└── ... (other type variants)
```

**Usage Patterns:**
- Types are immutable value objects
- Type checking done during CIR construction
- Type lowering happens during LLVM codegen
- Generic type instantiation creates specialized types

## Dependencies

- Depends on: `codon/cir/base.h`, `codon/util/`
- Used by: All CIR components requiring type information
