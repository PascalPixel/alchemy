# Assets

Every entry in `manifest.json` names its exact ROM range and deterministic
encoding. `tools/build_assets.py` refuses a region unless the tracked source
re-encodes byte-for-byte to the private ROM.

`graphics/080c5b30.4bpp.png` contains seven palette-independent 4bpp tiles.
ROM code copies the seven consecutive 32-byte units to seven consecutive VRAM
tile slots. The PNG palette is an index legend only; it does not claim the
scene's active colors.

`graphics/0800777c.gbapal.png` and `graphics/08073812.gbapal.png` preserve
BGR555 colors copied directly from ROM into palette RAM by DMA. Their ranges
and lengths come from the DMA setup; no scene names are inferred.

`graphics/direct_palettes/` contains three 64-color resources selected by
fixed pointer-table IDs. Their call sites copy exactly `0x80` bytes to palette
RAM; names beyond the independently derived IDs are intentionally omitted.

`graphics/resource_19/` is one compressed graphics package. Its tracked
sources are a BGR555 palette PNG, two 8bpp tile PNGs, a 32×32 text tilemap,
and a payload-free custom-LZ token plan. The plan records literal positions
and copy decisions; literal bytes come from the PNG/text components during
the build. Together they re-encode the complete compressed ROM stream.

`graphics/resources_d8_e3/` contains two alternative six-resource graphics
sets selected by a ROM-derived map condition. The palette streams use BGR555
PNG sources. Four decoded banks per set are hardware-proven VRAM inputs; the
last is a related RAM input whose downstream format is not yet established.
Until tile depth is proven, these ten decoded banks use format-neutral indexed
PNGs: one pixel index preserves one byte without asserting 4bpp or 8bpp.

`maps/world_map/` reconstructs resources D4-D7 as one independently traced map
subsystem. D4 is an offset-table archive of 640 compressed 16x16 chunks. Each
RGBA atlas pixel stores one little-endian 16-bit tile-pair index and one
little-endian 16-bit flag field; the low values address all 4,386 records in
D5 exactly. D5's RGBA pixels store the two little-endian 16-bit tilemap entries
in each record. D6 is a terminated 16-bit animation command stream represented
as text. D7 contains two banks of 512 four-byte terrain-property records. The
PNG row wrapping is presentational only; the manifest preserves linear order.
All four compression plans contain token choices but no literal payload bytes.
