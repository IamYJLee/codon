<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Codon Jupyter Kernel

## Purpose

Jupyter kernel integration enabling interactive Codon code execution in Jupyter notebooks and JupyterLab. Built on xeus kernel framework (ZMQ-based), provides code execution, completion, inspection, and syntax validation in a Jupyter environment.

## Key Files

| File | Description |
|------|-------------|
| `CMakeLists.txt` | CMake build system; downloads and configures xeus, xeus-zmq, libzmq, cppzmq, nlohmann/json dependencies; compiles codon_jupyter shared library |
| `jupyter.h` | C++ header; defines `CodonJupyter` class (inherits xeus::xinterpreter), declares Jupyter kernel protocol handlers |
| `jupyter.cpp` | C++ implementation; implements Jupyter protocol handlers (execute, complete, inspect, is_complete, kernel_info, shutdown) |
| `share/jupyter/kernels/codon/kernel.json` | Jupyter kernel spec; defines kernel display name, argv (kernel entrypoint), language declaration |
| `share/jupyter/kernels/codon/kernel.json.in` | CMake template for kernel.json; substitutes build-time variables |
| `xeus.patch` | Git patch applied to xeus 3.0.5 during CMake build; fixes compatibility issues |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `share/jupyter/kernels/codon/` | Jupyter kernel specification and metadata |

## For AI Agents

### Working In This Directory

**Build Configuration**:
- CMake 3.14+; downloads and builds all dependencies via CPM.cmake
- C++17 required; compiler flags: Clang-specific pedantic warnings, return-type-c-linkage disabled
- CODON_PATH env var (default: `~/.codon`) locates Codon headers and libraries
- Install prefix defaults to `${CODON_PATH}/lib/codon/`
- Requires LLVM CONFIG module (set via FindLLVMConfig)

**Dependencies**:
- xeus 3.0.5 (Jupyter kernel framework; patched via xeus.patch)
- xeus-zmq 1.0.3 (ZMQ transport layer)
- libzmq 4.3.4 (message queue; shared lib build)
- cppzmq 4.8.1 (C++ bindings)
- xtl 0.7.5 (utilities)
- nlohmann/json 3.11.2 (JSON serialization)
- LLVM (linked with Codon compiler headers)

**Kernel Architecture** (jupyter.h/cpp):
- `CodonJupyter` class wraps Codon JIT engine (`std::unique_ptr<codon::jit::JIT>`)
- Stores argv0 (executable path) and plugins (loaded modules)
- Implements xeus::xinterpreter virtual methods

**Protocol Handlers** (jupyter.cpp):
- `execute_request_impl()`: Parses code via JIT, handles parse/runtime errors, publishes results with MIME type support
  - Captures custom MIME type marker (`\x00\x00__codon/mime__\x00`) in output
  - Falls back to text/plain for standard output
  - Returns JSON response: status (ok/error), execution_count, data
- `complete_request_impl()`: Code completion (cursor position aware)
- `inspect_request_impl()`: Object inspection at cursor (detail level support)
- `is_complete_request_impl()`: Syntax validation (detects incomplete statements)
- `kernel_info_request_impl()`: Kernel metadata (name, version, language info)
- `shutdown_request_impl()`: Graceful shutdown
- `configure_impl()`: Kernel initialization (banner, language info)

**Error Handling**:
- ParserErrorInfo: Compilation errors with backtrace
- RuntimeErrorInfo: Execution errors with backtrace
- Both formats logged to Jupyter frontend

**MIME Type Support**:
- Custom protocol: Codon can emit `__codon/mime__` marker followed by MIME type (null-terminated) then content
- Enables rich output: HTML plots, LaTeX equations, etc.
- Falls back to text/plain if marker absent

### Common Patterns

**Kernel Registration**:
Jupyter discovers kernel via kernel.json in `share/jupyter/kernels/codon/`
- `display_name`: "Codon" (shown in Jupyter UI)
- `argv`: `["/usr/local/bin/", "jupyter", "{connection_file}"]` (kernel entrypoint)
- `language`: "python" (syntax highlighting, language detection)

**Build & Install**:
```bash
mkdir build && cd build
cmake -DCODON_PATH=~/.codon ..
make
make install  # Installs to ${CODON_PATH}/lib/codon/
```

**Runtime Execution**:
1. Jupyter launches kernel via argv
2. Kernel reads ZMQ connection file from `{connection_file}` argument
3. Establishes ZMQ sockets (shell, iopub, stdin, control, hb)
4. Listens for execute_request messages; executes via Codon JIT
5. Publishes results to iopub socket

**Output Handling**:
- Standard output captured and sent to notebook
- Rich MIME types (HTML, LaTeX) supported via custom marker
- Tracebacks formatted with backtrace information

### Common Issues & Solutions

**Issue**: Kernel fails to find Codon headers
- **Solution**: Set `CODON_PATH` before CMake: `export CODON_PATH=~/.codon && cmake ..`

**Issue**: Build fails with "LLVM not found"
- **Solution**: Ensure `llvm-config` is in PATH; set `LLVM_DIR`: `cmake -DLLVM_DIR=$(llvm-config --cmakedir) ..`

**Issue**: xeus patch fails
- **Solution**: xeus.patch expects specific xeus version (3.0.5); check CPM fetch version in CMakeLists.txt

## Dependencies

**Build Time**:
- CMake 3.14+
- C++17 compiler (Clang or GCC)
- Codon headers and libraries
- LLVM development libraries (CONFIG mode)
- Internet access (CPM.cmake downloads dependencies)

**Runtime**:
- Codon runtime libraries
- libzmq shared library
- Jupyter/JupyterLab with kernel discovery mechanism

**Environment**:
- `CODON_PATH`: Path to Codon installation (default: `~/.codon`)
- `CMAKE_INSTALL_PREFIX`: Installation target (default: `${CODON_PATH}/lib/codon/`)
- `LLVM_DIR`: Path to LLVM CMake config (auto-detected if llvm-config in PATH)

<!-- MANUAL: -->
