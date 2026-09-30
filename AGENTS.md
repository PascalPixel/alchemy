# Alchemy

Golden Sun: **The Broken Seal (TBS)** ☀️ and **The Lost Age (TLA)** ⚓️,
rebuilt byte for byte from readable C, assembly and editable assets. Japanese
releases are the source editions; localizations are measured differences.
Build IDs are `tbs` and `tla`. Today only the English editions build their
code from C.

This file is the only place Alchemy's rules live. `README.md` is for fans.
Every rule has an ID; a rule a tool can enforce names its check, and every
check in `make verify` names the rule it enforces. Pascal decides rule
wording, compilers, what counts as DONE, and deleting anything that cannot be
restored; git history records when. Everything else goes ahead without
asking him, including fixing a check that wrongly blocks honest work, as long
as it still refuses everything it refused before.

## Goal

**DONE = matching C + proven library, handwritten or veneer assembly**, over
each game's executable bytes. `make progress` prints the exact counts, shows
⚓️ in its parts (C, assembly and 8-byte stubs) and shows the bytes of
FAKEMATCH-steered code as their own number. ☀️ has the priority; the target is
☀️ 100% and ⚓️ 100%, sharing as much code between the games as possible.

## Rules

### Legal

- **L1** Never commit, upload or publish a ROM, the cartridge logo, private
  inputs or any other copy of the original game files. _Check: publication._
- **L2** Evidence comes only from our own ROMs, this repository and public
  documentation. Never use another project's Golden Sun work.
  _Check: publication._
- **L3** Never use leaked Nintendo or Camelot code or SDKs, and never open, read
  or mention sources that might contain them.

### Counting

- **C1** A function counts only when its complete extent, literal pool
  included, compiles exactly and every edition stays byte-identical. Until the
  other ten editions build their code from C, their byte-identical builds
  prove pictures, sound and text only.
  _Check: compare, compare-tla, compare-other-editions._
- **C2** DONE is pret's calcrom over the linker maps of byte-identical builds:
  the code the linker places from `games/`. Uncredited disassembly is
  `not-yet-c` in `recon/<game>/raw`; only proven library, handwritten and veneer
  assembly counts as assembly. Whole aligned 8-byte far-call stubs count as
  veneers. Padding a source marks as carrying no credit does not count.
  _Check: coverage-check._
- **C3** Main commits carry the verified percentage, README and both progress
  figures, written by `make land`. _Check: commit-msg hook, coverage-check._

### The oracle stays out of the build

The reference ROM judges the build and never feeds it. Any path by which the
expected answer steers the build or the count is cheating, whatever its form.

- **O1** ROM bytes reach the build only through pret's
  `.incbin "baserom.gba", OFFSET, SIZE` scaffold in `recon/<game>`, which never
  counts. Nothing else derived from the ROM or the expected output (bytes,
  addresses, sizes, tokens, tables, hashes) may steer the build or the count,
  `out/` included; reports under `out/` are for people only.
  _Check: publication._
- **O2** Every name is defined where its bytes are, as a C definition or a
  label, and layout is the linker scripts' object order. No equate, alias,
  `#define` or table gives a name an address, except Camelot's own fixed
  addresses: resident IWRAM routines called through fixed entries, and RAM
  buffers its code addressed as constants, each listed once in a game header
  and checked against where the linker placed it. One name per place; never a
  name that spells its own value. _Check: publication, fixed-address check._
- **O3** Scene and resource ids come only from their tables and are used whole.
  Overlay code never branches straight into the main image.
  _Check: publication._
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
  `FAKEMATCH` tag with a reason. `/* CAMELOT_ASM: proof */` marks assembly
  Camelot very likely wrote in C; the reviewed `Dma_Set` and `Iwram_*` macros
  stay. Never patch compiler output. _Check: no-asm (in lint-staged)._
- **S3** Write it the way Camelot would have in 2001: `games/<GAME>/SRC`,
  `INCLUDE`, `SOUND`, `TEXT`, with the same folder layout in both games and
  `games/COMMON`; uppercase 8.3-style names, C89, `Subsystem_VerbObject`
  functions, short locals, named struct fields and enums, Japanese character
  names and romaji plus one area word for places. Shared code and declarations
  live once in `games/COMMON`.
- **S4** Never throw work away: commit each adoption and attempt, and keep near
  misses as drafts with the remaining difference in their header. Workers may
  edit and delete `recon/` listing and linker lines when adopting a function.

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

- **K1** Game code uses the approved agscc (GCC 2.96); library code uses
  pret's agbcc with pret's flags (`-O` for flash, `-O2` for MusicPlayer2000);
  RAM-executed ARM code uses pret's agbcc_arm with `-fomit-frame-pointer`.
  Families and flags apply to whole files, with the reason in compiler routing;
  never tune a single function. _Check: routing tests._
- **K2** ⚓️ game code adds `-mthumb-split-constants` and `-mthumb-call-via-lr`,
  Camelot's constant and call-through-register rules found across ⚓️. A compiler change needs Pascal's approval and evidence
  across many functions; every compiler binary's digest is recorded.
  _Check: compiler-source-check, bundle validation._

### Work

- **W1** Scripts are TypeScript on Bun or Rust; no Python, no shell beyond a
  command line. New tooling solves a demonstrated recurring blocker and
  carries a test. _Check: language-check._
- **W2** Work on your own branch; `main` is the only long-lived branch. Land on
  main with `make land`, which builds and compares all twelve editions and runs
  the tests. _Check: land._
- **W3** Alchemy owns compilation, linking and verification; the ags crate owns
  encoding (agsgfx, mid2ags, wav2ags, po2ags); Psynergy owns reading, decoding,
  analysis and comparison. Look at how pret does something before inventing.

## Commands

```sh
git submodule update --init && git config core.hooksPath .hooks
make bootstrap
make worktree      # in a new worktree, before its first build
make compare-all   # make compare-editions for all twelve
make test
make deps          # dependency map for choosing targets
make drafts        # compile and score every draft against its listing
make verify
make land          # on main, before committing a landing
```
