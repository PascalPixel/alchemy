# Contributing to Alchemy

Finish the game, one coherent module at a time.

Alchemy reconstructs _Golden Sun_ and _Golden Sun: The Lost Age_ as readable,
ordinary C that reproduces the shipped ROMs. Japanese releases are the source
editions; localizations are measured differences. GS1 English is currently the
complete byte-identical production build, not proof that its code is all C.

The model is a game source tree with a reliable build. Pokeemerald is useful
methodological inspiration: related functions share C files and headers,
objects have explicit link order, and a normal edit rebuilds affected inputs
before comparing the ROM. Its compiler choices, assembly escape hatches, source
names and game-specific implementations are not Alchemy's reconstruction rules.

## What 100% means

Completion requires all of the following, not a rounded percentage:

- Every executable byte, including overlay code and gaps outside registered
  functions, belongs to a complete audited owner or an explained nonfunction
  region. No unknown code, unscored fragments or missing bindings remain.
- Every C claim is ordinary C used by production for its complete extent, with
  zero linked differences under the approved compiler. Names, plausible
  behavior, matching size and near matches do not establish this.
- Every retained assembly claim has independent, recorded compiler or
  machine-code evidence explaining why ordinary C under the approved route
  cannot emit it. Search exhaustion, a scheduling mismatch, repeated scripts,
  or an attractive percentage is not that evidence.
- Assets rebuild from their maintained source inputs. No unfinished code is
  filled from the reference ROM.
- Each of the twelve target ROMs builds and compares byte-identically from a
  clean checkout with approved local inputs. Correspondence and linked-owner
  checks are intermediate proofs, not complete ROM builds.
- The source is organized by evidenced subsystem and module, with shared
  interfaces in headers and no redundant per-function scaffolding left behind.

Proven C and Proven ASM describe verified implementation routes, not knowledge
of Camelot's original source language. Report exact C, justified ASM, unresolved
bytes and complete-ROM target coverage separately. A recovered C implementation
disproves an incompatible ASM classification: reconcile its complete affected
range immediately, without double-counting the bytes.

## Work on a module, not a pile of functions

### 1. Select a bounded piece of the game

Inspect the current tree, production coverage and candidate scores to find
work; historical dossiers are leads, not authority.

Rank complete modules by genuinely unresolved bytes, demonstrated sibling
repairs and the work needed to close them. Count the expected gain after
subtracting bytes already credited as C or ASM. Prefer closure batches large
enough to move DONE by at least 0.1 percentage points; do not abandon the small
or difficult remainder when it becomes the work left to reach 100%.

Choose both main-image and overlay work on that basis. A count such as "64
finished overlays" is useful only when each overlay has no unresolved executable
bytes, including unregistered gaps. Flattening files alone adds no DONE bytes.
Similar code does not prove shared source or an automatic repair.

Before editing, state the unit, complete owners, current residuals, expected
unresolved-byte gain and a finite experiment budget. Fix binding or boundary
failures that prevent this unit being scored before searching its source.

### 2. Establish the complete compilation context

Use the existing declaration in `games/<game>/recon/translation-units.json`.
For main-image work without one, scaffold an address-ordered working unit:

```sh
./alchemy unit scaffold gs1 <unit-id> <start-hex> <end-hex>
./alchemy diff --unit <unit-id> --first
```

Register the scaffold's manifest entry before scoring it. Include existing exact
C and candidate C in their evidenced order; declare whole-function assembly
holes explicitly. No hole earns C credit. Exact neighbors must remain exact
from the shared object. A retained overlay candidate unit may declare runtime
bindings for scoring, but stays outside production until adopted.

Recover common types, global layouts, function-pointer signatures, constants
and helper relationships from reference access patterns and verified calls.
Resolve conflicting declarations together. Give the decompiler this context
and credible exact siblings; do not independently regenerate each function with
different guesses about the same interface.

A reconstruction unit is a working hypothesis, not a claim that the original
source file has been recovered. Do not combine unrelated functions merely to
reach a file-count target. An overlay is a separate loadable module, not
necessarily one historical C file; do not merge distinct runtime address spaces
into one compilation unit.

### 3. Recover and repair ordinary C

Recover the algorithm, branches, loops, widths, signedness, object layouts,
calls and lifetimes in C89. Preserve observable game behavior, including bugs.
Do not encode optimized instructions as C statements just to resemble a listing.

```sh
./alchemy decompile <owner>
./alchemy inspect <owner>
./alchemy diff <candidate.c> --owner <owner> --align --first
./alchemy diff --unit <unit-id> --first
```

Compile the unit once and inspect every affected owner. Fix the largest
coherent disagreement first:

| Residual | Next action |
| --- | --- |
| Boundary, split region, missing binding | Establish the complete owner and actual runtime symbols. |
| Structural topology | Correct the control-flow model and shared interfaces before register work. |
| Type or width | Prove access widths, signedness and aggregate layout from the reference. |
| Missing or extra code | Recheck owner extent, calls, guards and omitted side effects. |
| Call target | Correct the declaration and binding; never compensate with source-position arithmetic. |
| Allocation, scheduling or pool placement | Read the deciding compiler pass, then change the responsible C structure. |
| Uncovered or unclassified | Make a bounded evidence-led investigation; do not assign a blind repair wave. |

Follow the router's `next=` result. A `repair_hint=` admits one guarded
experiment, not a global replacement. Retain only a measured improvement and
adopt only an exact result. If the suggested route contradicts the reference,
record the contradiction and correct the classification; do not obey stale
metadata or bypass verification.

For allocator and scheduling residuals, use `./alchemy inspect allocator
<owner>` or `./alchemy diff <candidate.c> --allocator-order`. GCC's `-da`
dumps expose RTL creation, CSE, combine, allocation, reload and scheduling.
Read the source of the pass making the decision. Search ordinary C, never
force a register assignment or patch the compiler.

A persistent small mismatch is a constraint to explain, not permission for an
endless spelling search. After distinct source hypotheses reproduce it,
identify source that naturally emits the required local structure, preserve
that structure as an admission check, and repair outward. If the budget ends
without a supported repair, record why and select another bounded unit.

### 4. Prove a repair before multiplying it

Close a representative owner before applying its source shape to siblings.
Check the same reference-side preconditions on each sibling; similar code may
have different constants, calls, widths or control flow.

The executable catalog is
`games/gs1/recon/compiler-repair-patterns.json`. A new operation needs a named
decoder signal, safety guards, finite search space, regression fixture and an
actual closure. `./alchemy match` runs only decoder-named catalog operations;
its acceptance fixtures are part of `make test`. Permutation is last-mile work
after the structure is credible, not a substitute for missing types or loops.

Apply a proven repair to a bounded group of related owners, scoring each complete
extent. Inspect siblings in their maintained source; do not generate another
cohort, template pack or parallel type context. Shared types belong in game
headers once established from evidence. Never adopt overlapping units
independently.

End a batch with exact-C bytes added, newly resolved bytes, parked bytes,
residual classes and time spent. A zero-adoption batch does not justify a
larger repeat of the same search. Use the result to choose the next repair.

### 5. Adopt, consolidate and finish the unit

Use `./alchemy adopt <owner> --source <candidate.c>` for a supported standalone
owner. For shared units, install the source and retire assembly together only
after every affected owner is exact from the shared object. Do not route a
shared owner through an isolated adoption path.

Consolidate related production functions into a named module after proving the
resulting compilation context. Use `./alchemy unit flatten --help` for the
existing overlay consolidation route. A completed module should not retain a
second forest of one-function files, duplicate declarations and include wrappers.
Keep genuine assembly and data separate. Never sacrifice a match for tidiness.

Establish Japanese correspondence before calling EN source shared game code.
Then verify the other localizations' real constants, bindings and extents.
Transfer to GS2 only where evidence supports shared behavior; recover its
differences explicitly. Close each game's main image and overlays, then the
remaining cross-edition and asset gaps. Do not count one game's recovery twelve
times.

Verify and commit each finished coherent batch before another wave. Keep the
full production build green throughout; do not postpone integration until a
large private pile of drafts has accumulated.

## Boundaries and overlays

A coverage interval or dossier span is not a function boundary. A branch into
a region without a prologue, live saved registers inherited from another region,
a shared epilogue or a long-branch veneer may identify a continuation. Recover
the whole function from its entry through the terminal fragment, including its
pools. Do not score a fragment as a standalone owner or shorten the expected
extent to fit the candidate.

For a reviewed split main owner, `diff --owner <head> --size <complete-bytes>`
scores the complete span. A topology decoder's lack of coverage does not
invalidate the byte comparison or prove compiler impossibility. Read pool
constants from the correctly aligned reference or object, not guessed listing
displacements; preserve the owner's address modulo four when assembling it.

Overlay identities are resource-qualified because load addresses repeat.
Offsets identify the resource; C is compiled and linked at the loader runtime
base, `0x02008000`. Require the inverse serialization and load round trip.
The loader rewrites BL-shaped pairs even inside literal-pool data. Never alter
GCC's words or choose bindings from candidate instruction positions.

The shared boundary resolver owns reviewed overlay extents. A retained
`--span` must equal the complete reviewed extent; it cannot grow to fit the
candidate. A disagreement or overlap blocks adoption pending a boundary audit.
Ambiguous legacy call names require distinct declarations and explicit runtime
symbols. Model a `bx rN` call-via slot as a typed indirect call, not a direct
declaration, even if the latter happens to emit identical bytes.

Main retained-ASM evidence is exposed by the assembly manifest; overlay evidence
lives in `games/gs1/semantic/overlay-assembly.json`. Entries marked `strong`
remain Draft ASM. Reconstructed scenes use `structured_scene_module`; the
retired `generated_call_script_module` is not an alternative proof category.
Missing evidence stays unresolved rather than becoming "compiler limitation."

## The compiler is evidence, not a matching knob

Use the approved `agbcc` and `agscc` submodules and compiler bundle. Game code
uses the canonical GCC 2.96 family; the recorded prebuilt-library families have
their own single routes. Membership follows provenance, not which flags score
best. `tools/compiler-core/src/routing.rs` owns the compiler and assembler
commands. No per-function flags, inline assembly, fixed-register variables,
empty barriers, forced scheduling, output patches or selected lucky runs.

**Do not modify agscc unless restoring the historical stock GCC 2.96 shipped
by Red Hat.** Every proposed change, including diagnostic or host-port changes,
requires Pascal's approval, the specific historical release and source package
or vendor patches, evidence of the discrepancy, and proof of restored fidelity.
Better scores, determinism, newer GCC behavior or a matching ROM are not
authorization. A version string or approved pin is not historical provenance.

Pins, executable hashes, family routes and compiler-output transformations are
maintainer-owned. Only Pascal may authorize an independently justified change.
A genuinely different shipped compiler family must be demonstrated across that
family, not invented to rescue one owner. Never weaken the guard that checks it.

The approved bundle uses historical GNU GAS 2.10 for compiler output and unit
slices, including its alignment fill. Modern binutils handles retained syntax,
symbols and linking with the recorded integer/soft-float ABI. Do not suppress
ABI mismatches. Cache identity includes the complete approved executable bundle.
The restored bundle is admitted on Apple Silicon macOS; other hosts require a
source build and full verification. Keep third-party source untouched.

## Work cleanly

Use locally held reference ROMs, independently reconstructed material here,
decoded data, verified calls and relocations, and public C, ARM, compiler and
file-format documentation. Compiler source explains compiler behavior only.
Do not inspect or copy leaked source or another Golden Sun reconstruction's
code, symbols, types, comments or target-specific conclusions. Methodology from
unrelated projects is not permission to import their game-specific material.

Never commit or transmit ROMs, ROM fragments, extracted binaries, objects, ELFs,
compiler output or patches containing reference bytes. Required assets are
rebuilt from approved local inputs; do not fill unfinished code from those
inputs. Preserve unrelated work and do not reset registers or source to an
older commit to make a gate pass.

README is the public introduction. **CONTRIBUTING is the only contributor
guide.** AGENTS and CLAUDE files, if needed, are pointers only, never additional
instructions. Do not create other Markdown or plain-text reports, plans,
handoffs or guides anywhere for this project, including ignored `out/`,
`scratch/` or temporary directories. Ignoring a file or changing its extension
does not create an exception. Do not hide prose guides inside JSON either.

Put a durable procedure here, a code constraint beside its source, and an
owner-specific result in the existing structured evidence record. Keep raw
diagnostics on stdout or in the tool's native disposable artifact; maintain
machine-readable receipts needed to reproduce scores, not parallel narratives.
Third-party source, including GCC's non-Markdown `.md` machine descriptions,
is not agent-authored documentation and must not be damaged by cleanup.

Each fact has one authority: owner names and paths in
`games/<game>/source-paths.json`; compile-local composition in translation-unit
manifests; code and asset data in their sources; measured status in current
build outputs. Derived coverage and correspondence reports are not
editable authorities. Dossiers retain dated reasoning, not current scores.

Current build outputs own measured coverage. `make coverage` refreshes the ROM
coverage figures; it does not recompile draft C or publish a draft-class
scoreboard. Score candidates directly with `alchemy diff` and check overlay
coverage with the existing overlay audit. Old experiment outputs are not inputs
to the build or evidence of current candidate status.

## Setup and the short development loop

Install Rust, Ninja and `arm-none-eabi-binutils`. Supply approved ROMs under
ignored `roms/` and the approved bundle under `out/compilers/dist/`.

```sh
git submodule update --init
git config core.hooksPath .hooks
./alchemy --help
make compiler-source-check
```

Run submodule setup only in the main checkout, never a worktree. Worktrees may
symlink its ROMs and approved bundle. `./alchemy build compilers` builds the
pinned sources without authorizing a replacement bundle. The `./alchemy`
launcher builds the current contributor executable offline before dispatch.

While iterating, use `diff --unit` and the narrow relevant build:
`make build-claimed`, `make build-asm`, `make build-assets`,
`make overlay-check` or `make check-owners`. Use `./alchemy --help` and command
help for supported arguments. `overlay` remains transitional; it is not an
alternative around the adoption or catalog gates.

Before a normal commit, stage only intended files and run:

```sh
git diff --cached --check
make verify
make progress-subject
```

`make verify` checks the staged-tree contract and the production ROM, overlays,
units, ownership, ordinary C, compiler provenance and repository rules. It does
not rescore every draft, run all tests or build twelve complete ROMs. The hook
rejects a staged tree different from the verified one.

| Changed surface | Additional check |
| --- | --- |
| Rust tooling | `make test` |
| Shared edition or preprocessor logic | `make targets` |
| Retained-ASM classification | `make classification-check` |
| Candidate-corpus policy | `make candidate-corpus-check` |
| Ownership, labels or coverage claims | `make coverage` |
| Contributor tooling index | `make tooling-index-check` |

`make targets` is a compile check, not twelve full-ROM proofs. The candidate
audit separates installed C, complete nonexact candidates, nonowners and
unverified fragments; an unscored complete owner is a failure, not a parked
match. Repair verification failures before resuming affected work.

Use `make audit` for exhaustive release or large compiler/ownership checks and
`make reports` to regenerate analysis outputs, not in every function's edit
loop. At final completion, also require actual complete builds for all twelve
targets; a successful aggregate check cannot substitute for missing builds.

Commit subjects use the value from `make progress-subject`. Report only the
checks run and gains proved. Commit completed work before starting another
batch; push only when requested.

## Tooling index

Use existing tools to close modules. New machinery must address a demonstrated
recurring blocker, replace duplication where possible, and prove a real
conversion with a regression test. Do not build another framework, wrapper or
report system in place of recovering the selected module.

Portable tooling is capped at 50,000 Rust, TypeScript, JavaScript and CSS lines,
as checked by `make tooling-size`. The ceiling and its scope are Pascal's:
do not raise it or move code outside the counted set to admit a change.

The reusable working kit is deliberately small:

| Operation | Command |
| --- | --- |
| Recover and inspect C | `alchemy decompile`, `disassemble`, `inspect` |
| Compile and compare complete owners or units | `alchemy diff` |
| Search a bounded, justified C repair | `alchemy match` |
| Install verified C and consolidate modules | `alchemy adopt`, `unit` |
| Build, verify and measure the game | `alchemy build`, `verify`, `coverage` |

For another GBA game, carry the compiler invocation, instruction decoding,
byte comparison and bounded matching machinery, plus the minimum build
integration. Choose that game's compiler from evidence; Golden Sun's compiler
route, address map, owner register and asset formats are not portable defaults.
Do not copy a framework for worker waves, template packs, inferred aggregate
contexts or report inventories.

Alchemy also retains the dependencies its actual game build requires:
`build assets` and its Golden Sun codecs rebuild maintained data; `overlay` handles
the loader's relocation format; `cross-edition` checks edition differences;
`check` enforces source, classification, compiler and publication contracts.
These are project integration, not tools to take to another game.

Asset tooling is organized by format and conversion, never by resource number,
ROM address or the first game asset that needed it. Name a directional converter
for its actual inputs and outputs, such as `png2gba4bpp`; distinguish raw tiled
pixels, palettes and file containers. A bidirectional codec library may use the
format name. Do not label an SMSH source parser as a MIDI converter unless it
actually performs that conversion. Game-specific offsets, dimensions, table
layouts and compression plans belong in the maintained asset description, not
a separate executable or crate per resource. Share the implementation across
assets of the same format and prove their rebuilt bytes before deleting the old
route. Renaming hard-coded resource logic does not make it portable.

The asset builder's `typed-table` format encodes contiguous source-described
segments: `u8`, `s8`, `le-u16`, `le-u32`, and zero-terminated, padded `ascii-fixed`.
Addresses, extents, strides and consumer evidence stay in the asset source.
Integer ranges, segment continuity, text encoding and output sizes are checked
by the shared builder; do not create another crate for a particular table.
Byte index tables declare `index_count` and, when required, `permutation: true`
to check their domain and uniqueness without a game-specific validator.

The same table encoder supports signed little-endian halfwords, byte fills and
fixed-stride records with named fields. Bounds, uniqueness, terminated-array
capacity and zero-padded `capacity` are declared beside those fields, not
hard-coded in a game-specific crate. A manifest may select a table by JSON
pointer within a shared source. Integer values may be spelled symbolically: a
`0x` literal, a constant from the field's or table's `names`, a segment `name`
for that segment's address, or `name[index]` for the address of its element;
internal directories and animation tables reference their targets that way
instead of through a per-package Rust builder.
Tiled PNG inputs may declare frame dimensions and atlas columns; the ordinary
tile converter then writes frame order. Keep those layouts in the manifest.

`thumb-pointer` table segments resolve named main-image callbacks from the owner
register, retain null slots, and encode the Thumb tag. An unregistered target
remains an explicit aligned numeric address; do not invent an owner or name to
serialize a pointer. Signed words use `le-s32`. Shared lookup views and consumer
evidence stay beside their backing values, not in a second layout catalog.

`record-table` describes ordered named integer fields, their radix, common bias
and encoding; it checks sequential record indices before serializing. `pointer-table`
declares its base and table addresses and preserves explicit nulls and backward
aliases. Both use the manifest's checked extent; neither assumes a game address,
resource count or field layout in Rust.

The inventory remains mandatory, including game-specific and internal libraries.
Build dependency status is not an exemption from review or consolidation.

Build maintained asset packages with `alchemy build assets`; full-ROM comparison
verifies their production extents. The former `alchemy assets` command tree and
its per-package extraction/build CLIs are retired. Their required codecs remain
internal build dependencies pending format migration, not supported commands.
Do not reintroduce parallel package-specific build or verify commands.

Use `alchemy convert FORMAT INPUT OUTPUT` for file conversions: `words2bin`,
`pairs2bin`, `tilemap2bin`, `png2gba4bpp`, `png2gba8bpp`, `png2gbapal`, and `wav2pcm8`.
`wav2pcm8` reads canonical mono 8-bit PCM WAV and emits signed 8-bit sample bytes;
game-engine headers and looping metadata belong to the asset build, not the converter.
These have no game addresses or default ROMs and refuse to overwrite an existing
output. `alchemy font` retains the project's shared font reconstruction pipeline;
it is project integration, not part of the portable kit.

The following is the complete library and command index. Every crate is a
member of the one `tools/` Cargo workspace with its one lockfile.
`make tooling-index-check` checks every immediate tool directory exactly once;
libraries are not additional public command surfaces.

| Tool | Responsibility |
| --- | --- |
| [alchemy](tools/alchemy/) | Unified build, verify, coverage, check, convert, font, decompile, disassemble, inspect, diff, adopt, match, and cross-edition commands. Verification and coverage retain their Makefile contracts. The overlay subgroup is transitional until owner-aware dispatch replaces it. |
| [decompile](tools/decompile/) | Library behind `alchemy decompile`: recover candidate C from retained Thumb code, with owner-aware decoding and source recovery. |
| [alignment-tail](tools/alignment-tail/) | Model and verify alignment tails. |
| [asset-paths](tools/asset-paths/) | Own canonical tracked asset paths. |
| [build-asm](tools/build-asm/) | Assemble retained regions and emit their classified manifest. |
| [build-claimed](tools/build-claimed/) | Compile, verify, cache, and manifest exact-C owners. |
| [build-full](tools/build-full/) | Compose claimed C, retained assembly, and assets into the ROM. |
| [cache-entry](tools/cache-entry/) | Provide deterministic SQLite-backed cache entries and receipts. |
| [canonical-json](tools/canonical-json/) | Serialize stable JSON for reproducible reports and hashes. |
| [decomp-targets](tools/decomp-targets/) | Define the twelve historical ROM targets and build paths. |
| [gba-header](tools/gba-header/) | Encode and verify the GBA cartridge header. |
| [generated-files](tools/generated-files/) | Track generated-file identity and freshness. |
| [candidate-compiler](tools/candidate-compiler/) | Compile candidate C and expose verification primitives. |
| [diff](tools/diff/) | Score and explain structural, allocator, type, and code residuals. |
| [compiler-core](tools/compiler-core/) | Own compiler bundles, routes, symbols, paths, targets, and translation units. |
| [integrate-matches](tools/integrate-matches/) | Adopt byte-exact main-image C through the integration gate. |
| [matching](tools/matching/) | Execute finite, decoder-named source repairs. |
| [overlay-adopt](tools/overlay-adopt/) | Score, adopt, park, audit, and compare overlay candidates. |
| [disassemble](tools/disassemble/) | Disassemble and compile overlay-qualified owners. |
| [check-commit-progress](tools/check-commit-progress/) | Enforce progress-bearing commit subjects. |
| [check-publication](tools/check-publication/) | Fail closed on invalid staged changes and outgoing history. |
| [check-unmatchable](tools/check-unmatchable/) | Audit owner registers and unmatchable classifications. |
| [core-retained-audit](tools/core-retained-audit/) | Verify retained main-image assembly classifications and extents. |
| [coverage-map](tools/coverage-map/) | Build coverage metrics and SVG figures. |
| [full-c-progress](tools/full-c-progress/) | Report Proven C and DONE progress over audited executable intervals. |
| [no-asm-c](tools/no-asm-c/) | Enforce source boundaries between C and retained assembly. |
| [archive-asset](tools/archive-asset/) | Rebuild offset-table palette-LZ archives from authored plans and atlases. |
| [extract-resource](tools/extract-resource/) | Extract resource payloads from approved ROMs. |
| [import-asset](tools/import-asset/) | Import images, text and WAV PCM into binary formats; shared MTF4 and delta7 pixel encoders. |
| [audio-engine-data](tools/audio-engine-data/) | Encode tracked audio-engine tables and runtime data. |
| [battle-assets](tools/battle-assets/) | Rebuild battle screen, display, effect, and compressed resource packages. |
| [character-catalog](tools/character-catalog/) | Extract and verify character catalog data. |
| [encounter-data](tools/encounter-data/) | Extract and rebuild encounter data. |
| [f0-archive](tools/f0-archive/) | Extract and rebuild the F0 archive. |
| [kind1-map-grid](tools/kind1-map-grid/) | Export and verify kind-1 map grids. |
| [kind2-resources](tools/kind2-resources/) | Build and verify kind-2 resources. |
| [localization-font](tools/localization-font/) | Extract and rebuild localized font data. |
| [map-container-components](tools/map-container-components/) | Extract map-container component series. |
| [message-archive](tools/message-archive/) | Extract and rebuild message archives. |
| [music](tools/music/) | Extract and verify music data. |
| [music-residuals](tools/music-residuals/) | Build and verify audio residual packages. |
| [namae-nyuuryoku](tools/namae-nyuuryoku/) | Build the fixed name-entry screen package. |
| [resource-5](tools/resource-5/) | Decode and rebuild resource 5. |
| [runtime-support-data](tools/runtime-support-data/) | Build runtime-support data. |
| [simple-resources](tools/simple-resources/) | Rebuild maintained stamp, font, word-table and screen records. |
| [skip-sprite-archive](tools/skip-sprite-archive/) | Extract the skip-sprite archive. |
| [staff-roll](tools/staff-roll/) | Build the staff-roll package. |
| [static-sprite-series](tools/static-sprite-series/) | Build static sprite series from maintained images and compression plans. |
| [title-resources](tools/title-resources/) | Build title-screen resources. |

## Owners and names

Production source paths describe modules and evidenced jobs, not addresses or
one file per function. Use enough directories to express subsystems; do not
invent a hierarchy for each tiny helper. Related functions belong together once
their shared compilation context is proven. File count and line count are not
completion metrics, and pokeemerald's flat layout is not a required template.

Keep addresses in the owner register and ABI aliases, and overlay membership
in resource-qualified owner keys. Do not add address filenames, ordinal
placeholders, game codenames or resource-number folders. `Func_<address>` is an
ABI compatibility alias, not a second semantic name. Update the one register
and affected consumers together when consolidating sources.

Use a neutral Japanese commercial C vocabulary appropriate to 2000-2001:
`Subsystem_VerbObject` for evidenced roles; short locals such as `pos`, `cnt`,
`tbl`, `buf` and `work`. Unknown roles stay `unk`, `field_<offset>` or a raw
offset; reserve `pad` for proved padding. Avoid fan-lore names, fake Japanese,
modern framework terminology and comments impersonating the lost source.
Names are reading aids, not evidence of the original spelling.

Promote a declaration to a header only for a real shared interface, stable
layout or table family. Keep owner-specific compiler constraints short and
beside the expression they explain.

Map code belongs under `map/locations/<location>/` only when a resource or
verified call site establishes the place. Shared collision, rendering and
event code belongs under `map/shared/`. `games/gs1/locations.tsv` owns reviewed
resource-to-location mappings. Music titles require actual identification;
sequence numbers and guessed locations do not establish titles.
