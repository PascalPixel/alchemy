# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the Game Boy Advance
releases of _Golden Sun_ and _Golden Sun: The Lost Age_. The Japanese edition
of each game is its canonical source base. English GS1 is the first complete
derived build and remains the ROM that `make verify` currently reproduces byte
for byte.

Readable code, useful names, fast tools, and good explanations make that work
possible. They are not progress by themselves: only exact C and accepted
permanent assembly contribute to DONE.

This is the sole contributor guide; `README.md` is for users. Keep project
procedure in this file instead of creating another guide.

Pascal's reusable clean-room method lives in the
[Cleanroom skill](/Users/pixel/.agents/skills/cleanroom/SKILL.md).
This guide remains authoritative for Alchemy's approved evidence, paths,
commands, compiler rules, and acceptance gates.

## The contract

### Use only approved evidence

Game-specific conclusions may come from this repository's reconstruction work
and these locally held reference ROMs:

```text
gs1-{en,ja,de,es,fr,it}.gba
gs2-{en,ja,de,es,fr,it}.gba
```

`gs1-ja.gba` and `gs2-ja.gba` are the reconstruction bases for their respective
games. Recover the JA owner and its source shape first, then derive EN, DE, ES,
FR, and IT by measuring their differences from it. `gs1-en.gba` is still the
only full-build target while the other edition build surfaces are brought
online. Base means the origin of the source model, not permission to assume
that every JA owner is smaller, older, or simpler; test those claims from
emitted structure.

Other editions cannot establish original English names or authorship. A shared
owner may support shared source, while a changed constant, message binding,
layout, or control-flow region belongs in an edition delta. Raw edition order
is never evidence by itself.
Do not inspect another Golden Sun decompilation, symbol list, pseudocode dump,
commit history, or private notes. Generic material about C, Rust, ARM, GCC,
binary formats, decompilation, and public pret conventions is allowed; public
game code is not evidence about this game.

Use names only as specific as the local evidence permits. Callers, callees,
message text, data tables, literal pools, and cross-edition correspondence are
evidence. Familiarity with the game is not.

### Publish reconstruction, never ROM material

The repository may contain reconstructed source, source-format assets,
deterministic transform plans, and semantic metadata. Never commit or transmit
ROMs, ROM fragments, patches containing reference bytes, object files, ELFs,
compiler output, built game images, opaque binary dumps, credentials, or
private analysis. Never upload local ROM content to a network service.

The publication checker examines staged and outgoing history, including files
added and later deleted. It cannot prove provenance; the evidence boundary is
still each contributor's responsibility.

### Keep products, editions, source, and ownership separate

There are three product trees:

- `games/gs1/` and `games/gs2/` are independent historical games. Each has one
  shared `src/`, `asm/`, `assets/`, `include/`, `metrics/`, `semantic/`, and
  `recon/` layout declared by its `project.json`.
- `games/alchemy/` is the future reintegration of both games. It has no
  reference ROM. Integration-only source and assets belong there, never in a
  historical tree.
- Ignored `scratch/` and `out/` hold disposable experiments, compiler output,
  and per-target build products.

The pret-style build model is one source tree per game, not one copied tree per
edition. The twelve historical targets compile that game's shared source with
exactly one edition define and write isolated outputs beneath
`out/<game>-<edition>/`:

```text
GS1_EDITION_{JA,EN,DE,ES,FR,IT}
GS2_EDITION_{JA,EN,DE,ES,FR,IT}
```

`make targets` compiles all twelve routes. `make gs1-ja` or `make gs2-fr`
compiles one. A compile-only target proves preprocessing, compiler routing, and
object production; it does not prove edition addresses, linking, ownership,
assets, or ROM identity. The registry therefore rejects a full build for every
target except `gs1-en` until those surfaces actually exist. Never use a
reference-ROM fallback to make an unfinished target look linkable.

Within each game, `recon/ja/` is the canonical tracked source corpus and
`recon/{en,de,es,fr,it}/` contains measured edition deltas or candidates not yet
rebased onto JA. An empty derived-edition directory means that no delta has
been established; it never means the edition is identical to JA. Main-image
files live under an edition's `main/` directory and decoded overlays under
`overlays/`.

The current GS1 ownership surface remains transitional but exact:

- `games/gs1/src/` installs C proven byte-exact for the EN derived build.
- `games/gs1/asm/` and overlay assembly retain every EN owner not represented
  by exact EN C.
- `games/gs1/recon/cross-edition.json` and the exact correspondence indexes map
  proved owners back to JA and across the other derived editions.

Tracked reconstruction C is a useful source state, not a progress tier. It may
be readable, behaviorally complete, or one instruction from matching and still
claims zero bytes. Exactness and assembly displacement are recorded separately
for every edition. This preserves the old semantic corpus without repeating
its accounting mistake.

The source graph is directional: JA is the shared base, and EN, DE, ES, FR,
and IT are measured deltas. The inherited sources under
`games/gs1/recon/en/` remain EN hypotheses until their JA owners are recovered.
Do not move one into `ja/` merely because a scan found the address. Establish
its JA boundary, emitted structure, and correspondence first.

The current `games/gs1/src/`, `games/gs1/asm/`, `games/gs1/assets/`, and
`games/gs1/metrics/gs1-en-*` paths are the installed EN ownership and build
surface. Their location inside a shared game tree does not make them exact for
another edition. Exactness and assembly displacement are recorded separately
per edition; a folder move or successful compile claims no bytes.

C must remain ordinary C. Do not use inline assembly, fixed-register variables,
empty assembly barriers, copied instruction bytes, or equivalent escape
hatches. `no-asm-c` enforces the mechanical part of this rule.

Permanent assembly is only for an instruction shape that the approved compiler
cannot emit. Record the compiler evidence and scope in
`games/gs1/asm/classification.json`. Size, difficulty, or a long-running search
is not evidence of permanence.

## Prepare a checkout

Install Rust, Ninja, and `arm-none-eabi-binutils`. Put approved ROMs in ignored
`roms/` and stage the approved compiler bundle in `alchemy-gcc/dist/`.

Configure hooks and the generated-file merge driver once:

```sh
git config core.hooksPath .hooks
git config merge.generated.driver true
```

Never run `git submodule` commands from a worktree. Reuse the main checkout's
private inputs instead:

```sh
ln -s /path/to/main/roms roms
mkdir -p alchemy-gcc
ln -s /path/to/main/alchemy-gcc/dist alchemy-gcc/dist
```

Prove the available build inputs before changing source, then build the
release-mode inner-loop host:

```sh
make build-claimed
cargo build --release --manifest-path tools/compiler/Cargo.toml
```

Use `tools/compiler/target/release/compiler` while iterating. Repeated
`cargo run` startup is significant beside a candidate compile.

A dist-only worktree can compile and score candidates, but `make verify` also
inspects the complete compiler source checkout. Run that authoritative gate
from a checkout containing the recorded `alchemy-gcc` revision before merging
or claiming verification.

## Recover one owner

Apply the reusable
[owner-recovery method](/Users/pixel/.agents/skills/cleanroom/references/reconstruction-method.md)
to one edition-qualified owner at a time. Alchemy adopts an owner only at zero
differing reference bytes. A shape that the approved compiler cannot emit may
be classified as permanent assembly only after its compiler evidence is
recorded.

### 1. Select an owner

An owner is a function-sized region with a fixed entry point, not merely a run
of unresolved bytes. Main-image owners begin in `games/gs1/asm/<address>.s`;
overlay owners live inside
`games/gs1/assets/code/resource_<id>_overlay.s`.

Exact source uses descriptive, domain-owned paths under `games/gs1/src/`, such
as `battle/inventory/draw_item_grid.c`. The canonical owner register,
`games/gs1/source-paths.json`, is the single address-to-name and
address-to-source authority. A plain source-path record takes its human name
from the `.c` filename; use an object only to override that name, retain a name
without exact C, or add owner-specific metadata. Do not add parallel symbol
maps or infer names outside this schema from assembly exports, chart labels, or
tool-local tables. Those consumers must resolve the stable owner identity
through this register. Owner-specific
compiler facts, such as an exceptional `call_via` bank, belong on the same
record rather than in a source-path lookup table. The legacy `Func_<address>`
spelling is derived directly from an unregistered main owner identity and is
not repeated as a database record. Existing flat
address-named files remain a supported migration fallback, but do not add new
ones: register a nested descriptive path before adopting a new owner.

The generated Targets table ranks useful scopes, but a listed run may begin in
a literal pool or include a continuation. Resolve the owner through
`games/gs1/semantic/regions.json` and confirm that its full span is inside one audited
interval in `games/gs1/metrics/gs1-en-executable.json`. Adoption rejects unaudited gaps.

Prefer related owners or case bodies over isolated guesses. Siblings built by
the same compiler often reveal which single source feature controls their
different output.

### 2. Establish the reference shape

Assemble and disassemble reference `.s` when resolving pools and table targets;
do not hand-count PC-relative offsets across alignment. Resolve message IDs
through `games/gs1/assets/text/message_archive.json`. Text and neighboring tables often
identify a scene or binding without importing outside knowledge.

#### Compare editions through relocations

A raw offset-for-offset ROM diff confuses code with layout. Localized builds
move functions, call targets, tables, message constants, literal pools, and
jump-table destinations; every moved address can recolor several instruction
bytes without changing the C.

Use a proved owner object and its relocations to locate and compare the same
owner in all six GS1 editions:

```sh
tools/compiler/target/release/compiler cross-edition 080bbb0c
tools/compiler/target/release/compiler cross-edition --calls 080bbb0c
tools/compiler/target/release/compiler cross-edition --all \
  --write games/gs1/recon/exact-correspondence.json
tools/compiler/target/release/compiler cross-edition --all-overlays \
  --write games/gs1/recon/exact-overlay-correspondence.json
```

The comparison is JA-relative. During the transition, an exact EN object may
supply relocation sites because those sites are proved rather than guessed;
that is a locator, not a declaration that EN owns the source model. The
object's disassembled literal fields are tracked separately.
`core_diff_bytes=0` means every edition byte outside relocations and literal
data agrees; differing branch encodings and pool words are then layout evidence,
not instruction differences. `--calls` decodes corresponding callees, allowing
one proved owner to seed neighboring correspondences. Use `--json` for ignored
machine-readable output.

The corpus scan uses every byte-exact EN main-image object as a relocation-aware
probe. A global match requires a unique relocation-free anchor of at least 16
bytes. Short owners receive a second, bounded locality pass only when their
entire unmasked core is exact near a proved neighbor; conflicting owner order
or duplicate destinations are rejected. The generated index separates
`shared_core`, `regional_core`, and unresolved owners and records how each
location was established. It maps evidence across editions; it does not make
the EN C exact for another edition or prove that edition's complete owner
boundary.

Exact overlay C is scanned separately because its owners live inside compressed
resource containers rather than at ROM addresses. The overlay scan locates each
edition's resource directory through its self-pointer, decodes resources
`36f`--`3ce`, and compares owners within the corresponding resource. Its mask is
limited to Thumb `bl` fields and words reached by PC-relative literal loads.
Long owners use global core anchors; short owners require either exact bytes at
the same resource offset or exact masked core near a proved neighbor. The same
duplicate and owner-order rejection applies. The result is recorded in
`games/gs1/recon/exact-overlay-correspondence.json`.

When core bytes remain, inspect those exact offsets as possible regional source
changes. A smaller JA owner is useful evidence for a different source shape,
but edition order alone does not establish why it differs. The current
calibration result is recorded in `games/gs1/recon/cross-edition.json`.
The full exact-corpus locator output is
`games/gs1/recon/exact-correspondence.json`; regenerate it with
`make correspondence` whenever exact main-image or overlay ownership changes.

### 3. Recover source structure

Use C89 declarations and short evidence-backed names. Preserve observable
original mistakes. Replace names such as `Func_02000f80`, `status_12c`, and
anonymous numeric cases only when local evidence supports something better;
honest names make incorrect bindings visible.

#### Recover source voice, not a modern API

This was a Japanese commercial C codebase built in 2000--2001. Its exact house
style is not recoverable, but a contemporary source voice is a better neutral
default than a modern English framework style:

- Prefer short, conventional C names at local scope: `dmg`, `hit`, `efx`,
  `res`, `pos`, `cnt`, `work`, `tbl`, `buf`, and similarly compact compounds.
  A longer name is warranted when it prevents two live roles from being
  confused, not merely because modern code usually spells words out.
- Preserve established domain abbreviations and compact numeric distinctions.
  For example, `EFX_RES_UP1` can be more honest than
  `BATTLE_EFFECT_RESISTANCE_UP_1`: the former describes a compact effect-table
  label, while the latter invents a global namespace and modern naming scheme.
- The recorded preprocessor admits ASCII letters and `_` at identifier start,
  then ASCII digits as well; it does not classify full-width or half-width kana
  as identifier characters. Romanized Japanese, English abbreviations, and
  mixed domain shorthand are all historically plausible, but the exact choice
  is not recoverable from the binary. Do not manufacture kana or romaji merely
  to make source look Japanese.
- Japanese comments already carrying useful technical or behavioral meaning
  may remain Japanese. Translate or rewrite a comment when that makes its
  evidence clearer, not for cosmetic language uniformity. Keep compiler-shape
  notes beside the expression or declaration whose form they constrain.
- Do not expand every identifier mechanically. Names such as `amount_dealt`,
  `BattleAction_ResolveTarget`, and `BattleWork_ApplyMode` may read smoothly but
  are wrong when the evidence supports only `dmg`, an address-based function,
  or an unresolved mode setter.
- Preserve terse or slightly awkward source when it explains the reference
  output. A raw offset, reused temporary, fallthrough, or unusual operand order
  should receive a concise reconstruction note rather than a fictitious
  abstraction.

Treat headers as coherent source groupings, not as a readability score or a
claim about the original directory tree. A large exact owner may move stable
declarations into focused headers even before a second owner uses them. Keep
the grouping narrow and source-like: `battle_efx.h`, `battle_msg.h`, and
`battle_calc.h` fit compact table vocabulary; `battle_action_messages.h` and a
new hierarchy of long English APIs usually invent a modern architecture.

Keep compiler-shaping macros, speculative aliases, one-off aggregate views,
and declaration-order notes beside the owner whose output they constrain.
Splitting declarations from implementation is useful; hiding the source shape
that made the owner exact is not.

A trailing address, register hint, or SSA counter in an identifier
(`temp_r3_1908`, `var_sl_1853`, `sp4C`, `Func_080ab5e4` used as a _local_
variable-style name) is a placeholder, not a name, regardless of how
plausible it reads. Humanizing a function means every symbol it declares or
touches loses its number once its role has local evidence: parameters,
locals, fields, and included/edited headers alike, not just the identifiers
that happen to repeat often. Partial passes that rename a few well-evidenced
locals and leave the rest as `tempN`/`spN` are progress, not completion; say
so plainly rather than calling the pass "humanized."

For a large or incomplete main-image owner, create a minimal compilable draft
with the project headers and evidence-backed signature, then start with the
structural workbench instead of hand-transcribing thousands of instructions:

```sh
git clone https://github.com/matt-kempster/m2c.git m2c
tools/compiler/target/release/compiler workbench games/gs1/recon/en/main/080bbb0c.c
```

The generated Ninja graph:

- preprocesses and compiles the draft through its routed gs1cc configuration;
- assembles the complete reference owner and symbolizes pools and jump tables;
- runs m2c with the actual gs1cc type context;
- compares reference and candidate objects with objdiff's ARMv4T engine; and
- probes the generated m2c source without treating missing recovered types as a graph failure.

Results stay in ignored `out/workbench/<address>/`. A second invocation rebuilds
only invalidated stages. Use `--m2c PATH`, `--output DIR`, or `--no-run` when
needed. m2c output is a source-shape draft, and objdiff is a structural compass;
neither can establish exactness.

### 4. Iterate against the compiler

Keep one active edition-qualified corpus file. New shared recovery begins in
`games/gs1/recon/ja/`; the EN path below is for an inherited EN candidate or a real
EN delta. First verify that an edit changed emitted assembly:

```sh
tools/compiler/target/release/compiler candidate-show games/gs1/recon/en/main/080bbb0c.c --asm
```

Then compare linked bytes:

```sh
tools/compiler/target/release/compiler candidate-show games/gs1/recon/en/main/080bbb0c.c --align
```

When measuring a candidate against an independently established GS2 boundary,
select the target and explicit owner size:

```sh
tools/compiler/target/release/compiler candidate-show \
  games/gs2/recon/ja/main/08120450.c --target gs2 \
  --rom roms/gs2-ja.gba --size 0x206c \
  --flags -Igames/gs1/include
```

Once the candidate's relocation sites align with the reference structure,
`--reference-symbols` derives call and literal targets from the chosen local
reference image. Inconsistent sites or a non-call where a call is expected are
errors, never guessed bindings.

`--first` crops the first residual window. `--patch FILE` scores a unified diff
without changing the corpus source.

Read the complete normalized instruction diff and repair its largest coherent
hunk. A large mismatch means the source shape is still wrong. Fix algorithms,
case ownership, types, declaration lifetimes, and aggregates before trying
spellings or flags.

### 5. Invert a stalled last mile

Apply the reusable
[stalled-last-mile inversion](/Users/pixel/.agents/skills/cleanroom/references/reconstruction-method.md#invert-a-stalled-last-mile)
when distinct source families preserve the same local emitted defect. State
and recheck Alchemy's invariant with the `candidate-show` assembly and linked-
byte views above; the adoption gate remains zero differing reference bytes.

### 6. Permute only the last mile

The native permuter is useful when the owner extent, behavior, control flow,
types, declarations, and major expression boundaries are already credible. It
can explore local expression, statement, and declaration order; it is not a
decompiler.

```sh
tools/compiler/target/release/compiler permute --help
```

Use explicit `PERM_*` choices or semantics-preserving AST mutations. Mixed
guided and unguided seeds are reasonable after a heat-guided run stagnates.
Keep every run under ignored output. Each strict improvement is published
immediately as `best.c` (or `best-UNVERIFIED.c` in classic mode) with a running
`report.json`, so a long or interrupted run does not hide its best source until
shutdown. Read a winner's complete diff and rescore it independently before
harvesting it. When a search has a frozen local invariant, apply that admission
check to every retained winner; a lower internal score that breaks the invariant
is a rejected candidate. A lower internal score is not proof of semantic
correctness.

### 7. Adopt only exact work

For a main-image owner, adoption is allowed only when linked
`differing_halfwords` is zero:

```sh
mkdir -p scratch/adopt
cp games/gs1/recon/en/main/<address>.c scratch/adopt/src_<address>.c
cargo run --release --manifest-path tools/check/Cargo.toml -- integrate scratch/adopt
cargo run --release --manifest-path tools/check/Cargo.toml -- integrate --apply scratch/adopt
```

Until then, retain the reference assembly. A nearly exact corpus source is useful
research but does not count as DONE.

## Work on overlays

The structural workbench currently targets main-image owners. Use the overlay
host for overlay decoding, scoring, adoption, parking, and audits:

```sh
cargo run --release --manifest-path tools/overlay/Cargo.toml -- show resource_373 034c
cargo run --release --manifest-path tools/overlay/Cargo.toml -- score games/gs1/recon/en/overlays/resource_373_c_0200034c.c --align
cargo run --release --manifest-path tools/overlay/Cargo.toml -- adopt resource_373:034c --source games/gs1/recon/en/overlays/resource_373_c_0200034c.c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- park resource_373:034c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- audit --all
```

Scoring is read-only and may run in parallel. Adoption temporarily splices one
row into a shared overlay, so apply one adoption at a time. After merging any
overlay adoption, regenerate coverage, run `overlay audit --all`, and prove the
full build. Main-image success does not imply overlay or asset identity.

## Interpret measurements correctly

Use comparisons in this order:

1. Linked `differing_halfwords` decides exactness and adoption.
2. Normalized instruction diffs locate structural disagreement.
3. Objdiff percentages and permuter scores rank experiments only.

## Treat the compiler as evidence

The compiler source is part of the reconstruction environment. Before sweeping
equivalent C spellings, inspect the relevant GCC 2.96 pass. Source order affects
switch-body layout; signed loads, bitfields, constants, address forms,
declaration order, alias sets, scheduling, and cross-jumping all change RTL and
register allocation.

Compiler changes belong in the separate compiler repository, together with its
GPL notices, corresponding source, tests, and reproducible build instructions.
The main repository records the staged compiler revision and approved bundle
digests. Never invent a target option or patch the compiler merely to conceal
an incorrect source reconstruction.

GCC pass-dump options such as `-da` write files like `08120454.c.12.life` into
the compiler process's working directory. Run diagnostic compiles from an
ignored owner-specific directory under `out/`, never from the repository root.
The repository-wide ignore rule is a backstop, not a substitute for containing
dump output.

## Keep tooling portable and small

Apply the reusable
[portable-tooling criteria](/Users/pixel/.agents/skills/cleanroom/references/reconstruction-method.md#keep-reconstruction-tooling-portable-and-small).
There are six public executable hosts: `build-assets`, `build-stage`, `assets`,
`compiler`, `overlay`, and `check`. Contributor operations are subcommands of
those hosts.

Tooling is Rust. Repository code describes GBA and reconstruction-specific
behavior.

`make tooling-size` enforces Alchemy's portable-toolkit budget.

## Prove the repository

Use the narrowest useful command while iterating, then finish with the full
gate:

```sh
make targets         # compile the shared source through all 12 edition routes
make build-claimed   # compile and link every adopted main-image owner
make build-asm       # assemble retained main-image regions
make build-assets    # rebuild tracked source assets
make build-full      # compose and compare every owned region
make build-rom       # produce the final ROM locally
make coverage        # regenerate metrics, figures, and Targets
make test            # tooling, policy, and focused regression tests
make verify          # authoritative byte-identical repository gate
```

The commands below `make targets` operate on `TARGET`, which defaults to the
only full-build target, `gs1-en`. Compile-only targets intentionally reject
full linking until their edition address map, ownership, assembly, and assets
are installed.

`make verify` must finish with a byte-identical full build and fresh generated
artifacts. A clean compile count is insufficient: overlay reconstruction,
compressed assets, ownership manifests, and final composition can fail later.
Do not describe a change as verified when the authoritative gate stopped at a
baseline or environment error; report that blocker separately.

## Merge and commit cleanly

When branches adopt different owners, merge the source changes, regenerate with
`make coverage`, audit overlays, and run `make verify`. Do not resolve generated
metrics by choosing one branch's copy.

Before committing:

1. Remove or relocate scratch output, generated binaries, and temporary links.
2. Run focused tests for the changed source or tooling.
3. Run `make verify` and state its exact outcome.
4. Generate, rather than guess, the required subject prefix.

```sh
make progress-subject
```

Commit subjects begin `☀️ N% – `. Codex-written commits are authored by Codex,
Claude-written commits by Claude, and human-written commits by that human. Do
not attribute an agent's work to Pascal.

If a large owner stops converging, leave one honest measured corpus source, record the
remaining structural problem, and move to a related owner. Exact siblings are
the best documentation of the approved compiler's source shapes; knowledge
compounds even when one target does not close immediately.

## Targets

This section is generated. It is the primary contributor target list:
non-overlapping audited source-owner scopes (or contiguous unresolved
executable runs), sorted largest to smallest. Regenerate with `make coverage` -- do not edit by hand.

- **Unfinished scopes:** 1,626
- **Address spaces scanned:** 97 (86 still contain targets)
- **Target bytes:** 650,170 tracked-C or unresolved-assembly bytes
- **Resolved-only bytes:** 538,658 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,348,458

### Main target list

This table contains every scope of at least 1,000 bytes (195 rows). The complete
1,626-row index, including the smallest audited owners, is
[`games/gs1/metrics/gs1-en-core-targets.json`](games/gs1/metrics/gs1-en-core-targets.json).

| Rank | Scope | Target | Namespace / owner |
|---:|---:|---:|---|
| 1 | 19,708 | 9,240 | `main:0x080e7404` |
| 2 | 18,914 | 15,338 | `main:0x080c91dc` |
| 3 | 14,744 | 11,128 | `main:0x080d1350` |
| 4 | 14,476 | 7,472 | `main:0x080a9e48` |
| 5 | 12,818 | 12,424 | `main:0x080d8988` |
| 6 | 9,968 | 2,796 | `main:0x080f6008` |
| 7 | 9,870 | 3,712 | `main:0x08022b44` |
| 8 | 8,560 | 8,268 | `main:0x080cde90` |
| 9 | 7,808 | 6,330 | `main:0x080e47b8` |
| 10 | 7,652 | 7,544 | `main:0x080dbbdc` |
| 11 | 6,458 | 1,300 | `main:0x080270ac` |
| 12 | 6,192 | 4,200 | `main:0x080b2110` |
| 13 | 6,070 | 4,380 | `main:0x0801a910` |
| 14 | 5,794 | 3,012 | `main:0x080b0a6c` |
| 15 | 5,688 | 3,376 | `main:0x080a414c` |
| 16 | 5,608 | 5,404 | `main:0x080e0000` |
| 17 | 5,450 | 2,228 | `main:0x0808c2dc` |
| 18 | 5,330 | 3,208 | `main:0x0801cf48` |
| 19 | 5,050 | 916 | `main:0x080dea70` |
| 20 | 4,866 | 4,616 | `main:0x080d5274` |
| 21 | 4,718 | 2,136 | `main:0x08017c8c` |
| 22 | 4,406 | 588 | `main:0x080a2324` |
| 23 | 4,302 | 2,640 | `main:0x08018efc` |
| 24 | 4,220 | 2,528 | `main:0x080be18c` |
| 25 | 4,170 | 2,900 | `main:0x08095bd8` |
| 26 | 4,158 | 1,920 | `main:0x0801edec` |
| 27 | 4,138 | 4,138 | `resource_380:0x02002c10` |
| 28 | 4,122 | 4,122 | `resource_373:0x02002284` |
| 29 | 4,046 | 3,908 | `main:0x080e2974` |
| 30 | 3,850 | 2,604 | `main:0x08077320` |
| 31 | 3,808 | 3,808 | `main:0x080d0000` |
| 32 | 3,756 | 3,712 | `main:0x080251d4` |
| 33 | 3,738 | 2,420 | `main:0x080b6cd0` |
| 34 | 3,702 | 3,702 | `resource_3c6:0x02000218` |
| 35 | 3,584 | 2,952 | `main:0x08026080` |
| 36 | 3,492 | 3,352 | `main:0x080e3a14` |
| 37 | 3,472 | 1,388 | `main:0x08010424` |
| 38 | 3,452 | 2,396 | `main:0x080ec100` |
| 39 | 3,442 | 3,442 | `resource_378:0x0200088c` |
| 40 | 3,408 | 3,408 | `resource_378:0x02001874` |
| 41 | 3,364 | 1,108 | `main:0x08096c80` |
| 42 | 3,274 | 2,500 | `main:0x080bae40` |
| 43 | 3,238 | 2,386 | `main:0x080e6638` |
| 44 | 3,156 | 2,514 | `main:0x080d765c` |
| 45 | 3,114 | 1,098 | `main:0x080fa678` |
| 46 | 3,086 | 2,980 | `main:0x08021e6c` |
| 47 | 3,074 | 3,074 | `resource_383:0x02000b48` |
| 48 | 3,068 | 1,300 | `main:0x080ece7c` |
| 49 | 2,946 | 2,426 | `main:0x080167e0` |
| 50 | 2,918 | 696 | `main:0x080b6148` |
| 51 | 2,908 | 2,908 | `resource_381:0x020003cc` |
| 52 | 2,854 | 2,854 | `resource_374:0x0200155c` |
| 53 | 2,778 | 904 | `main:0x08020bd8` |
| 54 | 2,772 | 260 | `main:0x0808f52c` |
| 55 | 2,734 | 2,734 | `resource_3a8:0x020026a8` |
| 56 | 2,706 | 1,588 | `main:0x080a345c` |
| 57 | 2,698 | 2,256 | `main:0x080a8914` |
| 58 | 2,678 | 1,324 | `main:0x08095160` |
| 59 | 2,672 | 2,672 | `resource_3a8:0x02000aac` |
| 60 | 2,658 | 2,180 | `main:0x080c1fa8` |
| 61 | 2,650 | 2,376 | `main:0x080999f0` |
| 62 | 2,630 | 1,586 | `main:0x080a7a34` |
| 63 | 2,596 | 200 | `main:0x080c0000` |
| 64 | 2,574 | 2,574 | `resource_39e:0x02002ec8` |
| 65 | 2,568 | 748 | `main:0x08012518` |
| 66 | 2,530 | 1,044 | `main:0x080b8574` |
| 67 | 2,508 | 460 | `main:0x08020000` |
| 68 | 2,476 | 2,476 | `resource_3af:0x0200252c` |
| 69 | 2,440 | 2,368 | `main:0x080bf678` |
| 70 | 2,430 | 2,244 | `main:0x080a6a00` |
| 71 | 2,426 | 2,426 | `resource_3b8:0x0200338c` |
| 72 | 2,418 | 2,418 | `resource_38d:0x02000894` |
| 73 | 2,382 | 2,382 | `resource_383:0x02003c6c` |
| 74 | 2,360 | 1,328 | `main:0x080dd9c0` |
| 75 | 2,310 | 2,310 | `resource_3b8:0x02002a84` |
| 76 | 2,310 | 388 | `main:0x080113e4` |
| 77 | 2,298 | 2,298 | `resource_380:0x02000a98` |
| 78 | 2,294 | 1,912 | `main:0x0808e0b0` |
| 79 | 2,290 | 1,876 | `main:0x080bd898` |
| 80 | 2,236 | 2,236 | `resource_39e:0x02001494` |
| 81 | 2,222 | 2,222 | `resource_3bc:0x02001a08` |
| 82 | 2,218 | 2,218 | `resource_3c9:0x02003e8c` |
| 83 | 2,218 | 1,028 | `main:0x08079008` |
| 84 | 2,206 | 2,206 | `resource_377:0x0200063c` |
| 85 | 2,194 | 2,194 | `resource_376:0x02000640` |
| 86 | 2,182 | 1,696 | `main:0x0808d828` |
| 87 | 2,172 | 1,336 | `main:0x0808a5f8` |
| 88 | 2,170 | 2,170 | `resource_3ca:0x02000430` |
| 89 | 2,168 | 2,168 | `resource_3bd:0x02002c44` |
| 90 | 2,158 | 2,158 | `resource_373:0x02001a14` |
| 91 | 2,118 | 2,118 | `resource_371:0x020028e8` |
| 92 | 2,070 | 2,070 | `resource_3b1:0x02001b34` |
| 93 | 2,068 | 2,068 | `resource_3c2:0x02000240` |
| 94 | 2,056 | 1,048 | `main:0x080f0254` |
| 95 | 2,046 | 124 | `main:0x0800300c` |
| 96 | 2,026 | 242 | `main:0x0800a97c` |
| 97 | 2,018 | 2,018 | `resource_3a5:0x020004e4` |
| 98 | 2,014 | 2,014 | `resource_3c9:0x02000a6c` |
| 99 | 2,006 | 2,006 | `resource_3a4:0x02001830` |
| 100 | 2,002 | 2,002 | `resource_3af:0x02003a0c` |
| 101 | 1,928 | 836 | `main:0x08097b54` |
| 102 | 1,906 | 1,906 | `resource_378:0x0200290c` |
| 103 | 1,902 | 1,292 | `main:0x080983a0` |
| 104 | 1,866 | 1,722 | `main:0x080b9b30` |
| 105 | 1,810 | 500 | `main:0x08006088` |
| 106 | 1,800 | 644 | `main:0x0809b0dc` |
| 107 | 1,782 | 1,660 | `main:0x080de2f8` |
| 108 | 1,778 | 1,778 | `resource_3b9:0x02001cd4` |
| 109 | 1,770 | 1,770 | `resource_3b1:0x020028c8` |
| 110 | 1,750 | 1,688 | `main:0x080d82b0` |
| 111 | 1,734 | 1,734 | `resource_383:0x02001e80` |
| 112 | 1,722 | 1,722 | `resource_3b8:0x02002014` |
| 113 | 1,718 | 620 | `main:0x080ba2c0` |
| 114 | 1,702 | 1,702 | `resource_381:0x02002150` |
| 115 | 1,702 | 1,702 | `resource_3b7:0x020001d8` |
| 116 | 1,692 | 1,692 | `resource_3b1:0x02005d10` |
| 117 | 1,692 | 400 | `main:0x0808b824` |
| 118 | 1,638 | 1,638 | `resource_373:0x02004084` |
| 119 | 1,634 | 1,056 | `main:0x080ae2f4` |
| 120 | 1,632 | 1,632 | `resource_3a2:0x020001ec` |
| 121 | 1,562 | 1,170 | `main:0x080a63e4` |
| 122 | 1,560 | 832 | `main:0x08098cd8` |
| 123 | 1,514 | 1,514 | `resource_3a4:0x02000d24` |
| 124 | 1,506 | 1,264 | `main:0x080b5534` |
| 125 | 1,462 | 1,462 | `resource_373:0x020039e8` |
| 126 | 1,458 | 1,458 | `resource_3c8:0x020047ac` |
| 127 | 1,448 | 1,448 | `resource_3b0:0x02000af8` |
| 128 | 1,448 | 1,448 | `resource_3b1:0x020050dc` |
| 129 | 1,426 | 444 | `main:0x0809a8c4` |
| 130 | 1,410 | 1,410 | `resource_383:0x020036e8` |
| 131 | 1,410 | 1,138 | `main:0x08090000` |
| 132 | 1,410 | 908 | `main:0x080c1a24` |
| 133 | 1,408 | 1,408 | `resource_39e:0x020038d8` |
| 134 | 1,402 | 1,402 | `resource_3b1:0x0200234c` |
| 135 | 1,398 | 1,398 | `resource_372:0x02001b18` |
| 136 | 1,392 | 842 | `main:0x080d4ce8` |
| 137 | 1,382 | 1,382 | `resource_3c9:0x02003924` |
| 138 | 1,354 | 676 | `main:0x08017364` |
| 139 | 1,346 | 160 | `main:0x080b04dc` |
| 140 | 1,318 | 1,024 | `main:0x08092b94` |
| 141 | 1,310 | 1,310 | `resource_373:0x020034c8` |
| 142 | 1,310 | 656 | `main:0x080994d0` |
| 143 | 1,308 | 1,308 | `resource_37a:0x02002094` |
| 144 | 1,306 | 1,306 | `resource_3a8:0x02000590` |
| 145 | 1,298 | 1,298 | `resource_39c:0x02004a08` |
| 146 | 1,290 | 1,290 | `resource_3af:0x02000c94` |
| 147 | 1,270 | 1,270 | `resource_3b1:0x020037d8` |
| 148 | 1,230 | 1,230 | `resource_371:0x02001f6c` |
| 149 | 1,230 | 226 | `main:0x080a7380` |
| 150 | 1,222 | 1,222 | `resource_37a:0x0200155c` |
| 151 | 1,222 | 930 | `main:0x080ba978` |
| 152 | 1,218 | 996 | `main:0x0801c46c` |
| 153 | 1,208 | 1,208 | `resource_3b8:0x02000b40` |
| 154 | 1,198 | 1,198 | `resource_3b1:0x020057ec` |
| 155 | 1,194 | 964 | `main:0x080a1090` |
| 156 | 1,182 | 1,182 | `resource_3c9:0x02004738` |
| 157 | 1,180 | 1,180 | `resource_37b:0x0200101c` |
| 158 | 1,176 | 1,176 | `resource_37b:0x02000a18` |
| 159 | 1,170 | 1,170 | `resource_3b3:0x0200174c` |
| 160 | 1,162 | 392 | `main:0x08094730` |
| 161 | 1,160 | 768 | `main:0x080a847c` |
| 162 | 1,158 | 344 | `main:0x0808ec50` |
| 163 | 1,150 | 1,150 | `resource_375:0x020014f4` |
| 164 | 1,136 | 908 | `main:0x080d0ee0` |
| 165 | 1,130 | 460 | `main:0x080b0070` |
| 166 | 1,126 | 456 | `main:0x080798b4` |
| 167 | 1,114 | 1,114 | `resource_3c7:0x02000880` |
| 168 | 1,106 | 1,106 | `resource_39e:0x02001dbc` |
| 169 | 1,102 | 1,102 | `resource_3bb:0x020010dc` |
| 170 | 1,102 | 124 | `main:0x08003c80` |
| 171 | 1,098 | 1,098 | `main:0x080a5b94` |
| 172 | 1,096 | 1,096 | `resource_3b9:0x020011c4` |
| 173 | 1,094 | 1,094 | `resource_3bc:0x02001474` |
| 174 | 1,094 | 378 | `main:0x0800b684` |
| 175 | 1,082 | 148 | `main:0x080216b4` |
| 176 | 1,078 | 1,078 | `resource_373:0x020015dc` |
| 177 | 1,078 | 568 | `main:0x08078aa0` |
| 178 | 1,074 | 1,074 | `resource_3ba:0x02000db8` |
| 179 | 1,068 | 896 | `main:0x080bd424` |
| 180 | 1,066 | 780 | `main:0x08091814` |
| 181 | 1,052 | 1,052 | `resource_3b1:0x02004254` |
| 182 | 1,052 | 1,052 | `resource_3c8:0x020026f8` |
| 183 | 1,046 | 1,046 | `resource_380:0x020027f8` |
| 184 | 1,046 | 1,046 | `resource_381:0x02001b34` |
| 185 | 1,042 | 1,042 | `resource_3a5:0x02001490` |
| 186 | 1,036 | 956 | `main:0x08029504` |
| 187 | 1,034 | 652 | `main:0x080b9724` |
| 188 | 1,028 | 724 | `main:0x08090658` |
| 189 | 1,026 | 1,026 | `resource_3ae:0x02000e40` |
| 190 | 1,024 | 856 | `main:0x080a5fe0` |
| 191 | 1,022 | 1,022 | `resource_3b8:0x02000ff8` |
| 192 | 1,022 | 464 | `main:0x08016018` |
| 193 | 1,020 | 1,020 | `resource_3b9:0x02000db0` |
| 194 | 1,014 | 1,014 | `resource_39e:0x02002ad0` |
| 195 | 1,010 | 1,010 | `resource_37a:0x02001ca0` |
