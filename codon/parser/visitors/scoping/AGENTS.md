# Scope Resolution Visitor

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
AST visitor for scope resolution and name binding. First semantic analysis pass.

## Key Files
- `scoping.h/cpp` - Scope resolution implementation

## For AI Agents
**Purpose**: Resolves variable/function names to declarations. Builds symbol tables and scope chains.

**Pass Position**: First semantic pass, runs before type checking.

**Output**: Annotates AST with scope information for subsequent passes.

**Key Operations**: Name resolution, import handling, scope chain construction.

## Dependencies
- `../visitor.h` - Base visitor
- `../../cache.h` - Symbol cache
- `../../ast/` - AST nodes
