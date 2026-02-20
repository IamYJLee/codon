<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# CMake Build Configuration

## Purpose

CMake configuration files for building the Codon compiler. Manages dependency resolution, C++ configuration templates, and build system integration for the LLVM-based compiler infrastructure.

## Key Files

| File | Description |
|------|-------------|
| deps.cmake | CPM.cmake-based dependency management for all external packages (peglib, fmt, toml, semver, zlib-ng, xz, llvm-config, etc.) |
| config.h.in | C++ header template for version and configuration macros (CODON_VERSION, etc.) |
| config.py.in | Python configuration template for runtime version detection |
| CMakeRC.cmake | CMake resource compilation for embedding static resources |
| backtrace-config.h.in | GCC backtrace support configuration |
| backtrace-supported.h.in | GCC backtrace feature detection |

## Subdirectories

None - all files are direct CMake configuration templates and utilities.

## For AI Agents

### Working In This Directory

- **CMake Conventions**: Follow standard CMake patterns; all files are processed during build configuration
- **Template Variables**: Files with `.in` suffix contain CMake variable substitutions using `@VARIABLE@` syntax
- **Package Management**: deps.cmake uses CPM.cmake for transparent dependency downloading and building
- **Version Management**: config.h.in and config.py.in should be updated when version changes

### Key Configuration Elements

- **CPM.cmake Version**: Specified at top of deps.cmake (currently 0.40.8)
- **External Dependencies**: All managed through CPMAddPackage() calls in deps.cmake
- **Build Options**: Each dependency configured with CMAKE_* variables for optimization (e.g., FIC, position-independent code)
- **Conditional Targets**: Some targets excluded from ALL or INSTALL with EXCLUDE_FROM_ALL option

### Common Patterns

- **Package Fetching**: Dependencies downloaded from GitHub via CPM.cmake with specific version/tag
- **Build Isolation**: Each dependency gets separate cache and configuration
- **Platform Detection**: Backtrace support conditional based on platform capabilities
- **Static Linking**: Most dependencies built as static libraries (EXCLUDE_FROM_ALL, no separate install)
- **Version Pinning**: Specific git tags or commit hashes used for reproducibility

### Testing Requirements

- Verify CMake processes all .in templates correctly during configuration
- Check that all dependencies download and build without errors
- Ensure generated config.h and config.py have correct version values
- Validate that backtrace detection works on target platforms
- Test clean builds from scratch to verify dependency caching works properly
- Verify version macros match project version in main CMakeLists.txt

### Dependencies Configuration in deps.cmake

- **peglib**: C++ parsing library (Codon fork) - required for parser
- **fmt**: Format string library - used for code generation
- **toml**: TOML configuration parser - for project files
- **semver**: Semantic versioning library - for version handling
- **zlibng**: High-performance zlib - for compression
- **xz**: XZ compression support
- **llvm-config**: LLVM configuration discovery - critical for compiler backend
- Plus additional platform/architecture-specific configs

## Integration with Build System

- Called from main CMakeLists.txt via `include(cmake/deps.cmake)`
- Template files processed by CMake `configure_file()` command
- Generated files placed in build directory for inclusion in compilation

## Dependencies

- CMake 3.15+ (required by CPM.cmake)
- Git (for cloning repositories via CPM)
- C++ compiler with C++17 support
- Platform development tools (gcc backtrace support on Unix systems)

<!-- MANUAL: Update when adding new external dependencies, changing version constraints, or modifying build configuration -->
