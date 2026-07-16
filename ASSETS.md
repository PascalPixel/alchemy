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
