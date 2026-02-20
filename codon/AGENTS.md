<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# codon/ - Core Compiler Implementation

The heart of the Codon compiler infrastructure. This directory contains the complete pipeline for transforming Python code into optimized LLVM IR and native binaries. The architecture follows a classic three-stage compiler design: parsing (AST generation), intermediate representation (CIR), and code generation (LLVM).

## Purpose

Implement a production-quality Python-to-LLVM compiler targeting high-performance computing workloads. The directory contains:

- **Entry point** (`app/`) - CLI interface supporting run, build, doc, jit, and jupyter modes
- **Intermediate representation** (`cir/`) - Codon's type-safe IR bridging AST and LLVM
- **Compiler driver** (`compiler/`) - Orchestrates parsing, type checking, IR generation, and optimization
- **Configuration** (`config/`) - Build flags, debug settings, version information
- **Language extensions** (`dsl/`) - Plugin system for domain-specific language support
- **Parser pipeline** (`parser/`) - PEG parser, AST nodes, type system, and visitor patterns
- **Runtime support** (`runtime/`) - Compiled libraries for floating point and numpy operations
- **Utilities** (`util/`) - Logging, serialization, common functions shared across modules

## Key Files

| File | Location | Description |
|------|----------|-------------|
| `main.cpp` | `app/` | CLI entry point handling run/build/doc/jit/jupyter modes with LLVM command-line parsing |
| `compiler.h` | `compiler/` | Main Compiler class orchestrating parse → compile → codegen pipeline |
| `module.h` | `cir/` | Core CIR Module representation, container for all IR nodes and functions |
| `func.h` | `cir/` | CIR Function definition with support for generators, async, and methods |
| `value.h` | `cir/` | Base class for all IR values with type information and SSA semantics |
| `ast.h` | `parser/` | AST node hierarchy including statements, expressions, types, and attributes |
| `types.h` | `parser/ast/` | Type system implementation (classes, functions, unions, traits) |
| `cache.h` | `parser/` | AST compilation context tracking scopes, types, and symbol tables |
| `common.h` | `parser/` | Parser utilities (tokenization, string handling, position tracking) |
| `error.h` | `compiler/` | Error handling with detailed reporting and error code enumeration |
| `common.h` | `util/` | Logging macros, timing utilities, and debug flags |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `app/` | **Application entry point** - Single `main.cpp` implementing command-line interface for all Codon modes (run/build/doc/jit/jupyter) |
| `cir/` | **Codon Intermediate Representation** - Type-safe IR system including values, functions, types, control flow, transforms, LLVM codegen, and formatting |
| `compiler/` | **Compiler driver** - Orchestrates entire compilation pipeline: parsing, type checking, CIR generation, optimization passes, and code output |
| `config/` | **Configuration management** - Build configuration, compile-time settings, version constants, and environment variables |
| `dsl/` | **Domain-specific language support** - Plugin system allowing language extensions and custom passes |
| `parser/` | **Parser and front-end** - PEG parser, AST nodes (statements, expressions, types, attributes), cache management, and visitor patterns |
| `runtime/` | **Runtime libraries** - Compiled native support for floating-point operations, numpy compatibility, and standard library functions |
| `util/` | **Utility infrastructure** - Logging framework, serialization, timing utilities, common string/collection functions, and debug helpers |

## For AI Agents

### Working In This Directory

**Entry Points for Different Tasks:**
- **Compilation pipeline changes**: Study `compiler/compiler.h` and trace through `app/main.cpp` → `processSource()` → `compiler->compile()`
- **IR design/optimization**: Work in `cir/` module, understanding `Module` (container) → `Func` (functions) → `Value` (values/SSA)
- **Type system changes**: Primary files are `parser/ast/types/*.h`, especially class/function/union type implementations
- **Parser improvements**: Start with `parser/ast/` for AST nodes, use `parser/common.h` utilities, understand visitor patterns in `ACCEPT` macro
- **Error handling**: Modify `compiler/error.h` for error codes and `app/main.cpp` for error display formatting
- **New language features**: Implement in `parser/ast/` (AST nodes), then add CIR generation in `compiler/` visitors

**Compilation Pipeline Overview:**
```
input.py
  ↓
app/main.cpp (CLI parsing & mode selection)
  ↓
compiler/compiler.h::Compiler::parseFile() (PEG parser)
  ↓
parser/cache.h::Cache (AST + type checking)
  ↓
compiler/compiler.h::Compiler::compile() (CIR generation via visitors)
  ↓
cir/module.h::Module (IR representation)
  ↓
cir/transform/manager.h (optimization passes)
  ↓
cir/llvm/llvisitor.h (LLVM IR generation)
  ↓
app/main.cpp (output: executable/library/bitcode/LLVM IR)
```

**Key Architectural Patterns:**

1. **Visitor Pattern**: Every AST node implements `void accept(Visitor &v)` via `ACCEPT` macro in `stmt.h`. Used for type checking and CIR generation.

2. **CIR Transform Pipeline**: All IR optimizations flow through `cir/transform/manager.h` which applies passes in sequence with validation.

3. **Type System**: Dual representation - `parser/ast/types/*.h` for source types, `cir/types/` for IR types, with correspondence tracking during CIR generation.

4. **Error Handling**: Three error categories via llvm::Error and custom handlers:
   - `ParserErrorInfo` - parsing and type checking (parser/ast/error.h)
   - `RuntimeErrorInfo` - execution failures (stack traces via JIT)
   - `PluginErrorInfo` - plugin loading issues

5. **Module-as-Container**: `cir/module.h::Module` holds all IR nodes. Use iterators from `cir/util/iterators.h` to traverse functions, types, and variables.

### Testing Requirements

**Unit Test Organization:**
- Parser tests: `test/parser/` - Test PEG parser, AST construction, and visitor patterns
- CIR tests: `test/cir/` - Test IR construction, type checking, and transformations
- Integration tests: `test/compiler/` - End-to-end compilation with expected IR output
- Runtime tests: `test/runtime/` - Verify compiled code behavior

**Testing Key Components:**

1. **Parser Testing**: Feed Python source → verify AST structure → check error reporting
   - Use `ast::Cache` for incremental parsing
   - Test error recovery and position tracking
   - Verify type inference correctness

2. **CIR Generation Testing**:
   - Parse → compile → inspect `Module` structure
   - Verify SSA form and type correctness
   - Check function signature compatibility

3. **Optimization Testing**:
   - Enable specific passes via `--disable-opt` flags
   - Verify IR transformation correctness (use LLVM-Verifier if available)
   - Check that semantics are preserved

4. **Mode Testing**:
   - **Run mode**: Execute compiled code, verify output and exit codes
   - **Build mode**: Generate different output formats (exe/lib/bc/ll/cir)
   - **JIT mode**: Interactive execution with error recovery
   - **Doc mode**: Docstring extraction and formatting
   - **Jupyter mode**: Integration with Jupyter kernel protocol

**Common Test Pattern:**
```cpp
// Create compiler in test mode
auto compiler = std::make_unique<codon::Compiler>(
    "test_argv", /*debug=*/true, /*disabledPasses=*/{},
    /*isTest=*/true, /*pyNumerics=*/false, /*pyExtension=*/false);

// Parse test code
auto result = compiler->parseCode("test.py", "def foo(x: int) -> int: return x + 1");
ASSERT_FALSE(result);  // Should succeed (no error)

// Compile
auto compileResult = compiler->compile();
ASSERT_FALSE(compileResult);

// Inspect CIR
auto* module = compiler->getModule();
// ... assertions on IR structure
```

### Common Patterns

**Pattern 1: Adding a Language Feature**
1. Define AST node in `parser/ast/stmt.h` or `expr.h` with `ACCEPT` macro
2. Implement type checking in visitor (`parser/`) that fills in types
3. Implement CIR generation in `compiler/` visitor that walks AST and generates IR
4. Add optimization pass if needed in `cir/transform/`
5. Test: parse → verify AST → compile → inspect Module

**Pattern 2: IR Optimization**
1. Create new pass class inheriting from `cir::ConstVisitor` in `cir/transform/`
2. Implement `visitFunc()` to analyze/modify functions
3. Register pass in `PassManager` with dependencies and invalidation info
4. Test: compile code → run pass → verify semantic equivalence (LLVM-Verifier recommended)

**Pattern 3: Error Handling**
1. Add error code enum to `compiler/error.h::Error`
2. Emit error via `compilationError()` or visitor's error handling
3. For recoverable errors, collect in `ParserErrorInfo` and display via `display()` in main.cpp
4. For parser errors, populate via `ast::Cache` error tracking

**Pattern 4: Working with Modules**
```cpp
// Iterate functions
for (auto* func : module->getFuncs())
    if (func->isGeneric()) { /* handle generic function */ }

// Access types
for (auto& [name, type] : module->getTypes())
    if (auto* cls = type->cast<ir::types::RecordType>()) { /* handle class */ }

// Find a function by name (mangled)
if (auto* func = module->getOrRealizeFunc("__main__.foo.0"))
    func->setAsGenerated();

// Check runtime values (e.g., exception types)
auto* excType = module->getOrRealizeType("Exception");
```

**Pattern 5: Logging and Debugging**
```cpp
// In util/common.h - use these macros
LOG("debug message");                    // Always logged if enabled
LOG_REALIZE("realizing: {}", func->getName());  // Conditioned on FLAG_REALIZE
LOG_TYPECHECK("type: {}", type);        // Conditioned on FLAG_TYPECHECK
DBGI("nested info");                     // With indentation based on nesting level

// Enable via environment: CODON_DEBUG=realize,typecheck
// Or command line: --log realize,typecheck
```

## Dependencies

### Internal

| Module | Used By | Purpose |
|--------|---------|---------|
| `util/` | All modules | Logging (DBG/LOG macros), serialization, timing, common utilities |
| `parser/` | `compiler/`, `cir/` | AST, type system, error info, cache for compilation state |
| `compiler/` | `app/`, none | Main pipeline orchestration, error definitions |
| `cir/` | `compiler/`, `app/` | IR representation, transforms, LLVM codegen |
| `config/` | `compiler/`, `app/` | Compile-time settings and version info |
| `dsl/` | `compiler/` | Plugin loading and management |
| `runtime/` | Linked at compile time | Native implementations for stdlib (floatlib, numpy, etc.) |

### External

| Library | Used For |
|---------|----------|
| **LLVM** (llvm::*) | IR generation, codegen, bitcode, assembly output, linking |
| **fmt** (fmt::*) | String formatting (replaces sprintf/printf for safety) |
| **OpenMP** (pragma omp) | Parallel code generation (optional, controlled by flags) |
| **Python C API** | Python extension module generation (pyext mode) |
| **Jupyter protocol** (zmq/protobuf) | Jupyter kernel implementation |

### Build-Time Tools

| Tool | Purpose |
|------|---------|
| CMake | Build configuration and dependency management |
| C++17 compiler | Source compilation (gcc/clang with C++17 support) |
| LLVM toolchain | IR optimization and codegen |
| Python development headers | For extension module generation |

## Architecture Summary

```
┌─────────────────────────────────────────────────────────────┐
│  app/main.cpp - CLI Interface                               │
│  (run|build|doc|jit|jupyter modes with LLVM cmd parsing)    │
└──────────────────┬──────────────────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────────────────┐
│  compiler/compiler.h - Compiler Driver                      │
│  • parseFile/parseCode → parser/cache.h                     │
│  • compile() → AST visitors → CIR generation                │
│  • getModule() → CIR Module for introspection               │
└──────────────┬─────────────────────┬──────────────────────┘
               │                     │
    ┌──────────▼──────────┐  ┌───────▼────────────────────┐
    │ parser/ - Front End │  │ dsl/ - Plugin System       │
    ├─────────────────────┤  ├────────────────────────────┤
    │ • cache.h - Scopes  │  │ • Plugin manager           │
    │ • ast/*.h - Nodes   │  │ • DSL extensions           │
    │ • types/*.h - Types │  │ • Custom IR passes         │
    │ • Visitors          │  └────────────────────────────┘
    └─────────────────────┘
               │
    ┌──────────▼──────────────────────────────────────────┐
    │ cir/ - Intermediate Representation                 │
    ├───────────────────────────────────────────────────┤
    │ • module.h - Module container                     │
    │ • func.h - Functions (generators, async support) │
    │ • value.h - SSA values                            │
    │ • types/ - Type system                            │
    │ • transform/ - Optimization passes                │
    │ • llvm/ - LLVM codegen visitor                    │
    └────────────────────┬─────────────────────────────┘
                         │
            ┌────────────▼────────────────┐
            │ Output Generation           │
            ├─────────────────────────────┤
            │ • LLVM IR (.ll)             │
            │ • Bitcode (.bc)             │
            │ • Object files (.o)         │
            │ • Executables               │
            │ • Libraries (.so/.dylib)    │
            │ • Python extensions         │
            └─────────────────────────────┘
```

<!-- MANUAL: Any manually added notes below this line are preserved on regeneration -->
