# IR Lowering Transform Directory

<!-- Parent: ../AGENTS.md -->
<!-- Generated: 2026-02-09 | Updated: 2026-02-09 -->

## Purpose

High-level IR lowering passes for translating complex language constructs into simpler, more primitive IR operations.

## Key Files

- `async_for.h/cpp` - Async for-loop lowering to coroutine-based iteration
- `await.h/cpp` - Async/await lowering to coroutine state machines
- `imperative.h/cpp` - Imperative construct lowering (loops, conditionals)
- `pipeline.h/cpp` - Pipeline operator lowering to function composition

## For AI Agents

**Lowering Passes:**
- **Async/Await**: Transforms async functions into coroutine state machines
- **Async For**: Converts async iteration into coroutine-based loops
- **Pipeline**: Lowers `|>` pipeline operators to function calls
- **Imperative**: Simplifies complex control flow constructs

**Lowering Strategy:**
- High-level → Mid-level → Low-level IR
- Each pass simplifies specific language features
- Enables backend code generation by removing complex constructs
- Preserves semantics while simplifying representation

**Execution Order:**
- Typically runs after type checking and before optimization
- Some lowering enables further optimization opportunities
- Final lowering before LLVM code generation

## Dependencies

- Depends on: `codon/cir/transform/pass.h`, `codon/cir/flow.h`, `codon/cir/func.h`
- Used by: Compilation pipeline between frontend and optimizer
