# Semantic-C speed lane

This tree contains behaviorally reconstructed C that is not yet claimed to
reproduce the original machine code.

The byte-exact build under `assets/code/` remains authoritative and unchanged.
When a semantic source later becomes byte-exact, promote it through the normal
overlay adoption workflow rather than editing this tree in place.

Overlay files use the existing address convention:

```text
semantic/overlays/resource_NNN_c_0200AAAA.c
```

Main-image files use `semantic/main/080AAAAA.c`. Both layouts preserve the
existing `Func_ADDRESS` entry symbol.

Each file must define `Func_0200aaaa`, compile as freestanding ARM7TDMI C, and
contain no inline assembly. Validate the complete semantic lane with:

```sh
bun run build:semantic
```

Admission means:

1. the complete executable owner and its control flow were reconstructed from
   clean project evidence; a manifest row or decompiler seed is not sufficient
   when live frame or register state continues into another range;
2. the C compiles for the target ABI;
3. calls, memory accesses, signedness, and observable side effects were
   reviewed against the disassembly;
4. known uncertainties are recorded next to the source.

Admission does **not** mean byte-exact code generation or historically original
source. Those remain separate later stages.

`out/decomp/overlays.json` supplies normal region boundaries. When that
diagnostic inventory misses a genuine outer owner and seeds hidden-context
entries inside it, record the reviewed prologue-to-return span and its evidence
in `semantic/regions.json`; never rename an internal seed into a fake function.

For throughput, assign one complete owner to each fresh agent and require an
accounting of every assembly call. Use m2c output as a drafting hint, not as
authority: missing stack arguments, fake thunk arguments, unset call-clobbered
values, and conflated high-register lifetimes must be recovered from assembly
dataflow and typed call-site evidence. Work in independent three-owner cohorts,
run `bun run build:semantic` during drafting, and run one `bun run verify`
before banking the settled cohort.

Before assigning a candidate labelled `split_first` or
`merge_with_continuations`, resolve its transitive continuation graph and size
the complete owner. The queue row may describe only a dispatcher or first
fragment, so using its byte count as the work budget can turn a nominally small
task into an unplanned multi-kilobyte rewrite.
