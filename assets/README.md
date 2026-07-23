# Assets

Every entry in `manifest.json` names its exact ROM range and deterministic
encoding. `tools/build_assets.ts` refuses a region unless the tracked source
re-encodes byte-for-byte to the private ROM, and rejects tracked images that
are not inputs to that verified build.

`graphics/system/nintendo_logo.1bpp.png` is the canonical 104×16 monochrome
firmware-logo source required by every GBA cartridge header. Its encoder
recreates the tiled 1bpp bitmap, additive halfword transform, and fixed Huffman
field; typed metadata supplies the standard fixed, unit, device, and reserved
fields. These edition-independent sources own 168 header bytes. The entry
branch, title and product identity, software version, and dependent checksum
remain explicit edition-specific work rather than being copied into the logo.

`audio/song_table.json` reconstructs the sound engine's 312-entry selection
table. Each eight-byte entry names a symbolic sequence header, the player index
read by the ROM-side selector, and a mirrored second halfword whose role is not
yet proven. Repeated empty slots share one symbol instead of duplicating bytes.
The adjacent music-player records, sequence streams, tone banks, and wave data
are not claimed by this table.

`audio/midi/` and `audio/data/` reconstruct all 260 nonempty native sequence
units selected by the sound table. Each unit is a tracked Standard MIDI file
(`audio/midi/sound_NNN.mid`, the canonical core) plus a minimal deviation
sidecar (`audio/data/sound_NNN.json`) that records only where the ROM's byte
encoding departs from the codec's default running-status, note-parameter, and
wait-split rules; 163 units match the defaults exactly and carry no sidecar.
The pair retains exact engine duration codes, symbolic track and tone-bank
references, priority, reverb, loops, pattern calls, and command-status choices,
and rebuilds 150,254 byte-verified bytes, including absolute-address alignment
between streams and headers.

`audio/residuals/` reconstructs seven regions left outside the nonempty sequence
series: the shared four-byte empty header, four sound-table-selected eight-byte
empty SMSH headers, the 18-byte `yobi` (予備) command stream between sounds
138 and 139, and the 2,408-byte zero alignment ending at `0x08185000`. The
sources retain typed header fields and semantic sequence events rather than
copied binary spans, while the verifier proves the relevant sound-table links
and both neighbouring one-track headers. Together they rebuild 2,462 bytes
exactly.

`audio/engine/` reconstructs the complete 3,826-byte native audio-data prefix
at `0x080fb792..0x080fc684`: derived alignment, consumer-sized command and
pitch/volume tables, two tone banks with 225 typed records, eighteen 16-byte
CGB waveforms, and eight music-player records. The corrected waveform boundary
is `0x080fc504`; the preceding twelve bytes complete tone-bank record 81 rather
than belonging to an arbitrary configuration span.

`audio/waves/` reconstructs 32 tone-referenced signed-PCM records as
standard mono 8-bit WAV sources. Its index preserves exact fixed-point
frequencies and loop points, while the verified builder restores native signed
samples, 16-byte headers, and alignment for 404,160 ROM bytes. Five adjacent
tone-referenced Direct Sound records remain a separate 112-byte recovery unit;
the pointer-driven probe now preserves their exact headers and padding without
guessing their sample parameters.

`graphics/080c5b30.4bpp.png` contains seven palette-independent 4bpp tiles.
ROM code copies the seven consecutive 32-byte units to seven consecutive VRAM
tile slots. The PNG palette is an index legend only; it does not claim the
scene's active colors.

`graphics/0800777c.gbapal.png` and `graphics/08073812.gbapal.png` preserve
BGR555 colors copied directly from ROM into palette RAM by DMA. Their ranges
and lengths come from the DMA setup; no scene names are inferred.

`data/runtime_support.json` reconstructs the early runtime's hexadecimal and
decimal formatting tables, transfer-test identity and packet defaults, flash
device directory, four timing schedules, six typed device descriptors, and
the proven zero alignment ending at `0x08009000`. The adjacent seven-word ARM
dot-product kernel is reconstructed as instructions because its caller copies
it to the stack before execution. Four unreferenced address pools totaling 220
bytes remain fallback until their ownership is independently proven.

`data/byte_henkan/hyou.json` reconstructs nine 256-byte conversion tables as
fourteen explicit 16-byte rows followed by two derived zero rows. Five tables
are complete permutations of `0..223`; four are bounded mappings. The adjacent
caller-sized ARM modules remain structured assembly because the approved
compiler emits Thumb only, and the preceding fixed long-call veneer remains
linker reconstruction. Together these sources cover all 5,836 bytes at
`0x080092b0..0x0800a97c`.

`graphics/palettes/direct_palettes/` contains three 64-color resources selected by
fixed pointer-table IDs. Their call sites copy exactly `0x80` bytes to palette
RAM; names beyond the independently derived IDs are intentionally omitted.

`data/resource_directory.json` reconstructs the complete 1,000-slot resource
pointer directory. Semantic tokens distinguish the ROM base, the table's self
pointer, 971 unique resource targets, two aliases, and 25 reserved null slots;
the builder resolves those tokens back to the exact 4,000-byte table.

`text/message_archive.json` reconstructs the English localization archive as
10,723 message slots in 42 banks. Printable symbols are ordinary ASCII source;
consumer-proven controls use semantic commands for page and line breaks, timed
pauses, runtime names and numbers, grammar suffixes, articles, and button icons.
Command operands are grouped with their operation instead of masquerading as
text, while unproven control IDs remain numeric. The builder derives all 107
context-selected prefix trees from transition frequencies using stable
first-occurrence tie ordering, then derives every compressed message, length
table, absolute bank pointer, and zero padding. It verifies the complete
`0x08037464..0x08073808` package byte-for-byte. Six former assembly islands
inside that range were scanner false positives in the packed message streams.

`text/localization_font.json` reconstructs the localization renderer's font
package. Seven pointer/alias banks select 467 unique 16x16 MTF-coded glyph
images; eight palette-LZ records are coherent 32x32 symbols; the proportional
font stores 224 16x15 one-bit glyphs and their advance widths; and the article
table names its printable ASCII strings and pointer aliases. Direct 4bpp banks
remain atlas PNGs because their consumers address complete tile sequences.
Three intervening JSON sources retain the consumer-indexed numeric tables and
fixed ASCII labels with exact element types and strides. Together these eight
regions rebuild the complete 56,148-byte `0x08029910..0x08037464` localization
prefix. A four-byte scanner island inside the first package was packed glyph
data, not a function.

`graphics/fonts/namae_nyuuryoku/gamen.json` reconstructs the localized name-entry
screen package (名前入力) as four resource IDs, a 24×13 tilemap with explicit
24×9 consumer dimensions, palette and flip flags, its payload-free general-LZ
decisions, the UI tile-bank pointer, and structural alignment. The builder
derives the 13,972-byte section fill rather than storing it. The complete
`0x08073852..0x08077000` package rebuilds all 14,254 bytes exactly; cross-ROM
comparison was essential to avoid misclassifying its compressed stream as
executable code.

`data/resource_5_database.json` reconstructs the consumer-indexed gameplay
tables shared byte-for-byte by all six Golden Sun localizations: eight level
experience curves, inventory counters and party order, 324 item records, 519
abilities, 165 combatant templates, hero growth, summons, 203 classes,
elemental profiles, interpolation curves, and 80 Djinn. Typed records omit
reserved bytes and regenerate them as zero; the source also declares the final
2,516-byte resource alignment. The complete `0x0807a828..0x0808a000` range is
an exact build input, not a raw database slice.

`data/battle_effect_data/layout.json` reconstructs the complete coherent battle
effect data package as six object-tile sheets, 104 weighted result records,
consumer-indexed rule and lookup tables, a generated sine curve, 32 gradients,
symbolic pointers, and 29 typed object scripts. Field widths and signedness
follow the runtime consumers; selectors with no proven semantic name remain
numeric. The package rebuilds all 19,440 bytes at
`0x0809c410..0x080a1000` exactly, including structural alignment derived by the
builder rather than stored payload.

`data/sentou_menu/` reconstructs the battle-menu display package as five
consumer-framed 4bpp atlases, cell and row offsets, threshold and selector
tables, 35 fixed-stride loadout records, sound selectors, multipliers, and
derived structural alignment. Two following long-branch veneers remain
assembly because their fixed linker form is not representable in C. Together
the package and veneers cover all 5,840 bytes at
`0x080b3940..0x080b5010` without an arbitrary tile-sheet dump.

`code/sentou_kouka/` reconstructs the complete battle-effect runtime package:
fixed-point constants, consumer-bounded lookup tables, a symbolic 407-slot
callback directory, relocatable ARM composition fragments, the copied packed
decoder, and 4,012 bytes of derived structural fill. Null callback slots and
aliases are preserved explicitly. The package rebuilds all 10,204 bytes at
`0x080eda78..0x080f0254` without copied executable blobs.

`graphics/sentou_hyouji/` reconstructs the contiguous battle-display package
as consumer-indexed actor poses, action descriptors, party layouts, debug-party
presets, equipment and ability IDs, weapon classes, object curves, and two
frame-arranged 4bpp atlases. The atlas geometry preserves the runtime frame
boundaries instead of presenting the banks as arbitrary tile sheets. Together
the sources rebuild all 12,582 bytes at `0x080c2a0a..0x080c5b30` exactly.

`code/resource_3ce_` reconstructs the battle descriptor's final compressed
Thumb overlay as relocatable assembly and a payload-free general-LZ token plan.
The 1,945-byte stream expands to 5,772 bytes at `0x02000000`; the builder also
derives the final 11,076-byte zero-filled ROM tail. Three unconsumed alignment
bytes remain fallback rather than being hidden in the compression source, so
the package contributes 13,021 exact source bytes.

`data/encounter_data/` reconstructs the consumer-proven encounter database as
two signed brightness phases, 380 formations, 20 preload IDs, 172 metadata
records, and explicit zero alignment to the next code section. Reserved record
bytes are regenerated structurally. Three source regions rebuild 13,288 bytes
inside `0x080c5c10..0x080c9000`; the intervening eight bytes remain fallback
because no consumer proves their meaning.

Resource 2 preserves its twelve-digit numeric stamp as text plus an explicit
four-byte trailer. `graphics/resource_13_font.4bpp.png` is the complete 256-
glyph 8x8 UI font in character-code order. Resource 14 preserves 1,120
BGR555-compatible words in a neutral 16-column RGBA visualization without
claiming unproven palette rows. Resource 18 is the coherent 240x160 8bpp
Nintendo screen selected by `Func_080f2b70`; its indexed PNG carries the exact
224-color palette and a payload-free tag-1 compression plan.

`graphics/resource_15_` through `graphics/resource_17_` reconstruct the title
sequence's native graphics packages. Resource 15 separates the OBJ light,
sun, and rainbow pieces according to their runtime tile starts. Resource 16 is
the coherent 240x160 8bpp title raster. Resource 17 is a vertically streamed
240x344 4bpp sky (`sora`) followed by a 240x184 8bpp mountain (`iwa`). Their
256-color palettes live in the indexed PNGs, while payload-free flag/copy
plans restore the compressed streams. Resources 16 and 17 include structural
zero alignment; resource 15's final unproven nonzero byte remains fallback.

`graphics/resource_19_` is one compressed graphics package. Its tracked
sources are a BGR555 palette PNG, two 8bpp tile PNGs, a 32×32 text tilemap,
and a payload-free custom-LZ token plan. The plan records literal positions
and copy decisions; literal bytes come from the PNG/text components during
the build. Together they re-encode the complete compressed ROM stream.
A generated, non-source preview may render the four proven dynamic states after
applying the real palette, 32x32 tilemap, tile flips, and the runtime
`0x340`-byte dynamic upload. Such previews remain ignored build products.

`graphics/resource_01c_` reconstructs resource 1C as a 16-color glyph bank:
one 16-entry BGR555 palette followed by thirty-six 8×8 4bpp glyphs, stored as
one canonical 9×4 indexed PNG. A payload-free general-LZ token plan restores
the complete 692-byte stream at `0x0833ac08..0x0833aebc` exactly. `kana` is a
semantic reconstruction label, not a claim that the historical filename
survived.

`graphics/sentou/index.json` groups 57 independently decoded battle-resource
streams shared across both games. `sentou` (戦闘) is the period-style grouping;
each resource keeps one canonical `koma` (コマ), `haikei` (背景), or neutral
`naiyou` (内容) indexed image and, when present, its `iro` (色) palette. The
token plans contain compression decisions rather than literal payload copies.
Together the sources rebuild 135,743 bytes exactly. Eighty-seven trailing
compressor bytes remain fallback because they vary independently of the
semantic decoded content instead of being mislabeled as image data.

`graphics/Pre-rendered Backgrounds/` contains 34 descriptively named,
palette-correct indexed PNGs for resources 1D through 3E: each is a 128-color,
256×120 raster proven
by the palette DMA and the fixed `15×8×32×8` ARM decode loop. The LSB-first
delta7 predictor is continuous in raster order, wraps modulo 128, and has a
unique shortest-code inverse, so the PNG needs no opaque token-plan sidecar.
The encoder restores every meaningful bit and the zero padding through the
last halfword. Eighteen records have a separate two-byte alignment gap; those
36 bytes remain fallback rather than being disguised as image lookahead.
`index.json` preserves the resource IDs and separates visible scene descriptions
from independently proven in-game location names. The earlier `ichimaie`
(一枚絵) working name was not a claim that a historical filename survived.

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

`graphics/resources_d1_d3/` reconstructs the three directory entries directly
before the world-map package. D1 is the consumer-framed 64-color BG palette
and tag-1 compressed 8bpp tile bank loaded by `Func_080d41a4`; its 112x96 PNG
is a presentational 14x12 tile wrap, while linear tile order remains canonical.
D2 and D3 are big-endian initial coordinate pairs followed by signed-byte XY
motion deltas consumed by `Func_080dc968` and `Func_080e89ec`. D2 preserves one
additional boundary delta as a named semantic record. The package rebuilds
4,769 bytes; D1's three nonsemantic suffix bytes remain explicit fallback.

`maps/world_map/` reconstructs resources D4-D7 as one independently traced map
subsystem. D4 is an offset-table archive of 640 compressed 16x16 chunks. Each
RGBA atlas pixel stores one little-endian 16-bit tile-pair index and one
little-endian 16-bit flag field; the low values address all 4,386 records in
D5 exactly. D5's RGBA pixels store the two little-endian 16-bit tilemap entries
in each record. D6 is a terminated 16-bit animation command stream represented
as text. D7 contains two banks of 512 four-byte terrain-property records. The
PNG row wrapping is presentational only; the manifest preserves linear order.
All four compression plans contain token choices but no literal payload bytes.

`graphics/resource_f1_` contains 49 independently compressed 32x32 8-bit
indexed graphics blocks in one 7x7 PNG atlas. Multiple consumers decode one
selected `0x400`-byte block; one path remaps its byte indices through palette
RAM and uploads the result to VRAM. The archive uses a 16-bit offset table and
32-byte-aligned compressed slots. Its final two table-range bytes are outside
the proven archive boundary and remain ROM fallback bytes.

`graphics/palettes/palette_animation_e8.rgba.png` through
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

`graphics/resource_e7_` reconstructs one compressed UI-graphics package as
five 16-color BGR555 palette banks and 256 4bpp tiles. Pixel-edge continuity
fixes the native tile order at four tiles wide: every consecutive group of
sixteen tiles is one coherent 32x32 frame, yielding sixteen frames and five
color variants. Non-source previews generated from those tracked sources remain
ignored build products.

`graphics/resource_f0_images_` contains 57 unique, palette-correct 32x32 4bpp
images. The ROM's 80-entry 16-bit offset table contains 23 null slots and one
offset for each image, represented by `archive.json`. Each package contains a
16-color palette followed by a terminating LSB-first run/move-to-front pixel
stream and zero alignment. The recovered encoder is canonical and rebuilds the
entire `0x592c`-byte archive exactly. `preview.atlas.png` is a generated
non-source contact sheet of the same PNGs.

`graphics/resource_f2_` through `graphics/resource_127_` contain 54
palette-correct indexed sprite archives. The runtime ARM decoder proves byte
values `1..0xdf` are pixel indices, `0xe0..0xff` skip 1..32 transparent
pixels, and zero terminates a frame. All 32x32 and 64x64 streams land exactly
on their metadata-defined pixel boundary and greedily re-encode byte-for-byte.
The shared 224-color OBJ palette is the independently reconstructed range
copied from ROM `0x0800779c`; each `preview.atlas.png` is a generated contact
sheet and remains ignored.

`data/chr_catalog.json` reconstructs the 512-record character descriptor table
and its adjacent animation archive. The 367 active descriptors name dimensions,
scale, draw kind, anchors, frame codec, and symbolic frame/animation tables;
the other 145 records are implicit zero entries. The 7,804 animation commands
use semantic operations and 1,997 symbolic labels, preserving pointer aliases
without embedding pointer bytes. The catalog and first three character banks
form one exact 139,260-byte runtime unit.

`graphics/characters/chr_0818_`, `graphics/characters/chr_081a_`, `graphics/characters/chr_081e_`,
`graphics/characters/chr_081f_`, `graphics/characters/chr_zenhan/`, `graphics/characters/chr/`,
`graphics/characters/chr_0828_`, `graphics/characters/chr_082b_`, and `graphics/characters/chr_0830_`
reconstruct 3, 22, 10, 17, 18, 35, 36, 105, and 64 descriptor-linked static
character banks as per-frame palette-correct `koma` (コマ) PNGs, one
`koma_NNN.png` (zero-padded to three digits) per unique frame beside each
bank's `bank.json`, sized from the bank header. Each family directory keeps
only its `index.json`; the descriptor-linked banks themselves are stored by
role, large battle-animation banks under `graphics/battle/characters/chr_NNN/`
and small field walk-sprite banks under `graphics/field/characters/chr_NNN/`,
resolved back to their series by the index plan name. Banks whose sprite is
referenced by exactly one enemy in the traced combatant table (ROM
`0x08080ec8`) are renamed to that enemy's shipped, kebab-cased name (for
example `battle/characters/bandit/`, `field/characters/fenrir/`), with the plan
string in the owning `index.json` updated to match; `data/enemy_sprites.json`
is the evidence register mapping every combatant record to its name, sprite id,
and directory. The 310 banks (279
battle, 31 field) hold 5,727 unique frames across as many files; the codec
derives each frame's filename from its index, so no atlas geometry is stored. Their payload-free plans
preserve logical pointer aliases and all three
descriptor-selected storage modes. Mode 0 is a canonical zero-skip stream;
mode 1 preserves either the kind-zero bitstream tokens or kind-one flag-group
tokens; mode 3 adds a little-endian literal/control split, LSB-first flags, and
back-references into earlier encoded bytes of the same bank around that
zero-skip stream. Zero alignment is explicit, and the builder emits the
trailing absolute frame directories and null terminators. The complete
`0x0818d554..0x081a7020`, `0x081a7020..0x081e120c`,
`0x081e120c..0x081f60c8`,
`0x081f60c8..0x08220160`, `0x08220160..0x08244fc0`,
`0x08244fc0..0x0828cfd8`, `0x0828cfd8..0x082b5060`,
`0x082b5060..0x08300f74`, and `0x08300f74..0x08320000` series re-encode
1,649,324 bytes byte-for-byte. The middle split records a four-byte zero suffix,
the `chr` series records its eight-byte prefix, and the final 4,136 zero bytes
are explicit arena alignment.
`zenhan` (前半) is a period-style reconstruction grouping. Numeric grouping
and bank names remain because no historical character names have been
recovered.

`graphics/map/resource_128_` through `graphics/map/resource_369_` contain the 121 map
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
assigned to a bank. `resource_152_objects/ougonmon.png` (黄金門) and
`resource_190_objects/iwamuro.png` (岩室) are period-style semantic sources;
their remaining unclaimed slots stay in neutral fallback atlases.

`maps/resource_128_grid_` through `maps/resource_369_grid_` reconstruct all
174 traced containers' tagged kind-1 map components as 128x128 spatial record
grids; 53 containers have no palette or tile-bank resources of their own and
share a neighbouring family's graphics.
Four grayscale PNG planes preserve the transformed 16-bit metatile/flag value
and two independently accessed attribute bytes. A one-bit PNG records which
pre-transform values were sentinels, avoiding the transform's otherwise
ambiguous inverse. The kind-1 token trace plus these five source planes rebuild
every compressed component and its alignment bytes exactly.

`maps/resource_128_components/` through `maps/resource_369_components/`
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

`maps/tokushu/` reconstructs six complete map containers that were absent from
the original traced family list. The same strict header, metatile, descriptor,
kind-1 grid, animation, blend, and sparse-cell codecs rebuild resources 1F3,
1F6, 28C, 2D4, 307, and 331 without fallback bytes. Thirty indexed 128x128
planes are active spatial sources; the remaining JSON and tilemaps preserve
consumer-shaped records and payload-free compression decisions. Together the
six containers contribute 39,756 exact source bytes.

`maps/chiiki/` reconstructs complete map containers 1E5 and 320 using the same
consumer-shaped codecs as the main map series: headers, metatile tilemaps,
descriptor records, five-plane 128×128 spatial grids, animation queues,
optional blend commands, and sparse cells. Payload-free compression plans
rebuild 6,188 bytes at `0x085bb860` and 8,256 bytes at `0x08753b18`, for
14,444 exact source bytes. `chiiki` (地域) is a period-style reconstruction
grouping, not a recovered historical filename.

Three manual entrypoints support asset maintenance and presentation:
`bun tools/split_sprite_bank.ts` splits an atlas-shaped bank into per-frame
PNGs during asset work, `bun tools/rewrap_tiles.ts` losslessly renormalizes a
sequential tile-bank PNG to the 32-column authoring wrap and verifies the
tile stream is unchanged, and a one-shot script (now removed)
regenerates the README's tracked Djinn idle animations from recovered
graphics sources. None of them is a canonical build stage.
