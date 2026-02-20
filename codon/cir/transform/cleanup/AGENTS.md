# Cleanup Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

IR cleanup and canonicalization passes for removing dead code, canonicalizing IR structure, demoting unnecessary global variables, and replacing IR patterns.

## Key Files

- `canonical.h/cpp` - IR canonicalization pass (normalize structure and patterns)
- `dead_code.h/cpp` - Dead code elimination pass
- `global_demote.h/cpp` - Global variable demotion to local scope when possible
- `replacer.h/cpp` - Pattern replacement utilities for IR transformations

## For AI Agents

**Cleanup Passes:**
- **Canonicalization**: Normalizes IR to standard forms for optimization
- **Dead Code Elimination**: Removes unreachable code and unused definitions
- **Global Demotion**: Converts globals to locals when escape analysis permits
- **Replacer**: Utility for systematic IR node replacement

**Usage:**
- Cleanup passes typically run early and late in pipeline
- Enable subsequent optimizations by simplifying IR
- Reduce code size and improve runtime performance

**Common Patterns:**
- Run dead code elimination after inlining/specialization
- Canonicalization before pattern-matching optimizations
- Global demotion after escape analysis

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/analyze/` (for dataflow and escape analysis)
- Used by: Optimization pipeline, code generation preparation
