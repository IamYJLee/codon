# Parallelization Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Parallelization passes for generating OpenMP parallel code and optimizing parallel scheduling strategies.

## Key Files

- `openmp.h/cpp` - OpenMP code generation for parallel constructs
- `schedule.h/cpp` - Parallel loop scheduling optimization (static, dynamic, guided)

## For AI Agents

**Parallelization Features:**
- **OpenMP Integration**: Generates OpenMP directives for parallel regions
- **Loop Parallelization**: Converts sequential loops to parallel execution
- **Scheduling**: Optimizes work distribution strategies
- **Synchronization**: Manages parallel region boundaries and data sharing

**Scheduling Strategies:**
- **Static**: Fixed chunk distribution (low overhead, predictable load)
- **Dynamic**: Runtime chunk distribution (better load balancing)
- **Guided**: Adaptive chunk sizing (compromise between overhead and balance)

**Parallel Constructs:**
```python
@par(schedule='dynamic')
for i in range(n):
    work(i)  # Parallelized with OpenMP
```

**Safety Considerations:**
- Data race detection and prevention
- Reduction variable handling
- Private/shared variable classification
- Critical section identification

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/analyze/` (for dependency analysis)
- Used by: High-performance computing and multi-core optimization
