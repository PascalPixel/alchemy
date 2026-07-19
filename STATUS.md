# Technical status

Alchemy is an in-progress, clean-room decompilation of Golden Sun and Golden
Sun: The Lost Age for Game Boy Advance. Its eventual target is one source tree
for English, Japanese, German, Spanish, French, and Italian builds of both
games. The tracked repository contains reconstructed source and independently
authored tooling; the twelve approved private ROMs are evidence and local
verification inputs, never published build inputs.

## Current Golden Sun English snapshot

The source-only ownership build currently accounts for:

| Source form | Regions | Bytes |
|---|---:|---:|
| Compiled C | 769 | 30,252 |
| Reconstruction assembly | 1,722 | 508,524 |
| Canonical assets | 2,276 | 7,813,515 |
| **Source-owned total** | **4,767** | **8,352,291** |
| **Unowned remainder** | **596 gaps** | **36,317** |
| **Canonical target size** |  | **8,388,608** |

The immediate milestone is **100% byte closure for `gs1-en.gba`**: all
8,388,608 bytes must come from tracked C, reconstruction assembly, or canonical
assets, with no copied gap or private-ROM fallback. That milestone is currently
8,352,291 bytes, or 99.57%. Exact reconstruction assembly counts toward byte
closure even when it remains C-decompilation work.

Within the reconstruction assembly, approximately 491,696 bytes remain active
C-decompilation, boundary-splitting, or function-merging work. The other 16,828
bytes are presently retained structural assembly: linker veneers, runtime
thunks, fixed hardware entry points, shared-literal modules, and proven
compiler-unproducible kernels.

The stricter build-report `byte_reconstruction` milestone is 7,860,595 of
8,388,608 bytes, or 93.71%. It counts exact C, deterministic asset round trips,
and positively retained structural assembly, while treating ordinary
reconstruction assembly as unfinished C work. The remaining 528,013 bytes are
the 491,696-byte assembly debt plus the 36,317 unowned bytes. It is useful for
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
| GS1 English byte closure | 8,352,291 / 8,388,608 bytes (99.57%) | A tracked producer owns the address; immediate target is no fallback |
| Byte reconstruction | 7,860,595 / 8,388,608 bytes (93.71%) | Exact C, round-tripping assets, or retained structural assembly |
| Code decompilation | 30,252 / 521,948 identified executable bytes (5.80%) | Active executable work represented as exact C |
| Asset semantic maturity | Audit pending | Round-trip success has not yet been classified as coherent or provisional |
| Repository organization | Audit pending | Flatness, naming, source formats, and generated-file boundaries need a formal audit |
| Build-target coverage | 1 active / 12 approved | GS1 English is canonical now; the other editions remain comparison evidence and future targets |
| **Project completion** | **Audit pending** | No arbitrary weighted score is published |

`build_full.ts` records these gates in `project_completion`. Until a tracked
asset-maturity register exists, its formal classified count is zero and all
2,276 round-tripping asset regions remain unclassified for project-completion
purposes. That is not a claim that every asset is poor; it prevents unreviewed
containers from silently receiving clean-source credit. The report also keeps
the former `total_decompilation_*` fields as explicitly deprecated aliases for
the byte-reconstruction milestone so existing local consumers do not break.

## The unowned 36,317 bytes

The remainder is 596 separate address intervals, not one large unknown blob.
The largest individual gap is 4,036 bytes.

| Category | Bytes |
|---|---:|
| Still-unresolved executable or mixed regions | 10,552 |
| Early table and layout blocks | 8,836 |
| Eight missing resource-directory bodies | 8,528 |
| Three late `0x080f` data tails | 7,294 |
| Identified residual data | 344 |
| Compiler-emitted, manifest-unclaimed alignment | 364 |
| Resource alignment and explicit exclusions | 207 |
| GBA header | 192 |
| **Total** | **36,317** |

The eight fully unowned directory resources are `049`, `04a`, `061`, `064`,
`066`, `073`, `0a5`, and `0a6`. Their pointer-derived boundaries total 8,528
bytes. The three early blocks occupy `08012f20..08013784`,
`0801403c..08015000`, and `08015430..08015e8c`. The late tails occupy
`080f38bc..080f4000`, `080f53ce..080f6000`, and
`080f86f8..080f9000`.

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
- `alchemy-gcc/` is an ignored private compiler bundle. Its required contents
  are specified in [ALCHEMY_GCC.md](ALCHEMY_GCC.md).

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
