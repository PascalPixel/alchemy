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
