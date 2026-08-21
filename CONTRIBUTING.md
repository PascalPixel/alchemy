# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the English Game Boy
Advance release of _Golden Sun_. The project is finished when reconstructed C
and independently described assets build the reference ROM byte for byte.

Readable code, useful names, fast tools, and good explanations make that work
possible. They are not progress by themselves: only exact C and accepted
permanent assembly contribute to DONE.

This is the sole contributor guide. `AGENTS.md` and `CLAUDE.md` point here;
`README.md` is for users. Keep project procedure in this file instead of
creating another guide.

## The contract

### Use only approved evidence

Game-specific conclusions may come from this repository's reconstruction work
and these locally held reference ROMs:

```text
gs1-{en,ja,de,es,fr,it}.gba
gs2-{en,ja,de,es,fr,it}.gba
```

`gs1-en.gba` is the build target. Other editions may demonstrate shared layout
or behavior, but they cannot establish original English names or authorship.
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

### Keep source states honest

- `exact/` contains C whose linked bytes exactly match one complete owner.
- `asm/` and overlay assembly retain every owner not yet represented by exact C.
- `draft/<owner>.c` and `draft/<owner>.json` hold one active, measured near-match.
- Ignored `scratch/` and `out/` hold experiments, generated candidates, and tool output.

There is no semantic-C progress tier. A plausible implementation, equal source
length, high objdiff percentage, or successful compilation does not displace
reference assembly.

C must remain ordinary C. Do not use inline assembly, fixed-register variables,
empty assembly barriers, copied instruction bytes, or equivalent escape
hatches. `no-asm-c` enforces the mechanical part of this rule.

Permanent assembly is only for an instruction shape that the approved compiler
cannot emit. Record the compiler evidence and scope in
`asm/classification.json`. Size, difficulty, or a long-running search is not
evidence of permanence.

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

The standard loop is:

1. Select one audited owner and establish its complete extent.
2. Read the reference assembly, pools, tables, and observable behavior.
3. Recover the algorithm, control flow, types, and aggregate layout in plain C.
4. Compare instruction structure, then linked bytes.
5. Correct the largest source-shape error and repeat.
6. Use permutation only after the remaining differences are local.
7. Adopt the owner only at zero differing reference bytes.

The order matters. Random source variation and compiler-option sweeps cannot
repair a wrong function boundary, switch binding, loop, type, or algorithm. A
candidate that is vastly non-matching is not a weak near-match to be nursed
forward: it is evidence that the overall C shape is wrong. Replace the bad
shape instead of preserving a lucky prefix, register assignment, or local
instruction coincidence. In some cases the missing shape is one the approved
compiler cannot emit at all; classify permanent assembly only after recording
that compiler evidence.

### 1. Select an owner

An owner is a function-sized region with a fixed entry point, not merely a run
of unresolved bytes. Main-image owners begin in `asm/<address>.s`; exact source
lands in `exact/<address>.c`. Overlay owners live inside
`assets/code/resource_<id>_overlay.s`; their exact source is named
`exact/resource_<id>_c_<address>.c`.

The generated Targets table ranks useful scopes, but a listed run may begin in
a literal pool or include a continuation. Resolve the owner through
`semantic/regions.json` and confirm that its full span is inside one audited
interval in `metrics/gs1-en-executable.json`. Adoption rejects unaudited gaps.

Prefer related owners or case bodies over isolated guesses. Siblings built by
the same compiler often reveal which single source feature controls their
different output.

### 2. Establish the reference shape

Before writing C, account for:

- the entry point, complete owner boundary, arguments, return, frame, and saved registers;
- branch direction, fallthrough, loops, shared tails, and early returns;
- switch values, jump-table targets, and source-order case layout;
- load and store widths, signedness, bitfields, and aggregate offsets;
- literal pools, addresses, constants, calls, and values live across calls;
- side effects and repeated reads that optimization is allowed to combine.

Assemble and disassemble reference `.s` when resolving pools and table targets;
do not hand-count PC-relative offsets across alignment. Resolve message IDs
through `assets/text/message_archive.json`. Text and neighboring tables often
identify a scene or binding without importing outside knowledge.

### 3. Recover source structure

Write the likely compiler input, not optimized assembly expressed in C syntax.
Begin with ordinary structs, arrays, loops, switches, and expressions. Let GCC
perform strength reduction, common-tail merging, and register allocation.
Manually imitating those passes usually changes earlier RTL and moves the
result farther away.

Use C89 declarations and short evidence-backed names. Preserve observable
original mistakes. Replace names such as `Func_02000f80`, `status_12c`, and
anonymous numeric cases only when local evidence supports something better;
honest names make incorrect bindings visible.

#### Recover source voice, not a modern API

Reconstructed identifiers are reading aids, not claims about the lost source's
original spelling. The ROM can establish roles and relationships, but it does
not contain local variable names, typedef names, comments, header boundaries,
or macro spellings. Do not turn behavioral confidence into authorship claims.

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

Use these promotion tests:

1. The declarations form one stable vocabulary, table family, or interface.
2. The proposed name is no more specific than the local evidence.
3. Moving the declaration does not conceal an owner-specific compiler shape.
4. The header is useful as a unit, rather than a bag of unrelated leftovers.

If any test fails, keep the declaration local. Humanization should expose the
recovered algorithm, roles, and constraints while leaving uncertainty visible.

A trailing address, register hint, or SSA counter in an identifier
(`temp_r3_1908`, `var_sl_1853`, `sp4C`, `Func_080ab5e4` used as a *local*
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
tools/compiler/target/release/compiler workbench draft/080bbb0c.c
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

Keep one active `draft/<address>.c`. First verify that an edit changed emitted
assembly:

```sh
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --asm
```

Then compare linked bytes:

```sh
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --align
```

`--first` crops the first residual window. `--patch FILE` scores a unified diff
without changing the draft. Put independent theories in separate working
directories rather than racing edits against one tracked source file.

Read the complete normalized instruction diff and repair its largest coherent
hunk. A large mismatch means the source shape is still wrong. Fix algorithms,
case ownership, types, declaration lifetimes, and aggregates before trying
spellings or flags.

### 5. Invert a stalled last mile

Large mismatches and tiny stubborn mismatches require opposite responses. For
a large mismatch, discard the overall source shape. For an otherwise close
candidate whose same small scheduling, ordering, or allocation hunk survives
several genuinely different source families, stop trying to save the aggregate
score. Make the stubborn hunk the starting constraint instead.

These are early- and late-stage forms of the same rule: let the scale and
persistence of the emitted disagreement challenge the source shape. Do not
mistake the effort already invested in a candidate for evidence that its shape
must be preserved.

1. State the exact emitted invariant: the contiguous instructions, register
   roles, frame, branch direction, or other local shape that must not move.
2. Find ordinary C that emits that invariant naturally, even if hundreds of
   surrounding instructions become worse.
3. Freeze the invariant as a hard admission check. Keep a near-exact candidate
   that violates it only as diagnostic evidence; do not continue tuning it.
4. Repair outward from the admitted candidate, one coherent diff region at a
   time, rejecting every edit that loses the invariant.
5. Move the best admitted candidate into permutation once the remaining source
   boundaries and behavior are credible.

This inversion is warranted when independent source families keep reproducing
the same local defect, compiler dumps explain why its ancestry or scheduling is
locked, and new edits merely recolor the same values or move the defect. Do not
spend an open-ended search on two halfwords because the rest of the function
happens to match. Once that evidence exists, inversion is the next structural
experiment, not an emergency tactic reserved for exhaustive spelling search.
Conversely, do not invert on the first bad score: establish the repeated local
constraint with complete diffs and distinct structural experiments first.

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
Keep every run under ignored output, read a winner's complete diff, and rescore
it independently before harvesting it. When a search has a frozen local
invariant, apply that admission check to every retained winner; a lower internal
score that breaks the invariant is a rejected candidate. A lower internal score
is not proof of semantic correctness.

### 7. Adopt only exact work

For a main-image owner, adoption is allowed only when linked
`differing_halfwords` is zero:

```sh
mkdir -p scratch/adopt
cp draft/<address>.c scratch/adopt/src_<address>.c
cargo run --release --manifest-path tools/check/Cargo.toml -- integrate scratch/adopt
cargo run --release --manifest-path tools/check/Cargo.toml -- integrate --apply scratch/adopt
```

Until then, retain the reference assembly. A nearly exact draft is useful
research but does not count as DONE.

## Work on overlays

The structural workbench currently targets main-image owners. Use the overlay
host for overlay decoding, scoring, adoption, parking, and audits:

```sh
cargo run --release --manifest-path tools/overlay/Cargo.toml -- show resource_373 034c
cargo run --release --manifest-path tools/overlay/Cargo.toml -- score draft/resource_373_c_0200034c.c --align
cargo run --release --manifest-path tools/overlay/Cargo.toml -- adopt resource_373:034c --source draft/resource_373_c_0200034c.c --apply
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

When candidate and reference lengths differ, one insertion can shift every
later halfword, so the raw differing count is a poor compass. When lengths are
equal, zero is still the only completion state.

Work the largest coherent hunk in the complete instruction diff, not whichever
colored line appears first. Register names, spills, and frame size are compiler
consequences. A frame mismatch means some live range differs; adding dummy
locals to imitate the number does not fix the cause.

A matching prefix is real but fragile because one changed live range can
recolor the rest of an owner. Conversely, a plausible but incorrectly bound
body can align by luck. Prefer reference-backed structural conclusions over an
aggregate score that happens to improve.

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

## Keep tooling portable and small

There are six public executable hosts: `build-assets`, `build-stage`, `assets`,
`compiler`, `overlay`, and `check`. Contributor operations are subcommands of
those hosts. A game-specific encoder may remain a library behind an asset host;
it should not become another public command.

Tooling is Rust. Use maintained crates or established external tools for
generic plumbing such as JSON, PNG, DEFLATE, hashing, regex, temporary files,
file locking, directory walking, object comparison, and build scheduling.
Repository code should describe GBA and reconstruction-specific behavior.
Names are neutral and descriptive, not prefixed with the project name.

`make tooling-size` enforces the portable-toolkit budget. Keep a tool only when
it is required to build, inspect, prove, adopt, or permute a reconstruction and
would still be worth carrying into a new GBA decompilation project.

## Prove the repository

Use the narrowest useful command while iterating, then finish with the full
gate:

```sh
make build-claimed   # compile and link every adopted main-image owner
make build-asm       # assemble retained main-image regions
make build-assets    # rebuild tracked source assets
make build-full      # compose and compare every owned region
make build-rom       # produce the final ROM locally
make coverage        # regenerate metrics, figures, and Targets
make test            # tooling, policy, and focused regression tests
make verify          # authoritative byte-identical repository gate
```

`make verify` must finish with a byte-identical full build and fresh generated
artifacts. A clean compile count is insufficient: overlay reconstruction,
compressed assets, ownership manifests, and final composition can fail later.
Do not describe a change as verified when the authoritative gate stopped at a
baseline or environment error; report that blocker separately.

## Merge and commit cleanly

Generated coverage files are products, not merge authorities. When branches
adopt different owners, merge the source changes, regenerate with
`make coverage`, audit overlays, and run `make verify`. Do not resolve generated
metrics by choosing one branch's copy.

Before committing:

1. Inspect the complete diff and preserve unrelated work already in the tree.
2. Remove or relocate scratch output, generated binaries, and temporary links.
3. Run focused tests for the changed source or tooling.
4. Run `make verify` and state its exact outcome.
5. Generate, rather than guess, the required subject prefix.

```sh
make progress-subject
```

Commit subjects begin `☀️ N% – `. Codex-written commits are authored by Codex,
Claude-written commits by Claude, and human-written commits by that human. Do
not attribute an agent's work to Pascal.

If a large owner stops converging, leave one honest measured draft, record the
remaining structural problem, and move to a related owner. Exact siblings are
the best documentation of the approved compiler's source shapes; knowledge
compounds even when one target does not close immediately.

## Targets

This section is generated. It is the primary contributor target list:
non-overlapping audited source-owner scopes (or contiguous unresolved
executable runs), sorted largest to smallest. Regenerate with `make coverage` -- do not edit by hand.

- **Unfinished scopes:** 1,938
- **Address spaces scanned:** 97 (87 still contain targets)
- **Target bytes:** 855,068 semantic-C or unresolved-assembly bytes
- **Resolved-only bytes:** 323,520 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,347,122

### Main target list

This table contains every scope of at least 1,000 bytes (270 rows). The complete
1,938-row index, including the smallest audited owners, is
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json).

| Rank | Scope | Target | Namespace / owner |
|---:|---:|---:|---|
| 1 | 19,708 | 8,992 | `main:0x080e7404` |
| 2 | 18,914 | 9,506 | `main:0x080c91dc` |
| 3 | 14,744 | 5,028 | `main:0x080d1350` |
| 4 | 14,476 | 12,360 | `main:0x080a9e48` |
| 5 | 12,818 | 4,708 | `main:0x080d8988` |
| 6 | 9,968 | 8,988 | `main:0x080f6008` |
| 7 | 9,870 | 9,788 | `main:0x08022b44` |
| 8 | 8,560 | 5,372 | `main:0x080cde90` |
| 9 | 7,808 | 6,330 | `main:0x080e47b8` |
| 10 | 7,652 | 1,468 | `main:0x080dbbdc` |
| 11 | 6,458 | 5,524 | `main:0x080270ac` |
| 12 | 6,358 | 6,332 | `main:0x080bbb0c` |
| 13 | 6,192 | 4,200 | `main:0x080b2110` |
| 14 | 6,070 | 4,380 | `main:0x0801a910` |
| 15 | 5,794 | 3,456 | `main:0x080b0a6c` |
| 16 | 5,688 | 3,696 | `main:0x080a414c` |
| 17 | 5,608 | 956 | `main:0x080e0000` |
| 18 | 5,450 | 2,228 | `main:0x0808c2dc` |
| 19 | 5,330 | 3,208 | `main:0x0801cf48` |
| 20 | 5,050 | 3,656 | `main:0x080dea70` |
| 21 | 4,866 | 2,240 | `main:0x080d5274` |
| 22 | 4,718 | 1,704 | `main:0x08017c8c` |
| 23 | 4,710 | 4,044 | `main:0x080f4168` |
| 24 | 4,406 | 3,844 | `main:0x080a2324` |
| 25 | 4,302 | 2,640 | `main:0x08018efc` |
| 26 | 4,220 | 2,528 | `main:0x080be18c` |
| 27 | 4,170 | 2,900 | `main:0x08095bd8` |
| 28 | 4,158 | 2,296 | `main:0x0801edec` |
| 29 | 4,138 | 4,138 | `resource_380:0x02002c10` |
| 30 | 4,122 | 4,122 | `resource_373:0x02002284` |
| 31 | 4,046 | 1,720 | `main:0x080e2974` |
| 32 | 3,920 | 2,470 | `main:0x080e15e8` |
| 33 | 3,850 | 3,124 | `main:0x08077320` |
| 34 | 3,808 | 1,532 | `main:0x080d0000` |
| 35 | 3,756 | 1,876 | `main:0x080251d4` |
| 36 | 3,738 | 2,660 | `main:0x080b6cd0` |
| 37 | 3,702 | 3,702 | `resource_3c6:0x02000218` |
| 38 | 3,584 | 2,952 | `main:0x08026080` |
| 39 | 3,492 | 1,740 | `main:0x080e3a14` |
| 40 | 3,472 | 2,288 | `main:0x08010424` |
| 41 | 3,452 | 2,396 | `main:0x080ec100` |
| 42 | 3,442 | 3,442 | `resource_378:0x0200088c` |
| 43 | 3,408 | 3,408 | `resource_378:0x02001874` |
| 44 | 3,364 | 1,212 | `main:0x08096c80` |
| 45 | 3,274 | 1,024 | `main:0x080bae40` |
| 46 | 3,238 | 1,250 | `main:0x080e6638` |
| 47 | 3,156 | 2,514 | `main:0x080d765c` |
| 48 | 3,114 | 1,098 | `main:0x080fa678` |
| 49 | 3,104 | 2,810 | `main:0x080ad6d4` |
| 50 | 3,086 | 2,980 | `main:0x08021e6c` |
| 51 | 3,078 | 3,078 | `resource_3c9:0x0200124c` |
| 52 | 3,074 | 3,074 | `resource_383:0x02000b48` |
| 53 | 3,068 | 1,648 | `main:0x080ece7c` |
| 54 | 3,058 | 3,058 | `resource_3c5:0x0200186c` |
| 55 | 3,056 | 2,084 | `main:0x0800ebec` |
| 56 | 3,046 | 3,046 | `resource_395:0x02000470` |
| 57 | 3,046 | 3,046 | `resource_39d:0x02001adc` |
| 58 | 2,946 | 2,426 | `main:0x080167e0` |
| 59 | 2,918 | 2,512 | `main:0x080b6148` |
| 60 | 2,908 | 2,908 | `resource_381:0x020003cc` |
| 61 | 2,854 | 2,854 | `resource_374:0x0200155c` |
| 62 | 2,778 | 904 | `main:0x08020bd8` |
| 63 | 2,772 | 2,576 | `main:0x0808f52c` |
| 64 | 2,734 | 2,734 | `resource_3a8:0x020026a8` |
| 65 | 2,706 | 1,588 | `main:0x080a345c` |
| 66 | 2,698 | 2,256 | `main:0x080a8914` |
| 67 | 2,678 | 1,652 | `main:0x08095160` |
| 68 | 2,672 | 2,672 | `resource_3a8:0x02000aac` |
| 69 | 2,658 | 1,388 | `main:0x080c1fa8` |
| 70 | 2,650 | 2,376 | `main:0x080999f0` |
| 71 | 2,630 | 2,166 | `main:0x080a7a34` |
| 72 | 2,596 | 1,316 | `main:0x080c0000` |
| 73 | 2,574 | 2,574 | `resource_39e:0x02002ec8` |
| 74 | 2,568 | 2,252 | `main:0x08012518` |
| 75 | 2,534 | 2,534 | `resource_3bf:0x0200298c` |
| 76 | 2,530 | 1,688 | `main:0x080b8574` |
| 77 | 2,508 | 1,868 | `main:0x08020000` |
| 78 | 2,502 | 2,502 | `resource_3bf:0x02003c84` |
| 79 | 2,476 | 2,476 | `resource_3af:0x0200252c` |
| 80 | 2,440 | 2,368 | `main:0x080bf678` |
| 81 | 2,430 | 2,244 | `main:0x080a6a00` |
| 82 | 2,426 | 2,426 | `resource_3b8:0x0200338c` |
| 83 | 2,418 | 2,418 | `resource_38d:0x02000894` |
| 84 | 2,386 | 2,386 | `resource_3b8:0x02001d7c` |
| 85 | 2,382 | 2,382 | `resource_383:0x02003c6c` |
| 86 | 2,360 | 1,328 | `main:0x080dd9c0` |
| 87 | 2,342 | 2,342 | `resource_3aa:0x020010f0` |
| 88 | 2,310 | 2,310 | `resource_3b8:0x02002a84` |
| 89 | 2,310 | 116 | `main:0x080113e4` |
| 90 | 2,298 | 2,298 | `resource_380:0x02000a98` |
| 91 | 2,294 | 2,294 | `resource_3c9:0x02002360` |
| 92 | 2,294 | 1,912 | `main:0x0808e0b0` |
| 93 | 2,236 | 2,236 | `resource_39e:0x02001494` |
| 94 | 2,230 | 2,230 | `resource_3b8:0x02000b40` |
| 95 | 2,230 | 2,230 | `resource_3bd:0x02001d4c` |
| 96 | 2,222 | 2,222 | `resource_3bc:0x02001a08` |
| 97 | 2,218 | 2,218 | `resource_378:0x020027d4` |
| 98 | 2,218 | 2,218 | `resource_3c9:0x02003e8c` |
| 99 | 2,218 | 1,028 | `main:0x08079008` |
| 100 | 2,206 | 2,206 | `resource_377:0x0200063c` |
| 101 | 2,194 | 2,194 | `resource_376:0x02000640` |
| 102 | 2,190 | 2,190 | `resource_396:0x0200069c` |
| 103 | 2,182 | 1,696 | `main:0x0808d828` |
| 104 | 2,180 | 2,180 | `resource_3bd:0x02002c38` |
| 105 | 2,172 | 1,336 | `main:0x0808a5f8` |
| 106 | 2,170 | 2,170 | `resource_3ca:0x02000430` |
| 107 | 2,166 | 2,166 | `resource_38f:0x020011ec` |
| 108 | 2,166 | 2,166 | `resource_3c9:0x02002c58` |
| 109 | 2,158 | 2,158 | `resource_373:0x02001a14` |
| 110 | 2,118 | 2,118 | `resource_371:0x020028e8` |
| 111 | 2,106 | 2,106 | `resource_3bd:0x020013d4` |
| 112 | 2,070 | 2,070 | `resource_3b1:0x02001b34` |
| 113 | 2,068 | 2,068 | `resource_3c2:0x02000240` |
| 114 | 2,056 | 1,252 | `main:0x080f0254` |
| 115 | 2,052 | 2,052 | `resource_39e:0x020038d8` |
| 116 | 2,046 | 1,200 | `main:0x0800300c` |
| 117 | 2,026 | 1,882 | `main:0x0800a97c` |
| 118 | 2,018 | 2,018 | `resource_3a5:0x020004e4` |
| 119 | 2,014 | 2,014 | `resource_3c9:0x02000a6c` |
| 120 | 2,006 | 2,006 | `resource_3a4:0x02001830` |
| 121 | 2,002 | 2,002 | `resource_38d:0x020019b0` |
| 122 | 2,002 | 2,002 | `resource_3af:0x02003a0c` |
| 123 | 1,978 | 1,978 | `resource_3ad:0x02000808` |
| 124 | 1,928 | 904 | `main:0x08097b54` |
| 125 | 1,902 | 1,292 | `main:0x080983a0` |
| 126 | 1,866 | 1,722 | `main:0x080b9b30` |
| 127 | 1,854 | 1,854 | `resource_3c8:0x020026f8` |
| 128 | 1,822 | 1,822 | `resource_38f:0x02001e6c` |
| 129 | 1,816 | 1,712 | `main:0x08090a5c` |
| 130 | 1,810 | 1,810 | `resource_383:0x02002fd4` |
| 131 | 1,810 | 1,300 | `main:0x08006088` |
| 132 | 1,808 | 1,808 | `resource_3b1:0x02005c9c` |
| 133 | 1,800 | 992 | `main:0x0809b0dc` |
| 134 | 1,796 | 1,664 | `main:0x080f3078` |
| 135 | 1,782 | 1,660 | `main:0x080de2f8` |
| 136 | 1,778 | 1,778 | `resource_3b9:0x02001cd4` |
| 137 | 1,770 | 1,770 | `resource_39e:0x020027dc` |
| 138 | 1,770 | 1,770 | `resource_3b1:0x020028c8` |
| 139 | 1,734 | 1,734 | `resource_383:0x02001e80` |
| 140 | 1,730 | 1,730 | `main:0x080f2028` |
| 141 | 1,718 | 708 | `main:0x080ba2c0` |
| 142 | 1,704 | 1,704 | `resource_3a2:0x02000924` |
| 143 | 1,702 | 1,702 | `resource_381:0x02002150` |
| 144 | 1,702 | 1,702 | `resource_3b7:0x020001d8` |
| 145 | 1,698 | 1,698 | `resource_3ae:0x02000ba0` |
| 146 | 1,692 | 400 | `main:0x0808b824` |
| 147 | 1,682 | 1,682 | `resource_39c:0x02004888` |
| 148 | 1,648 | 1,648 | `resource_3a2:0x020001dc` |
| 149 | 1,638 | 1,638 | `resource_373:0x02004084` |
| 150 | 1,636 | 1,636 | `main:0x0800cacc` |
| 151 | 1,634 | 1,056 | `main:0x080ae2f4` |
| 152 | 1,632 | 1,632 | `resource_39c:0x02002f58` |
| 153 | 1,626 | 1,626 | `resource_375:0x020000dc` |
| 154 | 1,614 | 1,614 | `resource_3bf:0x02003374` |
| 155 | 1,586 | 1,586 | `resource_3c5:0x02001238` |
| 156 | 1,562 | 1,170 | `main:0x080a63e4` |
| 157 | 1,560 | 832 | `main:0x08098cd8` |
| 158 | 1,550 | 1,550 | `resource_379:0x02000484` |
| 159 | 1,514 | 1,514 | `resource_3a4:0x02000d24` |
| 160 | 1,506 | 1,264 | `main:0x080b5534` |
| 161 | 1,480 | 1,480 | `resource_399:0x02000f84` |
| 162 | 1,462 | 1,462 | `resource_373:0x020039e8` |
| 163 | 1,458 | 1,458 | `resource_3c8:0x020047ac` |
| 164 | 1,448 | 1,448 | `resource_3b0:0x02000af8` |
| 165 | 1,448 | 1,448 | `resource_3b1:0x020050dc` |
| 166 | 1,426 | 912 | `main:0x0809a8c4` |
| 167 | 1,410 | 1,410 | `resource_383:0x020036e8` |
| 168 | 1,410 | 1,138 | `main:0x08090000` |
| 169 | 1,410 | 908 | `main:0x080c1a24` |
| 170 | 1,402 | 1,402 | `resource_3b1:0x0200234c` |
| 171 | 1,398 | 1,398 | `resource_372:0x02001b18` |
| 172 | 1,392 | 842 | `main:0x080d4ce8` |
| 173 | 1,382 | 1,382 | `resource_3c9:0x02003924` |
| 174 | 1,378 | 1,378 | `resource_380:0x02001780` |
| 175 | 1,366 | 1,366 | `resource_37a:0x02000488` |
| 176 | 1,366 | 1,366 | `resource_3aa:0x0200077c` |
| 177 | 1,354 | 676 | `main:0x08017364` |
| 178 | 1,346 | 324 | `main:0x080b04dc` |
| 179 | 1,326 | 1,326 | `resource_391:0x02002004` |
| 180 | 1,318 | 1,024 | `main:0x08092b94` |
| 181 | 1,310 | 1,310 | `resource_373:0x020034c8` |
| 182 | 1,310 | 656 | `main:0x080994d0` |
| 183 | 1,308 | 1,308 | `resource_37a:0x02002094` |
| 184 | 1,306 | 1,306 | `resource_3a8:0x02000590` |
| 185 | 1,290 | 1,290 | `resource_3af:0x02000c94` |
| 186 | 1,274 | 1,274 | `resource_387:0x0200066c` |
| 187 | 1,270 | 1,270 | `resource_371:0x02001064` |
| 188 | 1,270 | 1,270 | `resource_3b1:0x020037d8` |
| 189 | 1,262 | 1,262 | `resource_38d:0x020012a0` |
| 190 | 1,238 | 1,238 | `resource_373:0x02004b24` |
| 191 | 1,234 | 1,234 | `resource_39d:0x02001608` |
| 192 | 1,234 | 1,234 | `resource_3af:0x02001db0` |
| 193 | 1,230 | 1,230 | `resource_371:0x02001f6c` |
| 194 | 1,230 | 1,230 | `resource_374:0x02000b8c` |
| 195 | 1,230 | 826 | `main:0x080a7380` |
| 196 | 1,222 | 1,222 | `resource_37a:0x0200155c` |
| 197 | 1,222 | 930 | `main:0x080ba978` |
| 198 | 1,218 | 1,218 | `resource_37b:0x02000554` |
| 199 | 1,218 | 996 | `main:0x0801c46c` |
| 200 | 1,210 | 1,210 | `resource_391:0x02001740` |
| 201 | 1,198 | 1,198 | `resource_3b1:0x020057ec` |
| 202 | 1,196 | 678 | `main:0x08004d2c` |
| 203 | 1,194 | 964 | `main:0x080a1090` |
| 204 | 1,190 | 1,190 | `resource_3bd:0x02002604` |
| 205 | 1,182 | 1,182 | `resource_3c9:0x02004738` |
| 206 | 1,180 | 1,180 | `resource_37b:0x0200101c` |
| 207 | 1,176 | 1,176 | `resource_37b:0x02000a18` |
| 208 | 1,176 | 1,176 | `resource_380:0x020043bc` |
| 209 | 1,174 | 1,174 | `resource_3bc:0x020040bc` |
| 210 | 1,170 | 1,170 | `resource_370:0x020003cc` |
| 211 | 1,170 | 1,170 | `resource_3b3:0x0200174c` |
| 212 | 1,162 | 392 | `main:0x08094730` |
| 213 | 1,160 | 908 | `main:0x080a847c` |
| 214 | 1,158 | 344 | `main:0x0808ec50` |
| 215 | 1,154 | 1,154 | `resource_37f:0x02001604` |
| 216 | 1,152 | 1,152 | `main:0x080f26ec` |
| 217 | 1,150 | 1,150 | `resource_375:0x020014f4` |
| 218 | 1,142 | 1,142 | `resource_373:0x02004ffc` |
| 219 | 1,136 | 908 | `main:0x080d0ee0` |
| 220 | 1,130 | 460 | `main:0x080b0070` |
| 221 | 1,126 | 1,126 | `resource_37f:0x02000f6c` |
| 222 | 1,126 | 456 | `main:0x080798b4` |
| 223 | 1,114 | 1,114 | `resource_3c7:0x02000880` |
| 224 | 1,110 | 1,110 | `resource_372:0x02003394` |
| 225 | 1,110 | 1,110 | `resource_3ce:0x02000244` |
| 226 | 1,106 | 1,106 | `resource_39e:0x02001dbc` |
| 227 | 1,102 | 1,102 | `resource_3bb:0x020010dc` |
| 228 | 1,102 | 1,102 | `resource_3c9:0x02004fec` |
| 229 | 1,102 | 124 | `main:0x08003c80` |
| 230 | 1,098 | 1,098 | `main:0x080a5b94` |
| 231 | 1,096 | 1,096 | `resource_3b9:0x020011c4` |
| 232 | 1,094 | 1,094 | `resource_371:0x02000c1c` |
| 233 | 1,094 | 1,094 | `resource_3bc:0x02001474` |
| 234 | 1,094 | 378 | `main:0x0800b684` |
| 235 | 1,090 | 1,090 | `resource_372:0x020037ec` |
| 236 | 1,090 | 1,090 | `resource_39e:0x0200064c` |
| 237 | 1,082 | 148 | `main:0x080216b4` |
| 238 | 1,078 | 1,078 | `resource_373:0x020015dc` |
| 239 | 1,078 | 1,078 | `resource_373:0x020046ec` |
| 240 | 1,078 | 1,078 | `resource_399:0x020019bc` |
| 241 | 1,078 | 568 | `main:0x08078aa0` |
| 242 | 1,074 | 1,074 | `resource_3ba:0x02000db8` |
| 243 | 1,068 | 896 | `main:0x080bd424` |
| 244 | 1,066 | 780 | `main:0x08091814` |
| 245 | 1,060 | 456 | `main:0x08010000` |
| 246 | 1,054 | 1,054 | `resource_39d:0x020011e8` |
| 247 | 1,052 | 1,052 | `resource_3b1:0x02004254` |
| 248 | 1,052 | 978 | `main:0x0808bec0` |
| 249 | 1,050 | 1,050 | `resource_372:0x020028a4` |
| 250 | 1,050 | 1,050 | `resource_3aa:0x02000360` |
| 251 | 1,050 | 1,050 | `resource_3aa:0x02000cd4` |
| 252 | 1,046 | 1,046 | `resource_380:0x020027f8` |
| 253 | 1,046 | 1,046 | `resource_381:0x02001b34` |
| 254 | 1,042 | 1,042 | `resource_377:0x02000f90` |
| 255 | 1,042 | 1,042 | `resource_3a5:0x02001490` |
| 256 | 1,042 | 1,042 | `resource_3c9:0x02004bd8` |
| 257 | 1,038 | 1,038 | `resource_379:0x02000074` |
| 258 | 1,036 | 956 | `main:0x08029504` |
| 259 | 1,034 | 1,034 | `resource_39d:0x02000ddc` |
| 260 | 1,034 | 652 | `main:0x080b9724` |
| 261 | 1,030 | 1,030 | `resource_37b:0x020015d4` |
| 262 | 1,030 | 1,030 | `resource_38f:0x02001a64` |
| 263 | 1,030 | 1,030 | `resource_391:0x02001bfc` |
| 264 | 1,026 | 1,026 | `resource_37b:0x02000150` |
| 265 | 1,024 | 856 | `main:0x080a5fe0` |
| 266 | 1,022 | 1,022 | `resource_3b8:0x020006dc` |
| 267 | 1,022 | 636 | `main:0x08016018` |
| 268 | 1,020 | 1,020 | `resource_3b9:0x02000db0` |
| 269 | 1,010 | 1,010 | `resource_37a:0x02001ca0` |
| 270 | 1,006 | 1,006 | `resource_394:0x020003f0` |
