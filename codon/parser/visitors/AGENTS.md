# AST Visitors

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Visitor pattern implementations for AST traversal and transformation. Semantic analysis passes that transform or analyze AST.

## Key Files
- `visitor.h/cpp` - Base visitor infrastructure

## Subdirectories
- `doc/` - Documentation generation visitor
- `format/` - Code formatting visitor
- `scoping/` - Scope resolution visitor
- `translate/` - AST-to-CIR translation visitor
- `typecheck/` - Type checking visitor

## For AI Agents
**Visitor Pattern**: All visitors inherit from `ASTVisitor` with `visit()` methods for each node type.

**Pass Order**: Typical pipeline: scoping → typecheck → translate

**Transformation**: Visitors can modify AST in-place or create new nodes.

**Context**: Each visitor maintains state via context objects.

## Dependencies
- `../ast/` - AST nodes being visited
- `../../cir/` - Target IR for translation
