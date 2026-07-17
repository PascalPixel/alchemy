# Asset boundary

Private extractions and generated files stay under ignored `work/` or `out/`.
Tracked assets are appropriate only after their ROM range, format, and exact
re-encoding have been independently established.

`tools/import_asset.py png` converts a non-interlaced indexed PNG into row-major
GBA 8×8 tiles and a BGR555 palette. It does not quantize, reorder, deduplicate,
compress, or infer a tilemap. Palette channels must be multiples of eight, so
the conversion cannot silently lose color bits.

```
python3 tools/import_asset.py png image.png --bpp 4 \
  --tiles out/image.4bpp --palette out/image.gbapal
```

`tools/import_asset.py midi` validates format-0/1 PPQN Standard MIDI and writes
a stable, lossless event JSON intermediate. It is not a GBA music encoder:
the ROM's sequence, instrument, sample, timing, and compression formats must be
recovered before a deterministic engine-specific backend can be written.

```
python3 tools/import_asset.py midi song.mid -o out/song.events.json
python3 tools/import_asset.py --self-test
```

For exact rebuilding, PNG alone is insufficient until tile ordering, palette
length, tilemaps, flips, compression, and alignment are represented explicitly.
MIDI alone is insufficient to preserve engine commands, voice allocation, and
sample data. Those facts belong in future per-asset manifests, not heuristics.

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

## Map animation-source tile banks

The map-family `animation_source.4bpp.png` files are lossless, sequential tile
sources, not composed scenes or animation previews. Each indexed PNG contains
512 GBA 4bpp 8x8 tiles in ROM order, arranged 32 columns by 16 rows solely to
make the linear bank editable. Tile 0 in the PNG has the engine's virtual tile
ID `0x600`; tile 511 has ID `0x7ff`. Pixel indices are palette-independent.

The adjacent `animation_source.kind2.json` records the exact tag-2 codec token
plan, lookahead bytes, decoded size, and the explicit sequential layout. The
PNG importer restores the exact 0x4000-byte packed tile bank, and the plan then
reproduces the compressed ROM resource byte-for-byte. Palette application,
destination tile IDs, timing, and composition belong to separate map animation
commands and are deliberately not asserted by these source images.

Thirteen of the fourteen traced map families have this field-5 tag-2 bank.
Family `0x164` is excluded: its `base + 5` resource is the next container, not
an animation-source bank.
