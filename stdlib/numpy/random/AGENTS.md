# stdlib/numpy/random

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Pseudo-random number generation with multiple PRNG algorithms and probability distributions.

## Key Files
- `__init__.codon` - Random module interface
- `bitgen.codon` - Bit generator base infrastructure
- `mt19937.codon` - Mersenne Twister 19937 PRNG
- `pcg64.codon` - PCG64 PRNG (fast, statistical quality)
- `philox.codon` - Philox PRNG (counter-based, GPU-friendly)
- `sfc64.codon` - SFC64 PRNG (simple, fast counting)
- `splitmix64.codon` - SplitMix64 PRNG (used for seeding)
- `seed.codon` - Seeding infrastructure
- `ziggurat.codon` - Ziggurat algorithm for normal/exponential distributions
- `logfactorial.codon` - Log factorial tables for distributions

## For AI Agents
- Multiple PRNG algorithms with different trade-offs:
  - MT19937: Traditional, well-tested, large state
  - PCG64: Modern, fast, good statistical properties
  - Philox: Counter-based, excellent for parallel/GPU
  - SFC64: Very fast, small state
- Ziggurat method for efficient sampling from normal/exponential distributions
- Support for many distributions: uniform, normal, binomial, poisson, etc.
- Seedable for reproducibility
- Compatible with NumPy's random API

## Dependencies
- `../ndarray.codon` - For array-based random generation
- `../dtype.codon` - Numeric types
- Bit manipulation operations
