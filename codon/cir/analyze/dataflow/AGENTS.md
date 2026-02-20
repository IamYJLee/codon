# Dataflow Analysis Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Dataflow analysis passes for control flow graph construction, dominator tree analysis, reaching definitions, and variable capture analysis.

## Key Files

- `capture.h/cpp` - Capture analysis for identifying variables captured by closures/coroutines
- `cfg.h/cpp` - Control flow graph (CFG) construction and representation
- `dominator.h/cpp` - Dominator tree analysis for control flow optimization
- `reaching.h/cpp` - Reaching definitions analysis for dataflow optimization

## For AI Agents

**Dataflow Analyses:**
- `CFGAnalysis` - Builds control flow graphs from function bodies
- `DominatorAnalysis` - Computes dominator trees and dominance frontiers
- `ReachingDefinitionAnalysis` - Tracks which definitions reach each program point
- `CaptureAnalysis` - Identifies variables captured across scope boundaries

**Usage Patterns:**
```cpp
auto *cfg = manager->getAnalysis<CFGAnalysis>(func);
auto *dom = manager->getAnalysis<DominatorAnalysis>(func);
auto *reaching = manager->getAnalysis<ReachingDefinitionAnalysis>(func);
```

**Common Use Cases:**
- CFG required by most optimizations
- Dominator analysis for SSA construction and loop analysis
- Reaching definitions for constant propagation and dead code elimination
- Capture analysis for closure optimization and coroutine lowering

## Dependencies

- Depends on: `codon/cir/func.h`, `codon/cir/flow.h`, `codon/cir/analyze/analysis.h`
- Used by: Transform passes in `codon/cir/transform/`
