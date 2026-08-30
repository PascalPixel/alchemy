# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the Game Boy Advance
releases of _Golden Sun_ and _Golden Sun: The Lost Age_. The Japanese releases
are the source editions; localized releases are measured deltas. English GS1 is
currently the only complete byte-identical build.

The goal is readable, ordinary C that reproduces the shipped bytes. A plausible
draft, a close compiler match, or a useful name is progress, but it is not DONE.
Only byte-exact C and assembly proved permanent count.

## Rules

### Work cleanly

Use only the reference ROMs held locally, independently reconstructed material
in this repository, decoded data, relocations, verified calls, and public
documentation about C, ARM, GCC, and file formats. Compiler source is evidence
about compiler behaviour only.

Do not inspect or copy leaked source, another Golden Sun decompilation, or its
symbols, pseudocode, types, comments, or target-specific conclusions. Candidate
names and neighboring drafts are leads, not evidence.

Never commit or transmit ROMs, ROM fragments, extracted binaries, objects,
ELFs, compiler output, or patches containing reference bytes. Generated work
belongs under ignored `out/` or `scratch/`.

### Keep exactness literal

Adopt an owner only when its linked `differing_halfwords` is zero and the
production build uses the source for its complete audited extent. Similarity,
matching size, and behavioral plausibility do not count.

Production C must compile through the recorded GCC 2.96 route. Do not use inline
assembly, fixed-register variables, empty barriers, copied instructions, forced
scheduling, or compiler patches to disguise an incorrect source model.

Permanent assembly needs evidence that the approved compiler cannot emit the
required instruction shape, or that the bytes are genuinely structural
assembly such as a veneer, alignment, or runtime primitive. Record that reason
in the appropriate main-image or overlay classification.

Never fill an unfinished build from the reference ROM.

## Setup

Install Rust, Ninja, and `arm-none-eabi-binutils`. Put approved ROMs in ignored
`roms/` and the approved compiler bundle in `alchemy-gcc/dist/`.

Configure the repository hooks once, then build the contributor host:

```sh
git config core.hooksPath .hooks
cargo build --offline --release --manifest-path tools/compiler/Cargo.toml
```

Worktrees may symlink `roms/` and `alchemy-gcc/dist/` from the main checkout.
Do not run submodule commands from a worktree.

## Tooling index

This is the authoritative index of the machinery beside the decompilation.
Every immediate tool directory with a Cargo manifest or executable script
appears exactly once. `make tooling-index-check` proves that this index and the
filesystem agree.

Contributor-facing work goes through six hosts. Run one with `cargo run
--offline --quiet --release --manifest-path tools/<host>/Cargo.toml --
<command>`; the Makefile supplies the usual routes. Libraries remain separate
for testing and provenance, not as public commands.

### Public hosts

| Tool | Responsibility |
| --- | --- |
| [assets](tools/assets/) | Archival extractors and game-specific asset/data codecs; `assets --list` names the commands. |
| [build-assets](tools/build-assets/) | Rebuild tracked assets and data packages from source. |
| [build-stage](tools/build-stage/) | Assemble retained regions, compile claimed C, and compose or verify the ROM. |
| [check](tools/check/) | Publication, ownership, retained-code, coverage, integration, routing, and progress checks. |
| [compiler](tools/compiler/) | Candidate scoring, families, cross-edition work, waves, permutation, and the dashboard. |
| [overlay](tools/overlay/) | Overlay scoring, reconstruction, adoption, parking, audit, twins, and disassembly. |

### Direct workflows

| Tool | Responsibility |
| --- | --- |
| [allocator-lens](tools/allocator-lens/) | Read GCC 2.96 pseudos, costs, conflicts, assignments, spills, and reloads. |
| [shape-search](tools/shape-search/) | Run bounded source-shape searches through the real scorer. |
| [unit-scaffold](tools/unit-scaffold/) | Scaffold declared translation units and owner composition. |
| [weyard-font](tools/weyard-font/) | Extract and verify the shared text font across historical ROMs. |

### Build and repository foundations

| Tool | Responsibility |
| --- | --- |
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

### Reconstruction and compiler analysis

| Tool | Responsibility |
| --- | --- |
| [candidate-compiler](tools/candidate-compiler/) | Compile candidate C and expose verification primitives. |
| [candidate-show](tools/candidate-show/) | Score and explain structural, allocator, type, and code residuals. |
| [compiler-core](tools/compiler-core/) | Own compiler bundles, routes, symbols, paths, targets, and translation units. |
| [integrate-matches](tools/integrate-matches/) | Adopt byte-exact main-image C through the integration gate. |
| [permuter](tools/permuter/) | Execute finite, decoder-named source repairs. |

### Overlay support

| Tool | Responsibility |
| --- | --- |
| [overlay-adopt](tools/overlay-adopt/) | Score, adopt, park, audit, and compare overlay candidates. |
| [overlay-call-targets](tools/overlay-call-targets/) | Decode overlay-specific call-target words. |
| [overlay-disasm](tools/overlay-disasm/) | Disassemble and compile overlay-qualified owners. |
| [overlay-show](tools/overlay-show/) | Render and reconstruct overlay owners and resources. |

### Verification and reporting

| Tool | Responsibility |
| --- | --- |
| [check-commit-progress](tools/check-commit-progress/) | Enforce progress-bearing commit subjects. |
| [check-publication](tools/check-publication/) | Fail closed on invalid staged changes and outgoing history. |
| [check-unmatchable](tools/check-unmatchable/) | Audit owner registers and unmatchable classifications. |
| [core-retained-audit](tools/core-retained-audit/) | Verify retained main-image assembly classifications and extents. |
| [coverage-map](tools/coverage-map/) | Build coverage metrics, SVGs, and dashboard data. |
| [dashboard-server](tools/dashboard-server/) | Serve the local reconstruction dashboard. |
| [full-c-progress](tools/full-c-progress/) | Report exact-C progress over audited executable intervals. |
| [no-asm-c](tools/no-asm-c/) | Enforce source boundaries between C and retained assembly. |

### Asset primitives

| Tool | Responsibility |
| --- | --- |
| [archive-asset](tools/archive-asset/) | Encode and decode archive-backed assets. |
| [export-asset](tools/export-asset/) | Export GBA graphics into editable assets. |
| [extract-resource](tools/extract-resource/) | Extract resource payloads from approved ROMs. |
| [import-asset](tools/import-asset/) | Import editable assets into GBA formats. |
| [map-load-table](tools/map-load-table/) | Encode and verify map-loading tables. |
| [pairtable](tools/pairtable/) | Decode paired table records. |
| [tilemap](tools/tilemap/) | Encode and decode GBA tilemaps. |
| [wordstream](tools/wordstream/) | Decode word-oriented ROM streams. |

### Game data and resource packages

These preserve the typed transform for committed game data. Most are commands
behind `assets` or libraries used by `build-assets`.

| Tool | Responsibility |
| --- | --- |
| [audio-engine-data](tools/audio-engine-data/) | Encode tracked audio-engine tables and runtime data. |
| [audio-wave](tools/audio-wave/) | Extract and verify waveform resources. |
| [battle-effect-data](tools/battle-effect-data/) | Build the battle-effect data package. |
| [byte-henkan](tools/byte-henkan/) | Build byte-conversion tables. |
| [byte-value-regions](tools/byte-value-regions/) | Build typed byte-value regions. |
| [character-catalog](tools/character-catalog/) | Extract and verify character catalog data. |
| [early-runtime-data](tools/early-runtime-data/) | Build the early-runtime data package. |
| [encounter-data](tools/encounter-data/) | Extract and rebuild encounter data. |
| [executable-gap-sources](tools/executable-gap-sources/) | Decode typed source packages in executable gaps. |
| [f0-archive](tools/f0-archive/) | Extract and rebuild the F0 archive. |
| [indexed-still](tools/indexed-still/) | Extract indexed still-image packages. |
| [kind1-map-grid](tools/kind1-map-grid/) | Export and verify kind-1 map grids. |
| [kind2-resources](tools/kind2-resources/) | Build and verify kind-2 resources. |
| [late-runtime-data](tools/late-runtime-data/) | Define typed layouts for late-runtime encoding. |
| [late-runtime-residual](tools/late-runtime-residual/) | Build late-runtime residual regions. |
| [localization-font](tools/localization-font/) | Extract and rebuild localized font data. |
| [localization-tables](tools/localization-tables/) | Build localization lookup tables. |
| [map-container-components](tools/map-container-components/) | Extract map-container component series. |
| [map-resources](tools/map-resources/) | Build and verify map-container resource series. |
| [message-archive](tools/message-archive/) | Extract and rebuild message archives. |
| [music](tools/music/) | Extract and verify music data. |
| [music-residuals](tools/music-residuals/) | Build and verify audio residual packages. |
| [namae-nyuuryoku](tools/namae-nyuuryoku/) | Build the fixed name-entry screen package. |
| [resource-01c](tools/resource-01c/) | Decode and rebuild resource 01C. |
| [resource-3ce](tools/resource-3ce/) | Decode and rebuild resource 3CE. |
| [resource-5](tools/resource-5/) | Decode and rebuild resource 5. |
| [resource-byte-canvases](tools/resource-byte-canvases/) | Encode byte-canvas resources. |
| [resource-d1-d3](tools/resource-d1-d3/) | Decode and rebuild resources D1 through D3. |
| [resource-directory](tools/resource-directory/) | Extract and verify the resource directory. |
| [runtime-support-data](tools/runtime-support-data/) | Build runtime-support data. |
| [sentou-gamen-data](tools/sentou-gamen-data/) | Build battle-screen data. |
| [sentou-hyouji](tools/sentou-hyouji/) | Build battle-display data. |
| [sentou-kouka-runtime](tools/sentou-kouka-runtime/) | Build and verify the battle-effect runtime. |
| [sentou-menu-data](tools/sentou-menu-data/) | Build battle-menu data. |
| [sentou-resources](tools/sentou-resources/) | Build and verify battle resource series. |
| [simple-resources](tools/simple-resources/) | Extract and rebuild simple resource records. |
| [skip-sprite-archive](tools/skip-sprite-archive/) | Extract the skip-sprite archive. |
| [staff-roll](tools/staff-roll/) | Build the staff-roll package. |
| [static-sprite-series](tools/static-sprite-series/) | Export, build, and verify static sprite series. |
| [title-resources](tools/title-resources/) | Build title-screen resources. |

`tools/target/` is ignored compiler output, not a tool. Empty working
directories are excluded. A new immediate directory enters the index when it
gains a `Cargo.toml` or a direct `.ts`, `.js`, `.py`, or `.sh` executable.

## Owners and names

`games/<game>/source-paths.json` is the one authored project-wide register
connecting an address-qualified owner to its semantic name and production
source path. Translation-unit manifests may describe compile-local structure,
but they derive owner names from this register. Do not create another address
map in a header, tool, dashboard, or document. Derived coverage, family, twin,
correspondence, and build reports are not name authorities and should never be
edited by hand.

Production paths describe the evidenced job of the code:

```text
battle/effects/random_particle/start_emitter.c
psynergy_menu/draw_list_page.c
party/apply_state_preset.c
```

Do not add address filenames, ordinal placeholders such as
`runtime_owner_119.c`, game codenames, or resource-number folders. Keep the
address in the register and ABI alias; keep overlay membership in the owner key.
If the role is not known, use the narrowest honest classification instead of a
confident guess.

The neutral naming voice is a Japanese commercial C codebase from 2000–2001:

- Use a stable project label such as `Subsystem_VerbObject` for a proved owner
  role; it is a navigation label, not recovered original spelling.
- Prefer short, conventional local names such as `pos`, `cnt`, `tbl`, `buf`,
  and `work` when their meanings are evidenced.
- Use `unk`, `field_<offset>`, or a raw offset when they are not. Reserve `pad`
  for bytes proved to be padding.
- Avoid fan-lore names, modern abstractions such as `EffectManager`, fake
  Japanese, and comments written as if they came from the lost source.
- Keep compiler-shape comments short and beside the constrained expression.
- `Func_<address>` is the ABI compatibility alias, not a second semantic name.

Promote declarations into a header only for a real shared interface, table
family, or stable ABI layout. Owner-specific compiler shaping stays with its
owner.

## Read the compiler first

The compiler is not a black box. GCC 2.96 is a small, 26-year-old program
whose complete source sits in `alchemy-gcc/agscc`, and the staged `cc1`
ships GCC's own dump machinery: `-da` writes every pass — RTL generation,
cse, combine, local and global allocation, reload, scheduling — for any
function you compile. Every codegen decision this project fights is either
written down in those dumps or readable in that source.

So when a candidate diverges, the order of work is:

1. **Read the decision.** Run `bun tools/allocator-lens/lens.ts <owner>` for
   the per-pseudo record (creation order, class costs, preferences, global
   ordering, conflicts, assignments, spills, reloads), or read the pass
   dumps directly.
2. **Read the code that made it.** The deciding function is small and
   findable: costs in `config/arm/arm.c` and `arm.h`, allocation in
   `local-alloc.c` and `global.c`, substitution in `reload1.c`, merging in
   `cse.c` and `combine.c`. Find the comparison, read the numbers, and you
   know exactly which property of the source is decisive.
3. **Change that property in ordinary C** — or, where the shipped ROM
   demonstrably implies the original compiler decided differently (a cost,
   a tie-break, an emission heuristic), propose a target switch. Compiler
   changes land only as a flag uniform across every file of a family,
   measured across the whole corpus with zero exact regressions, and only
   by Pascal's decision.

Spelling search and the permuter are the last resort, not the first move,
and for allocation-class residuals they are measured to regress. A week of
refuted-hypothesis dossiers was spent probing decisions the compiler
prints when asked. Do not repeat that: look at the compiler.

## Translation units with holes

The standard shape for main-image work is the declared translation unit: an
address-ordered include composite under `games/<game>/recon/en/units/`,
declared in `games/<game>/recon/translation-units.json`, holding every owner
in its range. Exact owners are included as their production sources; owners
still being recovered are included as their candidate drafts; owners with no
C anywhere stay retained assembly and appear in the manifest as holes. This
is the pret discipline at function granularity — the hole is always a whole
function, never `asm()` inside a C body, and DONE never counts a hole.

The point is authentic compile context. GCC 2.96 codegen depends on the
translation unit around a function — symbol names, shared declarations,
neighbors — so owners scored in isolation can shift when they later join
their real unit. Scoring inside the unit removes that cliff, and the unit
forces the true shared interface into the open: two members calling one
function through different prototypes cannot coexist, and resolving the
conflict recovers the original declaration. The first scaffolded unit proved
this immediately — unifying `Func_08077008` on an `s32` argument kept both
exact members exact, refuting one member's earlier `u16` guess.

Scaffold a new unit with:

```sh
bun tools/unit-scaffold/scaffold.ts gs1 <unit-id> <start-hex> <end-hex>
```

then add the printed manifest entry, resolve declaration collisions in the
composite or in shared headers, and score with `candidate-show --unit` until
every previously exact owner is exact again. Unit boundaries are provisional
working divisions, not recovered history — the original boundaries remain
unknown; merge or split units freely as evidence accumulates.

## Split functions

Several large owners are fragments of one C function split across asm
regions by literal-pool and branch-range boundaries. The asm headers say
so — `FunctionHead_`, `Fragment_`, 分割 / 断片 markers — and the shape is
unmistakable: a region with no prologue reached by a plain branch, live
r8–r11 it never saved, an epilogue restoring a frame it never pushed, or a
`mov r12, pc / bx r4` veneer between regions. Never reconstruct a fragment
in isolation: no standalone C function can match a reference with no
prologue, and the score pins at a fixed value regardless of content.

Reconstruct the whole function as one C source at the head address and
score the complete span explicitly:

```sh
out/cargo-target/release/compiler candidate-show   games/gs1/recon/en/main/<head>.c --owner <head> --size <span-bytes>
```

where span-bytes runs from the head to the end of the terminal fragment.
The inter-region veneers are the compiler's own long-branch mechanism and
fall out of compiling the function whole. The `topology=uncovered` banner
is expected on such spans; the byte diff underneath is live.

Resolve literal-pool constants from ground truth, never from displacement
arithmetic by eye. Assemble the region's `.s` standalone and objdump the
result — padding the file first so its start matches the real load
address mod 4, or an odd-parity fragment misaligns its pool and reads
garbage. When a pool physically lives in the gap between two fragment
files, compute the pc-relative target against the region's load address
and read the bytes straight out of the reference ROM.

## Recover and adopt

1. Confirm a complete function owner, and place the work inside its declared
   translation unit — scaffold one over the region first if none exists. Main owners begin in retained assembly;
   overlay owners use their audited overlay extent. Metrics account for bytes
   but do not prove function boundaries.
2. Recover the algorithm, control flow, types, access widths, constants,
   lifetimes, and call relationships in ordinary C89. Do not transcribe assembly
   instruction by instruction.
3. Compile and inspect the linked result:

   ```sh
   out/cargo-target/release/compiler candidate-show \
     games/gs1/recon/en/main/<address>.c --align --first
   ```

4. For a declared shared translation unit, compile it once and score every
   member from the same object:

   ```sh
   out/cargo-target/release/compiler candidate-show \
     --unit <translation-unit-id> --first
   ```

5. Repair structural mismatches before experimenting with declarations,
   temporary lifetimes, expression order, or loop spelling. Search the C; never
   force the allocator or scheduler. The `--allocator-order` decoder names
   both layers: register-role repairs when the instruction streams align, and
   reachability-filtered branch evidence — guard counts, loop shapes,
   mirrored or inverted guards — when they do not. Strong structural findings
   take precedence over an allocator proposal, and `compiler permute`
   searches only the catalogued repairs the decoder names.
6. Register the semantic destination and adopt only at zero linked differences.
   The `integrate` command is for standalone owners. Shared-unit owners are
   installed and retire their assembly together only after every affected owner
   is exact from the shared object.

JA correspondence must be established before EN work is treated as shared game
source. A normalized matching core does not by itself prove identical constants,
bindings, boundaries, or ownership in another edition.

Overlay identity is resource-qualified because load addresses repeat. Exact
main-image C does not prove an overlay. Overlay scoring and audit are available;
do not retire overlay assembly through an adoption route whose decoded inventory
cannot be reproduced from a clean checkout.

## Route residual work

The triage router runs as part of `compiler candidate-show` and prints a
`next=` line: the literal command to run for that owner's residual class.
Follow it — and for any allocation, scheduling, or pool-placement residual,
start from **Read the compiler first** above before touching the source. Do not improvise a different route from the raw diff, and do not
hand-probe an owner whose `next=` line already names a mechanical route —
`allocation-covered` goes through `compiler permute`, `unclassified` and
`allocation-uncovered` go to the smart queue, and uncovered allocation is
measured to regress under source respelling, not merely suspected to.

The executable repair catalog is
`games/gs1/recon/compiler-repair-patterns.json`. Add a repair only with a named
decoder signal, guarded finite operation, recorded verdict, and regression
fixture; never encode fixed registers or instruction scheduling. Run
`compiler permute --acceptance-test` (included by `make test`) to prove the
catalog still names and reverses the controlled perturbations before using a
catalog operation in a wave.

The residual dispatcher is `compiler waves`. Use `inventory` and `bucket` to
classify the existing candidate corpus. For no-candidate drafting, run `draft
prepare` once, run one or more read-only `draft score --shard I/N` workers, then
run `draft collect`. Preparation binds the evidence-mined aggregate context and
ranked exact-family templates into immutable packs; scoring tries the bound m2c
seed followed by those prepared templates in recorded order. Complete-owner,
continuation, and split-region records are parked under their audited route
rather than misreported as standalone functions. The collector rejects stale,
duplicate, incomplete, or unclassified results and publishes the prediction
comparison and class scoreboard. All receipts, packs, shard verdicts, and reports
belong beneath ignored `out/gs1-en/waves/`, never `/tmp`. Cheap agents execute
only prepared shards and named playbooks; exact results still pass through the
explicit integrator before repository source changes.

`make coverage` also refreshes the main-ROM residual class scoreboard. It
re-triages ordinary tracked candidate source with the current router, keeps
absent candidates explicit, and records deterministic source, reference, and
scoring-environment receipts. Aggregate-aware generated drafts remain wave
output and do not affect this tracked-source scoreboard.

## Validate and commit

Portable tooling is capped at 40,000 Rust, TypeScript, JavaScript, and CSS
source lines.
This is a maintainer-owned ceiling: consolidate or remove machinery when the
gate fails; do not raise the limit to accommodate a change.

Use narrow commands while iterating:

```sh
make build-claimed       # adopted main-image C
make build-asm           # retained main-image assembly
make build-assets        # source assets
make overlay-check       # exact overlay owners
make check-owners        # owner paths, names, and aliases
```

Before a normal commit, stage the intended files and run the fast production
gate:

```sh
git diff --cached --check
make verify
make progress-subject
```

`make verify` rebuilds the current production target, proves the complete ROM,
overlay, translation-unit, owner-register, source-tracking, compiler-route, and
ordinary-C contracts. It deliberately does not rescore every draft, rebuild all
twelve editions, regenerate reports, or run every tool test. The commit hook
also rejects a staged tree that differs from the one most recently verified.

Run the extra gate only when the change owns it:

| Change | Additional gate |
| --- | --- |
| Rust tooling | `make test` |
| Shared edition/preprocessor logic | `make targets` |
| Retained-assembly classification | `make classification-check` |
| Candidate corpus policy | `make candidate-corpus-check` |
| Exact ownership, owner labels, or README/coverage figures | `make coverage` |

`make audit` is the explicit exhaustive audit for a release, merge, or large
compiler/ownership batch. It covers every edition, retained candidate,
cross-edition correspondence, family, and progress contract. Use
`make reports` when the ignored analysis reports and coverage figures need to
be refreshed. Neither belongs in the ordinary commit loop.

The tooling ceiling (`TOOLING_LINE_LIMIT`) is Pascal's number, and raising it
is never part of a task. New machinery fits by paring old machinery; a diff
that touches the ceiling without his recorded decision is invalid regardless
of how good the new tools are. Headroom is granted after a tool proves
conversion, not before.

Commit subjects begin with the value printed by `make progress-subject`, for
example `☀️ 52% – `. Attribute work to its actual author.

## Find work

Run `make dashboard` for the live coverage view and unresolved owners. The
dashboard derives its view from the current tree; Alchemy does not maintain a
second generated target list in this guide.
