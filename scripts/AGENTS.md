<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Build and Deployment Scripts

## Purpose

Utility scripts for building Codon from source, managing dependencies, generating documentation, and deploying Docker containers. Handles platform-specific setup, system library discovery, and release packaging.

## Key Files

| File | Description |
|------|-------------|
| deps.sh | LLVM dependency builder; clones LLVM fork, configures, builds, and installs with OpenMP/Clang support |
| install.sh | Binary installer for pre-built Codon releases; downloads platform-specific tarball and updates shell PATH |
| docgen.py | API documentation generator; converts JSON AST into Markdown documentation tree structure |
| get_system_libs.sh | System library discovery; identifies libgfortran and other platform-specific libraries |
| fix_loader_paths.sh | macOS binary tool; updates RPATH and loader paths for dylib dependencies |
| Dockerfile.codon-build | Docker image for building Codon from source (development/CI builds) |
| Dockerfile.llvm-build | Docker image for pre-building LLVM dependency (layer caching optimization) |
| Dockerfile.codon-jupyter | Docker image with Codon + Jupyter for interactive notebooks |
| Dockerfile.gpu | Docker image with CUDA support for GPU-accelerated Codon |

## Subdirectories

None - all scripts are utility functions in the scripts/ directory root.

## For AI Agents

### Working In This Directory

- **Shell Scripts**: bash scripts (`.sh`) with error handling (`set -e` and `set -o pipefail`)
- **Python Scripts**: docgen.py generates documentation from compiler-produced JSON
- **Docker**: Multiple Dockerfile variants for different build/deployment scenarios
- **Permissions**: Shell scripts typically require `chmod +x` to execute
- **Exit Codes**: Scripts use non-zero exit codes on failure; follow bash error conventions

### Script Purposes and Key Variables

**deps.sh**:
- Accepts single argument: number of parallel build jobs (default 1)
- Sets `INSTALLDIR` to `./llvm` and `SRCDIR` to `./llvm-project`
- Clones from `exaloop/llvm-project` with branch `codon`
- Builds with OpenMP and optionally Clang; installs to local prefix

**install.sh**:
- Detects OS (`darwin` or `linux`) and architecture (`x86_64` or `arm64`)
- Downloads platform-specific release tarball from GitHub
- Installs to `~/.codon` by default (customizable via CODON_INSTALL_DIR)
- Offers to update shell profile (.bashrc, .zshrc, etc.) with PATH export

**docgen.py**:
- Takes 3+ arguments: `<json_input> <output_dir> <root1> [root2] ...`
- Reads JSON AST produced by Codon compiler with `--dump-ast` flag
- Creates Markdown documentation tree mirroring source structure
- Groups items by module, class, function with proper hierarchy

**get_system_libs.sh**:
- Locates system libraries (especially libgfortran) for linking
- Platform detection with fallbacks for different package managers
- Critical for BLAS/LAPACK integration with Fortran backend

**fix_loader_paths.sh**:
- macOS-specific tool using `install_name_tool`
- Updates RPATH entries in Mach-O binaries
- Fixes dylib references for reproducible builds
- Important for release binary portability

### Docker Images

- **llvm-build**: Multi-stage layer for LLVM compilation (base layer for other images)
- **codon-build**: Full Codon build from source (for development/CI)
- **codon-jupyter**: Codon + Jupyter for interactive analysis notebooks
- **gpu**: CUDA-enabled image for GPU acceleration

### Common Patterns

- **Parallel Jobs**: Use `$JOBS` variable to control build parallelism (affects compilation time)
- **Installation Directories**: Use environment variables for configuration (`INSTALLDIR`, `SRCDIR`, `CODON_INSTALL_DIR`)
- **Error Handling**: All critical operations guarded with conditional checks (`if [ ! -f ... ]`)
- **Cleanup**: Temporary build artifacts removed after successful installation
- **Platform Detection**: Use `uname -s` and `uname -m` for portability

### Testing Requirements

- **deps.sh**: Verify LLVM builds completely and `llvm-config` is available
- **install.sh**: Test on both Linux and macOS; verify PATH update in shell profiles
- **docgen.py**: Parse sample JSON AST; verify output Markdown structure matches source
- **get_system_libs.sh**: Test on clean systems to verify library discovery
- **fix_loader_paths.sh**: Verify macOS binaries have correct @rpath entries
- **Dockerfiles**: Build all images locally; test image execution and tool availability

### Documentation Generation Workflow

1. Compiler produces JSON AST with `--dump-ast` flag
2. docgen.py reads JSON and traverses module tree
3. Creates directory structure matching source organization
4. Generates .md files with module documentation
5. Output placed in docs/libraries/api/ for MkDocs inclusion

## Dependencies

- bash 4.0+ (for all shell scripts)
- Python 3.7+ (for docgen.py)
- git (for cloning repositories in deps.sh)
- CMake 3.15+ (for LLVM build in deps.sh)
- make (for LLVM build in deps.sh)
- Docker (for running container builds)
- curl (for downloading releases in install.sh)
- GNU coreutils (standard Unix utilities)
- macOS: install_name_tool (Xcode tools) for fix_loader_paths.sh
- Linux: GCC/Clang for compilation

## Integration with Build System

- **CI/CD**: Docker images used in GitHub Actions workflows
- **Release Process**: install.sh used by end users for binary installation
- **Development**: deps.sh used for local LLVM setup
- **Documentation**: docgen.py called during docs build phase

<!-- MANUAL: Update when adding new scripts, changing LLVM version, modifying release process, or adding new Docker targets -->
