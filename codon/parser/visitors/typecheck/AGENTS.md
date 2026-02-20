# Type Checking Visitor

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
AST visitor for type inference and type checking. Core semantic analysis pass.

## Key Files
- `typecheck.h/cpp` - Type checker infrastructure and orchestration
- `ctx.h/cpp` - Type checking context
- `infer.cpp` - Type inference engine
- `call.cpp` - Function call type checking
- `class.cpp` - Class/type definition handling
- `function.cpp` - Function definition type checking
- `assign.cpp` - Assignment type checking
- `collections.cpp` - Collection literal type checking
- `loops.cpp` - Loop construct type checking
- `cond.cpp` - Conditional expression type checking
- `op.cpp` - Operator type checking
- `access.cpp` - Member access type checking
- `basic.cpp` - Basic expression type checking
- `special.cpp` - Special form type checking
- `import.cpp` - Import statement handling
- `error.cpp` - Type error reporting

## For AI Agents
**Purpose**: Infers and validates types throughout AST. Resolves generic instantiations.

**Pass Position**: Second semantic pass, runs after scoping and before translation.

**Algorithm**: Constraint-based type inference with unification.

**Context**: `TypecheckContext` maintains type environment and inference state.

**Key Operations**: Type unification, generic instantiation, overload resolution, trait checking.

**Modularity**: Split into multiple .cpp files by language construct for maintainability.

## Dependencies
- `../visitor.h` - Base visitor
- `../../ast/types/` - Type system
- `../../cache.h` - Type cache
