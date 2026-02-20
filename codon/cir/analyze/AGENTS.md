# CIR Analysis Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Analysis passes for the Codon intermediate representation. Provides dataflow analysis and module-level analysis infrastructure used by optimization and transformation passes.

## Key Files

- `analysis.h/cpp` - Base class for all IR analyses (analyze::Analysis, analyze::Result)

## Subdirectories

- `dataflow/` - Dataflow analysis passes (CFG, dominator analysis, reaching definitions, capture analysis)
- `module/` - Module-level analysis passes (global variable analysis, side effect analysis)

## For AI Agents

**When working with analyses:**
- All analyses extend analyze::Analysis base class
- Analyses return analyze::Result subclasses containing analysis data
- Analyses are registered with transform::PassManager
- Analyses can depend on other analyses via PassManager
- Results are cached and invalidated when IR changes

**Common Analysis Types:**
- Dataflow analyses (CFG, reaching definitions, dominators)
- Module-level analyses (global variables, side effects)
- Use getAnalysis<T>() on PassManager to access results

**Adding New Analyses:**
1. Create subclass of analyze::Analysis
2. Implement getKey() for unique identification
3. Implement run() to perform analysis
4. Return analyze::Result subclass with data
5. Register with PassManager

## Dependencies

- Depends on: `codon/cir/base.h`, `codon/cir/module.h`, `codon/cir/transform/pass.h`
- Used by: All transformation passes requiring analysis data
