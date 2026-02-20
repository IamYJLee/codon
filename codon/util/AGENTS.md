# Utilities

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Common utilities used across Codon compiler components.

## Key Files
- `common.h/cpp` - Common utility functions and helpers
- `serialize.h` - Serialization support for AST/IR
- `tser.h` - Type-safe serialization templates
- `jupyter.h/cpp` - Jupyter notebook integration support
- `peg2cpp.cpp` - PEG grammar to C++ converter utility

## For AI Agents
**Purpose**: Shared utilities to avoid duplication across compiler components.

**Serialization**: Support for AST and IR persistence (e.g., for caching, debugging).

**Jupyter**: Allows Codon to run in Jupyter notebooks.

**PEG Compiler**: `peg2cpp` converts `.peg` grammar files to C++ parser code.

**Note**: This is separate from `../cir/util/` which contains CIR-specific utilities.

## Dependencies
- Standard C++ library
