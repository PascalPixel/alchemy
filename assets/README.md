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
`preview.frames.png` is a generated, non-source rendering of the four proven
dynamic states after applying the real palette, 32x32 tilemap, tile flips, and
the runtime `0x340`-byte dynamic upload. Regenerate it with
`python3 tools/render_resource_19.py -o assets/graphics/resource_19/preview.frames.png`.

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

`graphics/resource_f1/` contains 49 independently compressed 32x32 8-bit
indexed graphics blocks in one 7x7 PNG atlas. Multiple consumers decode one
selected `0x400`-byte block; one path remaps its byte indices through palette
RAM and uploads the result to VRAM. The archive uses a 16-bit offset table and
32-byte-aligned compressed slots. Its final two table-range bytes are outside
the proven archive boundary and remain ROM fallback bytes.

`graphics/palette_animation_e8.rgba.png` through
`palette_animation_ef.rgba.png` are eight consecutive 128-color frames. A
consumer selects `(counter & 7) + E8` and copies exactly `0x100` bytes through
the palette allocator. RGB channels preserve the five-bit GBA color. Alpha
255 represents bit 15 clear and alpha 254 represents bit 15 set, retaining the
otherwise non-visual high bit while leaving the displayed colors effectively
opaque.

`data/resource_e4_e6/` reconstructs three compressed fixed-capacity tables,
each containing 7,200 little-endian 16-bit pairs. Their active prefixes have
3,918, 4,380, and 5,048 pairs respectively; every successive active pair
moves by no more than eight in either component, and the remaining entries are
`0,0` padding. This proves ordered coordinate/path data and rules out the
visually noisy 4bpp interpretation. The pair names remain numeric because the
runtime selector and coordinate system are not yet proven.

`graphics/resource_e7/` reconstructs one compressed UI-graphics package as
five 16-color BGR555 palette banks and 256 4bpp tiles. Pixel-edge continuity
fixes the native tile order at four tiles wide: every consecutive group of
sixteen tiles is one coherent 32x32 frame, yielding sixteen frames and five
color variants. `preview.frames.png` is generated from those tracked sources
with `python3 tools/render_resource_e7.py -o
assets/graphics/resource_e7/preview.frames.png`.

`graphics/resource_f0/images/` contains 57 unique, palette-correct 32x32 4bpp
images. The ROM's 80-entry 16-bit offset table contains 23 null slots and one
offset for each image, represented by `archive.json`. Each package contains a
16-color palette followed by a terminating LSB-first run/move-to-front pixel
stream and zero alignment. The recovered encoder is canonical and rebuilds the
entire `0x592c`-byte archive exactly. `preview.atlas.png` is a generated
non-source contact sheet of the same PNGs.
