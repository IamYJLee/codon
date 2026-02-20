# DSL Plugin Support

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Domain-Specific Language plugin system. Allows extending Codon with custom language features.

## Key Files
- `dsl.h` - DSL plugin interface definition
- `plugins.h/cpp` - Plugin manager and loading infrastructure

## For AI Agents
**Purpose**: Plugin architecture for extending Codon compiler with custom syntax, types, or transformations.

**Loading**: Plugins are dynamically loaded at compile-time.

**Extension Points**: Plugins can provide custom AST nodes, CIR nodes, and transformations.

**See Also**: `../cir/dsl/` contains CIR-level DSL node support.

## Dependencies
- `../parser/` - AST for plugin extensions
- `../cir/` - CIR for plugin code generation
