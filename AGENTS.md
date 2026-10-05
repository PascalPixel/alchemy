# Alchemy

Golden Sun: **The Broken Seal (TBS)** ☀️ and **The Lost Age (TLA)** ⚓️,
rebuilt byte for byte from readable C, assembly and editable assets, in all
twelve editions: each game in Japanese, English, German, Spanish, French and
Italian. Japanese releases are the source editions; localizations are measured
differences. Build IDs are `tbs` and `tla`, then the language: `tbs-ja`,
`tla-en`.

This file is the only place Alchemy's rules live. `README.md` is for fans.
Every rule has an ID, and a rule a tool can enforce names its check. Pascal
decides rule wording, compilers, what counts as DONE, and deleting anything
that cannot be restored; git history records when. Everything else goes ahead
without asking him, including fixing a check that wrongly blocks honest work,
as long as it still refuses everything it refused before.

## Goal

**DONE = matching C + proven library, handwritten or veneer assembly**, over
each game's executable bytes in all six of its editions together. 100% means
every language of a game builds all of its code from source. `make progress`
prints the exact counts with one line per edition, shows ⚓️ in its parts (C,
assembly and 8-byte stubs) and shows the bytes of FAKEMATCH-steered code as
their own number. ☀️ has the priority; the target is ☀️ 100% and ⚓️ 100%,
sharing as much code between the games as possible.

## Where things are

- `games/<GAME>/SRC`, `INCLUDE`, `SOUND`, `TEXT` and `games/COMMON` hold the
  source. `games/<GAME>/MAIN.LD` is the Japanese object order.
- `recon/<game>/raw` holds the English disassembly of code that is not C yet,
  one listing per function or run. `recon/<game>/en/MAIN.LD` is the English
  object order; German, Spanish, French and Italian follow it through their
  own `recon/<game>/<lang>/MAIN.LD`.
- `recon/<game>/<lang>` holds that edition's ROM scaffold (`rom.s`,
  `overlays.s`) and its drafts (`main/`, `overlays/`).
- `tools/` holds alchemy (one `alchemy` command for both games: build,
  checks, the count, matching, camelotizing and reports; see Commands) and
  ags (asset encoders and decoders). `out/` and `tools/out/` are build
  output and are ignored.
- `PROGRESS.png` (the repository by kind) and `PROGRESS_CHART.png` (DONE over
  time, with the models that did the work) are README's figures, drawn by
  `make figure` from `recon/tbs/metrics/history.tsv`.

## Rules

### Legal

- **L1** Never commit, upload or publish a ROM, the cartridge logo, private
  inputs or any other copy of the original game files. _Check: publication._
- **L2** Evidence comes only from our own ROMs, this repository and public
  documentation. Never use another project's Golden Sun work, and never open
  one to compare.
- **L3** Never use leaked Nintendo or Camelot code or SDKs, and never open, read
  or mention sources that might contain them.

### Counting

- **C1** A function counts only when its complete extent, literal pool
  included, compiles exactly and every edition stays byte-identical. A
  function counts in an edition only when that edition's build links its C.
  _Check: compare, compare-tla, compare-other-editions._
- **C2** DONE is pret's calcrom over the linker maps of byte-identical builds:
  the code the linker places from `games/`, counted once for each of the
  game's six editions whose build links it, out of six; sizes are the English
  build's, so code that exists only in another edition is outside the total.
  Uncredited disassembly is `not-yet-c` in `recon/<game>/raw`; only proven
  library, handwritten and veneer assembly counts as assembly. Whole aligned
  8-byte far-call stubs count as veneers. Padding a source marks as carrying
  no credit does not count. _Check: coverage-check._
- **C3** Main commits carry the verified percentage, README and both progress
  figures, written by `make land`. _Check: commit-msg hook, coverage-check._

### The oracle stays out of the build

The reference ROM judges the build and never feeds it. Any path by which the
expected answer steers the build or the count is cheating, whatever its form.
Reconstructing Camelot's compiler from habits that run through a whole game is
allowed (K3); fitting a compiler, an option or a routing to particular files
or functions is exactly this cheating.

- **O1** ROM bytes reach the build only through pret's
  `.incbin "baserom.gba", OFFSET, SIZE` scaffold in `recon/<game>`, which never
  counts. Nothing else derived from the ROM or the expected output (bytes,
  addresses, sizes, tokens, tables, hashes) may steer the build or the count,
  `out/` included; reports under `out/` are for people only.
  _Check: publication._
- **O2** Every name is defined where its bytes are, as a C definition or a
  label, and layout is the linker scripts' object order. No equate, alias,
  `#define` or table gives a name an address, except Camelot's own fixed
  addresses: resident IWRAM routines called through fixed entries, RAM
  buffers its code addressed as constants, and the hardware cartridge restart
  entry, each listed once in a game header and checked against where the
  linker placed it. One name per place; never a
  name that spells its own value. _Check: publication, fixed-address check._
- **O3** Scene and resource ids come only from their tables and are used whole.
  Overlay code never branches straight into the main image.
  _Check: the build's id gate._
- **O4** A mismatch is fixed in source, or the code stays disassembly or a
  draft. Before announcing a milestone, run an independent audit for leaks.
- **O5** Old git history, deleted files, backups and unreachable git objects
  are never evidence. Re-derive an answer from our ROMs and the current tree,
  or ask Pascal.

### Source

- **S1** `games/` holds only real source: C from an approved compiler, or
  proven assembly written as instructions, never copied bytes. Drafts and
  scaffolding live under `recon/` and shrink toward zero. _Check: publication._
- **S2** Tagged `/* FAKEMATCH: reason */` code counts until both games reach
  100%, and is then removed as the final step, as pret's was. Until then
  compiler-steering C (wrappers, volatile, forced temporaries, dead code),
  inline assembly and fixed-register variables are allowed when each carries a
  `FAKEMATCH` tag with its measured reason inside the function. A device is
  for a difference of instruction order or register choice only, or an
  unused local of a real type whose size exactly explains the native stack
  frame, and only after reshaping the source has failed; a function is
  never written as
  inline assembly. A fake match stays in the source, tagged; it is never
  replaced by a compiler option or routing that fits only those functions
  (K1, K3). `/* CAMELOT_ASM: proof */` marks assembly Camelot very likely
  wrote in C; the reviewed `Dma_Set` and `Iwram_*` macros stay. Never patch
  compiler output. _Check: no-asm (in lint-staged)._
- **S3** Write it the way Camelot would have in 2001: `games/<GAME>/SRC`,
  `INCLUDE`, `SOUND`, `TEXT`, with the same folder layout in both games and
  `games/COMMON`; uppercase 8.3-style names, C89, `Subsystem_VerbObject`
  functions, short locals, named struct fields and enums, Japanese character
  names and romaji plus one area word for places. Shared code and declarations
  live once in `games/COMMON`. Adopt a function into the module that holds its
  linker neighbours, not into a new file of its own.
- **S4** Never throw work away: commit each adoption and attempt, and keep a
  near miss as a draft with the remaining difference in its header. One draft
  per listing, overwritten as it improves: a trial is a commit or a line in
  that header, never another file or folder. Workers may edit and delete
  `recon/` listing and linker lines when adopting a function. An edition whose
  bytes differ stays on its scaffold until its branch is written; it never
  blocks the adoption in the others.

### Assets

- **A1** Assets are editable files built by our tools in every build, as pret's
  are: indexed PNGs with the game's real palettes, TSV tables and tilemaps,
  WAV, MIDI, PO and text definitions. Never raw dumps, grey sheets without a
  built palette, whole-area blobs or stored compression tokens. Sources may
  include the files the build makes from these inputs. _Check: publication._
- **A2** A picture is drawn the way the game shows it: tile sheets at their real
  width or sprite shape, pictures with a tilemap as the map shows them,
  animations one frame wide. _Check: layout._
- **A3** A PNG may carry an all-grey palette only when the build writes that
  palette into the ROM from it or from the first part of its part list.
  _Check: publication._
- **A4** Encoder options live beside their assets; recipes may carry a sprite
  shape (one of the 12 hardware sizes) in the file name.
- **A5** No JSON anywhere: tables are TSV. _Check: publication._

### Compilers

Camelot built each game with one compiler and one set of options, as one
Makefile would. Alchemy rebuilds that compiler: GCC 2.96 as Camelot's
engineers used it and, where the whole game shows it, as they modified it.
The line between that and bending a compiler to fit is K3: a compiler, an
option or a change counts only when it explains a game's code as a whole,
never a file or a function.

- **K1** Game code compiles with agscc (GCC 2.96, the 2000-07-31 snapshot) and
  one option set per game, the same for every game C file:
  `-O2 -mthumb -mcpu=arm7tdmi -nostdinc -fcall-used-r4`, plus
  `-mthumb-interwork` for ☀️ and K2's options for ⚓️. `-fcall-used-r4` is a
  stock GCC option: 265 of 266 ☀️ and 1,648 of 2,302 ⚓️ functions that save lr
  start saving at r5. Nintendo's library code is not Camelot's and keeps
  the compiler and flags its own bytes show, one setting per whole library
  file, as pret names its library files: pret's agbcc for MusicPlayer2000
  (`-O2`) and the flash library (`-O`, except two helper files whose bytes
  show `-O2`), and agbcc_arm for RAM-executed ARM code
  (`-fomit-frame-pointer`). No other routing: never a per-file or
  per-function compiler, option or flag for game code.
  _Check: routing tests._
- **K2** ⚓️'s compiler is agscc with two changes a Camelot engineer made to
  GCC's Thumb backend between the games, rebuilt as options in GCC's own
  style, and one stock scheduling option, all turned on for all of ⚓️'s game
  code:
  - `-mthumb-split-constants` builds a constant that no single Thumb
    immediate holds inline, from a shifted byte and an add, instead of loading
    it from the literal pool. 13,907 of the 14,101 constant-building sequences
    in ⚓️'s code follow the rule exactly; ☀️ has 18.
  - `-mthumb-call-via-lr` calls through a register by moving the target into
    lr and branching with the second half of a BL, instead of calling a
    `_call_via_rX` stub. ⚓️ makes 2,549 such calls and 3 stub calls, all in the
    separately compiled sound library; ☀️ makes 596 stub calls.
  - `-mtune=arm9tdmi` (stock GCC; Pascal, 2026-10-05) schedules loads for the
    ARM9TDMI pipeline. About 30 ⚓️ near misses that differed only by load
    order matched under it; 14 objects that matched before changed, 12 were
    repaired and `Menu_CenterResourceEntries` and
    `Menu_LayoutResourceEntries` went back to listings with their C kept as
    drafts. It is not yet the whole answer: some native sequences fit a load
    latency of 3 cycles and others leave a load's delay slot unfilled, and
    many remaining ⚓️ drafts differ by one swapped load. A load model that
    explains both is an open K3 study for Pascal.

  Why a person at Camelot made the two changes: each habit runs through the whole game,
  so it belongs to the compiler, not to some functions; neither is in ☀️,
  built a year earlier with the same compiler family; no public GCC from
  2000-2002 has either (the study in agscc's README read all 417 ARM backend
  changes and 28 snapshots and releases); and each is a small, local change a
  toolchain engineer makes for speed: the first skips a slow cartridge-ROM
  load for the constant, the second skips a stub on every indirect call. The
  second only works because ⚓️'s game code no longer interworks with ARM code,
  one coherent change to one compiler. Without them, 37 credited ⚓️ functions
  no longer match. _Check: compiler-source-check, bundle validation._
- **K3** A compiler, option or change is admitted only when one option set
  explains the great majority of a game's C and assembly, counted across the
  whole game rather than listed by function; no public compiler of the time
  already explains it; and it is written as a plausible change by a Camelot
  engineer, in GCC's own style, with its evidence and reason recorded here and
  in agscc's README. Pascal approves each. Every compiler binary is built
  reproducibly from pinned source and its digest recorded.
  _Check: compiler-source-check, bundle validation, routing tests._

### Work

- **W1** Scripts are TypeScript on Bun or Rust; no Python, no shell beyond a
  command line. New tooling solves a demonstrated recurring blocker and
  carries a test; tooling nothing needs is removed. Pret builds its games with
  about 15,000 lines of tools. _Check: language-check._
- **W2** Work on your own branch; `main` is the only long-lived branch, and a
  worktree is removed when its branch lands. Land on main with `make land`,
  which builds and compares all twelve editions and runs the tests.
  _Check: land._
- **W3** Alchemy owns compilation, linking, verification, reading and analysis,
  behind one `alchemy` command for both games; the ags crate owns asset
  encoding and decoding (agsgfx, mid2ags, wav2ags, po2ags). Look at how pret
  does something before inventing.
- **W4** Choose what to match from `make deps`: first the functions other
  unfinished functions call, with the structures they share, then the ones
  with nothing left to name, largest first.
- **W5** The whole ROM judges. Before every commit, `make compare-editions`
  must print OK for all twelve editions; a per-object or per-function diff
  misses a scaffold section placed two bytes early or a lost alignment pad.
  `alchemy objects` names both; `alchemy keep` checks all twelve.
- **W6** A change to what counts, a rule, a compiler option or routing lands
  as its own commit, with DONE before and after in its message, never mixed
  with new C. Most of DONE's large jumps and drops have been changes of this
  kind, not matching.
- **W7** Parallel work divides by owner: `alchemy queues` gives each worker
  whole modules or overlays, and each camelotize worker one area. Before
  naming a field or function, check no other branch named it
  (`git log --all -S`). When branches merge, an adoption wins over edits to
  its draft, and code that used an old name takes the new one. A worker
  commits only verified work; anything unverified at a stop is discarded.
- **W8** A mechanical rewrite is `alchemy camelotize` followed by
  `alchemy keep`; a rewrite by hand meets the same test. Message names stay
  linker symbols: as an enum, small ids become immediate moves instead of
  literal-pool loads and the bytes change.

### Notes from matching

Not rules; what earlier work measured.

- Gaps between locals and spill slots come from register allocation, not
  from locals; an unused local exactly the size of a native frame gap is the
  only frame device S2 allows (`alchemy sizes` lists the candidates).
- An empty asm barrier between two tests blocks jump threading; `const`
  declarations change scheduling; a store through a struct field and one
  through a cast pointer build a halfword constant differently.
- CSE can reuse a register that already holds 0 or 1, so a native store of a
  register is often a constant in the source.
- A two-byte padding piece next to a function becomes the compiler's
  alignment once the function is C; the listing or scaffold section that
  follows then starts with `.balign 4`.
- Editions differ: Japanese is often an older layout of the same function
  and gets its own `#if` branch, or stays on its scaffold (S4). The
  German, Spanish, French and Italian far-call tables sometimes name a stub
  differently; `alchemy label` names a scaffold-only stub.
- Nested functions adopt with their parent.

## Commands

Setup needs Apple Silicon macOS with Xcode's command-line tools, Rust with
Cargo, and Bun. Put your own ROMs under `roms/` as `tbs-en.gba`,
`tla-ja.gba` and so on; each is checked before use and none is ever
committed (L1).

```sh
git submodule update --init && git config core.hooksPath .hooks
make bootstrap         # pinned compilers; BUNDLE=/path imports an approved bundle
make worktree          # in a new worktree, before its first build
make compare           # the Japanese base of ☀️; compare-tla for ⚓️
make compare-tbs-en    # one edition: tbs or tla, then ja, en, de, es, fr or it
make compare-editions  # all twelve editions (W5)
make build-rom TARGET=tbs-en  # build a changed game without comparing
make test
make verify
make land              # on main, before committing a landing
```

The `alchemy` binary is `tools/out/cargo-target/release/alchemy`; it acts on
the checkout it runs in, and `alchemy --help` lists every command.

```sh
# Choose what to match
make deps                              # what unblocks the most (W4)
alchemy similar                        # nearest C twins in either game
# Match one function
alchemy diff DRAFT.c [ADDR] [--show]   # a draft against its listing; differ 0 matches
alchemy sizes DRAFT.c [SIZE]           # struct sizes; the locals that fill a frame gap
alchemy show EDITION FUNCTION          # a built function beside the ROM
alchemy adopt ADDR LABEL NAME [--into MODULE.C]   # into its module, out of all scaffolding
alchemy label NAME ENGLISH_ADDR LEN    # name English-only scaffold data in other editions
# Check the whole build
alchemy objects EDITION...             # what differs from the ROM, object or placement
make progress                          # DONE, one line per edition (alchemy progress)
# Camelotize
make worst                             # maintained C ranked by how mechanical it reads
alchemy camelotize REWRITE FILE...     # actors, events, registers, declarations, barriers
alchemy keep [PATH...]                 # keep only edits that leave all twelve identical
# Work in parallel
alchemy queues --workers N --out-dir DIR   # work that cannot collide (W7)
alchemy integrate BRANCH...            # merge, reconcile linker scripts, build all twelve
# Report for people
make figure                            # PROGRESS.png and PROGRESS_CHART.png after a build
```
