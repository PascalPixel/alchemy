# Technical status

Alchemy is an in-progress, clean-room decompilation of the English Golden Sun
release for Game Boy Advance. The tracked repository contains reconstructed
source and independently authored tooling; private ROMs are evidence and
verification inputs, never published build inputs.

## Current snapshot

The source-only ownership build currently accounts for:

| Source form | Regions | Bytes |
|---|---:|---:|
| Compiled C | 761 | 29,654 |
| Reconstruction assembly | 1,730 | 509,122 |
| Canonical assets | 2,276 | 7,813,515 |
| **Source-owned total** | **4,767** | **8,352,291** |
| **Unowned remainder** | **596 gaps** | **36,317** |
| **Canonical ROM size** |  | **8,388,608** |

Within the reconstruction assembly, approximately 492,294 bytes remain active
C-decompilation, boundary-splitting, or function-merging work. The other 16,828
bytes are presently retained structural assembly: linker veneers, runtime
thunks, fixed hardware entry points, shared-literal modules, and proven
compiler-unproducible kernels.

Using the fan-facing “Total decompilation” definition, 7,859,997 of 8,388,608
bytes are already in pret-level clean source form: canonical assets, exact C,
or positively retained structural assembly. The remaining 528,611 bytes are
the 492,294-byte assembly debt plus the 36,317 unowned bytes, giving 93.70%.

Source ownership therefore measures whether every ROM address has a canonical
tracked producer. It does not measure how much executable code has reached C.

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

## Evidence boundary

The English Golden Sun ROM is the byte-identical build target. Approved private
regional ROMs can provide local differential evidence for distinguishing
shared engine code and data from localization, graphics, and story content.
They never become tracked files or fallback build inputs.

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

For a private complete build, `build_rom.ts` generates a GNU linker script that
places every reconstructed region in address order and fills only the remaining
unowned skeleton from the user's private English ROM. Generated linker files,
objects, ELFs, ROM images, reports, and work directories stay ignored.

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

Reconstruction is complete only when the tracked sources and generic tooling,
given the canonical private English ROM and approved compiler, independently
produce a byte-identical full ROM with no copied source gaps. Full C
decompilation is the further condition that no assembly remains except regions
with positive evidence that C cannot or should not reproduce them.
