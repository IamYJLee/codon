# AST Node Definitions

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Abstract syntax tree node definitions for Codon language. Defines expression, statement, and type hierarchy used throughout parser and semantic analysis.

## Key Files
- `node.h` - Base AST node class
- `expr.h/cpp` - Expression nodes (literals, operators, calls, etc.)
- `stmt.h/cpp` - Statement nodes (assignments, loops, functions, classes, etc.)
- `attr.h/cpp` - AST attributes and metadata
- `error.h` - AST error handling
- `types.h` - Type system umbrella header

## Subdirectories
- `types/` - Type system implementation (classes, functions, unions, traits)

## For AI Agents
**Node Hierarchy**: `ASTNode` → `Expr`/`Stmt`. All nodes support visitor pattern via `accept()`.

**Cloning**: Nodes are cloneable for AST transformations. Use `clone()` for deep copies.

**Serialization**: Nodes support serialization via SERIALIZE macros.

**Type Annotations**: Expression nodes carry `type` field populated during type checking.

## Dependencies
- `../common.h` - Source info and utilities
- `types/` - Type system
- `../../util/serialize.h` - Serialization support
