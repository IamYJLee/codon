# CIR Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

IR transformation and optimization passes for the Codon intermediate representation. Includes cleanup passes, constant folding, IR lowering, NumPy optimizations, parallelization, and Python semantic transforms.

## Key Files

- `pass.h/cpp` - Base class for all transformation passes (transform::Pass)
- `manager.h/cpp` - Pass manager for orchestrating transformation pipeline (transform::PassManager)
- `rewrite.h` - Rewrite rule infrastructure for pattern-based transformations

## Subdirectories

- `cleanup/` - Cleanup and canonicalization passes (dead code elimination, global demotion)
- `folding/` - Constant folding and propagation passes
- `lowering/` - High-level IR lowering (async/await, pipelines, imperative constructs)
- `numpy/` - NumPy array operation optimizations
- `parallel/` - Parallelization passes (OpenMP, scheduling)
- `pythonic/` - Python semantics preservation (dict, list, str, generator, I/O)

## For AI Agents

**Transform Pass Infrastructure:**
- All passes extend transform::Pass base class
- PassManager orchestrates pass execution and analysis dependencies
- Passes can invalidate analyses and request re-analysis
- Supports pass dependencies and ordering constraints

**Pass Categories:**
1. **Cleanup**: Remove dead code, canonicalize IR structure
2. **Folding**: Constant folding and propagation for optimization
3. **Lowering**: Lower high-level constructs to simpler IR
4. **Specialization**: NumPy, parallelization, Python semantics
5. **Optimization**: General-purpose optimizations

**Adding New Passes:**
1. Extend transform::Pass in appropriate subdirectory
2. Implement getKey() for unique identification
3. Implement run() for transformation logic
4. Register with PassManager in compilation pipeline
5. Declare analysis dependencies if needed

**Pass Execution:**
- Passes run in pipeline order configured by PassManager
- Analysis results cached and reused across passes
- Invalidation triggers re-analysis when IR modified

## Dependencies

- Depends on: `codon/cir/` (all IR nodes), `codon/cir/analyze/` (analysis results)
- Used by: `codon/compiler/` compilation pipeline
