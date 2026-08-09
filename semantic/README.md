# Semantic C

This tree contains behaviorally reconstructed C that is not yet claimed to
reproduce the original machine code.

The byte-exact build under `exact/` remains authoritative. When a semantic
source becomes byte-exact, promote it through the reviewed, owner-specific
adoption workflow; moving a file by hand does not establish ownership or update
progress.

This directory is flat: main-image and overlay files sit side by side,
distinguished by filename convention rather than subdirectory --

```text
semantic/resource_NNN_c_0200aaaa.c   (overlay; `Func_0200aaaa`)
semantic/080aaaaa.c                  (main-image; `Func_080aaaaa`)
```

The primary entry symbol follows the address bank in the filename: overlay
sources define `Func_0200aaaa`, while main-image sources define
`Func_080aaaaa` (lowercase hexadecimal in both patterns). `exact/` follows the
same stem convention for byte-exact counterparts.

Main-image exact candidates pass through `integrate_matches`, which installs a
proved `src_<main-address>.c` draft as `exact/<main-address>.c`. Code-overlay
candidates pass through `overlay_adopt`, which installs a proved
`resource_<overlay>_c_<overlay-address>.c` source as
`exact/resource_<overlay>_c_<overlay-address>.c`.

Main-image admissions are promoted from the hand-reviewed candidate queue using
approved-ROM and local disassembly/build evidence: each one already represented
the corresponding ordinary compiler-output region but was parked because its
generated instructions were not byte-identical. Admission removes that
byte-exact constraint while retaining every other semantic requirement below.
The normal manifest boundary is only a seed -- follow live stack and
callee-saved-register state across direct continuation branches. If one owner
occupies noncontiguous executable ranges separated by pools, alignment, or
data, record all reviewed ranges in `semantic/main-regions.json` and exclude
the gaps; do not admit or count only the convenient first range.

Each file must define the primary entry symbol matching its filename pattern,
compile as freestanding ARM7TDMI C, and contain no inline assembly. Validate
the complete semantic tree with:

```sh
make inventory       # required input: out/decomp/overlays.json
make build-semantic
```

The inventory file is required input for semantic compilation and overlay
adoption, but its discovery rows are not byte-exact acceptance proof. An
explicit overlay span may be reviewed even when discovery did not list it.

Admission means:

1. the complete executable owner and its control flow were reconstructed from
   approved-ROM and local project evidence; a manifest row or decompiler seed
   is not sufficient when live frame or register state continues into another
   range;
2. the C compiles for the target ABI;
3. calls, memory accesses, signedness, and observable side effects were
   reviewed against the disassembly;
4. known uncertainties are recorded next to the source.

Admission does **not** mean byte-exact code generation or historically original
source. Those remain separate later stages.

## Ready for byte matching

Semantic admission and matching readiness are deliberately separate. Before a
source enters a broad shape or compiler search, confirm that:

1. every function and entry point in the owner has the reviewed boundary and
   ABI implied by local callers, branches, and data references;
2. types, signedness, aliases, calls, stack arguments, side effects, and control
   flow explain the disassembly rather than merely producing compilable C;
3. candidate size is plausible and the residual is localized into understood
   clusters; and
4. each proposed transform preserves the reviewed behavior.

A wrong-size or large diffuse residual is a reconstruction failure signal.
Return to boundaries, types, and source structure before spending a larger
permutation or compiler-mode budget. For a multi-function owner, score and
diagnose the functions separately even when the manifest requires atomic
adoption of the whole owner.

`out/decomp/overlays.json` supplies normal region boundaries. When that
diagnostic inventory misses a genuine outer owner and seeds hidden-context
entries inside it, record the reviewed prologue-to-return span and its evidence
in `semantic/regions.json`; never rename an internal seed into a fake function.

Reconstruct one complete owner at a time and account for every assembly call.
Use automatic pseudocode as a drafting hint, not as authority: missing stack arguments,
fake thunk arguments, unset call-clobbered values, and conflated high-register
lifetimes must be recovered from assembly dataflow and typed call-site evidence.
Run `make build-semantic` during drafting and `make verify` before
committing a settled group.

Before selecting a candidate labelled `split_first` or
`merge_with_continuations`, resolve its transitive continuation graph and size
the complete owner. The queue row may describe only a dispatcher or first
fragment, so using its byte count as the work budget can turn a nominally small
task into an unplanned multi-kilobyte rewrite.

The complete matching and escalation loop is in
[`../CONTRIBUTING.md`](../CONTRIBUTING.md); command discovery is in
[`../docs/TOOLS.md`](../docs/TOOLS.md).
