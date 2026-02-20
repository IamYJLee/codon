<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

# Documentation Site

## Purpose

MkDocs source for the official Codon compiler documentation website. This directory contains all user-facing documentation including language guides, API references, installation instructions, and integration examples.

## Key Files

| File | Description |
|------|-------------|
| index.md | Homepage and main entry point for documentation |
| css/extra.css | Custom CSS styling for the documentation site |
| js/mathjax.js | MathJax integration for mathematical rendering |
| overrides/main.html | Custom HTML template overrides for MkDocs theme |

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| start/ | Getting started guides: installation, basic usage, FAQ, changelog |
| language/ | Language documentation: overview, classes, generics, metaprogramming, LLVM integration, low-level features |
| libraries/ | Library documentation: standard library, NumPy support, API reference (auto-generated) |
| parallel/ | Parallel computing guides: multithreading, SIMD, GPU programming |
| integrations/ | Integration guides: C++ interop, Python interop, Jupyter support |
| developers/ | Developer documentation: build instructions, compilation internals, IR details, contribution guidelines, roadmap |
| labs/ | Experimental features and catalog of example programs |
| img/ | Images and SVG assets used throughout documentation |

## For AI Agents

### Working In This Directory

- **Documentation Format**: Markdown files with MkDocs YAML frontmatter where needed
- **File Organization**: Organized by topic/audience (getting started, language, libraries, integrations, developers)
- **Cross-References**: Use relative links like `[link text](../path/file.md)` for internal navigation
- **Code Examples**: Codon, Python, and C++ examples should be in appropriate markdown code blocks with syntax highlighting
- **Naming Convention**: Lowercase filenames with underscores (e.g., `python_interop.md`)

### Content Guidelines

- **Installation Docs** (start/install.md): Keep updated with latest release information
- **Language Features**: Document language constructs with working examples
- **API Reference** (libraries/api/): Auto-generated from compiler - do not edit directly
- **Integration Docs**: Include both theory and practical examples for C++/Python interoperability
- **Developer Guides**: Explain compiler internals, IR, and build process for contributors

### Common Patterns

- **Code Examples**: Always include runnable examples that demonstrate features
- **Prerequisites**: Clearly state version requirements and dependencies
- **Platform Notes**: Highlight Linux/macOS/Windows differences where applicable
- **Performance Notes**: Document performance characteristics and optimization guidance
- **Common Errors**: Include troubleshooting sections with solutions

### Testing Requirements

- Verify all code examples compile and run correctly
- Test all external links (GitHub, external resources)
- Check all internal cross-references resolve properly
- Ensure examples work with documented Codon version
- Validate MkDocs rendering: `mkdocs serve` and check all pages
- Test code snippets on both Linux and macOS systems

## Dependencies

- MkDocs (site generator)
- MkDocs Material theme (documentation theme)
- MathJax (mathematical notation rendering)
- Python 3.7+ (for MkDocs tooling)

## Related Documentation

- Building Codon: See `docs/developers/build.md`
- Contribution Guidelines: See `docs/developers/contribute.md`
- Project Roadmap: See `docs/developers/roadmap.md`

<!-- MANUAL: Update when major version released, API changes, or new features added -->
