# Source conventions

Assembly, asset, and naming conventions for this tree. Merged from the former ASSEMBLY.md, ASSETS.md, and NAMING.md.

# Assembly boundary

Assembly is not one undifferentiated decompilation backlog. Every tracked
`asm/*.s` region is byte-verified, but its origin and long-term source form are
recorded separately. A function remains decompilation work until asm-free C
from the approved compiler reproduces it exactly. Linker products, runtime
thunks, fixed hardware entries, and proven deliberately assembled kernels stay
in assembly.

The current boundary is generated and checked without a ROM by
`bun tools/build_asm.ts --source-only`.

Figures regenerated 2026-07-22 via `bun tools/build_asm.ts --source-only`:

| Classification | Files | Bytes | Long-term treatment |
|---|---:|---:|---|
| Linker long-call veneers | 608 | 4,864 | Keep assembly |
| Declarative adjacent-section alignment | 328 | 656 | Keep as linker structure |
| `_call_via_rN` runtime thunk bundle | 1 | 56 | Keep assembly |
| BIOS/SWI wrapper bundles | 2 | 16 | Keep assembly |
| ROM dispatch table | 1 | 768 | Keep assembly |
| Fixed ARM cartridge entry | 1 | 68 | Keep assembly |
| Relocated IWRAM payload | 1 | 5,120 | Keep structured assembly |
| IWRAM division veneers | 4 | 32 | Keep assembly |
| Fixed-point math and quarter-sine lookup module | 2 | 568 | Keep structured assembly/data |
| Compiler/assembler literal pool | 1 | 32 | Keep structured data |
| Relocated ARM runtime modules | 10 | 5,720 | Keep structured assembly |
| Contiguous ARMv4T helper bank | 7 | 2,652 | Keep structured assembly |
| Relocated stack ARM kernel | 1 | 28 | Keep structured assembly |
| Shared-literal Thumb helper module | 1 | 40 | Keep structured assembly pending module-aware C |
| Mixed or misbounded code/data regions | 30 | 27,338 | Split before decompiling |
| Structured runtime relocation-helper module | 1 | 128 | Keep structured assembly |
| Proven parent-function fragments with pools | 9 | 1,646 | Merge when each owner is reconstructed |
| Branch-connected executable-gap continuations | 43 | 2,812 | Merge with their tracked owners |
| Proven multi-region function heads with pools | 9 | 3,820 | Merge with their continuations before exact C |
| Proven multi-region function continuations with pools | 7 | 2,928 | Merge with their function owners before exact C |
| Cross-function shared-literal module | 2 | 692 | Keep structured assembly pending module-aware C build |
| Proven deliberate performance modules | 2 | 954 | Keep assembly |
| Nonstandard Thumb call modules | 15 | 348 | Keep structured assembly |
| Nonstandard Thumb branch modules | 13 | 576 | Keep structured assembly |
| Proven register-only busy-wait | 1 | 14 | Keep assembly |
| Mixed-mode multiply helper | 1 | 16 | Keep assembly |
| Likely ordinary compiler output | 1,090 | 439,910 | Convert to exact C |
| **Total** | **2,191** | **501,802** | |

These counts describe files, not callable entries. `080000c0.s` bundles 96
fixed-width dispatch entries, `08006864.s` bundles two BIOS wrappers, and
`080072e4.s` bundles fourteen `_call_via_rN` thunks.

## Proven infrastructure assembly

- `080000c0.s` is a 96-entry ROM dispatch table. Each eight-byte entry has the
  fixed `ldr r4; bx r4; literal` form. Its width, literal placement, and
  clobber contract are not C semantics.
- The 330 long-call veneers are linker products with the exact
  `ldr r4, [pc]; bx r4; .4byte target` form. They are not Camelot functions and
  do not count as C debt.
- `080072e4.s` is the compiler runtime's fourteen-entry `_call_via_r0` through
  `_call_via_r13` table. The approved compiler emits calls to those symbols for
  ordinary C function pointers. The verifier and linker resolve them at a
  four-byte stride from `0x080072e4`.
- `08006864.s` and `08006870.s` contain direct BIOS software-interrupt
  wrappers. The fixed `swi`/`svc` instruction is not expressible in asm-free C.
- `080003c0.s` is the ARM entry selected by the cartridge header. It switches
  CPU modes, installs the IRQ and system stack pointers, calls the Thumb
  initialization entry, and loops back to the fixed entry address if that call
  returns. Those hardware-entry and instruction-set contracts are not ordinary
  Thumb C.
- `080022ec.s` through `08002304.s` are four fixed veneers into the division
  runtime at `0x03000380..0x030003f0`. The adjacent ordinary routines now have
  their own boundaries and remain C targets.
- `0809b7e4.s` is the 32-byte PC-relative literal pool used by `0809b698`.
  The false function at `0809b7f8` has been removed; the real function at
  `0809b804` now builds exactly from C.

`asm/classification.json` is the machine-readable boundary. `build_asm.ts`
attaches its origin, retention decision, confidence, and evidence to every
built region and rejects changes to the proven category counts.

The former `08007320.s` was a false ARM decode. All fourteen words are odd
Thumb pointers—twelve repetitions of `08003009`, one `08003811`, then another
`08003009`—and `0800300c` DMA-copies the complete 56-byte span into the IWRAM
dispatch area at `030000e0`. It is now typed pointer-table data in the
executable-gap source package, not assembly or C debt.

## Recovered fragment and pool boundaries

The smallest former mixed regions at `0800f1fa`, `0808b7b8`, `080d12a8`,
`080d5094`, `080d8258`, `080e1724`, `080e1a48`, `080e4ab8`, and `080e547c` are
not callable functions. Direct branches enter them while the parent function's
stack frame and high registers remain live, and each fragment branches back
into or returns through that parent. Their internal pools, adjacent pools, and
alignment are now explicit structured data. Authorship is unknown; they remain
assembly only until their complete owning functions can be reconstructed as
single units.

The executable-gap package contributes another 43 continuation units totaling
2,812 bytes. Each has a tracked incoming owner edge; the units either inherit
live stack and high-register state or lie between the owner's branch and next
entry. The large continuation at `0800fd5c`, for example, returns through the
frame established by `0800fb38`. These bytes remain active decompilation work,
but they are merge-with-owner work rather than 43 independent C searches.

The former mixed regions at `080d765c`, `080dd9c0`, and `080ec100` are proven
callable function heads, not complete functions. Each begins with a complete
prologue, keeps its stack frame and saved high-register state live, and branches
across a local pool into a later continuation file. Direct calls prove the first
and third entries; a stored Thumb function pointer proves the second. Their
alignment and pools are now explicit, but exact C requires merging each whole
multi-region function first. Authorship remains unknown.

Four more callable heads at `0808b674`, `080e15e8`, `080e47b8`, and `080f4168`
now include their formerly omitted structured tails. The first, second, and
fourth branch into later files with their stack frames live; the third dispatches
through an explicit 101-entry table into its continuation fragments. The first
two corrected layouts match their approved Japanese counterparts after address
normalization. The fourth preserves the same layout with localized constants,
while the third has an identical code prefix and all 101 table targets relocate
by the same offset. This is boundary evidence only; authorship remains unknown.

Two additional former mixed regions at `0800ebec` and `080be378` are callable
heads with formerly omitted structured tails. `0800ebec` now includes its local
literal pool before branching into later continuations with its frame live.
`080be378` now exposes its 100-entry dispatch table, the final case body,
alignment, and local pool before control continues in later fragments. The
first corrected layout matches its approved Japanese counterpart after address
normalization. For the second, the non-pointer layout matches and all 100 table
entries relocate by the same offset. This is shared-boundary evidence only;
authorship remains unknown.

The former mixed regions at `080d76f0`, `080dda3c`, and `080ec190` are
continuations of those same three functions. They are entered by direct branch
with their owners' stack frames and high registers live, cross explicit local
pools, and branch onward into later continuation files. Their corresponding
normalized layouts occur intact in the approved Japanese ROM. This confirms
shared engine boundaries, not authorship; all three remain unknown-origin
assembly until their complete functions can be reconstructed.

The continuations at `0802691c`, `080de0d4`, and `080ec264` likewise use live
owner frames and continue into later files after explicit pools. The complete
corrected layouts for the latter two match the approved Japanese ROM after
normalization; the first retains 540 of 552 normalized bytes at its corresponding
layout, with the same control-flow boundary. This establishes structure only,
not authorship or original names.

`080e17c4` is a further continuation of the function headed at `080e15e8`.
It retains the owner's live frame across an internal pool and an indirect
arithmetic helper, completes the following loop tail, and branches into the
later `080e1a48` continuation. Its complete corrected layout matches the
approved Japanese ROM after address normalization. This establishes a shared
function boundary only, not authorship or an original name.

`08093fa0` proved to be a complete callable function followed only by two bytes
of alignment. After making that boundary explicit it returns to the ordinary
compiler-output category and remains an exact-C candidate. Its complete layout
also occurs intact in the approved Japanese ROM at the corresponding address.

`0800b168` and `080d5c48` likewise proved to be complete callable functions.
The former was missing only its trailing alignment, while the latter was
missing its adjacent literal pool. Both corrected layouts occur intact at the
corresponding approved Japanese addresses, so they return to the ordinary
compiler-output category as exact-C candidates. The comparison establishes
layout and boundary only, not authorship or original names.

`08006a78` is a compact runtime relocation-helper module. It contains two
direct-call entries, a four-byte Thumb helper template, and an installer that
copies the template by its source start and end addresses before storing the
relocated odd Thumb entry. Those source-position semantics are now represented
by labels rather than an opaque instruction word. The same complete layout is
present in the approved Japanese ROM, establishing a stable shared module but
not an original name or author. It remains structured assembly.

`0800230c.s` and `0800231c.s` form a fixed-point math and lookup module.
`0800231c.s` now owns the complete 256-entry unsigned halfword table at
`08002344`; every entry equals the rounded value of
`65536 * sin(pi * index / 512)`. The classification records the 40-byte Thumb
lookup routine and the 512-byte quarter-sine table as separate semantic
subregions even though their PC-relative relationship requires one source
unit. This mathematical identification does not establish an original symbol,
filename, or author.

The four files from `08002544.s` through `08002d5c.s` are caller-delimited ARM
routines copied before execution: two decompression formats, a transfer-command
executor, and a Thumb-BL relocation walker. Their instruction set and exact
copy bounds are proven, while the approved compiler emits Thumb code only.
They are therefore genuinely compiler-unproducible structured assembly rather
than ordinary C debt. Their program-versus-library origin and authorship remain
unknown.

The contiguous seven-region bank at `08015430..08015e8c` is likewise explicit
ARMv4T code. Its tracked index records the complete 2,652-byte span, seven
source boundaries, and independently recovered consumers. Every source unit
assembles in ARM mode while the approved compiler is configured and verified
as Thumb-only, so the bank cannot be ordinary approved-compiler C. It remains
structured assembly; the program-versus-library origin and authorship are not
inferred from that instruction-set boundary.

`08002dd8.s` now owns both callable Thumb helpers at `08002dd8` and `08002df0`,
their intervening alignment, and the literal at `08002dfc` referenced across
the former file boundary. The obsolete standalone `08002df0.s` boundary is
removed. The merged source remains structured assembly pending a module-aware
C match; neither its layout nor its shared literal establishes authorship.

`080f9c44` is a callable 66-byte function followed by alignment and a local
literal pool. Its first two PC-relative loads share literals at the end of the
next function, so the present one-function-at-a-time C build cannot emit it
independently. A stored Thumb function pointer proves that the next function
starts at `080f9c90`, not at the former `080f9c9e` boundary; the equal branch
falls through from the recovered prefix into `080f9c9e`. The two corrected
files therefore remain one structured shared-literal module with unknown
authorship.

## Deliberate performance assembly

`080f9674.s` reconstructs the complete 932-byte sound-mixing entry copied from
ROM `0x080f9674` to IWRAM `0x03007000`. One callable entry deliberately switches
Thumb to ARM, returns to Thumb for channel setup, switches back to ARM for the
sample-buffer accumulation loops, and finally returns to Thumb for its epilogue.
Its stored odd function pointer, copy destination, exact mixed-mode boundaries,
and PC-relative mode switches prove both the run address and the instruction
sets. The approved Thumb-only compiler cannot emit this module.

The adjacent `080f9a18.s` clears 64 bytes with four unrolled `stmia`
instructions, temporarily holds `r4` in `ip`, and uses no stack frame. These
two schedules are direct evidence of deliberately assembled performance code,
although the ROM alone cannot prove whether Camelot wrote them or bundled them
with a sound library. This repository records that uncertainty instead of
inventing authorship.

The startup copy at `0800300c` identifies a second hot payload:
`0x08000770..0x08001b6f` is copied to `0x03000000..0x030013ff`. The first
`0x658` bytes now build exactly from `asm/08000770.s` at their IWRAM run
address. That source separates the ARM IRQ dispatcher, fixed-point
`smull`/`smlal` math kernels, computed-entry unrolled clearing, packed-bit
expansion, transforms, decompression, and compiler-runtime division helpers.
The remaining `0xda8` bytes at ROM `0x08000dc8..0x08001b6f` are the mixed-mode
audio path and remain private-ROM fallback until their Thumb/ARM boundaries
are reconstructed.

## Approved-compiler prologue evidence

The approved compiler saves `lr` in every Thumb function that contains any
conditional branch or loop, even a leaf; only straight-line leaves compile to a
bare `bx lr` return. A byte-verified region that branches internally, never
saves `lr`, and returns through `bx lr` is therefore positive evidence of a
different toolchain or hand assembly, not ordinary C debt. The regions matching
that signature concentrate in the relocated runtime modules, the fixed-point
math module, and the sound engine around `080f9a30..080fb410`, alongside the
already-classified helpers. Two-byte `bx r3` trampolines inside the sound
engine and the bare multi-region epilogue fragments carry the same weight: the
approved compiler cannot emit them as standalone functions.

Fifteen sound-runtime source units at `080f9a80`, `080f9b40..080f9c2c`, and
`080fa1d4..080fa1e8` preserve the incoming return address in `ip`, issue a
direct `bl` that overwrites `lr`, and return through `bx ip` without saving
`lr` on the stack. This is a positive calling-convention boundary, not a
near-miss from ordinary C: the approved compiler's reproduced call prologue
cannot emit it. The complete 348-byte cohort therefore remains byte-exact
structured assembly with program-versus-library origin and authorship unknown.

Thirteen further source units—three early executable-gap modules and ten sound
runtime modules—contain conditional control flow or loops, never save `lr`,
and return through bare `bx lr`. Their exact boundaries and bytes are verified,
and the approved compiler's reproduced prologue rule excludes an exact C
emission. They remain structured assembly for that positive ABI reason, not
merely because a candidate search failed.

## Not assembly authorship evidence

Hardware-register literals, high-register allocation, `ldmia`/`stmia`, or an
isolated BIOS instruction do not by themselves prove hand assembly. Most
remaining files still look like ordinary optimized C and remain conversion
targets. Likewise, a failed register-pinning experiment says nothing about the
original source language.

The retained `080f9a80` source unit also owns the shared-tail entries at
`080f9a98` and `080f9a9a`; keeping the complete unit preserves that additional
structural relationship and its alignment word. Separately, `080f9b10`
branches directly into the middle of `080f9ac0` with live state. That still-
ordinary module remains C debt unless a module-level asm-free compilation
reproduces the same entry and continuation layout.

Six unreachable files inside `0x08037464..0x08073808` had incoherent stack
restoration and impossible fall-through because the scanner had decoded packed
message bits as Thumb instructions. The semantic context-code and message-bank
reconstruction now replaces every one of them. This is a pipeline-level guard:
unreachable islands inside a consumer-proven data package remain data recovery
work and never become assembly or C merely because a disassembler accepts them.

## Acceptance rule

A region moves from `asm/` to `src/` only when its asm-free C compiles to the
exact bytes at the exact address. A region is retained as intentional assembly
only when its ABI, placement, instruction schedule, or hardware entry contract
provides positive evidence. Unknown or mixed regions stay explicitly unknown;
absence of a C match is never treated as proof of hand-written assembly.

# Asset boundary

Private extractions and generated files stay under ignored `work/` or `out/`.
Tracked assets are appropriate only after their ROM range, format, and exact
re-encoding have been independently established.

## Target scope

Current ranges and byte totals describe the canonical Golden Sun English
target. The other eleven approved ROMs may be compared locally to prove which
assets are shared and which vary by game or language. Shared semantic sources
should remain single sources; proven differences belong in explicit
edition-aware metadata or separate canonical variants. Binary patches,
cross-ROM diff payloads, and copied target spans are never asset sources. Each
future target must encode completely from its own selected canonical inputs.

`tools/import_asset.ts png` converts a non-interlaced indexed PNG into row-major
GBA 8×8 tiles and a BGR555 palette. It does not quantize, reorder, deduplicate,
compress, or infer a tilemap. Palette channels must be multiples of eight, so
the conversion cannot silently lose color bits.

```
bun tools/import_asset.ts png image.png --bpp 4 \
  --tiles out/image.4bpp --palette out/image.gbapal
```

`tools/import_asset.ts midi` validates format-0/1 PPQN Standard MIDI and writes
a stable, lossless event JSON intermediate. It is not a GBA music encoder:
the ROM's sequence, instrument, sample, timing, and compression formats must be
recovered before a deterministic engine-specific backend can be written.

```
bun tools/import_asset.ts midi song.mid -o out/song.events.json
bun tools/import_asset.ts --self-test
```

For exact rebuilding, PNG alone is insufficient until tile ordering, palette
length, tilemaps, flips, compression, and alignment are represented explicitly.
MIDI alone is insufficient to preserve engine commands, voice allocation, and
sample data. Those facts belong in future per-asset manifests, not heuristics.

`assets/audio/song_table.json` is the first engine-native audio source. Its
symbolic header references, proven player selectors, and still-unknown trailing
halfwords rebuild the complete 312-entry selection table exactly. Every
trailing halfword mirrors its player selector, a storage invariant encoded
without assigning that field an unproven role. Sequence commands, tone banks,
and samples remain separate recovery units rather than
being flattened into MIDI or copied as opaque audio bytes.

`assets/audio/midi/sound_NNN.mid` plus the optional sidecar
`assets/audio/data/sound_NNN.json` are the engine-native sequence units. The
`.mid` is the canonical core: it carries notes as note-on/off, waits as
delta-time, and engine control and structural commands (voice, volume, pan,
pattern calls, repeats, unconditional loop targets, running-status markers) as
verbatim events, with tone banks, tracks, priority, reverb, and block count in
its conductor skeleton. The sidecar records only per-event exceptions where the
ROM's byte encoding departs from the codec's default rules: greedy
running-status, omission of note parameters equal to the running value, and
greedy largest-first wait-table splits. Those defaults match the ROM's
dominant encoding, so 163 of 260 units carry no sidecar. Each sidecar
reference is a stable `(track, event-index)` splice against the default stream,
fingerprinted by event count and hash so `tools/midi_sequence.ts` fails loudly
when a `.mid` and its sidecar disagree. The compact `sound_106` unit (one note,
its wait, modulation, and termination) is sidecar-free; the seven-track
`sound_075` unit keeps its pattern calls and non-canonical status choices in a
sidecar. `tools/midi_sequence.ts` is the codec, converter, and validator;
`tools/midi_roundtrip.ts` documents the underlying sequence-JSON↔SMF mapping
and its losses.

RGBA source PNGs may also be used as lossless four-byte record atlases. In that
case the per-asset documentation defines the channel-to-field mapping; the
colors are data visualization, not a claim that the records are display pixels.

For 16-bit GBA palette words that use bit 15, `gba-palette-rgba` stores the
BGR555 color as exact multiples-of-eight RGB and encodes bit 15 as alpha 255
(clear) or 254 (set). The importer rejects every other alpha value.

Recognizable renderings and exact sources are distinct. A generated preview
may combine source tiles, palettes, maps, flips, or animation states, but does
not itself claim extra ROM coverage. Source PNGs count only through a manifest
encoder that reproduces their named ROM region byte-for-byte.

`tools/compose_scene.ts` is such a preview generator. It reads a map
container's already-claimed byte-exact sources (the kind-1 grid, the
metatile tilemap, the charblocks, and the palette) and paints the 2D scene the
map renders: `grid cell -> low-12-bit metatile index -> 2x2 tile entries ->
header-selected 0x200-tile character window -> loader-linked resource`, after
which bits 10/11 flip the tile horizontally or vertically and bits 12-15 select
its palette bank. `assets/maps/map_load_table.json` is the byte-exact semantic
source for the runtime's 186 six-u16 records: map container, palette, three
VRAM charblocks, and animation source. Shared and partial families therefore
resolve through the same table the game uses instead of adjacency guesses.
`--all` writes every loader-linked variant to the ignored `out/scenes/`. It is
display-only, reads no build input the encoders do not already own, and is
never read back by the build.

`tools/bg_tile_usage.ts` expands the same linkage into a reusable background
tile-usage index. Each resource and canonical charblock slot records every map
position grouped by loader variant, header window, charblock role, palette
bank, and horizontal/vertical flip state. Reports are analysis products and the
tool refuses to write them outside ignored `out/`; for example,
`bun tools/bg_tile_usage.ts 184` writes the available variants for container
184 without modifying canonical assets. `--all` also lists load-table variants
whose map geometry has not yet been reconstructed instead of silently
inventing placements for them.

Map-container semantic sources are claimed component-by-component. Their
manifest entries must use the exact component span, never the enclosing
container, so the existing kind-1 grid claim and any still-opaque component
remain non-overlapping. Metatile transform modes, optional queue and
blend-animation components, compression tags and token choices,
compressed-stream lookahead, and raw-list terminator padding are explicit
source or plan fields rather than inferred build defaults. The 0x3c-byte
container header is its own exact-span claim: its six component offsets are
explicit source fields that the build verifies against the family's sibling
component and grid claims, and its remaining parameter and record fields
stay opaque until their semantics are independently established.

## Map charblock and animation-source tile banks

The map-family `charblockN.4bpp.png` and `animation_source.4bpp.png` files are
lossless, sequential tile sources, not composed scenes or animation previews.
Each indexed PNG contains 512 GBA 4bpp 8x8 tiles in ROM order. Map tile entries
establish the authored sheet geometry: adjacent 2x2 groups advance by `+1`
horizontally and `+0x20` vertically, so both charblocks and animation banks are
stored as 32-column by 16-row sheets. The previous 16-column debug wrap folded
every authored row in half and made coherent structures look like interleaved
strips even though their bytes were correct. Pixel indices are
palette-independent. Colorized sheets are generated previews, not sources:
palette-bank selection comes from the 16-bit text entry's top nibble, while
bits 10 and 11 flip the selected 10-bit tile index horizontally and vertically.

The adjacent `charblockN.kind2.json` and `animation_source.kind2.json` record
the exact tag-2 codec token plan, lookahead bytes, decoded size, and explicit
32x16 sequential layout. The PNG importer restores the exact 0x4000-byte
packed tile bank, and the plan then
reproduces the compressed ROM resource byte-for-byte. Palette application,
destination tile IDs, timing, and composition belong to separate map animation
commands and are deliberately not asserted by these source images.

A tile bank may be migrated incrementally to coherent object sources with
`tools/tile_objects.ts`. An object PNG is accompanied by its exact text
tilemap, whose tile IDs, palette banks, and flip flags map the displayed object
back to canonical bank slots. The neutral `remaining.4bpp.png` fallback then
covers only slots not yet represented by named objects. The builder requires
all repeated uses of a slot to agree after reversing their display flips. A
coherent rectangle may span both header-selected display windows: each bank
plan reads the same object and claims only the slots it owns. After the rebuilt
bank matches exactly, extraction switches the manifest to the object plan and
removes the superseded sequential atlas; old and new source forms are never
kept side by side.

The storage directories retain each compressed resource exactly once. Runtime
roles come from `map_load_table.json`, so a short adjacent group may contribute
only a changed VRAM or animation bank while the other roles reference an
earlier group. The manifest and preview resolver both use that linkage without
copying shared PNGs or retaining alternate generated atlases.

# 命名規約 / Naming

This repository deliberately gives reconstructed source the voice of a 2001
Japanese console-game codebase. Semantic names are terse, practical, and a
little playful; address-only names remain only until the data flow says what a
thing does.

## 名前

- Prefer short ASCII Japanese nouns for assets: `iwamuro`, `ougonmon`, `taki`,
  `ki`, `hashi`. Use the spelling a Japanese programmer of the period could
  plausibly have typed, not a modern localization phrase.
- Prefer compact period abbreviations in code where they stay readable:
  `Flg`, `Ev`, `Btl`, `Chr`, `Tbl`, `Work`, `Pos`, `No`, `Get`, and `Set`.
- Name from recovered function or composition. A visible rock chamber may be
  `iwamuro`; a tile that merely resembles foliage does not become a particular
  species until its repeated composition supports that reading.
- Japanese labels may accompany the source name (`岩室`, `黄金門`). English is
  a short gloss for contributors, not the canonical voice of the asset.
- Do not add `maybe`, `guess`, or confidence scores to source identifiers. If
  the semantics are too weak for a useful period name, retain the stable
  numeric resource name and revisit it when the pipeline supplies more context.

These are canonical reconstruction names. They are intentionally styled as if
they belonged to the original project, but an exact historical spelling is
claimed only when surviving game data actually contains it.

## コメント

Comments diagnose the overall pipeline, never one screenshot. State the data
relationship, invariant, or failure class and the family of outputs it affects.
A particular scene may be a regression fixture, but it is not the explanation.

ソース内のコメントはUTF-8の日本語だけで記述する。英文の併記はしない。
当時らしい簡潔な漢字かな交じり文を基本とし、外来語にはカタカナを使う。
半角カナも使用できるが、識別子、命令名、数値など必要なASCII表記はそのまま残す。
コメントは画像一枚の感想ではなく、処理系全体の関係、不変条件、障害分類を書く。
例：

```text
マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。
セル番号は下位12bit。上位4bitは別の属性値として扱う。
左右反転を戻す。表示時の反転を取り消して標準タイルへ戻す。
```

Avoid diary comments, image-by-image observations, model speculation, and
claims that a name or comment was recovered when only its semantics were.

# Approved compiler bundle


The approved compiler bundle lives in the sibling repository checkout
`../alchemy-gcc/dist/` ([PascalPixel/alchemy-gcc](https://github.com/PascalPixel/alchemy-gcc)):
compiler source, build scripts, and the ignored runtime bundle stay out of
this reconstruction tree entirely.

The bundle directory has one boundary and three approved compiler lanes:

```text
../alchemy-gcc/dist/
├── xgcc, cc1, cpp, tradcpp       # GS1: GCC 2.96 snapshot
├── gs2/
│   └── xgcc, cc1, cpp0, tradcpp0 # GS2: Camelot GCC 3.0 backend
├── agbcc/
│   └── old_agbcc                  # source-scoped stock m4a compiler
└── m2c-venv/          optional decompilation helper
```

The four top-level executables are the GS1 compiler. The four executables under
`gs2/` are the reconstructed GS2 backend. `agbcc/old_agbcc` is a separately
validated compiler used only by the explicit source-address allowlist in
`tools/alchemy_gcc.ts`. Run `./stage.sh gcc296`, `./stage.sh gs2`, and
`./stage.sh agbcc` in the sibling compiler repository after building those
targets; `./stage.sh --check <target>` verifies that an ignored runtime stage
still matches its local build. All nine programs are native arm64 Mach-O
executables and depend only on macOS `libSystem`; compiler source trees,
intermediate objects, nested Git repositories, duplicate drivers, and target
runtime archives do not belong in the bundle. Native `arm-none-eabi` binutils
remain a host dependency supplied through `PATH`, because they are shared
generic assembler/linker tools rather than part of the approved compiler's code
generation.

`bun tools/alchemy_gcc.ts gs1`, `bun tools/alchemy_gcc.ts gs2`, and
`bun tools/alchemy_gcc.ts agbcc` check the host architecture, target-specific
executable set, and approved SHA-256 digests before compiler use. Every build
and matching tool constructs its source-aware compiler invocation through that
module; no unvalidated compiler path is permitted.

The bundle stays outside this repository. Public commits contain only this specification,
the validator, reconstructed source, and generic tooling—not compiler binaries
or third-party checkouts.

## Provenance

The bundle's executables are built from
[PascalPixel/alchemy-gcc](https://github.com/PascalPixel/alchemy-gcc), a fork
of [Coaltergeist/camelot-gcc](https://github.com/Coaltergeist/camelot-gcc):
vendored FSF GCC source (the gcc-2.96 development snapshot of 2000-07-31 for
GS1 and GCC 3.0 with the reconstructed Camelot backend for GS2) with
host-portability patches, all documented in that repository.
One vendored patch affects code generation: `config/arm/elf.h` emits
`.align N, 0` so modern assemblers pad with zeros the way period binutils
did. The compiler contains no game-derived material; it is generic public
toolchain source under the GPL.

The selection of this compiler version originated outside this repository:
the public Golden Sun decompilation community identified the snapshot, and
camelot-gcc packaged and validated it. Within this repository the selection
is independently discharged by the strongest available evidence: hundreds of
byte-identical function matches and a byte-identical full-ROM source build
through this compiler alone. A wrong vintage could not produce either. That
external hint concerns only which public tool to run; every piece of game
knowledge in this tree remains derived from the approved ROM set.

The stock m4a ("Sappy") audio library that period licensees linked verbatim
requires pret's `old_agbcc` rather than gcc-2.96. The source-address allowlist
currently covers `080fa1fc`, `080fa2a0`, `080fa324`, `080fa350`, `080fa39c`,
`080fa3f0`, `080fa424`, `080fa458`, `080fa490`, `080fa514`, `080fa83c`,
`080fa8d4`, `080fa928`, `080fa9a4`, `080fa9e0`, `080fab3c`, `080facf8`,
`080fb2cc`, `080fb334`, `080fb3a8`, `080fb430`, `080fb4a4`, `080fb670`,
and `080fb6a4`; each was independently
assembled and linked to an exact target interval before adoption. No generic
directory or subsystem switch selects this compiler, following the per-unit
compiler precedent of `pret/pokeruby`.

`080fb670` additionally uses old_agbcc's default-off
`-mliteral-before-shift` compatibility mode. It only moves an independent
constant-pool load ahead of an adjacent left shift after proving the two
destinations do not overlap; the other routed units retain stock ordering.

`080fa514` uses `-O1` plus old_agbcc's default-off
`-mcommutative-copy-constant` mode. The backend swaps a destructive
commutative copy/AND pair only when the AND's other register was loaded with a
constant by the immediately preceding instruction and all three registers are
distinct.

`080fb2cc`, `080fb334`, and `080fb3a8` use old_agbcc's default-off
`-mprologue-next-high-reg` mode. After register allocation, it finds the
highest live callee-saved high register and marks its successor live only
during prologue generation. This retains the stock object's otherwise-unused
adjacent high-register save without changing the function body, and the
epilogue sees the same save mask when restoring registers.

The GS1 compiler also exposes a default-off `-mgrouped-dma-store` compatibility
mode for the historical three-word Thumb DMA descriptor sequence. It lowers
three explicit scalar words to `stmia` plus a base restore without inline
assembly or register pins. Only the independently verified `080049e8`,
`08004a28`, `08004a5c`, `080958a8`, and `0809bb34` sources enable it;
ordinary GS1 sources retain the default code generation path. `080049e8`
also uses the existing source-scoped `-O1` route because `-O2` swaps two
independent descriptor loads.

Nineteen independently verified overlay constructors enable GCC 2.96's
default-off `-mcall-arg0-move-first` compatibility mode. Immediately before a
call, it moves an independent `r0` register copy ahead of an adjacent `r1`
immediate; dependency and call adjacency checks prevent a broader scheduling
change. The cohort is `resource_380`, `382`, `385`, `387`, `38a`, `396`, `39b`,
`39c`, `39d`, `39e`, `3a0`, `3a1`, `3a5`, `3a6`, `3ab`, `3b3`, `3be`, `3c0`,
and `3c9`, each at `020000a0`. Because every member has the same source
filename, `tools/alchemy_gcc.ts` routes this mode by the complete canonical
resource path rather than by address stem. Sources with the same stem in any
other overlay, and every GS2 source, retain default ordering.
