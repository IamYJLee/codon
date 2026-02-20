# AST-to-CIR Translation Visitor

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
AST visitor for translating type-checked AST to Codon IR (CIR). Final semantic pass.

## Key Files
- `translate.h/cpp` - Translation visitor implementation
- `translate_ctx.h/cpp` - Translation context management

## For AI Agents
**Purpose**: Converts high-level AST to mid-level CIR after type checking.

**Pass Position**: Final semantic pass, runs after type checking.

**Output**: CIR module ready for optimization and code generation.

**Context**: `translate_ctx` manages IR construction state.

**Key Operations**: Type lowering, control flow translation, function/class instantiation.

## Dependencies
- `../visitor.h` - Base visitor
- `../typecheck/` - Type information
- `../../../cir/` - Target IR
- `../../cache.h` - Compilation context
