# Contributing to Alchemy

Game identities are **TBS** (_The Broken Seal_) and **TLA** (_The Lost Age_).
Use `tbs` and `tla` in paths and target names. The compiler option `-mgs2`
retains its approved upstream spelling. Pascal identified the first battle
character sheet as Robin (Isaac in English); it is `ROBIN_BATTLE.8bpp.png`.

Recover matching C, reuse what already works, and keep the build honest.

Alchemy reconstructs _Golden Sun_ and _Golden Sun: The Lost Age_ as ordinary,
readable C that reproduces the shipped games. Japanese releases are the source
editions; localizations are measured differences. The production gate requires
a complete byte-identical TBS English build. Passing that gate does not mean
all code is C or that all twelve target ROMs have been rebuilt.

## The two directives

1. **Never commit material that `pret` would not commit.** No ROM bytes, no
   extracted data dressed as source, no copyrighted assets, no output patched
   into place. Every file in the tree must be something a clean-room
   decompilation project could publish. This directive outranks everything
   below it, including the goal.
2. **Reconstruct the source Camelot most likely wrote and had sitting on their
   disk** — files, folders, names, types, macros, build, everything. Where the
   evidence says Camelot wrote C, we write C; where it says a shared macro with
   inline assembly, a hand-written `.s`, or a generated table with its writer
   and reader, we write that. DONE measures how much of the executable image is
   explained by such source.

Every other rule in this document is a consequence of these two, kept because
it has saved us from a specific mistake. When a rule and the evidence about
what Camelot wrote disagree, the evidence wins and the rule is repaired here.

## The working method

Start with the nearest verified example, not a blank page or another framework.

Melee demonstrated two useful techniques: matching near-duplicate functions from
an exact sibling plus decompiler output, and improving decompiler input with
shared types and concrete pointer/union context. Its
[duplicate-family PR](https://github.com/doldecomp/melee/pull/2102) produced 318
matches; its [typed-module example](https://github.com/doldecomp/melee/pull/2076)
used a short compile/diff loop. These are methodological examples, not sources
of Golden Sun code, compiler policy, names or types. Pokeemerald remains a model
for coherent modules, shared headers, explicit link order and incremental builds.

### 1. Select a family that can actually add C

Use current production ownership and coverage to distinguish exact C, retained
assembly, draft C and unknown code. Historical scores and dossiers are leads,
not current facts. Inspect existing tools and their help before writing scripts.

Find unresolved owners with an exact sibling in our own corpus. Compare
reference control flow, calls, data accesses and constants, not just filenames,
function size or a normalized similarity score. Rank bounded families by
expected new exact-C bytes per hour, then genuinely unresolved bytes. Prefer
batches capable of moving DONE by at least 0.1 points; do not skip small owners
when many related matches make a useful batch.

Record the starting owners, complete extents, sibling source, current residuals,
expected gain and time budget before editing. Count Assembly-to-C conversions as
new C at their full extent, but subtract any existing retained-ASM credit when
forecasting DONE. Renames, merges, new drafts, better
similarity and tooling changes add no DONE credit. Do not favor overlays or
main-image code merely because of layout.

No credible sibling? Select a coherent module with understood calls and types.
Recover one representative and then test its siblings. Do not launch blind
ROM-wide drafting or permutation just because agents are available.

### 2. Give the worker enough context

A bounded assignment contains:

- Resource-qualified owner identities, complete extents and canonical source.
- Reference instructions and the closest verified C sibling.
- Shared types, globals, helper signatures, compiler route and unit membership.
- The working compile/diff command, baseline, budget and integration owner.

Read the sibling and target together. Retarget constants, calls, offsets,
signedness and types from the target reference. Similar assembly does not mean
literal copy-and-paste is valid. Account for pointer returns and unused results,
indirect-call idioms, aggregate copies and overlapping lifetimes explicitly.
Do not invent structures or rewrite calls merely to obtain a better score.

Reuse established headers. When a decompiler supports type or union hints, give
it evidence-backed context; otherwise repair the ordinary C directly. The
native decompiler is `psynergy decompile`; it takes bytes and explicit addresses,
not Golden Sun owner identities. Alchemy supplies project ownership and build
context. Melee's PowerPC m2c options do not automatically apply to Thumb.
Do not build a second decompiler or context registry during a recovery batch.

### 3. Run the short loop

```sh
./alchemy extract <owner> --out out/owner.bin
./psynergy decompile out/owner.bin --base <address> --entry <address> --span <bytes> --out out/candidate.c
./alchemy inspect <owner>
./alchemy score <candidate.c> --owner <owner> --align --first
./alchemy score --unit <unit-id> --first
```

Keep one canonical candidate. Change one evidenced source hypothesis, compile
the affected object or unit, read the complete residual, and retain a supported
improvement. Use `--work out/<experiment>` for native diagnostics. Do not run
full asset, coverage or twelve-edition checks after every source spelling.

For a persistent local mismatch, walk back the complete draft to a smaller
experimental C scaffold and leave the unresolved owner on temporary assembly.
Freeze an explicitly bounded, byte-compared local invariant, then restore one
coherent source section at a time. Reject expansions that lose the invariant;
the first failing expansion identifies the context to reconstruct next. These
partial witnesses stay under ignored `out/`, earn no whole-owner C credit and
do not shorten reviewed extents. Retire assembly only after the complete owner
and its integration pass, not merely because the isolated section matches.

Extraction applies overlay loading before slicing and reports the runtime
base, entry and span; use those printed addresses for Psynergy, not the owner's
resource address. Keep these local
reference bytes under ignored `out/`; never commit them. Psynergy works outside
this repository and knows no default ROM, owner register, scene layout or
compiler route. Its output is a draft using the caller's `types.h` aliases;
resolve project symbols and review its control flow before scoring. Extraction
and decompilation refuse to overwrite an existing output.

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

Fix the largest coherent disagreement first:

| Residual | Action |
| --- | --- |
| Boundary or binding | Resolve the complete owner and actual runtime symbols before searching C. |
| Control flow | Correct branches, loops, switches and omitted effects using the reference and sibling. |
| Type or width | Establish offsets, access widths, signedness, pointer and aggregate types. |
| Missing or extra code | Recheck extent, helpers, calls, guards and side effects. |
| Allocation, scheduling or pools | Read the relevant compiler pass; repair ordinary C lifetimes or structure. |
| Unclassified | Investigate within the budget; do not assign an invented playbook. |

The triage router's `next=` is advice, not authority over the reference.
A `repair_hint=` admits a guarded experiment, not a global replacement.
For allocation evidence use `alchemy score <candidate.c> --allocator-order`.
To read native compiler decisions, run `alchemy build allocator <owner>`, then
`psynergy inspect allocator <printed-dump-directory>`. GCC's `-da` dumps expose
creation, CSE, combine, allocation, reload and scheduling. Never force registers,
scheduling or compiler output.

After five unsuccessful source hypotheses, hand off the concrete residual or
park the owner and move to another family. Two bounded searches with no new
structural fact close that axis. A small persistent mismatch is a constraint
to explain, not a reason for unlimited random spelling. Reopen only with new
evidence. Fixing a large semantic mismatch may be useful even before it matches,
but it remains a draft and must not be reported as adoption.

### 4. Multiply proved repairs, not guesses

When one owner closes, attempt its unresolved siblings before selecting an
unrelated target. Confirm target-side preconditions separately and score every
complete owner. Use each target's declarations and bindings, not the sibling's
addresses. Preserve exact neighbors; never adopt overlapping units independently.

`games/THE BROKEN SEAL/recon/compiler-repair-patterns.json` is the executable repair
catalog. `alchemy match` runs decoder-named, bounded catalog operations.
A new generalized operation needs safety guards, a regression fixture and an
actual closure; its acceptance tests belong in `make test`. Manual
sibling-guided C recovery does not require inventing a catalog operation.
Permutation is last-mile work after structure and types are credible.

Parallel workers should own disjoint files or isolated worktrees and bounded
families. One integrator owns shared headers, registers, manifests and staging.
Workers return exact candidates or honest residuals, not optimistic status.
They must not race edits or independently commit shared state. Use available
agent coordination; do not build another worker framework, claim database,
template-pack system or scoreboard service.

A time-boxed wave ends with starting and ending exact-C bytes, newly resolved
bytes, adopted owners, parked residual classes, elapsed time and validation
actually run. Measure verified new C per hour and DONE separately. Zero adoption
is a negative result, not permission for a larger identical wave.

### 5. Integrate and commit

Use `alchemy adopt <overlay>:<address> --source <candidate.c>` for standalone
overlay owners. For standalone main-image owners, register the name and source
path, place accepted address-named candidates in a bounded `out/` directory,
then use `alchemy check integrate <directory> --apply`. Both paths must verify
the complete owner before retiring assembly. Install a shared unit only after every affected
owner is exact from the shared object; isolated adoption cannot prove a unit.

Consolidate related functions when the resulting compilation context is proven.
Use `alchemy unit flatten --help` for complete-overlay consolidation.
Unknown executable gaps outside registered owners still block whole-overlay
completion. Keep genuine assembly and data separate. Do not trade byte equality
for tidiness or delay productive recovery for unrelated folder cleanup.

Run the integration gate once per finished coherent batch, repair failures,
then commit before starting another wave. Commits do not require a DONE increase:
verified cleanup, consolidation, corrections and recorded negative results may
be committed on their own. The 0.1-point preference guides recovery selection;
it is not a commit gate and must not block the long-term goal.
At a timed deadline, stop launching
searches, finish or revert experiments, verify accepted work and report the
actual result. Never lower the acceptance bar to hit a percentage.

## What counts as complete

Every executable byte, including overlay gaps, must have a complete audited
owner or explained nonfunction role. Exact C means ordinary production C emits
the complete linked extent with zero differences under the approved route.
Names, plausible behavior, matching size and fuzzy scores are not proof.

**☀️ DONE = retained ASM + exact C**, divided by audited executable bytes.
Use the coverage map's retained-ASM category, not every unresolved function
temporarily assembled to make the ROM build. Drafts and unknown code earn no
credit. Report exact-C share separately; converting already credited assembly
to exact C improves that share without increasing DONE.

Retained-ASM credit does not establish handwritten or third-party origin.
Search exhaustion, register mismatches, compiler non-emission, repeated scripts
and large functions are not evidence of authorship. Keep unresolved C candidates
in the recovery queue rather than relabeling them merely to increase DONE.

The classification records remain: main evidence is exposed by the assembly
manifest, overlay evidence lives in
`games/THE BROKEN SEAL/semantic/overlay-assembly.json`, and both are still parsed and
validated from the same inputs. Reconstructed scenes use
`structured_scene_module`, not the retired `generated_call_script_module`.
Only Pascal may establish a replacement standard, and it requires positive
evidence of real handwritten or third-party assembly, such as a verified match
to historical assembly runtime source or an independently established
hand-authored machine interface.

Report exact C, unresolved bytes and complete-ROM target coverage separately.
Historical Proven C/Proven ASM labels describe build routes, not knowledge of
Camelot's original language. All twelve targets must
eventually rebuild byte-identically from a clean checkout and approved local
inputs. Correspondence checks and compile-only targets do not establish this.

Recover Japanese correspondence before calling EN source shared. Measure actual
localization differences and TLA behavior; do not count one recovery twelve
times. Assets must rebuild from maintained inputs, and final source must be
organized into evidenced modules with shared interfaces.

## Boundary and relocation rules

TBS editions share C files and translation units. `INCLUDE/VERSION.H` selects
exactly one of `TBS_EDITION_JA`, `EN`, `DE`, `ES`, `FR` or `IT`; English is the
production default. Use small conditional sections only for measured code
changes. Keep localized text and address differences in bindings and assets,
not copied language trees. A separate module is justified only by substantially
different code, never merely by relocated addresses.

The existing translation-unit manifest owns optional `editions` entries. Each
edition's `owners` map uses registered function names and records independently
reviewed `address`, complete `extent`, and `source_variant` when code changes.
Its `absolute_symbols` map records named function, data and value bindings with
their `address` and `kind`. Omitted entries retain the normal correspondence
workflow; a source variant needs explicit bindings where call or literal sites
change. Do not use candidate length or candidate call offsets to invent these
facts. Conflicting, unknown, overlapping and invalid declarations are errors.

For one registered overlay owner, compile its shared unit for all six editions:

```sh
./alchemy cross-edition --span 472 --edition-build out/tbs-editions/lunpa-sequence.json resource_3bf:02001e94
```

This report proves only that complete owner, including linked calls, literals
and alignment; it does not prove its neighbors or whole overlay. Shared source
is compiled before selecting an owner for linking. Declared main-image variants
use `alchemy cross-edition --edition-build out/tbs-editions/owner.json <owner>`.
`--all-overlays --edition-build` compiles once per unit per edition and checks
every member at its regional address and extent. Core similarity is diagnostic;
only zero differences over the complete linked extent is exact. Keep per-edition
results under ignored `out/`; any failed edition makes the command fail.

A coverage span is not automatically a function. Follow continuations, shared
epilogues, inherited live registers and long-branch veneers through the complete
owner, including pools. Never shorten its expected extent to fit a candidate.
For reviewed main spans, use `alchemy score SOURCE --owner <head> --size <complete-bytes>`.
Read correctly aligned pool data and preserve address modulo four.

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

## Compiler invariants

GCC source and compiler modifications belong in the licensed `agscc` submodule,
never as source patches in this repository. Pascal approved moving the existing
TLA lowering into `agscc` behind `-mgs2` on 2026-09-13. Both games use the same
compiler bundle; only TLA game code enables that option. The TLA lowering is a
reconstruction, not a claim to recovered historical compiler source.

Use the approved `agbcc` and `agscc` submodules and bundle. Game code uses
canonical GCC 2.96; recorded prebuilt-library families have their own fixed
routes established by provenance, not score. Commands live in
`tools/alchemy/src/compiler/routing.rs`.

**Apart from the approved TLA option above, do not modify agscc unless
restoring historical stock GCC 2.96 shipped by Red Hat.** Every change, including diagnostic or host-port changes, requires
Pascal's approval, a specific historical release and source/vendor-patch
evidence, and proof of restored fidelity. Better matching, determinism, version
strings or an approved pin are not historical provenance or authorization.

Only Pascal may authorize justified changes to pins, executable hashes, family
routes or output transformations. No per-function flags, fixed-register
variables, empty barriers, forced scheduling, output patches or selected lucky
runs. Inline assembly is admitted only as a shared macro header that the
evidence shows Camelot used (a construct the approved compiler provably cannot
emit, recurring across otherwise compiler-shaped C bodies), never per function
and never to steer the compiler. A different shipped compiler family requires evidence
across that family. Do not weaken guards or copy another project's workarounds.

The approved bundle uses GNU GAS 2.10 for compiler output and unit slices,
including alignment fill. Modern binutils handles retained syntax, symbols and
linking with the recorded integer/soft-float ABI. Do not suppress ABI mismatches.
Cache identity includes the full executable bundle. The restored bundle is
admitted on Apple Silicon macOS; other hosts need source builds and verification.
Keep third-party source untouched.

## Evidence and repository discipline

Use locally held reference ROMs, independently reconstructed material here,
decoded data, verified calls/relocations and public language, hardware, compiler
and format documentation. Never inspect or copy leaks or another Golden Sun
reconstruction's code, symbols, types, comments or conclusions. Unrelated
projects supply methodology, not target-specific material.

Never commit or transmit ROMs, fragments, extracted binaries, objects, ELFs,
compiler output or patches containing reference bytes. Do not fill unfinished
code from the reference. Preserve unrelated work; no resets to make gates pass.

README is the public introduction. **CONTRIBUTING is the only contributor
guide.** AGENTS and CLAUDE may be pointers only. No other Markdown or plain-text
reports, plans or guides anywhere for this project, including ignored `out/`,
scratch and temporary directories. Do not hide prose guides inside JSON.
Durable procedure belongs here, code constraints beside source, owner results
in existing evidence records, and native disposable diagnostics under `out/`.
Third-party GCC `.md` machine descriptions are not disposable documentation.

Each fact has one authority:

| Fact | Authority |
| --- | --- |
| Owner name and source | `games/<game>/source-paths.json` |
| Compile-local composition and bindings | Translation-unit manifests |
| Code, types and asset layouts | Maintained sources and headers |
| Measured production status | Current build outputs |
| Historical reasoning | Dossiers, never treated as current scores |

`make coverage` rebuilds production coverage; it does not rescore all drafts.
Do not edit derived reports or treat old experiment outputs as build inputs.

## Setup and verification

Install Rust, Ninja and `arm-none-eabi-binutils`.
Supply approved ROMs under ignored `roms/` and
the bundle under `out/compilers/dist/`.

```sh
git submodule update --init
git config core.hooksPath .hooks
./alchemy --help
./psynergy --help
make compiler-source-check
```

Initialize submodules only in the main checkout. Worktrees may symlink its ROMs
and approved bundle. `alchemy build compilers` builds pinned sources without
authorizing bundle replacement. The launcher builds current tooling offline.

Use `alchemy score` during source iteration and narrow builds when needed:
`make build-claimed`, `make build-asm`, `make build-assets`,
`make overlay-check`, `make check-owners`. Before committing, stage only the
intended batch:

```sh
git diff --cached --check
make verify
make progress-subject
```

The index and worktree must agree. `make verify` checks staged source,
production TBS EN ROM, overlays, units, ownership, ordinary C, compiler
provenance and repository rules. It does not rescore every draft, run every test
or build twelve ROMs. The hook accepts only the verified tree; use the exact
progress prefix printed by `make progress-subject`. Push only when requested.

| Changed surface | Additional check |
| --- | --- |
| Rust tooling or dashboard | `make test` |
| Shared edition/preprocessor logic | `make targets` |
| Retained-ASM classification | `make classification-check` |
| Candidate-corpus policy | `make candidate-corpus-check` |
| Ownership, labels or coverage claims | `make coverage` |
| Tooling inventory | `make tooling-index-check` |

`make targets` is compile-only. An unscored complete candidate is not a parked
match. Use `make audit` for exhaustive release or major compiler/ownership
checks and `make reports` to regenerate analysis, not inside every edit loop.

## Tooling index

There are two public commands and two crates in one Cargo workspace:
`alchemy` for Golden Sun integration and `psynergy` for portable machinery.
Every immediate tool directory, including internal or game-specific libraries,
must remain listed here. Build dependency status does not excuse inventory.
`make tooling-index-check` enforces this; never remove the requirement to hide
tool growth.

| Tool | Responsibility |
| --- | --- |
| [alchemy](tools/alchemy/) | Golden Sun command dispatch, twelve-target registry, owner lookup/extraction, source adoption, scene integration, compiler routes and provenance, candidate compilation, bindings, translation units, residual classification, matching catalog, overlay loading/serialization/assembly/audits, ROM stages, asset manifests, coverage, publication checks, dashboard and optional music debugger. Its `compiler`, `recovery`, `score`, `matching`, `overlay`, `coverage` and asset/build modules are project integration. |
| [psynergy](tools/psynergy/) | Portable Thumb and objdump decoding, lifetime analysis, C recovery, normalization/alignment, structural and byte comparison, bounded C repair enumeration, GCC allocation-dump reading, format conversion, explicit subprocess execution, atomic writes, transactional cache storage, and image/MIDI/WAV/text/pixel/Huffman/LZ codecs. Callers supply addresses, symbols, paths, keys, formats and layouts; no Golden Sun owners, default ROMs or compiler routes. |

**The boundary is inputs and authority, not implementation language.** Psynergy
operates on explicit files and parameters without a checkout. Alchemy resolves
Golden Sun owners and applies project policy before calling the same portable
libraries. A reusable operation belongs in Psynergy even if its first caller
is Alchemy. Do not move game defaults into Psynergy or expose one operation
through both hosts as aliases.

Psynergy's commands use the same library as Alchemy's internal integration.
Psynergy shares decoding and comparison across recovery, scoring and overlays;
BL-shaped data is not automatically executable code. Its dependencies include
`regex`, `rusqlite`, `png`, and test-only `tempfile`. Alchemy owns cache
identity, project paths and compiler fingerprints. Do not duplicate these
responsibilities in another wrapper or registry.

| Portable command | Responsibility |
| --- | --- |
| `psynergy decompile` | Recover draft C from an image with explicit base, entry and span; optional name/output. |
| `psynergy disassemble` | Read reachable Thumb instructions in the same explicit image window; no owner lookup or game symbol annotations. |
| `psynergy diff` | Compare two supplied binary files, including length differences; `--width 1\|2\|4` sets the comparison unit. Exit 0 means equal bytes, 1 differences, 2 invalid input. No compilation or relocation. |
| `psynergy repair` | Enumerate one or two caller-named, guarded source repairs. Report the finite space; `--choice N` emits one alternative, optionally to `--out FILE`. No scoring, compiler selection or adoption. |
| `psynergy inspect allocator` | Read existing `.rtl`, `.lreg` and `.greg` GCC dumps from an explicit directory; no compiler invocation. |
| `psynergy convert` | Convert files using the directional formats listed below. No ROM offsets, engine headers or asset manifests. |

| Golden Sun command | Responsibility |
| --- | --- |
| `alchemy extract` | Resolve an owner and extract its reference bytes under ignored `out/`. |
| `alchemy inspect` | Resolve project call sites and symbols; `--asm` adds annotated owner disassembly. |
| `alchemy score` | Compile a candidate or whole declared unit with the approved route and compare its complete owner, including bindings and overlay serialization. |
| `alchemy match` | Resolve an owner, obtain a decoder-named repair, then compile/score bounded Psynergy alternatives under project policy. `--acceptance-test` checks the five catalog fixtures. |
| `alchemy adopt` | Verify and install a standalone overlay candidate; main integration uses `alchemy check integrate`. |
| `alchemy unit` | `scaffold` declares a main unit; `flatten` consolidates a verified overlay under project ownership. |
| `alchemy build` | `compilers`, `asm`, `claimed`, `full`/`rom`, `assets`, and `allocator`. The allocator stage generates canonical GCC dumps for Psynergy inspection. |
| `alchemy verify` | Run the staged repository's verification contract. |
| `alchemy coverage` | Rebuild and publish project coverage. |
| `alchemy check` | `publication`, `commit-progress`, `owners`, `retained`, `coverage`, `integrate`, `no-asm`, `progress`, and `routes`: repository contracts, not portable file operations. |
| `alchemy cross-edition` | Compare reviewed owner correspondence across Golden Sun editions. |
| `alchemy overlay` | `adopt`, `park`, and `audit`: Golden Sun loader and resource integration. |
| `alchemy dashboard` | Serve project coverage. |

Retired entry points are rejected, not forwarded: `alchemy decompile`,
`alchemy convert`, and `alchemy disassemble` now belong to Psynergy;
owner-annotated disassembly is `alchemy inspect OWNER --asm`.
The old `alchemy diff` is `alchemy score`, not `psynergy diff`.
The old `alchemy inspect allocator` is split into dump generation and reading
as shown above. Update maintained callers; historical dossiers retain their
original command transcripts and are not instructions to resurrect aliases.

Consult command help for supported arguments. No retired TypeScript entry
points or invented family/wave commands. For another GBA game, carry portable
decoding, comparison, bounded recovery and compiler invocation plus its minimal
build integration, not Golden Sun ownership, asset offsets or agent machinery.

Portable tooling is capped at **100,000 Rust, TypeScript, JavaScript and CSS
lines** by `make tooling-size`. Pascal owns that ceiling and scope. Do not
raise it or hide code outside it. New tooling must resolve a demonstrated
recurring blocker, reuse or replace existing machinery, and prove a conversion
with regression coverage. Prefer using the tools to adding tools.

## Assets and local servers

Asset layouts, offsets, widths, counts, pointers, frames and compression plans
belong in maintained data under `games/`, not resource-specific executables.
`alchemy build assets` uses shared typed/record/pointer tables, component
composition, pixel, text, sample and compression codecs. Preserve validation
of ranges, extents, ordering, padding and reference round trips. A layout is
data; renaming hard-coded game logic does not make a portable codec.

Do not maintain metadata that a shared encoder can derive. Zero-skip sprite
banks use PNG frames plus geometry and palette settings in the existing series
index; the encoder generates alignment and the frame pointer directory. They
do not need individual bank JSON files. Keep compression recipes only where
the encoder cannot yet reproduce the original stream without them.

`psynergy convert FORMAT INPUT OUTPUT [options]` supports `words2bin`,
`pairs2bin`, `tilemap2bin`, `png2bpp4`, `bpp42png`, `png2bpp8`,
`bpp82png`, `png2bgr555`, `wav2pcm8`, and `pcm82wav`.
Bpp formats are GBA tile-major pixels, BGR555 is little-endian palette data,
and PCM8 is signed sample data. Reverse tile conversion requires
`--palette FILE --tiles-wide N`; PCM-to-WAV requires `--rate HZ`.
WAV input is canonical mono 8-bit PCM. Invalid palette/transparency/index data
and overwriting an existing output are refused, not silently discarded.
Engine headers and loop metadata stay in asset manifests. Thumb is code, not
a music format. Keep format names portable and directional.

The dashboard at `http://127.0.0.1:4650/` shows one full-window ROM tree of files
and folders, shared with the README snapshot; it has no music player. Click a
folder to open it and use Back to return. Addresses belong in hover details,
not tile captions. The pixel font stays 16px when the layout changes.
`make dashboard-service-install`
installs its macOS login service; `make dashboard-restart` restarts it. Coverage
inputs are watched, but the service keeps the binary it started with, so restart
it after any tooling merge or it computes DONE under the old rules. Rust renders
the dashboard and handles folder navigation, file details and shared-resource
links through HTML requests. It serves no JavaScript; a script prohibition is
also enforced by its content security policy. Refresh reads the latest watched
coverage state. HTML forms can reveal a source in Finder through same-origin POST.

## Owners and names

Use modules and evidenced jobs, not one file per address. Merge related C only
when shared compilation stays exact. No address filenames, ordinal placeholders,
resource-number folders, codenames or invented geography. Use distinct
directories for separately loaded overlays, named from decoded locations, map
geometry and evidenced scene differences. Use short romaji prefixes from the
Japanese ROM location names and a short area suffix; repeat the location prefix
so related folders sort together. Reused areas may add a short scene-phase
qualifier. Shared overlays name the locations they serve. These are working
names, not recovered historical directory names.
Keep folder labels uppercase ASCII. Prefer a repeated location prefix and one
short area word: `HAIDIA_MURA`, `HAIDIA_HEYA`, `RUNPA_DOU`, `RUNPA_JO`.
Use the Japanese ROM's name rather than its English localization; shorten
romaji consistently, and record the decoded label and any abbreviation in the
existing owner evidence. Use `MURA` for an evidenced village, `MACHI` for a town,
`HEYA` for interiors, and `DOU` for a cave. A reused area can add `SAI` for a
return scene. Resource identities and addresses belong in metadata, not folder
labels. Inspect maps and scene behavior before choosing an area suffix.
Keep function addresses in the owner register and
`Func_<address>` compatibility aliases.

Use neutral Japanese commercial C vocabulary appropriate to 2000–2001:
`Subsystem_VerbObject`, short locals such as `pos`, `cnt`, `tbl`, `buf`
and `work`. Unknown roles remain `unk`, `field_<offset>` or raw offsets;
`pad` means proved padding. No fan lore, fake Japanese, modern framework
terminology or comments impersonating lost source. Names are reading aids,
not recovered historical spellings.

Shared interfaces and evidenced types belong in headers; local constraints
belong beside the relevant expression. Keep source ordinary C89 and preserve
observable behavior, including bugs.

### Project Atlas

Atlas replaces the current layout below with area workspaces under
`games/THE BROKEN SEAL/SRC/FIELD/`, shared field engine code in `FIELD/COMMON/`, and
cross-location modules under distinct named `FIELD/COMMON/<area>/` directories.
Each overlay has its own workspace; the three Lunpa modules use `RUNPA_MURA`,
`RUNPA_DOU` and `RUNPA_JO`. The `atlas_destination` column in
`games/THE BROKEN SEAL/locations.tsv` records the destination for each overlay. Source and
include files have moved; maps now accompany their evidenced areas, messages
and credits live in `TEXT`. The glyph sheet is in `SRC/GRAPHICS/FONT`; mixed item,
status, window and menu-image banks are in `SRC/GRAPHICS/TILE`, not classified as
fonts merely because their former package was called localization.
The localization glyph table lives beside its sheet in `SRC/GRAPHICS/FONT`:
each of its 224 records combines a two-byte advance with 15 two-byte bitmap rows.
Keep that interleaved layout intact; its 7,168 bytes are not all image pixels.
Sound inputs are under `SOUND/SEQUENCE`, `SOUND/SAMPLE` and
`SOUND/INSTRUMENT`; mixed residual sound definitions remain at the sound root.
Character banks live in `SRC/GRAPHICS/CHARACTER`. Confirmed Japanese ROM names
own `CHAR_<ROMAJI>.PNG` field sheets and `BATTLE_<ROMAJI>.PNG` battle sheets,
with both bank definitions in one character JSON file. Names require message
and runtime descriptor evidence; visual resemblance alone is insufficient.
Unproven and shared banks occupy sections of `CHAR_COMMON.PNG`, described once
in `COMMON.JSON`. Pixel indices remain separate from the common palette bank
pool; compression recipes remain in `GRAPHICS/COMMON/COMPRESSION.JSON`.
These extracted PNG inputs are private and ignored, registered in `SOURCE.JSON`;
Rust extraction and source checks verify their pixels and encoded bank hashes.
Use `alchemy build assets --audit-characters OUTPUT` for the Japanese message
and descriptor audit. The common descriptor catalog also feeds animation and UI
consumers; do not split banks by an export label alone.
Additional sprite archives use the same character atlas, with archive offsets
and frame order retained in metadata until runtime associations establish names.
Shared still images, dialogue portraits and eight-bit tile banks use indexed
atlases under `GRAPHICS/COMMON`; their palettes remain separate. Unknown location
and runtime-role fields stay null. Pixel appearance does not establish a world-map
or battle association. Palette tables preserve all sixteen bits, including the
high bit represented by alpha in former RGBA exports. Byte visualizations become
typed data streams rather than graphical source inputs. Extraction must reproduce
every registered atlas, palette bank and table as well as the ordinary rebuild.
Battle-effect tables now accompany their consumers in `SRC/BATTLE/DATA`.
Preserve separate compilation and load boundaries. The remaining source groups are `SYSTEM`, `LIB`, `GRAPHICS`,
`SOUND`, `GAME`, `BATTLE`, `MENU` and `DEBUG`; shared headers belong in `INCLUDE`.
Uppercase is a project convention, not recovered historical spelling. Ensure
uppercase `.C` is compiled as C, never inferred as C++, without changing the
approved compiler route or optimization flags.

Native TBS names use uppercase folders, basenames and extensions throughout
`SRC`, `INCLUDE`, `SOUND`, `TEXT`, `PREVIEW` and `SOURCE.JSON`: `.C`, `.H`,
`.JSON`, `.PNG`, `.BIN`, `.MID`, `.WAV` and `.TSV`. Repository and tooling
registries retain their established spellings outside these workspaces.
The first game's physical root is `games/THE BROKEN SEAL`; its stable build IDs
remain `tbs-ja`, `tbs-en` and the other edition-qualified targets. The second
game's physical root is `games/THE LOST AGE`, retaining its `tla-` build IDs.
`games/COMMON` owns the shared integration workspace
(`SRC`, `INCLUDE`, `ASSETS`, `PROJECT.JSON`).
Reuse there requires proved correspondence; it is currently an empty workspace,
not a claim that either game's implementation is interchangeable. Edition-specific
symbols and placements can build one C file for multiple ROMs, as the Lunpa
overlay demonstrates. A single source tree for all twelve ROMs also requires
proved cross-game ownership, target-specific compiler profiles and asset bindings.
Keep the game boundaries until those callers are supported and verified.
`alchemy format` formats native JSON; `alchemy format --check` gates both
formatting and uppercase names. JSON uses two-space indentation, a 120-column
layout, compact records and packed short data tuples. Long individual strings
remain intact. Preserve field order, values and the recorded binary boundaries.
Use JSON for these codec and table definitions; changing to TOML adds a parser
and migration without reducing their token arrays.

Keep editable inputs beside their native owner. Registered protected inputs
remain private and ignored even when their filename ends in `.PNG` or `.JSON`.
Tracked, shareable README illustrations belong in `PREVIEW`; disposable audit
images and exports belong in `out/<target>/previews`. Builds, caches and reports
belong in their existing stable target directories under `out`. Reuse those
destinations rather than accumulating a new directory for every export.
Migration backups are temporary and may be removed after the migration commit,
native extraction check and complete ROM verification pass. `make clean-preview`
lists generated trees; `make clean` removes them, including caches and reports.
Neither command removes tracked previews or native inputs.

Move area-exclusive maps and graphics beside their code only after their
consumers establish ownership. Shared map containers remain once under `SRC/FIELD/COMMON`;
reused tile banks and palettes remain in `SRC/GRAPHICS/COMMON`, sequences,
samples and instrument definitions in `SOUND`, and localized messages in `TEXT`.
Do not duplicate shared resources, invent asset associations, or publish
protected extracted inputs. Use `.gitkeep` only for necessary empty destinations.

`games/THE BROKEN SEAL/SOURCE.JSON` binds map containers, graphics resources and scene loaders
to their physical inputs. Each map owner has one named JSON file containing its
container sections and one packed binary file. Each grid section contains two
128×128 byte planes followed by a 128×128 little-endian word plane; its original
2×2 metatile words follow it. Recorded offsets and lengths preserve every flag,
row and component boundary. Each graphics owner has one indexed `CHR` sheet;
tile offsets select its independent 512-tile banks. Identical tile banks have one
physical section. Palette banks and compression recipes have one common owner.
These are reconstruction formats, not evidence of Camelot's authoring files.

Packed maps, new `CHR` sheets and the common palette file are private, ignored
inputs. Regenerate them with `alchemy build assets --extract-sources roms/tbs-en.gba`;
the Rust reader requires the registered ROM checksum and decoded input hashes.
The ordinary build encodes these sources and compares every stored region with
the ROM; `--source-only` builds them without reading the ROM. Do not commit the
private inputs or replace uncertain edge regions with invented blank cells.

Image round-trip equality proves storage bytes, not the intended picture layout
or colors. Establish frame boundaries, tile ordering and palette selection from
the runtime consumers before rearranging images. A grayscale index palette is
not evidence of monochrome artwork, and a narrow animation strip is not by
itself a bad export. Existing extraction metadata is a hypothesis to check,
not independent proof of Camelot's authoring format.
The menu-label sheet follows `RenderResource_CreatePair`: two 32×16 OBJ parts
per image, with the second part at X+32 and tile+8. `frames_per_image` groups
adjacent atlas parts for a single compressed image; it does not change ROM data.

The dashboard and README share one ROM viewer hierarchy and layout; the dashboard
fills its window, while the README snapshot retains its 9:16 frame,
counting represented ROM bytes once while preserving the executable-only DONE
denominator. An area's Shared files control links map resources to their existing
source folders without adding tiles or bytes. These links are derived from the
scene selector, map-load table, resource directory and Atlas destinations; they
do not claim to inventory every non-map asset an area uses.
Data-file details report the target's last asset-build comparison separately
from its content type. A ROM-byte match does not verify picture layout or colors;
missing comparison evidence is never displayed as a match.
The viewer follows leaf files across package boundaries. Component packages
use the build's recorded physical extents and primary inputs; headers and
padding remain with their index file. Compression is counted at stored size,
not decoded size. Missing or inconsistent component extents keep the package
unsplit rather than assigning guessed byte ranges to files.
Keep the optional music debugger separate. Migrate source, assets
and viewer in verified stages; completion requires the full TBS English ROM to
remain byte-identical and the migration to be committed. Do not claim TLA or
other editions verified from TBS evidence.

TBS `SRC/` uses the following responsibilities. These are our
reconstruction choices, not recovered Camelot directory names. Keep a shallow
module tree; do not create a folder for each function or sort whole scenes into
actor/dialogue/story categories. A scene's actors, dialogue and events belong
with its area or loadable module. Moving a file does not change its translation
unit, overlay identity, compiler route or completion credit.

Use `COMMON` for genuinely common runtime code and interfaces, rather than a
generic `SHARED` catch-all. Prefer this short commercial C vocabulary appropriate
to the period; it does not establish a recovered historical spelling. Common
engine code has one owner. A separately loaded overlay used by several locations
still has its own named directory and compilation identity; its
`FIELD/COMMON/<locations>/` directory groups those overlays, not merged engine
code. Reused graphics belong under `SRC/GRAPHICS/COMMON/`; exclusive map inputs
stay beside their overlay. Remove empty directories left by moves.

| Folder | Responsibility |
| --- | --- |
| `SYSTEM/` | Startup, scheduling, memory, input, save, link and resource loading. |
| `LIB/` | Library support and shared math routines; retain compiler provenance. |
| `GRAPHICS/` | Display, palette, animation, tile, text, window and icon rendering. |
| `SOUND/` | Sound and music player code, not the sound assets. |
| `GAME/` | Shared character, party, inventory, item, ability, Djinn and flag rules. |
| `FIELD/COMMON/` | Shared map, camera, object, event and script runtime. |
| `FIELD/<location-or-scene>/` | Distinct loadable area modules and their exclusive assets. |
| `FIELD/COMMON/<locations>/` | Distinct loadable modules serving multiple areas. |
| `BATTLE/` | Battle rules, presentation, motion and effect modules. |
| `MENU/` | Menu interaction, shops, inns and selection screens. |
| `DEBUG/` | Debug facilities. |

Area-specific code belongs in the overlay directory recorded by
`atlas_destination`; one location may have several separately compiled overlays.
Their source and assets remain in distinct directories without changing compiler
composition. Shared library code and genuinely reused assets keep one common
owner. Retained assembly and recovery candidates remain in their existing
registries. These directory choices do not establish original Camelot names.
`games/THE BROKEN SEAL/locations.tsv` retains the
ROM-backed TBS English location assignments for all 96 overlays. The scene
selector is bounded to 0–200 by `08029094`; `0808ab48` loads the resource from
the eight-byte scene table at `0809f1a8`. The location resolver `0808b158`
(through veneer `0808a5d0`) scans ordered rules at `0809ddd8`, matching either
scene or group and an entry selector. `Menu_DrawSelectionRow` adds `0x99b` to
the result to select the location message, and separately displays the debug
scene label at `0xa07 + scene`. Preserve entry-specific and shared-location
assignments; a default title-screen fallback is not a place. These are English
display labels, not recovered Japanese folder names. The historical
`battle_effect_tail.json` and `battle_effect_rules.json` filenames do not make
these tables battle effects. Existing filenames and function names alone do
not independently prove an assignment: establish new locations through resource,
map or call-site evidence. Unidentified ownership remains unresolved until
its location is established. Never invent a
place to fill a folder, and never merge distinct overlay address spaces simply
because they share a location. Shared field engine code belongs in `FIELD/COMMON/`.

Keep classification metadata outside source folders; provisional-source records live
in `games/THE BROKEN SEAL/semantic/provisional-source.json`. Code stays out of assets: overlay assembly
and compression recipes belong in `asm/overlays/`, battle assembly in
`asm/battle/`. TBS non-executable battle-effect tables are in `SRC/BATTLE/DATA/`;
TBS sound inputs are already in
the game-root `SOUND/`, separate from player code in `SRC/SOUND/`.
TLA retains its existing layout until independently mapped. Update paths,
caches and coverage together. Music titles require actual identification,
not guessed numbering.
