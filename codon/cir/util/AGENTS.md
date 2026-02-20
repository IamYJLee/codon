# CIR Utilities Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

Utility classes and functions for CIR manipulation, including cloning, formatting, inlining, matching, outlining, and visitor infrastructure.

## Key Files

- `cloning.h/cpp` - IR cloning utilities (CloneVisitor for deep copying IR nodes)
- `format.h/cpp` - IR pretty-printing and formatting
- `inlining.h/cpp` - Function inlining utilities
- `irtools.h/cpp` - General IR manipulation tools
- `iterators.h` - Iterator utilities for IR traversal
- `matching.h/cpp` - Pattern matching infrastructure for IR transformations
- `operator.h` - Operator overloading utilities for IR node operations
- `outlining.h/cpp` - Code outlining (extract code into separate functions)
- `packs.h` - Variadic pack manipulation utilities
- `side_effect.h/cpp` - Side effect analysis utilities
- `visitor.h/cpp` - Visitor pattern infrastructure (Visitor, ConstVisitor base classes)
- `context.h` - Context management for IR operations

## For AI Agents

**Core Utilities:**

**Visitor Pattern:**
- `Visitor` - Mutable visitor base class for IR transformation
- `ConstVisitor` - Immutable visitor for IR analysis
- All IR node types accept visitors for double dispatch
- Extend visitor classes for custom traversals

**Cloning:**
- `CloneVisitor` - Deep copy IR subgraphs
- Preserves structure and relationships
- Used by inlining, outlining, and code duplication

**Matching:**
- Pattern matching for IR node identification
- Used by optimization passes to recognize patterns
- Rewrite rules apply matched transformations

**Inlining/Outlining:**
- `inlining.h` - Inline function calls into call sites
- `outlining.h` - Extract code regions into functions
- Used for optimization and code organization

**Formatting:**
- Pretty-print IR for debugging and visualization
- Human-readable IR representation
- Useful for testing and debugging transformations

**Common Operations:**
```cpp
// Clone IR subtree
auto cloned = CloneVisitor().clone(original);

// Format IR for display
std::string ir_text = format(module);

// Pattern matching
if (auto *call = match<CallInstr>(instr)) {
    // Transform call instruction
}

// Visit all nodes
class MyVisitor : public util::Visitor {
    void visit(CallInstr *instr) override {
        // Process call
    }
};
```

## Dependencies

- Depends on: `codon/cir/base.h`, `codon/util/common.h`
- Used by: All CIR components (transforms, analyses, codegen)
