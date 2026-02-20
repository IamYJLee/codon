<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Performance Benchmarking Suite

## Purpose

Comprehensive benchmark suite for measuring Codon performance against Python, C++, and other languages. Includes standard algorithm benchmarks from Python's pyperformance suite plus domain-specific benchmarks for bioinformatics, data processing, and numerical computing.

## Key Files

| File | Description |
|------|-------------|
| run.sh | Main benchmark orchestration script (compiles and runs all benchmarks) |
| README.md | Benchmark documentation and descriptions of each test |
| codon/binary_trees.codon | Binary tree allocation benchmark (Boehm GC test) |
| codon/chaos.codon | Chaotic double precision computation benchmark |
| codon/fannkuch.codon | Permutation/list reversal benchmark (multithreaded Codon version) |
| codon/float.py | Floating-point arithmetic benchmark from pyperformance |
| codon/go.codon | Go-like concurrency benchmark from pyperformance |
| codon/mandelbrot.codon | Mandelbrot set generation (GPU-optimized Codon version) |
| codon/nbody.py | N-body simulation from pyperformance |
| codon/npbench.codon | NumPy-based benchmarks for array operations |
| codon/primes.codon | Prime number computation (multithreaded dynamic schedule) |
| codon/set_partition.py | Set partition calculation benchmark |
| codon/spectral_norm.py | Spectral normalization benchmark |
| codon/sum.py | Integer summation benchmark |
| codon/taq.py | NYSE TAQ financial data processing benchmark |
| codon/word_count.py | Dictionary-based word counting benchmark |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| codon/ | All benchmark source files (.codon, .py, .cpp) and implementations |

## For AI Agents

### Working In This Directory

- **Benchmark Format**: Mix of .codon (Codon source), .py (Python), and .cpp (C++) implementations
- **Naming Convention**: Benchmark name with appropriate extension (e.g., `chaos.codon`, `chaos.py`)
- **Script Organization**: Main orchestrator is `run.sh` which manages compilation, execution, and timing
- **Data Files**: TAQ and word_count benchmarks reference external data via environment variables

### Benchmark Modifications

- **Codon-Specific Optimization**: Benchmarks can use `@par` decorators for parallelization/GPU (documented in README)
- **Pyperformance Adaptations**: Original Python benchmarks adapted for Codon's type system; marked in README
- **Multi-implementation**: Some benchmarks have parallel .codon, .py, and .cpp versions for comparison

### Common Patterns

- **Compilation Logging**: Each benchmark produces `run/log/{name}.compile.txt` with build output
- **Execution Logging**: Each benchmark produces `run/log/{name}.run.txt` with runtime output and `run/log/{name}.time.txt` for timing
- **Timing Measurement**: Uses `/usr/bin/time` with memory and exit code tracking
- **Benchmark Structure**: Follow pattern: `bench <path> [args]` in run.sh
- **Data Files**: Some benchmarks require pre-downloaded data (TAQ, word lists) - see run.sh for URLs

### Testing Requirements

- Each new benchmark must be registered in run.sh with timing estimates
- All benchmarks must compile without warnings
- Compilation and execution must be timed separately
- Output should follow standard logging structure in run/log/
- Verify memory usage tracking with `/usr/bin/time`
- Test on both single-threaded and multi-threaded systems

### Performance Considerations

- **Timing Accuracy**: Use `date +%s.%N` for high-resolution timing between operations
- **Data Staging**: Large data files can be downloaded via curl in get_data() function
- **Cleanup**: cleanup() function removes temporary outputs (important for repeated runs)
- **Dynamic Scheduling**: Some benchmarks use `schedule='dynamic'` for load-balanced parallelization
- **GPU Support**: Mandelbrot and similar compute-intensive benchmarks may use GPU acceleration

## Dependencies

- Codon compiler (with support for .codon language)
- GCC/Clang (for C++ benchmarks)
- Python 3.7+ (for .py benchmarks)
- samtools (for biological data processing in some benchmarks)
- Git (for cloning test data repositories)
- curl (for downloading benchmark data)
- gzip/bzip2 (for data decompression)

## Related Build Systems

- CMake (see cmake/ directory in parent)
- Scripts for dependency installation (see scripts/ directory in parent)

<!-- MANUAL: Update when adding new benchmarks, modifying existing tests, or changing data sources -->
