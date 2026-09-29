# Alchemy

Golden Sun: **The Broken Seal (TBS)** ☀️ and **The Lost Age (TLA)** ⚓️,
rebuilt byte for byte from readable C, assembly and editable assets. Japanese
releases are the source editions; localizations are measured differences.
Build IDs are `tbs` and `tla`.

This is the working guide; `README.md` is for fans. Keep it short. Implementation
details belong in source and build rules. No separate notes, plans or reports.

## Goal

**DONE = matching C + proven library, handwritten or veneer assembly**, over
each game's executable bytes. `make progress` prints the exact counts.
Optimise **☀️ percentage points landed on main per hour**, measured in complete
hourly bins over 12 hours. Use exact DONE and executable bytes; show credit
corrections separately. Timebox work that cannot move this number.

## Rules

1. Count a function only when its complete extent, including its literal pool,
   compiles exactly and the ROM remains byte-identical.
2. Tagged `/* FAKEMATCH: reason */` C counts. Never patch compiler output or use
   inline assembly in game C except the reviewed `Dma_Set` and `Iwram_*` macros.
3. Uncredited disassembly is `not-yet-c`. Only proven library, handwritten and
   veneer modules count as assembly; difficulty never changes that classification.
4. Commit each adoption and attempt; keep near misses as drafts with the
   remaining difference in their header. Never throw work away.
5. Commit code, tooling, our documentation and editable assets, as pret does:
   indexed PNGs with the game's palettes, tilemaps, sound, text and game
   definitions, built into the ROM by our tools. Never commit ROMs, the
   cartridge logo, raw dumps (grey sheets, whole-area blobs, compression
   tokens), another project's Golden Sun work, SDK or leaked code. Evidence
   comes from our ROMs, this repository and public documentation.
6. Only Pascal changes credit standards, approves compiler source, binaries
   and digests, or authorises pushes and parallel workflows. Record approvals below.

## Oracle leakage

The reference ROM is the test oracle: it judges the build and never feeds it.
Oracle leakage (Goodhart's law, specification gaming) is any path by which the
expected answer steers the build or the count, so the comparison passes by
construction. Every past cheat was one leak in a new form: a cloned ROM,
catalogs, receipts, name-decoded equates, `#define` aliases, addresses stored
in assets. Banning forms only moves the answer; hold these invariants instead.

1. **One door.** ROM bytes reach the build only through pret's
   `.incbin "baserom.gba", OFFSET, SIZE` scaffold in `recon/<game>`, which never
   counts. Nothing else derived from the ROM or the expected output (bytes,
   addresses, sizes, tokens, tables, per-asset hashes) may steer the build or
   the count, in any file or location, `out/` included.
2. **The linker places.** Every name is defined where its bytes are, as a C
   definition or a label, and layout is the linker scripts' object order. No
   equate, alias, `#define` or table gives a name an address, except
   Camelot's own: ROM code calls the resident IWRAM routines through fixed
   entry addresses or offsets from the bank's first routine, and reaches
   fixed RAM buffers its code addressed as constants, each listed once in a
   game header beside its linked name and checked by the build against
   where the linker placed it.
3. **The map counts.** DONE is pret's calcrom over the linker maps of
   byte-identical builds: the code the linker places from `games/`. `games/`
   holds only real source: C from an approved compiler, or proven library,
   handwritten or veneer assembly written as instructions, never as copied
   bytes. Unfinished code is disassembly in `recon/<game>/raw`, `not-yet-c`.
4. **Verification is fixed.** A mismatch is fixed in source, or the code stays
   disassembly or a draft. Only Pascal changes gates, credit or the comparison.
5. **Audit before claiming.** No list is complete: before announcing a
   milestone, run an independent adversarial audit for leaks and fix each at
   its owner.

Reports may be generated under `out/` for people to read, never for the build
or the count to consume. Keep each decision once: names in code, declarations
in headers, layout and bindings in linker rules, encoder options beside assets.
Approved checksums, dependency pins and published progress are intentional
records.

## Source

Use `~/Developer/pret/pokeemerald` for method only, never code or symbols.
Our presentation follows a plausible Camelot project from 2001:
`games/<GAME>/SRC`, `INCLUDE`, `SOUND`, `TEXT`; uppercase 8.3-style names,
C89, `Subsystem_VerbObject` functions and short locals. Use Japanese character
names and romaji plus one area word for places. Share declarations in headers
and proven common code in `games/COMMON`; instanced code spells no address names.
Scaffolding stays under `recon/tbs` and `recon/tla` and shrinks toward zero.

Assets are individual indexed PNGs with real palettes, identified tilemap/table
BINs, WAV, MIDI, PO and editable game definitions, converted by our tools in
the build as pret's gbagfx, mid2agb and preproc convert theirs. Compression
comes from these inputs and per-file encoder options. Asset tooling and
identified assets are source: never delete them as dumps. Bytes not yet
identified stay in the baserom scaffold.

## Work and build

Work on the assigned branch; `main` is the only long-lived branch. Give agents
disjoint slices. Inspect complete functions and their neighbours, reuse proven
source, and change one hypothesis at a time. After 30 minutes or three attempts
without a new idea, commit the draft and move on.

Alchemy owns compilation, linking and verification; the ags crate owns
encoding, with the pret-style tools agsgfx, mid2ags, wav2ags and po2ags;
Psynergy owns portable reading, decoding, analysis and comparison. Prefer
existing commands and read `--help`. Scripts are TypeScript on Bun or Rust. New tooling must solve
a demonstrated recurring blocker and carry a test.

Use approved agscc (GCC 2.96) for both games with stock options. Compiler family and flags
apply to whole files with their reason recorded in compiler routing; never tune
individual functions. Library routes remain agbcc `-O` for flash and `-O2` for
MusicPlayer2000; RAM-executed ARM code uses `-marm -mno-apcs-frame`.

```sh
git submodule update --init && git config core.hooksPath .hooks
make bootstrap
make compare-all
make test
make coverage
make verify
make land          # on main, before committing a landing
```

Commit hooks only check: every commit runs the staged checks, and on main the
commit-msg hook writes the verified percentage or pending prefix. Land on main
with `make land` before committing: it builds and compares both ROMs, runs the
tests and writes and stages README and both progress figures. Pre-push checks
outgoing history and main's publication. DONE is `?` until the current tree has
a verified build.

## Pascal's decisions

- 2026-09-23: tagged fake matches count; ARM uses `-marm -mno-apcs-frame`.
  Private inputs are never published.
- 2026-09-24: whole aligned 8-byte main-image far-call stubs count as
  reconstructed veneers, like overlay entry veneers.
- 2026-09-27: attribution after noon September 26 (Lisbon) is Sol 6 or Astra 6.
  New trailers name the actual session model.
- 2026-09-28: tooling baseline is `2db71499f1f991c9a4899641737212bfba8104c0`.
  Main commits own verified percentages, README and both progress figures.
- 2026-09-28: the oracle-leakage rule supersedes the September 22/23 exceptions for stored
  compression answers and tracked machine ledgers.
- 2026-09-28: approved the stock `da598c1` agbcc rebuild; its exact binary
  digest is recorded in the compiler admission table.
- 2026-09-28: the dashboard is removed.
- 2026-09-28: prime directive: what pret published, we may; what pret did not
  publish, we may not. Not-yet-sourced data links through pret's early
  `.incbin "baserom.gba", OFFSET, SIZE` scaffolding in `recon/<game>`.
- 2026-09-28: the cheating rule is restated as oracle leakage: five invariants
  that keep the answer out of the build and the count, not a list of forms.
- 2026-09-28: compilers take stock options only; a game-specific compiler flag
  is an invented answer. agscc is GCC 2.96 with its host ports, and agbcc is
  called with pret's flags.
- 2026-09-28: pret publishes graphics, sound, text and maps as editable files
  built by its tools; so does Alchemy.
- 2026-09-29: do what Camelot did for IWRAM calls. The ROM loads a resident
  IWRAM routine's address before the arguments, which only a call through a
  fixed address reproduces; a label compiles to a direct `bl` and `long_call`
  loads the address last.
- 2026-09-29: compiler-steering idioms are fakes and carry a FAKEMATCH tag:
  inline call or value wrappers, volatile on plain RAM, forced temporaries,
  dead code and jumps into blocks, unless rewritten as plain C.
- 2026-09-29: fixed RAM buffers that Camelot's code addressed as constants
  are checked entries, as the IWRAM routines are.
- 2026-09-29: gates: overlay code never branches straight into the main
  image; scene and resource ids come only from their tables and are used
  whole; one name per place; sound sources may include the files the build
  makes from MIDI and WAV.
