# Application Entry Point

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Command-line interface entry point for Codon compiler.

## Key Files
- `main.cpp` - Main function, CLI argument parsing, compiler invocation

## For AI Agents
**Purpose**: Standalone executable entry point. Parses command-line args and invokes compiler.

**Usage**: Handles compile, run, JIT, and other CLI modes.

**Exit Point**: Returns to OS with appropriate exit code.

## Dependencies
- `../compiler/` - Compiler driver
