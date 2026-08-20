# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the English release of
_Golden Sun_ for the Game Boy Advance. The repository succeeds when its C and
independently described assets rebuild the released ROM byte for byte.

This is the sole contributor guide. `AGENTS.md` and `CLAUDE.md` are symlinks to
it; `README.md` is for end users. Do not add more Markdown or text guides.

Readable C, names, documentation, and fast tools matter. They do not count as
decompilation progress until the rebuilt bytes match.

## Ground rules

### Evidence

Game-specific knowledge may come only from the approved local ROMs and this
repository's own reconstruction work:

```text
gs1-{en,ja,de,es,fr,it}.gba
gs2-{en,ja,de,es,fr,it}.gba
```

`gs1-en.gba` is the build target. Other editions may establish shared layout
or behavior, but never an original name or authorship. Do not inspect another
Golden Sun decompilation, symbol list, pseudocode dump, history, or notes.
Generic compiler, architecture, C, Rust, and pret repository conventions are
fine. Public pret game code is not evidence for this game.

Names must say only what local evidence demonstrates. Message text, tables,
callers, pools, and cross-edition correspondence are evidence. Familiar lore
that has not been derived here is not.

### Publication

Publish reconstructed source, source assets, deterministic transform plans,
and semantic metadata. Never commit or transmit ROMs, ROM fragments, binary
patches, object files, ELFs, compiler output, opaque dumps, private analysis,
credentials, or built game images. Never send local ROM content to a network
service.

`check-publication` checks staged and outgoing content, including material
added and later deleted in the pushed range. It cannot verify provenance;
contributors must honor the evidence boundary themselves.

### C and assembly

There are two build tiers:

- Exact C: C that compiles to every reference byte.
- Assembly: everything not yet expressed as exact C.

There is no semantic-C coverage tier. Active near-matches live as one
`draft/<owner>.c` plus `draft/<owner>.json`. Disposable variants and tool
output live in ignored `scratch/`. Neither counts toward progress.

Do not use inline assembly, fixed-register variables, empty assembly barriers,
copied bytes, or equivalent escape hatches in C. An unfinished owner stays in
`asm/` or its overlay assembly. `no-asm-c` enforces this.

Permanent assembly is reserved for instruction shapes the compiler cannot
emit. The claim and evidence belong in `asm/classification.json`. Difficulty,
size, or a stubborn score is not evidence of permanence.

## Setup

Install Rust and `arm-none-eabi-binutils`. Put the approved ROMs in ignored
`roms/`. Build the staged compiler in `alchemy-gcc/dist/`.

Enable the hooks and generated-file merge driver once:

```sh
git config core.hooksPath .hooks
git config merge.generated.driver true
```

In a worktree, do not run `git submodule` commands. Link the main checkout's
ROM directory and staged compiler output instead:

```sh
ln -s /path/to/main/roms roms
mkdir -p alchemy-gcc
ln -s /path/to/main/alchemy-gcc/dist alchemy-gcc/dist
```

Start by proving the checkout:

```sh
make build-claimed
make verify
cargo build --release --manifest-path tools/compiler/Cargo.toml
```

Use release binaries in the inner loop. Repeated `cargo run` startup is large
relative to a candidate compile.

## The reconstruction loop

1. Pick an audited owner.
2. Read its complete reference assembly and literal pools.
3. Reconstruct plain, era-appropriate C from that evidence.
4. Confirm the source edit changes assembly.
5. Compare linked bytes.
6. Fix the largest structural misread.
7. Adopt only at zero differing bytes.

This order is deliberate. Compiler-option sweeps and random permutation are
last-mile tools. They cannot repair a wrong algorithm, case binding, type,
loop, or aggregate layout.

### Pick an owner, not a byte run

An owner is one function-sized region with a fixed entry point. Main-image
assembly is `asm/<address>.s`; adopted C is `exact/<address>.c`. Overlay rows
live in `assets/code/resource_<id>_overlay.s`; adopted C is
`exact/resource_<id>_c_<address>.c`.

The generated Targets table ranks unresolved byte runs. A row may begin in a
pool and is not necessarily a function entry. Select entries from
`semantic/regions.json`, then ensure the full span lies inside one audited
executable interval in `metrics/gs1-en-executable.json`. The adoption gate
rejects spans crossing an unaudited gap.

Prefer cohorts: owners or case bodies with the same instruction shape.
Comparing siblings exposes the one source feature that controls codegen and
prevents endless spelling changes to a single file.

### Read before writing

Establish these facts from the reference first:

- prologue, saved registers, frame, arguments, and owner boundary;
- load widths and signedness;
- loop and branch structure, fallthrough, and shared tails;
- switch table values, targets, and source-order body layout;
- literal-pool constants and addresses;
- aggregate bases and field offsets;
- calls, side effects, and values live across calls.

Assemble and disassemble reference `.s` when resolving pools. Hand-counting PC
offsets over alignments is not reliable. Message IDs can be resolved through
`assets/text/message_archive.json`; nearby text often identifies a scene or
binding directly.

Write the compiler's likely input, not a transcription of optimized output.
Start with ordinary arrays, structs, loops, switches, and expressions. GCC's
strength reduction, tail merging, and register allocation should recreate the
machine shape. Hand-performing those optimizations usually changes earlier
passes and makes the result worse.

Use C89 declarations, short evidence-backed names, and macros for genuinely
repeated case tails. Preserve observable original mistakes. Do not keep names
such as `Func_02000f80`, `status_12c`, or anonymous numeric cases once their
meaning is locally established; good names expose bad bindings.

## Main-image workflow

Create or update `draft/<address>.c`, then ask whether the edit changed the
instruction stream:

```sh
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --asm
```

Score linked bytes only after the source shape moved:

```sh
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --align
```

`--first` crops the first residual window. `--patch FILE` scores one unified
diff without modifying the draft. Independent theories belong in separate
work directories, not concurrent edits to one file.

Adopt only an exact owner:

```sh
make dispatch-decomp ARGS='integrate_matches draft/<address>.c --apply'
```

## Overlay workflow

Use the overlay host for decoding, reconstruction, scoring, adoption, parking,
and audits:

```sh
cargo run --release --manifest-path tools/overlay/Cargo.toml -- show resource_373:034c
cargo run --release --manifest-path tools/overlay/Cargo.toml -- score draft/resource_373_c_0200034c.c --align
cargo run --release --manifest-path tools/overlay/Cargo.toml -- adopt resource_373:034c --source draft/resource_373_c_0200034c.c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- park resource_373:034c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- audit --all
```

Adoption temporarily splices the row into its overlay, so run one adoption at
a time. Scoring is read-only and may run in parallel. After merging branches
that adopted overlays, regenerate coverage, run `overlay audit --all`, and
verify the full build.

## Measuring honestly

`differing_halfwords` is the gate: adoption requires zero. It is not a useful
compass while lengths differ, because one missing instruction shifts every
later byte.

The normalized instruction diff is the compass. Work the largest hunk in the
whole-file diff, not the first colored row. Register names and spill slots are
allocator output; normalize them when deciding whether source structure is
still wrong.

A matching prefix is real but fragile. One changed live range can recolor an
entire function. A frame-size difference means the maximum live spill set
differs somewhere; do not pad the source to imitate the number. Equal file
size is not completeness.

When a structurally justified edit makes an aggregate score worse, trust the
reference reading. Plausible but incorrectly bound bodies can align by luck.

## The permuter

Use the permuter after the algorithm, control flow, declarations, and owner
extent are credible. It is good at local expression, declaration, and
statement-order choices. It is not a decompiler.

The native permuter supports explicit `PERM_*` choices, AST randomization,
parallel workers, heat guidance, chained improvements, resume/import, and an
exact stop. See the live CLI before a run:

```sh
tools/compiler/target/release/compiler permute --help
```

Keep run output outside tracked source. Harvest a candidate only after reading
its complete diff and independently rescoring it. Do not let a heat map prevent
exploration; mixed guided and unguided seeds are appropriate when a run has
stagnated.

## Read the compiler

The compiler source is part of the project. Before sweeping spellings, inspect
the relevant GCC 2.96 pass. Source order controls switch-body layout. Signed
loads, bitfields, constant construction, address forms, and declaration order
all alter RTL before register allocation.

Compiler changes belong in the separate compiler repository with its GPL
notices and corresponding source. The main repository records only the staged
compiler revision/digests it accepts. Do not invent target options or use a
compiler patch to hide a source reconstruction error.

## Tooling standard

There are six public executables: `build-assets`, `build-stage`, `assets`,
`compiler`, `overlay`, and `check`. Contributor commands are subcommands of
those hosts. Game-specific encoders may remain small libraries behind the
asset host; they are not separate public tools.

Repository tooling is Rust. Use maintained crates for generic formats and
plumbing—JSON, PNG, DEFLATE, hashing, regex, temporary files, file locking,
directory walking—instead of maintaining local substitutes. Our code should
describe GBA and project-specific transformations. Tool and crate names are
neutral and descriptive; do not prefix them with the project name.

Keep tooling below the line budget enforced by `make tooling-size`. A tool
survives only if it is required to build, prove, inspect, adopt, or permute a
reconstruction and would be useful when starting another GBA decompilation.

## Build and gates

```sh
make build-claimed   # compile and link every adopted main-image owner
make build-asm       # assemble retained main-image regions
make build-assets    # rebuild tracked source assets
make build-full      # compose and compare all owned regions
make build-rom       # produce the final ROM locally
make coverage        # regenerate metrics, figures, and Targets
make test            # focused tests and policy checks
make verify          # authoritative gate
```

`make verify` must end with a byte-identical full build and fresh generated
artifacts. A successful compilation count is not a full build: overlay and
asset mismatches can fail later in the pipeline.

## Merging and committing

Generated metrics conflict when two branches adopt owners. Do not choose one
side as truth. Complete the merge, run `make coverage`, audit overlays, and
run `make verify`.

Before committing:

1. Inspect the entire diff and preserve unrelated user work.
2. Remove scratch output and generated binaries.
3. Run the focused test for changed tooling.
4. Run `make verify`.
5. Regenerate progress and use the required subject prefix.

```sh
make progress-subject
```

Subjects begin `☀️ N% – `. The percentage is generated; do not guess it.
Codex commits are authored by Codex, Claude commits by Claude, and human work
by the human who wrote it. Do not attribute an agent's changes to Pascal.

When a large owner will not converge, record its honest draft measurement and
move to another cohort. Finished owners are the best examples of this
compiler's source shapes; understanding compounds even when one target does
not close.

## Targets

This section is generated. It is the primary contributor target list:
non-overlapping audited source-owner scopes (or contiguous unresolved
executable runs), sorted largest to smallest. Broader multi-owner campaign cuts
belong in [Status](#status); they may overlap and therefore are not used for
byte accounting. Regenerate with `make coverage` -- do not edit by hand.

- **Unfinished scopes:** 2,232
- **Address spaces scanned:** 97 (87 still contain targets)
- **Target bytes:** 855,068 semantic-C or unresolved-assembly bytes
- **Resolved-only bytes:** 490,298 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,347,122

### Main target list

This table contains every scope of at least 1,000 bytes (228 rows). The complete
2,232-row index, including the smallest audited owners, is
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json).

| Rank | Scope | Target | Namespace / owner |
|---:|---:|---:|---|
| 1 | 6,332 | 6,332 | `main:0x080bbb0c` |
| 2 | 4,888 | 4,888 | `main:0x080ab5e4` |
| 3 | 4,224 | 4,224 | `main:0x08027114` |
| 4 | 4,138 | 4,138 | `resource_380:0x02002c10` |
| 5 | 4,122 | 4,122 | `resource_373:0x02002284` |
| 6 | 3,804 | 3,804 | `main:0x080f6440` |
| 7 | 3,702 | 3,702 | `resource_3c6:0x02000218` |
| 8 | 3,656 | 3,656 | `main:0x080dea70` |
| 9 | 3,442 | 3,442 | `resource_378:0x0200088c` |
| 10 | 3,408 | 3,408 | `resource_378:0x02001874` |
| 11 | 3,320 | 3,320 | `main:0x08023178` |
| 12 | 3,128 | 3,128 | `main:0x080a2680` |
| 13 | 3,104 | 2,810 | `main:0x080ad6d4` |
| 14 | 3,078 | 3,078 | `resource_3c9:0x0200124c` |
| 15 | 3,074 | 3,074 | `resource_383:0x02000b48` |
| 16 | 3,058 | 3,058 | `resource_3c5:0x0200186c` |
| 17 | 3,046 | 3,046 | `resource_395:0x02000470` |
| 18 | 3,046 | 3,046 | `resource_39d:0x02001adc` |
| 19 | 2,908 | 2,908 | `resource_381:0x020003cc` |
| 20 | 2,854 | 2,854 | `resource_374:0x0200155c` |
| 21 | 2,756 | 2,756 | `main:0x08023e70` |
| 22 | 2,734 | 2,734 | `resource_3a8:0x020026a8` |
| 23 | 2,672 | 2,672 | `resource_3a8:0x02000aac` |
| 24 | 2,574 | 2,574 | `resource_39e:0x02002ec8` |
| 25 | 2,534 | 2,534 | `resource_3bf:0x0200298c` |
| 26 | 2,508 | 2,508 | `main:0x080cbc0c` |
| 27 | 2,502 | 2,502 | `resource_3bf:0x02003c84` |
| 28 | 2,476 | 2,476 | `resource_3af:0x0200252c` |
| 29 | 2,444 | 2,444 | `main:0x080eb754` |
| 30 | 2,426 | 2,426 | `resource_3b8:0x0200338c` |
| 31 | 2,418 | 2,418 | `resource_38d:0x02000894` |
| 32 | 2,388 | 2,388 | `main:0x080f7460` |
| 33 | 2,386 | 2,386 | `resource_3b8:0x02001d7c` |
| 34 | 2,382 | 2,382 | `resource_383:0x02003c6c` |
| 35 | 2,356 | 2,356 | `main:0x080d2464` |
| 36 | 2,342 | 2,342 | `resource_3aa:0x020010f0` |
| 37 | 2,316 | 2,316 | `main:0x0808f52c` |
| 38 | 2,310 | 2,310 | `resource_3b8:0x02002a84` |
| 39 | 2,300 | 2,300 | `main:0x08021e6c` |
| 40 | 2,298 | 2,298 | `resource_380:0x02000a98` |
| 41 | 2,294 | 2,294 | `resource_3c9:0x02002360` |
| 42 | 2,268 | 2,268 | `main:0x080d91dc` |
| 43 | 2,236 | 2,236 | `resource_39e:0x02001494` |
| 44 | 2,230 | 2,230 | `resource_3b8:0x02000b40` |
| 45 | 2,230 | 2,230 | `resource_3bd:0x02001d4c` |
| 46 | 2,222 | 2,222 | `resource_3bc:0x02001a08` |
| 47 | 2,218 | 2,218 | `resource_378:0x020027d4` |
| 48 | 2,218 | 2,218 | `resource_3c9:0x02003e8c` |
| 49 | 2,206 | 2,206 | `resource_377:0x0200063c` |
| 50 | 2,194 | 2,194 | `resource_376:0x02000640` |
| 51 | 2,190 | 2,190 | `resource_396:0x0200069c` |
| 52 | 2,180 | 2,180 | `resource_3bd:0x02002c38` |
| 53 | 2,170 | 2,170 | `resource_3ca:0x02000430` |
| 54 | 2,166 | 2,166 | `resource_38f:0x020011ec` |
| 55 | 2,166 | 2,166 | `resource_3c9:0x02002c58` |
| 56 | 2,158 | 2,158 | `resource_373:0x02001a14` |
| 57 | 2,138 | 2,138 | `main:0x08026080` |
| 58 | 2,124 | 2,124 | `main:0x08024934` |
| 59 | 2,118 | 2,118 | `resource_371:0x020028e8` |
| 60 | 2,106 | 2,106 | `resource_3bd:0x020013d4` |
| 61 | 2,070 | 2,070 | `resource_3b1:0x02001b34` |
| 62 | 2,068 | 2,068 | `resource_3c2:0x02000240` |
| 63 | 2,052 | 2,052 | `resource_39e:0x020038d8` |
| 64 | 2,024 | 2,024 | `main:0x08077428` |
| 65 | 2,018 | 2,018 | `resource_3a5:0x020004e4` |
| 66 | 2,014 | 2,014 | `resource_3c9:0x02000a6c` |
| 67 | 2,006 | 2,006 | `resource_3a4:0x02001830` |
| 68 | 2,002 | 2,002 | `resource_38d:0x020019b0` |
| 69 | 2,002 | 2,002 | `resource_3af:0x02003a0c` |
| 70 | 1,980 | 1,980 | `main:0x080acab8` |
| 71 | 1,978 | 1,978 | `resource_3ad:0x02000808` |
| 72 | 1,968 | 1,968 | `main:0x080e823c` |
| 73 | 1,920 | 1,920 | `main:0x080f7f78` |
| 74 | 1,876 | 1,876 | `main:0x0802592c` |
| 75 | 1,854 | 1,854 | `resource_3c8:0x020026f8` |
| 76 | 1,822 | 1,822 | `resource_38f:0x02001e6c` |
| 77 | 1,816 | 1,816 | `main:0x080e99c0` |
| 78 | 1,816 | 1,712 | `main:0x08090a5c` |
| 79 | 1,810 | 1,810 | `resource_383:0x02002fd4` |
| 80 | 1,808 | 1,808 | `resource_3b1:0x02005c9c` |
| 81 | 1,796 | 1,664 | `main:0x080f3078` |
| 82 | 1,778 | 1,778 | `resource_3b9:0x02001cd4` |
| 83 | 1,770 | 1,770 | `resource_39e:0x020027dc` |
| 84 | 1,770 | 1,770 | `resource_3b1:0x020028c8` |
| 85 | 1,768 | 1,768 | `main:0x080d52c8` |
| 86 | 1,734 | 1,734 | `resource_383:0x02001e80` |
| 87 | 1,724 | 1,724 | `main:0x080e89ec` |
| 88 | 1,720 | 1,720 | `main:0x080e2974` |
| 89 | 1,704 | 1,704 | `resource_3a2:0x02000924` |
| 90 | 1,702 | 1,702 | `resource_381:0x02002150` |
| 91 | 1,702 | 1,702 | `resource_3b7:0x020001d8` |
| 92 | 1,698 | 1,698 | `resource_3ae:0x02000ba0` |
| 93 | 1,688 | 1,688 | `main:0x080b63c8` |
| 94 | 1,682 | 1,682 | `resource_39c:0x02004888` |
| 95 | 1,680 | 1,680 | `main:0x080a6ccc` |
| 96 | 1,660 | 1,660 | `main:0x080de2f8` |
| 97 | 1,648 | 1,648 | `main:0x080ed408` |
| 98 | 1,648 | 1,648 | `resource_3a2:0x020001dc` |
| 99 | 1,640 | 1,640 | `main:0x0800aa0c` |
| 100 | 1,638 | 1,638 | `resource_373:0x02004084` |
| 101 | 1,636 | 1,636 | `main:0x0800cacc` |
| 102 | 1,632 | 1,632 | `resource_39c:0x02002f58` |
| 103 | 1,626 | 1,626 | `resource_375:0x020000dc` |
| 104 | 1,614 | 1,614 | `resource_3bf:0x02003374` |
| 105 | 1,592 | 1,592 | `main:0x080168f4` |
| 106 | 1,588 | 1,588 | `main:0x08022b44` |
| 107 | 1,586 | 1,586 | `resource_3c5:0x02001238` |
| 108 | 1,576 | 1,576 | `main:0x080d2d98` |
| 109 | 1,556 | 1,556 | `main:0x080cf8e0` |
| 110 | 1,550 | 1,550 | `resource_379:0x02000484` |
| 111 | 1,540 | 1,540 | `main:0x080e3aa0` |
| 112 | 1,514 | 1,514 | `resource_3a4:0x02000d24` |
| 113 | 1,512 | 1,512 | `main:0x080e4e0c` |
| 114 | 1,504 | 1,504 | `main:0x08012518` |
| 115 | 1,480 | 1,480 | `resource_399:0x02000f84` |
| 116 | 1,468 | 1,468 | `main:0x080dbc30` |
| 117 | 1,462 | 1,462 | `resource_373:0x020039e8` |
| 118 | 1,458 | 1,458 | `resource_3c8:0x020047ac` |
| 119 | 1,448 | 1,448 | `resource_3b0:0x02000af8` |
| 120 | 1,448 | 1,448 | `resource_3b1:0x020050dc` |
| 121 | 1,410 | 1,410 | `resource_383:0x020036e8` |
| 122 | 1,408 | 1,408 | `main:0x08020244` |
| 123 | 1,402 | 1,402 | `resource_3b1:0x0200234c` |
| 124 | 1,398 | 1,398 | `resource_372:0x02001b18` |
| 125 | 1,382 | 1,382 | `resource_3c9:0x02003924` |
| 126 | 1,378 | 1,378 | `resource_380:0x02001780` |
| 127 | 1,376 | 1,376 | `main:0x0801a98c` |
| 128 | 1,366 | 1,366 | `resource_37a:0x02000488` |
| 129 | 1,366 | 1,366 | `resource_3aa:0x0200077c` |
| 130 | 1,360 | 1,360 | `main:0x080c91dc` |
| 131 | 1,336 | 1,336 | `main:0x080a9f10` |
| 132 | 1,326 | 1,326 | `resource_391:0x02002004` |
| 133 | 1,324 | 1,324 | `main:0x080bf678` |
| 134 | 1,310 | 1,310 | `resource_373:0x020034c8` |
| 135 | 1,308 | 1,308 | `main:0x080aa768` |
| 136 | 1,308 | 1,308 | `resource_37a:0x02002094` |
| 137 | 1,306 | 1,306 | `resource_3a8:0x02000590` |
| 138 | 1,290 | 1,290 | `resource_3af:0x02000c94` |
| 139 | 1,274 | 1,274 | `resource_387:0x0200066c` |
| 140 | 1,272 | 1,272 | `main:0x080b0aac` |
| 141 | 1,270 | 1,270 | `resource_371:0x02001064` |
| 142 | 1,270 | 1,270 | `resource_3b1:0x020037d8` |
| 143 | 1,264 | 1,264 | `main:0x08099da4` |
| 144 | 1,262 | 1,262 | `resource_38d:0x020012a0` |
| 145 | 1,252 | 1,178 | `main:0x0800f2f8` |
| 146 | 1,248 | 1,248 | `main:0x080d9ae8` |
| 147 | 1,238 | 1,238 | `resource_373:0x02004b24` |
| 148 | 1,234 | 1,234 | `resource_39d:0x02001608` |
| 149 | 1,234 | 1,234 | `resource_3af:0x02001db0` |
| 150 | 1,230 | 1,230 | `resource_371:0x02001f6c` |
| 151 | 1,230 | 1,230 | `resource_374:0x02000b8c` |
| 152 | 1,228 | 1,228 | `main:0x080b6f44` |
| 153 | 1,226 | 1,226 | `main:0x080f4f04` |
| 154 | 1,222 | 1,222 | `resource_37a:0x0200155c` |
| 155 | 1,218 | 1,218 | `resource_37b:0x02000554` |
| 156 | 1,210 | 1,210 | `resource_391:0x02001740` |
| 157 | 1,204 | 1,204 | `main:0x080ce034` |
| 158 | 1,198 | 1,198 | `resource_3b1:0x020057ec` |
| 159 | 1,192 | 1,192 | `main:0x080da6cc` |
| 160 | 1,190 | 1,190 | `resource_3bd:0x02002604` |
| 161 | 1,182 | 1,182 | `resource_3c9:0x02004738` |
| 162 | 1,180 | 1,180 | `resource_37b:0x0200101c` |
| 163 | 1,176 | 1,176 | `resource_37b:0x02000a18` |
| 164 | 1,176 | 1,176 | `resource_380:0x020043bc` |
| 165 | 1,174 | 1,174 | `resource_3bc:0x020040bc` |
| 166 | 1,170 | 1,170 | `resource_370:0x020003cc` |
| 167 | 1,170 | 1,170 | `resource_3b3:0x0200174c` |
| 168 | 1,168 | 1,168 | `main:0x080a4924` |
| 169 | 1,154 | 1,154 | `resource_37f:0x02001604` |
| 170 | 1,152 | 1,152 | `main:0x080191cc` |
| 171 | 1,152 | 1,152 | `main:0x0801d4cc` |
| 172 | 1,152 | 1,152 | `main:0x080f26ec` |
| 173 | 1,150 | 1,150 | `resource_375:0x020014f4` |
| 174 | 1,144 | 1,144 | `main:0x080f2028` |
| 175 | 1,142 | 1,142 | `resource_373:0x02004ffc` |
| 176 | 1,128 | 1,128 | `main:0x080d0000` |
| 177 | 1,126 | 1,126 | `resource_37f:0x02000f6c` |
| 178 | 1,116 | 1,116 | `main:0x080c02a4` |
| 179 | 1,114 | 1,114 | `resource_3c7:0x02000880` |
| 180 | 1,110 | 1,110 | `resource_372:0x02003394` |
| 181 | 1,110 | 1,110 | `resource_3ce:0x02000244` |
| 182 | 1,106 | 1,106 | `resource_39e:0x02001dbc` |
| 183 | 1,104 | 1,104 | `main:0x080beb08` |
| 184 | 1,102 | 1,102 | `resource_3bb:0x020010dc` |
| 185 | 1,102 | 1,102 | `resource_3c9:0x02004fec` |
| 186 | 1,098 | 1,098 | `main:0x080fae58` |
| 187 | 1,096 | 1,096 | `main:0x080d41a4` |
| 188 | 1,096 | 1,096 | `resource_3b9:0x020011c4` |
| 189 | 1,094 | 1,094 | `resource_371:0x02000c1c` |
| 190 | 1,094 | 1,094 | `resource_3bc:0x02001474` |
| 191 | 1,090 | 1,090 | `resource_372:0x020037ec` |
| 192 | 1,090 | 1,090 | `resource_39e:0x0200064c` |
| 193 | 1,078 | 1,078 | `resource_373:0x020015dc` |
| 194 | 1,078 | 1,078 | `resource_373:0x020046ec` |
| 195 | 1,078 | 1,078 | `resource_399:0x020019bc` |
| 196 | 1,076 | 1,076 | `main:0x080030f8` |
| 197 | 1,074 | 1,074 | `main:0x080f4318` |
| 198 | 1,074 | 1,074 | `resource_3ba:0x02000db8` |
| 199 | 1,066 | 1,066 | `main:0x080e5e28` |
| 200 | 1,056 | 1,056 | `main:0x080ae2f4` |
| 201 | 1,054 | 1,054 | `resource_39d:0x020011e8` |
| 202 | 1,054 | 1,028 | `main:0x080d77b4` |
| 203 | 1,052 | 1,052 | `resource_3b1:0x02004254` |
| 204 | 1,052 | 978 | `main:0x0808bec0` |
| 205 | 1,050 | 1,050 | `resource_372:0x020028a4` |
| 206 | 1,050 | 1,050 | `resource_3aa:0x02000360` |
| 207 | 1,050 | 1,050 | `resource_3aa:0x02000cd4` |
| 208 | 1,046 | 1,046 | `resource_380:0x020027f8` |
| 209 | 1,046 | 1,046 | `resource_381:0x02001b34` |
| 210 | 1,044 | 1,044 | `main:0x0808d9a4` |
| 211 | 1,044 | 1,044 | `main:0x080bfba4` |
| 212 | 1,044 | 1,044 | `main:0x080cb7f8` |
| 213 | 1,042 | 1,042 | `resource_377:0x02000f90` |
| 214 | 1,042 | 1,042 | `resource_3a5:0x02001490` |
| 215 | 1,042 | 1,042 | `resource_3c9:0x02004bd8` |
| 216 | 1,040 | 1,040 | `main:0x080ceb54` |
| 217 | 1,040 | 1,040 | `main:0x080e90a8` |
| 218 | 1,038 | 1,038 | `resource_379:0x02000074` |
| 219 | 1,034 | 1,034 | `resource_39d:0x02000ddc` |
| 220 | 1,030 | 1,030 | `resource_37b:0x020015d4` |
| 221 | 1,030 | 1,030 | `resource_38f:0x02001a64` |
| 222 | 1,030 | 1,030 | `resource_391:0x02001bfc` |
| 223 | 1,028 | 1,028 | `main:0x0801de5c` |
| 224 | 1,026 | 1,026 | `resource_37b:0x02000150` |
| 225 | 1,022 | 1,022 | `resource_3b8:0x020006dc` |
| 226 | 1,020 | 1,020 | `resource_3b9:0x02000db0` |
| 227 | 1,010 | 1,010 | `resource_37a:0x02001ca0` |
| 228 | 1,006 | 1,006 | `resource_394:0x020003f0` |
