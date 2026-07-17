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

`graphics/resources_d8_e3/` contains two alternative six-resource affine
background sets selected by a ROM-derived map condition. D8/DE are 224-color
BGR555 BG palettes. D9-DC and DF-E2 are palette-correct 8bpp tile sheets: the
loader fills affine BG3 charblock 2 and BG2 charblock 3 with their paired
128-tile halves. DD/E3 are 256-tile streamed BG3 banks shown as 16x16 tile
atlases. The runtime addresses them as virtual tile IDs 0x200-0x2ff, then a
four-u16 source/count/destination/delay command stream reindexes selected tiles
into BG3 charblock 2;
the BG3 affine map is generated separately, so no full-scene composition is
claimed from these banks alone.

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

`graphics/resource_f2/` through `graphics/resource_127/` contain 54
palette-correct indexed sprite archives. The runtime ARM decoder proves byte
values `1..0xdf` are pixel indices, `0xe0..0xff` skip 1..32 transparent
pixels, and zero terminates a frame. All 32x32 and 64x64 streams land exactly
on their metadata-defined pixel boundary and greedily re-encode byte-for-byte.
The shared 224-color OBJ palette is the independently reconstructed range
copied from ROM `0x0800779c`; each `preview.atlas.png` is a generated contact
sheet. Regenerate the series with `python3 tools/export_sprite_series.py
baserom.gba`.

`graphics/resource_12e/` through `graphics/resource_17c/` contain fourteen map
families, each with a 224-color BG palette and three 0x4000-byte GBA charblocks
as 4bpp tile PNGs. The generic loader copies each family's resources +2..+4 to
charblocks 1-3. Their tag-2 codec uses LSB-first LZ tokens and move-to-front
nibbles; payload-free plans reproduce all compressed bits and alignment bytes
exactly. Palette-bank selection belongs to map data, so the tile PNGs use a
neutral index legend rather than claiming one of fourteen 16-color banks.

`maps/resource_12e/grid/` through `maps/resource_17c/grid/` reconstruct the
same families' tagged kind-1 map components as 128x128 spatial record grids.
Four grayscale PNG planes preserve the transformed 16-bit metatile/flag value
and two independently accessed attribute bytes. A one-bit PNG records which
pre-transform values were sentinels, avoiding the transform's otherwise
ambiguous inverse. The kind-1 token trace plus these five source planes rebuild
every compressed component and its alignment bytes exactly.

`maps/resource_12e/components/` through `maps/resource_17c/components/`
reconstruct each container's 0x3c-byte header and components 0, 1, 3, 4,
and 5. `header.json` holds twelve opaque parameter bytes, three opaque
four-u16 records whose second, third, and fourth words are 0x1010, 0x0000,
and 0xffff in every traced family, and the six little-endian u32 component
offsets. The build cross-checks every offset against the sibling component
and grid claims of the same family and requires zero for absent components,
so the header, grid, and component sources cannot drift apart.
Component 0 is a text tilemap with four little-endian
u16 entries per 2x2 metatile; its plan preserves the planar transform mode and
the exact general-LZ token choices. Thirteen families use mode 1 and resource
15E uses mode 2. Component 1 is a JSON sequence of opaque four-byte descriptor
records. Present component-3 streams are JSON animation queues with their full
FDxx headers, pairs of u16 command words, FE00 queue terminators, and a final
FFFF; resources 140 and 158 have no component 3. Component 4 is present in
nine families and contains blend-register animation commands. Its semantic
JSON distinguishes single-word blend-control writes, value/duration pairs,
pair-index jumps, stream resets, and stops. The loader accepts both tag-0
general-LZ and tag-1 palette-LZ forms, so each plan records the exact codec,
token trace, and lookahead; families 12E, 134, 13A, 140, and 158 have no
component 4. Component 5 is a JSON sequence of three-byte sparse records plus
its FFFFFF terminator and zero alignment. All compressed plans retain trailing
lookahead bytes and reproduce their named component spans exactly. Regenerate
the series with `python3
tools/export_map_component_series.py baserom.gba`.
