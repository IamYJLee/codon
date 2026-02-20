# Constant Folding Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Constant folding and propagation optimization passes for evaluating compile-time constant expressions and propagating constant values through the IR.

## Key Files

- `folding.h/cpp` - Main constant folding infrastructure
- `const_fold.h/cpp` - Constant folding pass (evaluate constant expressions)
- `const_prop.h/cpp` - Constant propagation pass (propagate known constant values)
- `rule.h` - Folding rule definitions and pattern matching

## For AI Agents

**Constant Optimization:**
- **Constant Folding**: Evaluates operations on constants at compile time
- **Constant Propagation**: Replaces variable uses with their constant values
- **Rule-Based**: Uses pattern matching rules for folding decisions

**Examples:**
```
x = 2 + 3          → x = 5
y = x * 2  (x=5)   → y = 10
if (true) {...}    → {...}  (eliminate branch)
```

**Optimization Benefits:**
- Reduces runtime computation
- Enables further optimizations (dead code elimination)
- Simplifies control flow
- Smaller generated code

**Implementation:**
- Rule-based pattern matching for foldable expressions
- Dataflow analysis for constant value tracking
- Safe handling of side effects and undefined behavior

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/analyze/dataflow/reaching.h`
- Used by: Optimization pipeline (run early and repeatedly)
