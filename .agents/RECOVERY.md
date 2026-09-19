# Recovery

Recover matching C, reuse what already works, and keep the build honest. Start
with the nearest verified example, not a blank page or another framework.

## 1. Select a family that can actually add C

Use current production ownership and coverage to distinguish exact C, retained
assembly, draft C and unknown code (see [EVIDENCE](EVIDENCE.md) on stale
reports). Inspect existing tools and their help before writing scripts.

Find unresolved owners with an exact sibling in our own corpus. Compare
reference control flow, calls, data accesses and constants, not just
filenames, function size or a normalized similarity score. Rank bounded
families by expected new exact-C bytes per hour, then genuinely unresolved
bytes. Prefer batches capable of moving DONE by at least 0.1 points; do not
skip small owners when many related matches make a useful batch.

Record the starting owners, complete extents, sibling source, current
residuals, expected gain and time budget before editing. Count assembly-to-C
conversions as new C at their full extent, but subtract any existing
retained-assembly credit when forecasting DONE. Renames, merges, new drafts,
better similarity and tooling changes add no DONE credit. Do not favor
overlays or main-image code merely because of layout.

No credible sibling? Select a coherent module with understood calls and types.
Recover one representative and then test its siblings. Do not launch blind
ROM-wide drafting or permutation.

## 2. Gather enough context

A bounded assignment contains:

- Resource-qualified owner identities, complete extents and canonical source.
- Reference instructions and the closest verified C sibling.
- Shared types, globals, helper signatures, compiler route and unit membership.
- The working compile and diff command, baseline, budget and integration owner.

Read the sibling and target together. Retarget constants, calls, offsets,
signedness and types from the target reference. Similar assembly does not mean
literal copy-and-paste is valid. Account for pointer returns and unused
results, indirect-call idioms, aggregate copies and overlapping lifetimes
explicitly. Do not invent structures or rewrite calls merely to obtain a
better score.

Reuse established headers. When a decompiler supports type or union hints,
give it evidence-backed context; otherwise repair the ordinary C directly. The
native decompiler is `psynergy decompile`; it takes bytes and explicit
addresses, not Golden Sun owner identities. Alchemy supplies project ownership
and build context. Melee's PowerPC m2c options do not automatically apply to
Thumb. Do not build a second decompiler or context registry during a recovery
batch.

## 3. Run the short loop

```sh
./alchemy extract <owner> --out out/owner.bin
./psynergy decompile out/owner.bin --base <address> --entry <address> --span <bytes> --out out/candidate.c
./alchemy inspect <owner>
./alchemy score <candidate.c> --owner <owner> --align --first
./alchemy score --unit <unit-id> --first
```

Keep one canonical candidate. Change one evidenced source hypothesis, compile
the affected object or unit, read the complete residual, and retain a supported
improvement. Use `--work out/<experiment>` for native diagnostics; a relative
work path outside `out/` litters the checkout and fails `make verify`. Do not
run full asset, coverage or twelve-edition checks after every source spelling.

Extraction applies overlay loading before slicing and reports the runtime base,
entry and span; use those printed addresses for Psynergy, not the owner's
resource address. Keep these local reference bytes under ignored `out/`.
Psynergy knows no default ROM, owner register, scene layout or compiler route.
Its output is a draft using the caller's `types.h` aliases; resolve project
symbols and review its control flow before scoring. Extraction and
decompilation refuse to overwrite an existing output.

Use the existing declaration in `games/<game>/recon/translation-units.json`.
For a main module that needs one:

```sh
./alchemy unit scaffold tbs <unit-id> <start-hex> <end-hex>
```

Register the unit before scoring it. Preserve function order and shared
declarations, including exact neighbors. Declare whole-function assembly holes
explicitly; they earn no C credit. Verify every affected owner from the one
shared object. A working unit is a reconstruction hypothesis, not proof of
Camelot's original file boundaries. Do not combine unrelated functions or
different overlay address spaces to meet a file-count target.

For a persistent local mismatch, walk back the complete draft to a smaller
experimental C scaffold and leave the unresolved owner on temporary assembly.
Freeze an explicitly bounded, byte-compared local invariant, then restore one
coherent source section at a time. Reject expansions that lose the invariant;
the first failing expansion identifies the context to reconstruct next. These
partial witnesses stay under ignored `out/`, earn no whole-owner C credit and
do not shorten reviewed extents. Retire assembly only after the complete owner
and its integration pass, not merely because an isolated section matches.

## Reading residuals

Fix the largest coherent disagreement first:

| Residual | Action |
| --- | --- |
| Boundary or binding | Resolve the complete owner and actual runtime symbols before searching C. |
| Control flow | Correct branches, loops, switches and omitted effects using the reference and sibling. |
| Type or width | Establish offsets, access widths, signedness, pointer and aggregate types. |
| Missing or extra code | Recheck extent, helpers, calls, guards and side effects. |
| Allocation, scheduling or pools | Read the relevant compiler pass; repair ordinary C lifetimes or structure. |
| Unclassified | Investigate within the budget; do not assign an invented playbook. |

Facts residuals have taught us:

- **Argument setup order is evidence about the callee's return type.** A call
  to a `void` function loads `r0` first; a value-returning one loads it last.
  A wrong order is a wrong prototype, not a spelling choice.
- **Differing halfwords on a large owner measure drift, not distance.** One
  early difference shifts everything after it. When the extent is short,
  normalize branch targets out of the diff before comparing.
- **An inert construct is a claim the compiler never supported.** A cast,
  store or block that changes nothing in the output is removed, not kept.
- **Shared assembly tails can come from separate source branches.** Do not
  pre-merge a common tail merely because it is shared in the final listing.
  GCC may schedule each branch before a later pass merges them; source-level
  merging changes that scheduling context. Compare the relevant pass dumps.
- **Ordinary spellings first.** Signed `/` and `%` are ordinary C: GCC emits
  `__divsi3` and `__modsi3` calls, which the unit's `absolute_symbols` and the
  production link bind to the runtime veneers. A variable initialized to a
  value the compiler provably has to hold is ordinary C; a store overwritten
  before it is read, or a duplicate store, is a trick (see
  [COMPILER](COMPILER.md)).

The triage router's `next=` is advice, not authority over the reference. A
`repair_hint=` admits a guarded experiment, not a global replacement. For
allocation evidence use `alchemy score <candidate.c> --allocator-order`. To
read native compiler decisions, run `alchemy build allocator <owner>`, then
`psynergy inspect allocator <printed-dump-directory>`. GCC's `-da` dumps expose
creation, CSE, combine, allocation, reload and scheduling.

After five unsuccessful source hypotheses, hand off the concrete residual or
park the owner and move to another family. Two bounded searches with no new
structural fact close that axis. A small persistent mismatch is a constraint to
explain, not a reason for unlimited random spelling. Reopen only with new
evidence, such as a tool that has improved since. Fixing a large semantic
mismatch may be useful even before it matches, but it remains a draft and must
not be reported as adoption.

## Owners, extents and bindings

A coverage span is not automatically a function. Follow continuations, shared
epilogues, inherited live registers and long-branch veneers through the
complete owner, including pools. Never shorten its expected extent to fit a
candidate. For reviewed main spans, use
`alchemy score SOURCE --owner <head> --size <complete-bytes>`. Read correctly
aligned pool data and preserve address modulo four.

Overlay identities are resource-qualified. Code links at the loader runtime
base `0x02008000`; resource offsets are not runtime call addresses. Require
inverse serialization and load round trips. The loader rewrites BL-shaped
pairs even inside literal data. Never alter compiler words or infer a binding
from candidate instruction position.

The boundary resolver owns reviewed overlay extents. `--size` and legacy
`--span` must equal the complete reviewed owner; overlaps block adoption.
Ambiguous legacy aliases need distinct declarations and explicit runtime
bindings. Model a `bx rN` call-via slot as a typed indirect call, even if a
direct call happens to emit identical bytes. Similarity and topology equality
do not replace linked byte comparison.

## 4. Multiply proved repairs, not guesses

When one owner closes, attempt its unresolved siblings before selecting an
unrelated target. Confirm target-side preconditions separately and score every
complete owner. Use each target's declarations and bindings, not the sibling's
addresses. Preserve exact neighbors; never adopt overlapping units
independently. Twins found by `alchemy inspect <owner> --siblings` join one
unit as instances ([SHARING](SHARING.md)).

`games/THE BROKEN SEAL/recon/compiler-repair-patterns.json` is the executable
repair catalog. `alchemy match` runs decoder-named, bounded catalog operations.
A new generalized operation needs safety guards, a regression fixture and an
actual closure; its acceptance tests belong in `make test`. Manual
sibling-guided C recovery does not require inventing a catalog operation.
Permutation is last-mile work after structure and types are credible.

## 5. Integrate

Use `alchemy adopt <overlay>:<address> --source <candidate.c>` for standalone
overlay owners. For standalone main-image owners, register the name and source
path, place accepted address-named candidates in a bounded `out/` directory,
then use `alchemy check integrate <directory> --apply`. Both paths must verify
the complete owner before retiring assembly. Install a shared unit only after
every affected owner is exact from the shared object; isolated adoption cannot
prove a unit.

The Lost Age adopts overlay C through the same listing route:
`alchemy overlay adopt <overlay>:<address> --source FILE --span N --target tla-en`
needs the owner's reviewed span in `games/THE LOST AGE/semantic/regions.json`,
its name and source in that game's register and, for a module of several
owners, a unit in `games/THE LOST AGE/recon/translation-units.json`. Each
game's manifest declares only its own units and compiles them with its own
route, register and ROM; `alchemy score --unit <unit-id> --target tla` proves
one.

Consolidate related functions when the resulting compilation context is proven.
Use `alchemy unit flatten --help` for complete-overlay consolidation.
Compiler-generated inter-function alignment may be declared in an exact
overlay unit's `compiler_gaps` as a two-byte `start`/`end` range between
adjacent exact owners, including owners in separate units. Preserve the
compiler-requested section alignment; the admitted assembler or native linker
must emit the gap in the preceding function section, and verification must
compare those emitted bytes with the loaded reference before coverage counts
them. Never manufacture fill from reference bytes. Keep function extents unchanged;
compiler fill is not handwritten or library assembly. Keep genuine assembly
and data separate. Do not trade byte equality for tidiness or delay productive
recovery for unrelated folder cleanup.

Run the integration gate once per finished coherent batch, repair failures,
then commit before starting another wave ([BUILD](BUILD.md)). Commits do not
require a DONE increase: verified cleanup, consolidation, corrections and
recorded negative results may be committed on their own. The 0.1-point
preference guides selection; it is not a commit gate. Never lower the
acceptance bar to hit a percentage.
