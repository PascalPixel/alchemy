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
5. Commit code, tooling, our documentation and identified editable inputs.
   Never commit ROMs, the cartridge logo, asset dumps, another project's Golden
   Sun work, SDK or leaked code. Evidence comes from our ROMs, this repository
   and public documentation.
6. Only Pascal changes credit standards, approves compiler source, binaries
   and digests, or authorises pushes and parallel workflows. Record approvals below.

## AI Cheating

AI Cheating is saving the expected answer or changing verification instead of
reconstructing the mechanism, or committing calculated bookkeeping as source.

- Maintained disassembly is a valid fallback for unfinished C. It remains
  `not-yet-c`; only proven library, handwritten and veneer modules earn assembly credit.
- Never commit grey sheets, whole-area BINs, `DATA.BIN` or compression tokens.
  Renaming or splitting stored answers changes nothing.
- Generate calculated offsets, sizes, per-asset hashes, pointer/symbol catalogs,
  owner inventories, reports and receipts in ignored `out/` or `tools/out`.
  Fix consumers that depend on tracked copies.
- Keep decisions once: names in code, declarations in headers, layout and
  bindings in build/linker rules, encoder options beside assets. Identified
  editable assets are inputs; approved reference checksums, dependency pins
  and published progress are intentional records.

## Source

Use `~/Developer/pret/pokeemerald` for method only, never code or symbols.
Our presentation follows a plausible Camelot project from 2001:
`games/<GAME>/SRC`, `INCLUDE`, `SOUND`, `TEXT`; uppercase 8.3-style names,
C89, `Subsystem_VerbObject` functions and short locals. Use Japanese character
names and romaji plus one area word for places. Share declarations in headers
and proven common code in `games/COMMON`; instanced code spells no address names.
Scaffolding stays under `recon/tbs` and `recon/tla` and shrinks toward zero.

Assets are individual indexed PNGs with real palettes, identified tilemap/table
BINs, WAV, MIDI, PO and editable game definitions. Compression comes from these
inputs and per-file encoder options; unresolved ROM-derived material stays local.

## Work and build

Work on the assigned branch; `main` is the only long-lived branch. Give agents
disjoint slices. Inspect complete functions and their neighbours, reuse proven
source, and change one hypothesis at a time. After 30 minutes or three attempts
without a new idea, commit the draft and move on.

Alchemy owns compilation, encoding, linking and verification; Psynergy owns
portable reading, decoding, analysis and comparison. Prefer existing commands
and read `--help`. Scripts are TypeScript on Bun or Rust. New tooling must solve
a demonstrated recurring blocker and carry a test.

Use approved agscc (GCC 2.96), with TLA's `-mgs2` route. Compiler family and flags
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
```

A main commit verifies both ROMs, derives its percentage or pending prefix, and updates
README and both progress figures. Branch commits run quick checks; pre-push
checks outgoing history. DONE is `?` until the current tree has a verified build.

## Pascal's decisions

- 2026-09-23: tagged fake matches count; ARM uses `-marm -mno-apcs-frame`.
  Private inputs appear only in the local dashboard.
- 2026-09-24: whole aligned 8-byte main-image far-call stubs count as
  reconstructed veneers, like overlay entry veneers.
- 2026-09-27: attribution after noon September 26 (Lisbon) is Sol 6 or Astra 6.
  New trailers name the actual session model.
- 2026-09-28: tooling baseline is `2db71499f1f991c9a4899641737212bfba8104c0`.
  Main commits own verified percentages, README and both progress figures.
- 2026-09-28: AI Cheating supersedes the September 22/23 exceptions for stored
  compression answers and tracked machine ledgers.
- 2026-09-28: approved the stock `da598c1` agbcc rebuild; its exact binary
  digest is recorded in the compiler admission table.
