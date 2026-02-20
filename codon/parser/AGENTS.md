# Parser Front-End

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Parser front-end for Codon compiler. Handles lexical analysis, syntax parsing, AST construction, and semantic analysis passes before CIR translation.

## Key Files
- `cache.h/cpp` - Import cache and compilation context management
- `common.h/cpp` - Common parser utilities, source info, error reporting
- `ast.h` - AST umbrella header
- `ctx.h` - Shared context structures
- `match.h/cpp` - Pattern matching utilities

## Subdirectories
- `ast/` - AST node definitions (expressions, statements, types)
- `peg/` - PEG parser implementation and grammar definitions
- `visitors/` - AST visitor implementations (formatting, type checking, translation)

## For AI Agents
**Entry Points**: Cache manages compilation context. PEG parser (`peg/`) generates AST nodes (`ast/`). Visitors (`visitors/`) transform AST through semantic passes.

**Workflow**: Source → PEG parser → AST → Visitors (scoping → typecheck → translate) → CIR

**Key Concepts**: AST nodes use visitor pattern. Cache maintains type info and import state. Semantic passes are sequential and order-dependent.

## Dependencies
- `../cir/` - Target IR for translation
- `../util/` - Common utilities
