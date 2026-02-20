# CIR (Codon Intermediate Representation) Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Core intermediate representation (IR) for the Codon compiler. This directory contains the complete CIR framework, including IR node definitions, type system, analysis passes, transformation passes, and LLVM code generation backend.

## Key Files

### Core IR Definitions
- `cir.h` - Main header aggregating all CIR components
- `base.h/cpp` - Base classes for IR nodes (IdMixin, AcceptorExtend, visitor patterns)
- `module.h/cpp` - Module class representing compilation units
- `func.h/cpp` - Function IR nodes
- `value.h/cpp` - Value IR nodes
- `var.h/cpp` - Variable IR nodes
- `instr.h/cpp` - Instruction IR nodes
- `flow.h/cpp` - Control flow IR nodes
- `const.h/cpp` - Constant value IR nodes
- `attribute.h/cpp` - Attribute system for IR nodes
- `pyextension.h` - Python extension integration

## Subdirectories

- `analyze/` - IR analysis passes (dataflow, module-level)
- `dsl/` - Domain-specific language support for IR extensions
- `llvm/` - LLVM backend code generation
- `transform/` - IR transformation and optimization passes
- `types/` - CIR type system
- `util/` - Utility classes (cloning, formatting, inlining, matching, visitors)

## For AI Agents

**When working with CIR:**
- The CIR is the central IR layer between the type-checked AST and LLVM IR
- IR nodes use visitor pattern extensively (util::Visitor, util::ConstVisitor)
- All IR nodes inherit from base classes in base.h with IdMixin for unique IDs
- Transformations are implemented as Pass classes in transform/
- Analyses are implemented as Analysis classes in analyze/
- LLVM code generation happens in llvm/ subdirectory

**Architecture:**
- Type-checked AST → CIR construction → Transform passes → Analysis passes → LLVM codegen
- Module contains Functions, Functions contain control flow, Values, and Instructions
- DSL support allows extending CIR with custom types and instructions

**Modification Guidelines:**
- When adding new IR nodes, update visitor interfaces in util/visitor.h
- Transform passes should extend transform::Pass base class
- Analysis passes should extend analyze::Analysis base class
- Maintain immutability where possible; use cloning utilities in util/cloning.h
- All IR modifications should preserve type safety and SSA properties

## Dependencies

- Depends on: `codon/util/` (common utilities), LLVM libraries
- Used by: `codon/parser/` (AST to CIR lowering), `codon/compiler/` (compilation pipeline)
