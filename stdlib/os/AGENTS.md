# stdlib/os

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose
Operating system interface functionality for file operations, path manipulation, and system interactions.

## Key Files
- `__init__.codon` - OS module interface with file operations
- `path.codon` - Path manipulation utilities (join, split, exists, etc.)

## For AI Agents
- Python-compatible os module API
- File operations: open, close, read, write, remove, rename
- Directory operations: mkdir, rmdir, listdir, getcwd, chdir
- Path operations in `path.codon`: join, split, exists, isfile, isdir, basename, dirname
- Environment variables access
- Process management: system(), exit()
- Cross-platform abstractions over POSIX/Windows

## Dependencies
- POSIX system calls (via C bindings)
- `internal/c_stubs.codon` - C library interface
- File system APIs
