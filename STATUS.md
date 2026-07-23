# Technical status

Alchemy is an in-progress, clean-room decompilation of Golden Sun and Golden
Sun: The Lost Age for Game Boy Advance. Its eventual target is one source tree
for English, Japanese, German, Spanish, French, and Italian builds of both
games. The tracked repository contains reconstructed source and independently
authored tooling; the twelve approved private ROMs are evidence and local
verification inputs, never published build inputs.

## Current Golden Sun English snapshot

Figures in this snapshot and the project-completion table were regenerated on
2026-07-23 via `bun tools/build_full.ts --source-only`.

The source-only ownership build currently accounts for:

| Source form | Regions | Bytes |
|---|---:|---:|
| Compiled C | 1,039 | 54,066 |
| Reconstruction assembly | 2,132 | 494,370 |
| Canonical assets | 2,428 | 7,840,172 |
| **Source-owned total** | **5,599** | **8,388,608** |
| **Unowned remainder** | **0 gaps** | **0** |
| **Canonical target size** |  | **8,388,608** |

The immediate milestone is **100% byte closure for `gs1-en.gba`**: all
8,388,608 bytes must come from tracked C, reconstruction assembly, or canonical
assets, with no copied gap or private-ROM fallback. That milestone is achieved:
8,388,608 bytes, or 100.00%. Exact reconstruction assembly counts toward byte
closure even when it remains C-decompilation work.

Within the reconstruction assembly, 471,022 bytes remain active
C-decompilation, boundary-splitting, or function-merging work. The other 23,348
bytes are presently retained structural assembly: linker veneers, runtime
thunks, fixed hardware entry points, shared-literal modules, and proven
compiler-unproducible kernels.

The stricter build-report `byte_reconstruction` milestone is 7,917,586 of
8,388,608 bytes, or 94.38%. It counts exact C, deterministic asset round trips,
and positively retained structural assembly, while treating ordinary
reconstruction assembly as unfinished C work. The remaining 471,022 bytes are
the assembly debt. It is useful for
tracking source quality, but it is not the immediate no-fallback byte-closure
milestone and it is not a forecast of time remaining.

That milestone is deliberately not called project completion. Source ownership
only asks whether an address has a canonical tracked producer, while byte
reconstruction only asks whether a source form reproduces the right bytes. A
round-tripping image can still have arbitrary columns, split objects, or
packing bands; comparable provisional layouts can exist in maps, audio, and
archives. Those sources remain useful build inputs, but do not receive semantic
completion credit until the whole producing pipeline is audited.

## Project-completion gates

Project completion means reaching a pret-like source tree for all twelve
approved builds: byte-identical targets, readable shared or edition-specific C
and justified structural assembly, coherent PNG/MID/WAV and semantic JSON
sources, deterministic TypeScript encoders, and a flat, intentional file
layout. The aggregate percentage is currently **withheld** rather than inferred
from unrelated byte totals. Unless stated otherwise, the measured dimensions
below describe only the current canonical Golden Sun English target.

| Dimension | Current result | Meaning |
|---|---:|---|
| GS1 English byte closure | 8,388,608 / 8,388,608 bytes (100.00%) | Every address has a tracked producer; canonical build uses no fallback |
| Byte reconstruction | 7,917,586 / 8,388,608 bytes (94.38%) | Exact C, round-tripping assets, or retained structural assembly |
| Code decompilation | 54,066 / 525,088 identified executable bytes (10.30%) | Active executable work represented as exact C |
| Asset semantic maturity | Audit pending | Round-trip success has not yet been classified as coherent or provisional |
| Repository organization | Audit pending | Flatness, naming, source formats, and generated-file boundaries need a formal audit |
| Build-target coverage | 1 active / 12 approved | GS1 English is canonical now; the other editions remain comparison evidence and future targets |
| **Project completion** | **Audit pending** | No arbitrary weighted score is published |

`build_full.ts` records these gates in `project_completion`. Until a tracked
asset-maturity register exists, its formal classified count is zero and all
2,428 round-tripping asset regions remain unclassified for project-completion
purposes. That is not a claim that every asset is poor; it prevents unreviewed
containers from silently receiving clean-source credit. The report also keeps
the former `total_decompilation_*` fields as explicitly deprecated aliases for
the byte-reconstruction milestone so existing local consumers do not break.

## Byte-closure result

The ownership audit reports zero uncovered address intervals. The canonical
full build produces all 8,388,608 bytes from tracked C, reconstruction
assembly, and canonical assets, with `rom_fallback_bytes=0` and `identical=True`
against the approved English verification input.

`bun tools/audit_residuals.ts` reproduces the ownership mask from the generated
C, assembly, and asset manifests, checks it against the full-build report, and
names completely uncovered resource IDs automatically.

## Evidence and target boundary

The approved evidence boundary consists only of
`gs1-{en,ja,de,es,fr,it}.gba` and `gs2-{en,ja,de,es,fr,it}.gba`.
`gs1-en.gba` is the immediate canonical byte-identical build target. The other
eleven ROMs can provide local differential evidence for distinguishing shared
engine code and data from localization, graphics, and story content, and each
is an eventual independent build target of this same repository. They never
become tracked files or fallback build inputs.

All game knowledge is derived anew with generic tools. Cross-ROM equality can
support a layout or shared-engine conclusion, but cannot establish original
authorship or an original filename. Public decompilation projects are used only
as generic repository and publication precedent.

## Source tree

- `src/` contains only byte-verified, asm-free C. A replacement enters this
  directory only when the approved compiler reproduces the tracked bytes at the
  exact fixed address.
- `asm/` contains byte-verified reconstruction assembly. Ordinary compiler
  output remains C debt; fixed dispatch stubs, thunks, BIOS wrappers, linker
  veneers, and positively identified hand-scheduled modules remain assembly.
- `assets/` contains canonical source graphics, palettes, maps, text, audio,
  tables, and deterministic archive plans with exact ROM ranges and encoders.
- `assets/code/` contains compressed EWRAM overlays reconstructed as structured
  Thumb assembly plus exact compression plans.
- `tools/` contains the independent Bun-based analysis, extraction, encoding,
  matching, linking, and verification pipeline.
- The approved compiler bundle lives in the sibling `alchemy-gcc` repository
  checkout; its required contents are specified in [ALCHEMY_GCC.md](ALCHEMY_GCC.md).

## Recovered systems and assets

- The battle-screen package at `080aea4c` is reconstructed from bounded 4bpp
  sources, display-glyph cells, masks, curves, ordering tables, selectors, and
  object IDs. Its fixed long-call veneers remain structural assembly.
- The staff-roll package at `080f0a5c` has an explicit preload table, canonical
  text pool, glyph widths, line references, 1bpp font, and derived alignment.
- Compressed code overlays are control-flow reconstructed, assembled, compressed
  with their tracked plans, and checked at their original boundaries.
- The 186-record map-load table links shared palettes, three VRAM banks, map
  cells, and animation sources without duplicating source images.
- Map objects preserve tile slots, palette banks, and horizontal and vertical
  flip flags, allowing coherent object PNGs to remain exact build sources.
- Fonts, regional text tables, message archives, battle menus, character
  animation banks, music sequences, PCM samples, and sound-engine tables have
  dedicated semantic encoders.
- Reconstructed names and source comments follow the period-authentic Japanese
  rules in [NAMING.md](NAMING.md).

## Verification workflow

The project uses Bun and native ARM binutils. On Apple Silicon, a native arm64
`arm-none-eabi-binutils` must precede an x86_64 installation in `PATH`.

```sh
# Generic encoder and pipeline tests
bun run test

# Compile and link every C claim without reading a ROM
bun tools/build_claimed.ts --source-only

# Assemble and classify every reconstruction-assembly claim
bun tools/build_asm.ts --source-only

# Encode every canonical asset and validate its structure
bun tools/build_assets.ts --source-only

# Rebuild the combined source-ownership map without producing a GBA image
bun tools/build_full.ts --source-only
```

`build_claimed.ts` checks compilation, external symbols, placement, extent, and
overlap. `integrate_matches.ts` accepts C only when its fixed-address linked
bytes exactly equal the existing verified assembly. `build_asm.ts` enforces the
machine-readable assembly classification boundary. `build_assets.ts` invokes
every semantic encoder. `build_full.ts` combines the three ownership manifests,
rejects overlap, and reports unowned ranges.

For the current private transitional build, `build_rom.ts` generates a GNU
linker script that places every reconstructed region in address order and fills
only the remaining unowned skeleton from the user's private English ROM. The
immediate byte-closure milestone retires that fallback entirely. Generated
linker files, objects, ELFs, ROM images, reports, and work directories stay
ignored.

## Publication boundary

The repository follows the practical source-tree boundary used by projects such
as `pret/pokeemerald`: reconstructed C and assembly, semantic metadata, source
PNGs, palettes, tilemaps, metatiles, audio sources, and deterministic encoders
are publishable canonical sources. ROM images, copied gaps, binary patches,
private differential reports, opaque extraction dumps, compiler bundles,
objects, ELFs, and generated builds are not.

The full operational rule and commit gate are in
[PUBLICATION.md](PUBLICATION.md). All private ROMs, `alchemy-gcc`, generated
analysis, compiler output, and build products are git-ignored.

## Definition of completion

The immediate byte-closure milestone is reached when the tracked sources and
generic tooling independently produce a byte-identical `gs1-en.gba` with no
copied source gaps or fallback bytes. Project completion is reached only when
that is true for all twelve approved targets from this one source tree;
ordinary executable assembly has become exact C; the remaining assembly has a
positive structural reason; every canonical asset is semantically coherent
rather than merely round-trippable; and the repository has a flat,
intentional, source-only organization. The aggregate stays unscored until
every one of those conditions has an auditable measurement.
