<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# test

## Purpose

Comprehensive test suite for the Codon compiler project (Python-to-LLVM compiler). Covers unit tests for the compiler intermediate representation (CIR), parser, type system, transformations, and integration tests for language features and standard library compatibility.

## Key Files

- **main.cpp** - Test runner main entry point. Links with Google Test framework (gtest).
- **types.cpp** - Type system unit tests. Basic test scaffolding for type-related functionality.
- **CMakeLists.txt.in** - Build configuration template for test compilation.

## Subdirectories

### Core Testing

- **cir/** - Codon Intermediate Representation (CIR) unit tests
  - **base.cpp** - Basic IR node operations, RTTI, attributes, cloning
  - **constant.cpp** - Constant value handling and operations
  - **value.cpp** - Value node tests
  - **var.cpp** - Variable handling
  - **instr.cpp** - Instruction node tests
  - **func.cpp** - Function definition and IR construction
  - **flow.cpp** - Control flow structures
  - **module.cpp** - Module-level IR operations
  - **test.h** - Shared test header with CIRCoreTest base class using gtest

- **cir/analyze/** - IR analysis pass tests
  - **dominator.cpp** - Dominator tree analysis
  - **reaching.cpp** - Reaching definitions dataflow analysis

- **cir/transform/** - IR transformation pass tests
  - **manager.cpp** - Transform pass manager and orchestration

- **cir/types/** - CIR type system tests
  - **types.cpp** - Type hierarchy, operations, and representations

- **cir/util/** - IR utility function tests
  - **matching.cpp** - Pattern matching on IR nodes

### Language Feature Tests

- **core/** - Core language features (.codon test files)
  - **arithmetic.codon** - Arithmetic operations, division, modular arithmetic
  - **arguments.codon** - Function arguments and parameter handling
  - **bltin.codon** - Built-in functions and types
  - **containers.codon** - Collections (lists, tuples, dicts)
  - **exceptions.codon** - Exception handling and error propagation
  - **generators.codon** - Generator functions and yield
  - **generics.codon** - Generic type parameters and polymorphism
  - **helloworld.codon** - Basic "hello world" test
  - **match.codon** - Pattern matching constructs
  - **numerics.codon** - Numeric types and operations (Int[128], UInt, etc.)
  - **parser.codon** - Parser behavior tests
  - **pipeline.codon** - Functional programming pipelines
  - **range.codon** - Range iteration and operations
  - **serialization.codon** - Serialization/deserialization
  - **sort.codon** - Sorting algorithms and operations
  - **trees.codon** - Tree data structures
  - **vec_simd.codon** - Vector and SIMD operations

### Parser Tests

- **parser/** - Parser and type checking tests
  - **llvm.codon** - LLVM IR inline assembly parsing
  - **simplify_expr.codon** - Expression simplification
  - **simplify_stmt.codon** - Statement simplification
  - **typecheck_expr.codon** - Expression type checking
  - **typecheck_stmt.codon** - Statement type checking
  - **types.codon** - Type checking behavior

- **parser/typecheck/** - Comprehensive type checking tests
  - **test_*.codon** - Feature-specific type checking:
    - test_access.codon
    - test_assign.codon
    - test_basic.codon
    - test_call.codon
    - test_class.codon
    - test_collections.codon
    - test_cond.codon
    - test_ctx.codon
    - test_error.codon
    - test_function.codon
    - test_import.codon
    - test_infer.codon
    - test_loops.codon
    - test_op.codon
    - test_parser.codon
    - test_python.codon
    - test_typecheck.codon
  - **a/b/** - Module recursion and import tests
    - rec1.codon, rec1_err.codon
    - rec2.codon, rec2_err.codon
  - **a/sub/** - Sub-module import tests

### Standard Library Tests

- **stdlib/** - Standard library implementation tests (.codon)
  - **asyncio_test.codon** - Async I/O functionality
  - **bisect_test.codon** - Binary search operations
  - **cmath_test.codon** - Complex math operations
  - **datetime_test.codon** - Date and time handling
  - **heapq_test.codon** - Heap queue operations
  - **itertools_test.codon** - Iterator tools
  - **llvm_test.codon** - LLVM interop
  - **math_test.codon** - Mathematical functions
  - **operator_test.codon** - Operator overloading
  - **random_test.codon** - Random number generation
  - **re_test.codon** - Regular expressions
  - **sort_test.codon** - Sorting utilities
  - **statistics_test.codon** - Statistical functions
  - **str_test.codon** - String operations and methods

### NumPy Compatibility Tests

- **numpy/** - NumPy API and functionality tests
  - **test_dtype.codon** - Data type handling
  - **test_elision.codon** - Memory elision optimizations
  - **test_fft.codon** - Fast Fourier Transform
  - **test_functional.codon** - Functional operations on arrays
  - **test_fusion.codon** - Loop fusion optimizations
  - **test_indexing.codon** - Array indexing operations
  - **test_io.codon** - I/O operations with arrays
  - **test_lib.codon** - NumPy library functions
  - **test_linalg.codon** - Linear algebra operations
  - **test_loops.codon** - Loop optimizations on arrays
  - **test_misc.codon** - Miscellaneous NumPy operations
  - **test_ndmath.codon** - N-dimensional array math
  - **test_npdatetime.codon** - NumPy datetime handling
  - **test_pybridge.codon** - Python interop with NumPy
  - **test_reductions.codon** - Reduction operations (sum, mean, etc.)
  - **test_routines.codon** - NumPy routines and functions
  - **test_sorting.codon** - Array sorting operations
  - **test_statistics.codon** - Statistical functions on arrays
  - **test_window.codon** - Window functions (FFT windows, etc.)

- **numpy/data/** - Test data directory
- **numpy/random_tests/** - Specialized random number generator tests
  - **test_mt19937.codon** - Mersenne Twister RNG
  - **test_pcg64.codon** - PCG64 RNG
  - **test_philox.codon** - Philox RNG
  - **test_sfc64.codon** - SFC64 RNG

### Transform Pipeline Tests

- **transform/** - IR transformation and optimization tests
  - **canonical.codon** - Canonical form transformations
  - **dict_opt.codon** - Dictionary optimization passes
  - **escapes.codon** - Escape analysis
  - **folding.codon** - Constant folding optimization
  - **for_lowering.codon** - For-loop lowering
  - **inlining.codon** - Function inlining optimization
  - **io_opt.codon** - I/O optimization passes
  - **kernels.codon** - Kernel generation and optimization
  - **list_opt.codon** - List optimization passes
  - **omp.codon** - OpenMP parallelization
  - **outlining.codon** - Code outlining transforms
  - **str_opt.codon** - String optimization passes

### Application/CLI Tests

- **app/** - Command-line interface and application tests
  - **argv.codon** - Command-line argument parsing
  - **build.codon** - Build system functionality
  - **exit.codon** - Exit code handling
  - **export.codon** - Library export functionality
  - **input.codon** - Standard input handling
  - **input.txt** - Test input data
  - **test.sh** - Integration test script (bash)
  - **test.c** - C interop test source

### Python Interoperability Tests

- **python/** - Python bridge and embedding tests
  - **__init__.py** - Python module initialization
  - **cython_jit.py** - Cython JIT compilation tests
  - **find-python-library.py** - Python library discovery utility
  - **mymodule.py** - Test Python module
  - **myextension.codon** - Codon extension module
  - **pyext.py** - Python extension testing
  - **setup.py** - Python module setup

## For AI Agents

### Working In This Directory

**Test Organization Pattern:**
- C++ unit tests use Google Test (gtest) with `TEST_F(CIRCoreTest, ...)` macros
- Codon language tests use `@test` decorator and assertions
- Integration tests use shell scripts with exit codes
- All paths are relative to test/ directory when building

**Key Patterns to Follow:**
1. **CIR Unit Tests** (.cpp files):
   - Inherit from `CIRCoreTest` base class (defined in test.h)
   - Call `SetUp()` which initializes module and CloneVisitor
   - Use `module->Nr<T>(...)` to create IR nodes
   - Use ASSERT_* macros for verification
   - Reset ID mixin: `codon::ir::IdMixin::resetId()` in SetUp

2. **Language Feature Tests** (.codon files):
   - Use `@test` decorator to mark test functions
   - Run with `codon run <file.codon>`
   - Use assertions: `assert condition`
   - Test decorators run automatically when file executes
   - Examples: `@test def test_name(): assert ...`

3. **Integration Tests** (.sh, .c files):
   - Shell scripts in app/ verify CLI behavior
   - Check exit codes: `[ $? -eq expected ] || exit code`
   - Compare outputs: `[ "$(command)" == "expected" ]`
   - Test different modes: run, build, build shared library

4. **Test Data Organization**:
   - Codon source files in feature subdirectories
   - Data files (txt) alongside test scripts
   - Python bridge code in python/ subdirectory
   - Test data in numpy/data/ for NumPy tests

### Testing Requirements

**Build & Execution:**
- C++ tests compile with CMake and link against gtest
- Run with: `ctest` or direct test binary execution
- Codon tests execute with: `codon run <test.codon>`
- Integration tests use: `./app/test.sh <binary_path>`

**Code Coverage:**
- Unit tests should cover IR node operations, type checking, and transforms
- Language feature tests should verify semantics match Python behavior
- Transform tests should validate optimization correctness
- Parser tests should check all grammar rules and error conditions

**Failure Modes to Test:**
- Type errors and inference failures
- Invalid IR construction
- Transform pass validation and correctness
- Parser error recovery
- NumPy semantics edge cases

**Test Independence:**
- Each test should be independently runnable
- CIR tests reset ID state in SetUp()
- No interdependencies between test files
- Codon tests should not depend on compilation state

### Common Patterns

**CIR Testing Pattern:**
```cpp
TEST_F(CIRCoreTest, TestName) {
  // Create IR nodes using module
  auto *node = module->Nr<NodeType>(args...);

  // Verify with assertions
  ASSERT_TRUE(condition);
  ASSERT_EQ(expected, actual);

  // Test operations
  auto *result = operation(node);
  ASSERT_FALSE(result->hasReplacement());
}
```

**Codon Language Test Pattern:**
```codon
@test
def test_feature():
    assert expression == expected_value
    assert another_condition
test_feature()
```

**Transform Test Pattern:**
```codon
# Test optimization before/after behavior
@test
def test_optimization():
    # Setup code that will be optimized
    result = computation()
    assert optimized_result_is_correct(result)
test_optimization()
```

**Type Checking Test Pattern:**
```codon
# test_typecheck/*.codon files test type inference and error handling
# Both success and error cases in same file
# Uses compiler's typecheck phase
```

**NumPy Test Pattern:**
```codon
@test
def test_numpy_operation():
    import numpy as np
    arr = np.array([...])
    result = np.operation(arr)
    assert np.allclose(result, expected)
test_numpy_operation()
```

## Dependencies

**Compiler Infrastructure:**
- Google Test (gtest) - C++ unit test framework
- LLVM - For IR generation and optimization
- Codon Compiler itself - For language feature compilation

**Runtime Dependencies:**
- Python 3.x - For Python interop tests
- GCC/Clang - For C interop compilation (test.c)
- GNU tools - For test.sh script execution

**Key Include Paths:**
- `codon/cir/cir.h` - CIR node definitions
- `codon/cir/util/cloning.h` - CloneVisitor for IR cloning
- `codon/compiler/compiler.h` - Compiler main interface
- `codon/parser/common.h` - Parser utilities
- `gtest/gtest.h` - Google Test framework

**Module Dependencies by Test Type:**
- Parser tests depend on: parser, type inference
- CIR tests depend on: IR module, type system
- Transform tests depend on: pass managers, specific passes
- NumPy tests depend on: numpy module implementation
- Python tests depend on: Python bridge layer

<!-- MANUAL: -->
