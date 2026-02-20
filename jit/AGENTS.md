<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Codon JIT Package

## Purpose

Python JIT compilation decorator and runtime integration for the Codon compiler. Provides the `@jit` decorator that enables Python functions to be compiled and executed via the Codon JIT engine, with automatic type inference from Python argument types.

## Key Files

| File | Description |
|------|-------------|
| `setup.py` | Package configuration; locates Codon installation, configures Cython extension compilation with proper library and include paths |
| `pyproject.toml` | Project metadata and build system specification |
| `codon/__init__.py` | Public API exports: `jit`, `convert`, `JITError`, `JITWrapper`, `_jit_register_fn`, `_jit` |
| `codon/decorator.py` | Core decorator implementation; handles function/class registration, type inference, and JIT callback execution |
| `codon/jit.pyx` | Cython extension binding C interface to Codon JIT library (libcodonc, libcodonrt); wraps JITWrapper class and execution routines |
| `codon/jit.pxd` | Cython declaration file defining C structs and function signatures for JIT interop |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `codon/` | Python package root; contains decorator logic, type inference system, and Cython bindings |

## For AI Agents

### Working In This Directory

**Setup & Build**:
- Codon must be installed; `setup.py` searches `CODON_DIR` env var, then `~/.codon`, then `$(pwd)/..`
- Build requires: Cython, setuptools, astunparse, numpy (for type conversions)
- On Linux: detects C++ ABI version via `libcodonc.so` symbol inspection; sets `_GLIBCXX_USE_CXX11_ABI` flag accordingly
- Platform-specific: macOS uses `libcodonrt` + `libcodonc` (dylib); Linux adds `-lcodonc` with RPATH

**Type System**:
- `pod_conversions` maps Python types (int, float, bool, str, complex, numpy dtypes) to Codon type strings
- Custom containers (list, dict, set, tuple) recursively inferred via `_codon_type()`
- numpy arrays infer dtype (int8 → i8, float64 → float, etc.) and ndim
- Unknown types fallback to "pyobj" (Python object)
- Debug mode 1: print type inference warnings; mode 2: print generated code

**Decorator Flow**:
1. `jit(fn)` calls `_jit_register_fn()` to parse and register function with Codon compiler
2. Function source extracted via `inspect.getsourcelines()` and unparsed via astunparse
3. Code executed in Codon JIT session; returns function name for later reference
4. Wrapper function captures decorator's `debug`, `sample_size`, `pyvars` parameters
5. On invocation: `_jit_callback_fn()` infers argument types from actual call, executes via JIT engine

**Special Features**:
- `pyvars`: List of Python variable names captured as closure variables in function
- `sample_size`: Number of collection items sampled for type inference (default 5)
- `debug`: 0 (off), 1 (print types/errors), 2 (print generated code); overridable via `CODON_JIT_DEBUG` env var
- `convert(cls)`: Register Python class with `__slots__` for efficient JIT conversion

**Cython Interop** (jit.pyx):
- `JITWrapper` class: Cython object holding C++ void pointer to Codon JIT instance
- `__cinit__`: calls `codon.jit.jit_init()` to create JIT
- `__dealloc__`: calls `codon.jit.jit_exit()` to cleanup
- `execute()`: compiles code string via `jit_execute_safe()`, raises JITError on failure
- `run_wrapper()`: invokes compiled function with type list and Python args via `jit_execute_python()`
- Memory management: allocates C char** arrays for types/pyvars, frees after execution

### Common Patterns

**Decorate a function**:
```python
@codon.jit
def add(a: int, b: int) -> int:
    return a + b

result = add(5, 3)  # Compiled and executed via JIT
```

**With debug output**:
```python
@codon.jit(debug=2)
def compute(x):
    return x * 2
# Prints AST and generated Codon code to stderr
```

**Capture closure variables**:
```python
factor = 10
@codon.jit(pyvars=['factor'])
def multiply(x):
    return x * factor
```

**Execute inline code**:
```python
codon.execute("def fib(n: int) -> int:\n    return n if n <= 1 else fib(n-1) + fib(n-2)")
# Function now available in JIT namespace
```

**Register custom type**:
```python
@dataclass
class Point:
    __slots__ = ['x', 'y']
    x: float
    y: float

codon.convert(Point)  # Now can be used in @jit functions
```

## Dependencies

**Build Time**:
- Codon compiler (libcodonc, libcodonrt, include headers)
- Cython
- setuptools
- astunparse
- numpy

**Runtime**:
- Codon runtime libraries (libcodonrt.so/.dylib)
- Python 3.6+
- numpy (optional, for numpy type inference)

**Environment**:
- `CODON_DIR`: Optional; path to Codon installation (auto-detected from PATH, ~/.codon, or parent dir)
- `CODON_PATH`: Set automatically to Codon stdlib location if not already defined
- `CODON_JIT_DEBUG`: Optional; override debug level (0, 1, 2)

<!-- MANUAL: -->
