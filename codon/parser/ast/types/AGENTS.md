# AST Type System

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Type system implementation for Codon AST. Handles type inference, unification, and representation during semantic analysis.

## Key Files
- `type.h/cpp` - Base type class and type hierarchy
- `class.h/cpp` - Class/record types
- `function.h/cpp` - Function types and signatures
- `link.h/cpp` - Type variable linking and unification
- `static.h/cpp` - Static/compile-time types
- `union.h/cpp` - Union types
- `traits.h/cpp` - Type traits and constraints

## For AI Agents
**Type Representation**: Types are hierarchical with base `Type` class. Types can be generic with type parameters.

**Unification**: Type linking handles type variable unification during type checking.

**Traits**: Type constraints enforce interface requirements (e.g., `CallableTrait`).

**Generics**: Support for generic types with parameter substitution.

## Dependencies
- `../node.h` - Base AST node
- `../../cache.h` - Type cache and context
