# Module Analysis Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Module-level analysis passes for global variable tracking and side effect analysis across compilation units.

## Key Files

- `global_vars.h/cpp` - Global variable analysis for tracking module-level variables
- `side_effect.h/cpp` - Side effect analysis for function purity and effect tracking

## For AI Agents

**Module Analyses:**
- `GlobalVarsAnalysis` - Identifies and tracks global variables across module
- `SideEffectAnalysis` - Determines function side effects (I/O, memory mutations, exceptions)

**Usage:**
- Side effect analysis enables optimizations like function inlining and dead code elimination
- Global variable analysis supports global-to-local promotion and escape analysis

**Effect Categories:**
- Pure functions (no side effects)
- Memory effects (heap allocation, mutations)
- I/O effects (file operations, network)
- Control flow effects (exceptions, non-termination)

## Dependencies

- Depends on: `codon/cir/module.h`, `codon/cir/analyze/analysis.h`
- Used by: Optimization passes, inlining decisions, parallelization passes
