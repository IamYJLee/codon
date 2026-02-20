# PEG Parser

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Parsing Expression Grammar (PEG) based parser for Codon language. Converts source code to AST.

## Key Files
- `peg.h/cpp` - PEG parser implementation
- `rules.h` - Parser rule definitions
- `grammar.peg` - Codon language grammar specification
- `openmp.peg` - OpenMP pragma grammar

## For AI Agents
**Grammar**: `grammar.peg` defines Codon syntax using PEG rules. Grammar is compiled into parser code.

**Parser Output**: Generates AST nodes from `../ast/` hierarchy.

**Error Recovery**: Parser provides error messages with source location info.

**Extensions**: `openmp.peg` shows how to extend grammar for pragmas.

## Dependencies
- `../ast/` - AST nodes constructed by parser
- `../common.h` - Source location tracking
