# Compiler Driver

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Main compiler orchestration layer. Coordinates parsing, semantic analysis, optimization, and code generation.

## Key Files
- `compiler.h/cpp` - Main compiler driver class
- `engine.h/cpp` - Compilation engine and execution
- `jit.h/cpp` - JIT compilation support
- `jit_extern.h` - JIT external function interface
- `error.h/cpp` - Compiler error handling
- `memory_manager.h/cpp` - Memory management for JIT
- `debug_listener.h/cpp` - Debug event listener

## For AI Agents
**Entry Point**: `Compiler` class orchestrates entire compilation pipeline.

**Modes**: DEBUG, RELEASE, JIT - different optimization and execution strategies.

**Pipeline**: Source → Parser (`../parser/`) → CIR (`../cir/`) → Optimization → LLVM → Native code

**JIT**: Just-in-time compilation mode for interactive execution.

**Error Handling**: Centralized error collection and reporting.

**Plugin System**: Integrates with DSL plugins (`../dsl/`).

## Dependencies
- `../parser/` - Front-end parsing and semantic analysis
- `../cir/` - Codon IR and transformations
- `../dsl/` - Plugin system
- LLVM - Backend code generation
