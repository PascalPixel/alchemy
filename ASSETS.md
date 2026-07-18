# Asset boundary

Private extractions and generated files stay under ignored `work/` or `out/`.
Tracked assets are appropriate only after their ROM range, format, and exact
re-encoding have been independently established.

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

`assets/audio/sound_075/sequence.json` and `sound_106/sequence.json` are
engine-native sequence units. They name tone banks and tracks symbolically,
preserve header priority, reverb, and block count, and spell commands as native
events. The seven-track `sound_075` source retains its pattern calls and
running-status omissions; the compact `sound_106` source covers modulation,
one note, its wait, and termination. The codec also represents repeats and
unconditional loop targets without converting streams through MIDI.

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
