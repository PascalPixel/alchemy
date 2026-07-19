# Assets

Every entry in `manifest.json` names its exact ROM range and deterministic
encoding. `tools/build_assets.ts` refuses a region unless the tracked source
re-encodes byte-for-byte to the private ROM, and rejects tracked images that
are not inputs to that verified build.

`audio/song_table.json` reconstructs the sound engine's 312-entry selection
table. Each eight-byte entry names a symbolic sequence header, the player index
read by the ROM-side selector, and a mirrored second halfword whose role is not
yet proven. Repeated empty slots share one symbol instead of duplicating bytes.
The adjacent music-player records, sequence streams, tone banks, and wave data
are not claimed by this table.

`audio/sequences/` reconstructs all 260 nonempty native sequence units selected by
the sound table. The sources retain exact engine duration codes, symbolic
track and tone-bank references, priority, reverb, loops, pattern calls, and
command-status choices. The indexed series claims 150,254 independently
round-tripped bytes, including absolute-address alignment between streams and
headers.

`audio/waves/` reconstructs all 32 tone-referenced signed-PCM records as
standard mono 8-bit WAV sources. Its index preserves exact fixed-point
frequencies and loop points, while the verified builder restores native signed
samples, 16-byte headers, and alignment for 404,160 ROM bytes. Five adjacent
zero-length synthesizer descriptors remain a separate recovery unit.

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
A generated, non-source preview may render the four proven dynamic states after
applying the real palette, 32x32 tilemap, tile flips, and the runtime
`0x340`-byte dynamic upload. Such previews remain ignored build products.

`graphics/resource_1d/` through `graphics/resource_3e/` each contain one
palette-correct `ichimaie.8bpp.png` (一枚絵): a 128-color, 256×120 raster proven
by the palette DMA and the fixed `15×8×32×8` ARM decode loop. The LSB-first
delta7 predictor is continuous in raster order, wraps modulo 128, and has a
unique shortest-code inverse, so the PNG needs no opaque token-plan sidecar.
The encoder restores every meaningful bit and the zero padding through the
last halfword. Eighteen records have a separate two-byte alignment gap; those
36 bytes remain fallback rather than being disguised as image lookahead.
`ichimaie` is a canonical period-style reconstruction name, not a claim that
the historical source filename survived.

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
color variants. Non-source previews generated from those tracked sources remain
ignored build products.

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
sheet and remains ignored.

`graphics/chr/` reconstructs 33 descriptor-linked static character banks as
one palette-correct `koma.8bpp.png` (コマ) sheet per physical bank. The sheets
hold 1,049 unique 32x32 frames instead of creating one file per frame. Their
payload-free plans preserve logical pointer aliases and the outer source-
dictionary recipe: a little-endian literal/control split, LSB-first flags, and
back-references into earlier encoded bytes of the same bank. PNG pixels feed a
canonical zero-skip encoder, zero alignment is explicit, and the builder emits
the trailing absolute frame directories and null terminators. The complete
`0x08244fc0..0x08287774` series re-encodes byte-for-byte; numeric `chr` names
remain because no historical character names have been recovered.

`graphics/resource_128/` through `graphics/resource_369/` contain the 121 map
families whose container is directly followed by a 224-color BG palette
stream and zero to four 0x4000-byte kind-2 tile banks. These directories are
canonical storage groups, not a claim that every adjacent bank is used by that
container. `maps/map_load_table.json` reconstructs the loader's 186-record
マップ読込表 and explicitly links each map index to its container, palette,
three VRAM charblocks, and animation-source bank. Partial groups therefore
reuse resources from another storage group without duplicating their PNGs.
Fifty of the groups carry only the palette. The tag-2 codec uses LSB-first LZ tokens and move-to-front
nibbles; payload-free plans reproduce all compressed bits and alignment bytes
exactly. Nine palette streams are tag-1 palette-LZ rather than tag-0
general-LZ, so each palette plan records its exact codec and tag.
Palette-bank selection belongs to map data, so the tile PNGs use a
neutral index legend rather than claiming one of fourteen 16-color banks.
Their 512 tiles use the map-authored 32x16 sheet geometry: text entries advance
by one tile across a row and by `0x20` down a row. A 16x32 wrap preserves bytes
but folds coherent structures into misleading alternating strips.
The reconstructed series covers containers 0x128 through 0x369 in the resource
table and claims each family's greedy palette-plus-banks tail, leaving every
resource that does not classify unclaimed.

Banks can be decompiled further into coherent object PNGs without losing their
exact slot order. `tools/tile_objects.ts extract-map` writes a colorized object,
its exact text tilemap, and a neutral remainder atlas; `--append` migrates more
objects from the same bank later. Once the plan reproduces the complete bank,
the tool updates the manifest and deletes the superseded sequential atlas. The
object becomes the
authoritative source for every referenced slot: the builder uses the tilemap's
10-bit index, palette bank, and H/V flags to reverse display flips and restore
the canonical 4bpp bank. The header selects which extracted charblock occupies
each 0x200-tile display window; this selection is resolved before an object is
assigned to a bank. `resource_152/objects/ougonmon.png` (黄金門) and
`resource_190/objects/iwamuro.png` (岩室) are period-style semantic sources;
their remaining unclaimed slots stay in neutral fallback atlases.

`maps/resource_128/grid/` through `maps/resource_369/grid/` reconstruct all
173 traced containers' tagged kind-1 map components as 128x128 spatial record
grids; 52 containers have no palette or tile-bank resources of their own and
share a neighbouring family's graphics.
Four grayscale PNG planes preserve the transformed 16-bit metatile/flag value
and two independently accessed attribute bytes. A one-bit PNG records which
pre-transform values were sentinels, avoiding the transform's otherwise
ambiguous inverse. The kind-1 token trace plus these five source planes rebuild
every compressed component and its alignment bytes exactly.

`maps/resource_128/components/` through `maps/resource_369/components/`
reconstruct each container's 0x3c-byte header and components 0, 1, 3, 4,
and 5. `header.json` holds twelve opaque parameter bytes, three opaque
four-u16 records whose second, third, and fourth words are 0x1010, 0x0000,
and 0xffff in all but eight traced families, and the six little-endian u32
component offsets. The build cross-checks every offset against the sibling
component and grid claims of the same family and requires zero for absent
components, so the header, grid, and component sources cannot drift apart.
Every compressed component stream is tag-0 general-LZ or tag-1 palette-LZ,
and each plan records the exact codec, token trace, and lookahead.
Component 0 is a text tilemap with four little-endian u16 entries per 2x2
metatile; its plan preserves the runtime transform mode. Mode 0 stores direct
little-endian entries, mode 1 splits high and low bytes into planes before an
XOR chain, and mode 2 applies the same previous-entry XOR to direct
little-endian words. The semantic decoder mirrors the byte-verified runtime
routine and the inverse encoder reproduces every compressed component exactly.
Component 1 is a JSON sequence of opaque four-byte descriptor records.
Component 3, absent in eighteen containers, is a JSON stream of animation
queues with their full FDxx headers, pairs of u16 command words, FE00 queue
terminators, and a final FFFF; container 225's stream deviates from that
grammar, so its source is an opaque u16 word list instead, and the builder
accepts either document shape. Component 4, absent in forty-five containers,
holds blend-register animation commands. Its semantic JSON distinguishes
single-word blend-control writes, value/duration pairs, pair-index jumps,
stream resets, and stops. Component 5 is a JSON sequence of three-byte sparse
records plus its FFFFFF terminator and zero alignment. All compressed plans
retain trailing lookahead bytes and reproduce their named component spans
exactly.
