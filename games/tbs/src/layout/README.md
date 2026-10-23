# Layouts

![A layout is one square grid of 128 by 128 cells, each 16 pixels square. Each of its three layers is a rectangle cut from that grid, and the layout's header says where each rectangle starts. In Goma Cave's entrance room the ground layer starts at cell 0, 0 and holds the cave floor and its pool; the middle layer starts at cell 0, 34, below it, and holds the rock walls; the overhead layer starts at cell 50, 0, to the right, and holds only the glow of the torches. The rest of the grid is unused.](https://alchemy.pascalpixel.com/illustrations/map/grid-windows.svg)

A town or dungeon [layout](../../glossary.md#layout) is one big square grid of 16-pixel [cells](../../glossary.md#cell), 128 on
a side, and each of its three layers is a rectangle cut from that grid; the
layout's header says where each one starts, and the rest of the grid goes
unused.

The layout data is kept as editable tables: 182 layouts for the towns,
dungeons and interiors, the world map's ground and default cells, and the
opponents' recorded paths in the Colosso finals. There is no code here. Each
[map](../../glossary.md#map)'s row of `gField_MapTable` ([`field/maps.s`](../field/maps.s))
gives a layout number, which picks a row of `gLayouts`
([`field/layouts.tsv`](../field/layouts.tsv)): the layout's header, palette,
three tile sheets and layer data. `MapGrid_LoadLayout`
([`field/field/map_grid.c`](../field/field/map_grid.c)) loads it, and
[Field](../field/README.md) describes how the engine draws and scrolls it.

## Cells

A cell is one 16 by 16 pixel square of a layout, a 32-bit word once
loaded, which says what is drawn there, what the ground is shaped like and
what standing on it does.

![The ground seen from the side, with the leader standing on it. A band around the leader's feet reaches 8 pixels up and 12 pixels down. Ground ahead whose height falls inside the band can be stepped onto: a ledge 8 pixels up and a dip 8 pixels down. Ground outside it blocks the leader as a wall would: a rim 32 pixels high, like the ones at the foot of Goma Cave's cliffs, and a drop of 16 pixels. Each cell is 16 pixels wide.](https://alchemy.pascalpixel.com/illustrations/map/step-heights.svg)

Every cell has a height, and a height can stop the leader just as a wall
mark does: the leader can step up at most 8 pixels and down at most 12, so a
sharp rise or drop blocks the way. A cell's word says how its ground is
shaped as well as what is drawn there and what stepping on it does.

A loaded cell is one word, four bytes. Its first two bytes (a halfword) hold
the [metatile](../../glossary.md#metatile) number in their low 12 bits and four flag bits above, the layer and priority an object standing
there takes; its third byte is the [metatile behavior](../../glossary.md#metatile-behavior), and its fourth the
shape's number in the layout's shape table.

A shape is four bytes, a row of `shapes.tsv`. The low four bits of the first
pick one of the 16 terrain kinds (flat ground, slopes, steps, ground shaped
through three heights and others), and the other three are height samples in
steps of 8 pixels, which the kind's height function interpolates
(`MapGrid_GetTerrainHeight`, through `gMapGrid_TerrainHeightFunctions`).

The metatile behavior is what walls and [map events](../../glossary.md#map-event) are keyed on. `MB_WALL`
blocks movement whatever the heights. The behaviors from `MB_EVENT_FIRST`
to `MB_EVENT_LAST` belong to the map: a [coord event](../../glossary.md#coord-event) fires when the leader
steps onto or pushes against a cell with its behavior, and a [bg event](../../glossary.md#bg-event) when
the player presses A facing one. The behaviors above them mean the same on
every map: `MB_ENCOUNTER_GROUND_1` and `_2` for ground with encounters
of its own, `MB_JAR` to `MB_WELL` for search spots with shared
lines, `MB_DESERT_SAND` for the Lamakan Desert's sand, and `MB_LEDGE`,
`MB_CLIMB` and `MB_JUMP_FORWARD`, which `Field_RunPushTileEvent`
acts on when the leader pushes against them.

A wall need not be a behavior. `MapGrid_CheckObjectStep` lets an object step at most
8 pixels up or 12 down, so ground that rises sharply stops the leader as
surely as `MB_WALL`. Goma Cave has no `MB_WALL` cells at all: a rim of
ground 32 pixels high runs along the foot of each cliff, some of it cut along
a cell's diagonal.

Behavior and height belong to a layer: code that asks about the ground
reads the cell in the window of the layer the object follows, so an object
stands in one layer at a time ([Field](../field/README.md#how-high-a-step-can-go)).

A cell is `struct MapGridCell` in [`map_grid.h`](../../include/map_grid.h), and its
behavior an `enum MetatileBehavior`.

| Bits of the cell word | Field | Meaning |
| --- | --- | --- |
| 0 to 11 | `metatileAndFlags` | the metatile drawn in the cell |
| 12 to 15 | `metatileAndFlags` | flags: the layer and priority an object standing there takes (15 and 14 read by `MapGrid_GetCellHighFlags`) |
| 16 to 23 | `collisionCode` | the metatile behavior |
| 24 to 31 | `attributeB` | the shape, a row of the layout's shape table |

| Name | Value | Meaning |
| --- | --- | --- |
| (open ground) | 0 | nothing happens |
| `MB_EVENT_FIRST` to `MB_EVENT_LAST` | 1 to 239 | the map's own coord events, [warps](../../glossary.md#warp) and bg events |
| `MB_ENCOUNTER_GROUND_1`, `_2` | 240, 241 | ground with encounters of its own: grounds 1 and 2 of the map's zones |
| `MB_JAR` | 242 | a search spot with lines of its own |
| `MB_BARREL` | 243 | a search spot with lines of its own |
| `MB_WOODEN_BOX` | 244 | a search spot with lines of its own |
| `MB_GRAVE` | 245 | a search spot with lines of its own |
| `MB_BOOKS` | 246 | a search spot with lines of its own |
| `MB_WELL` | 247 | a search spot with lines of its own |
| `MB_JUMP_FORWARD_UNREACHED` | 249 | jumps forward like `MB_JUMP_FORWARD`, but never reaches `Field_RunPushTileEvent` |
| `MB_DESERT_SAND` | 250 | ground that hurts the [party](../../glossary.md#the-party) over time: the Lamakan Desert's sand |
| `MB_LEDGE` | 252 | jumped down with Down held |
| `MB_CLIMB` | 253 | climbed up or down with Up or Down held |
| `MB_JUMP_FORWARD` | 254 | jumped across |
| `MB_WALL` | 255 | blocks whatever the heights |

## A layout's tables

Everything that makes one town or dungeon layout, from the pictures of its
squares to where its chests stand, is kept as a handful of plain tables in
the layout's own folder. Each table holds one part of the layout, and the
layout's `layout.s` puts the parts together behind its header.

| File | What it is | Built as |
| --- | --- | --- |
| `layout.s` | the header, three layers and six offsets, then the parts; in 129 layouts a tile set's palette and tile sheets follow (from `games/tbs/src/graphics/tileset`) | assembled |
| `metatiles.tsv` | the metatiles; a metatile's number is its row, from 0 | `metatiles.delta0.lz`, `.delta1.lz` or `.delta2.lz` |
| `shapes.tsv` | the shapes | `shapes.table.lz` |
| `grid.tsv` | the three planes, in order | `grid.parts.lz` |
| `grid_collision.tsv`, `grid_shape.tsv`, `grid_metatile.tsv` | each cell's metatile behavior, shape and metatile | parts of the grid |
| `animation.tsv` | the tile animation script | `animation.script.lz` |
| `blend.tsv` | the blend [script](../../glossary.md#script) | `blend.script.lz` |
| `markers.tsv` | the markers | `markers.table` |
| `*_ja.tsv`, `*_de.tsv`, `*_es.tsv` and so on | an [edition](../../glossary.md#editions)'s own version of a table | chosen in `layout.s` |
| `*_patch.tsv` | bytes [Camelot](../../glossary.md#camelot) changed after packing | read by the packer |

A plane's first line describes it: `cell:u8	grid:128x128	fill:0	at:2,3`
gives the size of each value, the grid, the value of every cell the table
leaves out, and the cell its first row starts at, column then row. The rows
are hexadecimal, one row of the grid per line.

### The header

In the north of Suhalla Desert a haze of sand streams across the screen
faster than the ground moves, and wraps round as it goes. The layout's header is
what sets that up: for each layer it says how fast the layer follows the
camera, whether it drifts by itself and where it wraps, as well as how far
the camera may go.

![In the north of Suhalla Desert, when the camera moves 16 pixels, the ground layer and the middle layer move 16 pixels with it, but the overhead layer, the sand haze, moves 20, a quarter faster. The haze also drifts by itself every frame, even while the camera stands still. Its picture is a square 256 pixels on a side, repeated across the screen: the layer wraps round. On screen, once a storm has risen and faded the haze in, it streams past Isaac while he stands still; on its own, the haze is a scatter of sand grains on BG1. The layout's header sets all of this for each layer, along with how far the camera may go.](https://alchemy.pascalpixel.com/illustrations/map/layer-scroll.svg)

A layer that follows the camera at the camera's own pace looks as far away
as the ground; one that moves faster looks nearer. Suhalla Desert's north
(`layout_147`) moves its ground and middle layers with the camera and its
overhead haze a quarter faster, drifts the haze by -121 and 35 sixteenths of
a pixel a [frame](../../glossary.md#frame), and wraps it every 32 tiles, 256 pixels.

The header is one `layout_header` line and three `layout_layer` lines in `layout.s`,
read as `struct LayoutHeader`. It gives the camera's bounds in 8-pixel
tiles, each layer's priority (0 leaves the layer off) and the char block its
tiles come from, and for each layer its origin in 8-pixel tiles (the
picture at the top of this page), its parallax (in sixteenths of the view's
motion), its drift (sixteenths of a pixel a frame) and its wrap.

Six offsets from the header's start follow: the metatiles, the shapes, the cell
grid, the tile animation, the blend script and the markers. An offset of 0
means the layout has none of that part.

`struct LayoutHeader` and `struct LayoutLayer` are defined in
`field/field/map_grid.c`, and the `layout_header` and `layout_layer` macros that
write them in `layout.inc`.

| Field | Unit | Meaning |
| --- | --- | --- |
| `origin[4]` | 8-pixel tiles | the camera's bounds: left, top, right and bottom |
| `priority[3]` | | each layer's BG priority, 1 to 3; 0 leaves the layer off |
| `charBase[3]` | | each layer's BG char block |
| `reserved` | | 0 in every layout; nothing reads it |
| `layers[n].x`, `.y` | 8-pixel tiles | the layer's origin |
| `layers[n].scrollX`, `.scrollY` | sixteenths of the view's motion | the layer's parallax |
| `layers[n].speedX`, `.speedY` | sixteenths of a pixel a frame | the layer's drift |
| `layers[n].periodX`, `.periodY` | 8-pixel tiles, less one | where the layer wraps, as a mask: `0x1f` wraps every 32 tiles |
| `tiles` | bytes from the header | the metatiles |
| `collision` | bytes from the header | the shapes |
| `tilemap` | bytes from the header | the cell grid |
| `animation` | bytes from the header | the tile animation script, or 0 for none |
| `blend` | bytes from the header | the blend script, or 0 for none |
| `markers` | bytes from the header | the marker list, read in place |

## How the build makes a layout

The tables are for people to read and edit; the game wants packed bytes. The
build turns each table into the bytes the game reads and packs them exactly as
Camelot's packer did, so the result is the same, byte for byte, as the
cartridge.

![How the build turns one of a layout's tables into what the game reads, using Suhalla Desert's shapes as the example. The file shapes.tsv is a table anyone can edit, one shape a row: its kind and three heights. The build tool writes the bytes the game reads, shapes.table, one record a row. Then it packs them, shapes.table.lz, exactly as Camelot's packer did, so the packed bytes are the same as on the cartridge. Last, the layout's layout.s places them behind its header. The name of each file says what to make: the table's name, then its form, then the packing.](https://alchemy.pascalpixel.com/illustrations/map/build-steps.svg)

The name of the file the build asks for says what to make:
`NAME.FORM[.CODEC]`, from `NAME.tsv`. `tools/ags-graphics` makes it (the
`Makefile`'s asset rules), and a layout uses these forms:

- `table`: the shapes and markers, one record per row, each column's type
  giving its size.
- `plane`: a grid plane, written out whole, row by row, with `fill` in every
  cell the table leaves out.
- `parts`: the grid, the encodings of the files it lists one after another.
- `delta0`, `delta1`, `delta2`: the metatiles, halfwords behind a method byte
  (`enum MetatilePacking`) that `TileMap_DecodeStagedBuffer` undoes. Method 0
  stores them as they are, 2 XORs each with the one before, and 1 does the
  same with all the high bytes stored before all the low bytes. Each
  `layout.s` names the method its stream uses.
- `script`: the tile animation and blend scripts below.

The codec `lz` then packs each stream, and the bytes must come out identical.
Where [the original](../../glossary.md#the-original) stream is followed by stray bytes the packer left in its
buffer, `layout.s` places them as literal bytes with a `PACKER:` comment naming
what wrote them. Where Camelot changed a stream after packing it, a
`_patch.tsv` lists each changed byte: its position, the byte the packer saw,
the byte that shipped, and the proof. The table holds what shipped, and the
patch gives the packer the byte it packed, so the packer makes the same
choices and the stream still decodes to the edited layout. Only Gondowan
Passage's grid and Venus Lighthouse's entrance's grid, with its Spanish and
French versions, have patches.

An edition's own version of a table (`grid_es.tsv`, `markers_ja.tsv`) is
chosen in `layout.s` with `.ifdef TBS_EDITION_*`, and editions that agree share
one file: Venus Lighthouse's entrance has a French grid and a Spanish one, and
the Italian edition uses the Spanish. A layout whose whole data differs by
edition has a block of its `layout.s` for each. A layout that uses another
layout's table names that layout's file.

`enum MetatilePacking` is in `field/field_internal.h`.

| Name | Value | Form | Meaning |
| --- | --- | --- | --- |
| `METATILE_PACKING_RAW` | 0 | `delta0` | the metatiles as they are |
| `METATILE_PACKING_SPLIT_DELTA` | 1 | `delta1` | all the high bytes, then all the low bytes, each value XORed with the one before |
| `METATILE_PACKING_DELTA` | 2 | `delta2` | each value XORed with the one before |

## Loading a layout

When the party enters a town or dungeon, the map's layers and [sprites](../../glossary.md#sprite-and-oam) are
hidden while its layout is unpacked into working memory. Most of what is
unpacked is the grid, which is stored in three separate pieces and put together into one
word per cell as it loads, so that everything about a cell can be read in
one go.

![A layout's grid is stored as three whole planes, one after another: what stepping on each cell does (a byte a cell, 16 KB), the shape of each cell's ground (a byte a cell, 16 KB) and the picture drawn in each cell, with the layer an object standing there takes (two bytes a cell, 32 KB). As the layout loads, a routine copied into RAM takes one value from each plane for every cell and puts them together into one four-byte word: the shape at the top, then what stepping does, then the picture. The 128 by 128 words take 64 KB.](https://alchemy.pascalpixel.com/illustrations/map/grid-weave.svg)

`MapGrid_LoadLayout` decodes each part into a fixed buffer in [EWRAM](../../glossary.md#iwram-and-ewram), the
console's 256 KB of work memory: the
metatiles into `gMetatiles`, the shapes into `gMapGridCollision`, the grid into
`Ram_MapGridCellBuffer`, and the two scripts into the space after the shapes. The
markers are not decoded: the loader keeps a pointer to them in the map grid work.

The grid is stored as three whole planes: metatile behaviors, then shapes,
then metatiles (16 KB, 16 KB and 32 KB, since the metatile plane has a
halfword per cell). `TileMap_ConvertBuffer` then runs `Tile_ConvertMapGrid`, a routine in the
processor's fast 32-bit ARM instructions copied into RAM, which weaves them into 128 by 128 cell words, 64 KB from the
start of `Ram_MapGridCellBuffer`, and numbers the placeholder metatiles as it goes.
The first two planes are saved to a temporary buffer first, since the cell
words overwrite them.

`gMapGridCellBuffer` itself is 32 KB, so the cells run on through
`gActorSpriteSlots` and `gDecodeBuffer`, and neither can hold anything else
while a layout is loaded. Only code that reloads the cells afterwards writes
there: a battle, after which the map's layout is loaded again; the link lobby,
which leaves the partner's party in `gActorSpriteSlots` for the link battle
that follows; and the world map's still view, which saves `BG1`'s tiles to
`gDecodeBuffer` and draws into the cells, which
`WorldMap_LoadDefaultCellsAndUpdateBlock` then puts back. The debug sprite viewer
also loads sprites there. The tile animation only reads from
`Ram_DecodeBuffer`, at offsets that land past it. [System](../system/README.md) has the
EWRAM layout.

| Plane, in stored order | Size | Values |
| --- | --- | --- |
| metatile behaviors | 16 KB | a byte a cell |
| shapes | 16 KB | a byte a cell |
| metatiles | 32 KB | a halfword a cell |
| the woven cell words | 64 KB | a word a cell, from the start of `Ram_MapGridCellBuffer` |

### One grid, three layers

All three layers are windows onto the one grid, as the picture at the top of
this page shows for Goma Cave. Each layer's origin in the header picks the
cell its window starts at (half its tile position, since a cell is 2 by 2
tiles), so a layout lays its layers out side by side or one above
another: `layout_000`'s start at tiles 0, 60 and 120 across, and Venus Lighthouse's
entrance puts them at (0, 0), (64, 0) and (0, 68).

The ground layer shows on `BG3`, the middle layer on `BG2` and the overhead
layer on `BG1`; [Field](../field/README.md#towns-and-dungeons-on-screen) explains
how their priorities put [object events](../../glossary.md#object-event) between them.

### Placeholder metatiles

A layout's grid doesn't store the number of every cell's picture from the
start. The first time a picture appears in the grid, the cell holds a
placeholder instead, and the game hands out metatile numbers in order as it
loads the layout.

![Eight cells of a layout's grid, in the order the game reads them, showing grass, grass, water, grass, rock, water, rock and grass. Where a picture appears for the first time, the grid stores a placeholder, the number 4,095, and as the layout loads the game gives it the next metatile number: the first grass becomes metatile 0, the first water metatile 1, the first rock metatile 2. Every later cell with the same picture already stores its number. So a layout has as many placeholders as it has metatiles.](https://alchemy.pascalpixel.com/illustrations/map/placeholders.svg)

A placeholder is a cell whose metatile is 4,095, the highest 12 bits can
hold. As the layout is converted, `Tile_ConvertMapGrid` gives each placeholder
a fresh metatile number, counting up from 0 in grid order. The ROM also
keeps an unused C version of that numbering, `TileMap_NumberPlaceholderMetatiles`.

Each layout's metatile table is ordered by where its metatiles first appear in
the grid, and the grid writes that first appearance as 4,095 and every later
one by number, so a layout has exactly as many placeholders as metatiles
(`layout_003` has 1,331 of each). Every layout follows the rule but Venus
Lighthouse's entrance, whose grid names 170 cells' metatiles before their
placeholders; those cells still get the right metatiles, since the numbering
depends only on the placeholders.

### Chests and Psynergy Stones

Several maps can share one layout, and each puts its own chests in it. The
layout lists the spots where a chest or a [Psynergy](../../glossary.md#psynergy-and-pp) Stone can stand, each with
the event that would place one there, and a map places one only where it has
that event.

![Two maps on Crossbone Isle, CrossboneIsleKeys and CrossboneIsleBane, use the same layout, and the layout lists eight spots where a chest can stand, each with the event that would place one there. CrossboneIsleKeys has events for four of them, in a row along the room's north side: 333 coins, a Fairy Ring, a Cookie and a Smoke Bomb. CrossboneIsleBane has events for the other four: 666 coins and an Elixir on its raised floor, a Water of Life below them, and the Demon Axe, whose chest is carried on a stone pillar. A spot a map has no event for stays empty.](https://alchemy.pascalpixel.com/illustrations/map/markers.svg)

CrossboneIsleKeys and CrossboneIsleBane share `layout_110`, whose eight markers
carry the [triggers](../../glossary.md#trigger) 100 to 107: CrossboneIsleKeys has chest events for 100 to
103, and CrossboneIsleBane for 104 to 107 (`map/crossbone_isle_keys.c`).
CrossboneIsleBane moves the Demon Axe's chest to 0, 0 until the stone pillar
carrying it reaches the ladder, since its marker's cell lies past the room's
east wall.

The marker list is (column, row, id) triples, a cell and the trigger of a
map event, ended by 255, 255. When a map loads, `Field_PlaceMapMarkers`
([`object/object/markers.c`](../object/object/markers.c)) places an
object at each marker whose id (100 to 239) the map has an event for, up to
`MARKER_MAX` (10): a chest for a chest event, shown open once its flag is set
and gone once its mimic has been beaten, and a Psynergy Stone for a search
event that gives one, until it is taken. A marker the map has no event for
takes no slot. 76 layouts place `layout_000/markers.table`, which holds only the end
marker.

| Name | Value | Meaning |
| --- | --- | --- |
| a marker's id | 100 to 239 | the trigger of the map event that places it |
| the end marker | column 255, row 255 | ends the list |
| `MARKER_MAX` | 10 | markers placed on a map (`include/unit/object/object.h`) |

### Tile animation and blend scripts

Some parts of a map move by themselves without anything on the map moving:
the game simply swaps the pictures of a few tiles on a timer. A
layout's tile animation script says which pictures to copy over which tiles and
how long to wait between them; its blend script changes, in the same way, how
the layers mix.

![One channel of a layout's tile animation. Several pictures of the same patch of the map are stored apart from the tiles on screen. The channel copies the first over the patch's tiles, waits a few frames, copies the second, waits, and so on, then goes back to the first. Nothing else changes: every cell that uses those tiles shows the new picture at once. A layout can run up to 16 channels, each on its own timer. On screen, Vale's waterfall and river as the game plays them, several channels changing their tiles at once.](https://alchemy.pascalpixel.com/illustrations/map/tile-animation.svg)

Suhalla Desert's north shows the usual shape: its channel 0 copies four rows
of four tiles from one of four stored pictures, waits 15 frames, takes the
next picture, and jumps back to the first after the fourth.

Both scripts are runs of halfwords, run each frame as [tasks](../../glossary.md#task)
([`field/field/tile_animation.c`](../field/field/tile_animation.c)). In the
tile animation's table, `channel n` (`0xfd00 + n`) starts channel n of the 16 at the next halfword; only the low
four bits pick the channel, and a marker with bit 7 set starts it paused.
Each `frame` is four values: the source tile, how many tiles, the destination
tile and the frames to wait. `jump n` (`0xfe00 + n`) goes to halfword 2n of
the channel's script, `stop` (`0xfeff`) halts the channel, and `end`
(`0xffff`) loops it to its start. A source tile from 1,536 on is copied from
RAM at `Ram_DecodeBuffer` plus its offset, which lands in `gMapGridLayerData`,
and a lower one from elsewhere in video memory; the line is 512 for a layout
whose animations copy 64-byte tiles (`MapGridState.wideTiles`).

The blend script changes how the layers blend over time. `control` sets
`REG_BLDCNT`; `level` sets `REG_BLDALPHA`, or `REG_BLDY` when the control's
mode is not alpha blending, and waits its second value in frames. `jump`,
`stop` and `end` act as in the tile animation.

The words are tested in `TileAnimation_Update` and `DisplayBlend_RunScript`,
which have no names for them; the names are the tables' (`animation.tsv`,
`blend.tsv`).

| Word | Value | Meaning |
| --- | --- | --- |
| `channel n` | `0xfd00 + n` | the tile animation's channel n starts at the next halfword |
| `frame` | four values below `0xfd00` | source tile, tile count, destination tile, frames to wait |
| `jump n` | `0xfe00 + n` | continue at halfword 2n of the channel's script |
| `stop` | `0xfeff` | halt the channel |
| `end` | `0xffff` | loop back to the channel's start |
| a source tile from RAM | 1,536 on (512 for 64-byte tiles) | copied from `Ram_DecodeBuffer` plus its offset, rather than from video memory |

## The world map's data

The world map is far too big to unpack at once, so its ground is cut into
squares that are packed one by one, and the game unpacks only the few around
the party, bringing in new ones as the party walks.

![The world map's ground is a grid of 16 by 16 squares, each 512 pixels on a side, and each square is packed on its own, once for each of the world map's two ground layers. Only the 2 by 2 squares around the party are unpacked at any time; the rest stay packed. As the party walks half a square further, the 2 by 2 moves with it, and the squares it now covers are unpacked, one a frame.](https://alchemy.pascalpixel.com/illustrations/map/world-squares.svg)

The world is 16 by 16 squares of 512 pixels, and a table in the world map's
work (`PerspectiveWork.tileIds`) says which stored square shows at each of
them. `WorldMap_UpdateCurrentTileBlock` unpacks the 2 by 2 squares whose top left
holds the point 256 pixels left of the party and 320 above, in each of the
two ground layers (`WorldMap_WriteLayerCellTile`), and
`WorldMap_PreloadCurrentTileBlock` brings in at most one new square a frame as the
party walks.

The ground's data, `world/blocks.tsv` (`Field_PerspectiveDataA`), is one grid
of 32-bit values in hexadecimal, 512 across and 320 down. The `blocks` form
cuts it into 640 squares of 16 by 16 values and packs each separately behind
a table of offsets; a square written all as `-` would be left out with an
offset of 0, and the world map leaves none out. The second ground layer's
squares are numbered 320 on from the first's.

The tables in `default/` are the world map's too.
`WorldMap_InitializePerspective` loads the default cells, block attributes and
tile animation when the world map starts, and
`WorldMap_LoadDefaultCellsAndUpdateBlock` puts the cells back after the world map's
still view ([Field](../field/README.md#the-world-map-in-perspective)).

| Part | Size |
| --- | --- |
| the world | 16 by 16 squares of 512 pixels |
| the ground's grid | 512 by 320 values, 32 bits each |
| a square | 16 by 16 values |
| squares | 640, each packed on its own, 320 for each ground layer |
| unpacked at once | the 2 by 2 squares around the party, in each of two ground layers |

## The Colosso paths

In each Colosso final Isaac races an opponent through an obstacle course. The
opponent isn't steered by any rule: it replays a run that was recorded once,
one point of ground a frame.

![In each of the three Colosso finals, the opponent racing Isaac replays a run recorded as one point of ground a frame, drawn here from the three tables at one scale, seen from above. Azart's run is 3,918 points, about 65 seconds; Satrage's is 4,380, about 73 seconds, and runs the other way; Navampa's is 5,048, about 84 seconds. The game can also play a run mirrored across the finish line. A dot marks every 300 frames, about five seconds. Each frame the opponent steps to the next point, turning by at most a sixteenth of a turn, and when the points run out it walks to the finish.](https://alchemy.pascalpixel.com/illustrations/map/colosso-path.svg)

`colosso/` holds the three runs, each 7,200 points of ground in pixels, the
size of the race's path buffer (`ColossoRaceWork.path`), with a row of two
zeros where the run ends. The tables call the columns x and y; the race code
reads each pair as x and z. The Azart match runs path A, the Satrage match
path B and the Navampa match path C (`Colosso_StartOpponent`,
`ColosseumSatrageMatch_StartOpponentPath`).

Each frame `Colosso_UpdatePathOpponent` steps the opponent to the next point,
mirrored across the finish when asked, turning at most a sixteenth of a turn,
and at the end walks it to the finish. Its recording mode writes down where
an object event stands every frame and then plays it back with the opponent. The
Colosso lettering, which differs by edition, is placed after the paths.

| Table | Match | Points |
| --- | --- | --- |
| `opponent_path_a.tsv` | Azart | 3,918 |
| `opponent_path_b.tsv` | Satrage | 4,380 |
| `opponent_path_c.tsv` | Navampa | 5,048 |

## Design

The grid is stored as whole planes rather than as cell words, which keeps
each plane's long runs of one value together for the packer; that is an
inference, since the bytes show only the layout.

The markers are the one part stored unpacked, and the loader reads them in
place rather than copying them, likely because only `Field_PlaceMapMarkers`
reads them, once as the map loads.

One grid serves all three layers, so every layout takes the same 64 KB of cells,
and a layer's window can sit anywhere in the grid.

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 metatiles a layout: a cell's metatile number has 12 bits, and 4,095
  is the placeholder.
- 256 shapes a layout, since a cell names its shape in one byte.
- 16 tile animation channels (`struct MapGridState`).
- `MARKER_MAX` (10) chests and Psynergy Stones placed on a map.

## Files

| Path | What it holds |
| --- | --- |
| `layout_000/` to `layout_179/` | one layout each: its `layout.s` and the tables it places |
| `gondowan_passage/`, `venus_entrance/` | two more layouts, Gondowan Passage and Venus Lighthouse's entrance, built like the others |
| `world/blocks.tsv` | the world map's ground (`Field_PerspectiveDataA`) |
| `default/` | the world map's default cells, tile animation and block attributes (`Field_DefaultGridCells` and the tables after it) |
| `colosso/` | the opponents' recorded paths, with the Colosso lettering placed after them |
| `world_map.s` | places `world/` and `default/` |
| `layout.inc` | the `layout_header`, `layout_layer` and `layout_offset` macros every `layout.s` uses |

The layout numbers are the order of the directories, not numbers the game
gives its layouts.

## Words used here

- A **cell** is a 16 by 16 pixel square, one 32-bit word once loaded
  (`struct MapGridCell` in [`map_grid.h`](../../include/map_grid.h)).
- A **metatile** is the picture of a cell: 2 by 2 of the 8-pixel
  tiles, stored as the four BG map entries that draw them, top left, top
  right, bottom left, bottom right.
- A **shape** is the form of the ground in a cell: a terrain kind and three
  height samples.
- A **metatile behavior** says what standing on, pushing against or searching a
  cell does (`enum MetatileBehavior`).
- A **layer** is one of the layout's three backgrounds, each a window onto the
  one cell grid.
- A **plane** is one value per cell over the whole 128 by 128 grid.
- A **marker** is a cell where a chest or a Psynergy Stone can stand.

## Evidence and open questions

The game keeps no names for any of this, so each part is named for what the
code does with it: the 16-pixel pictures become BG map entries, so they are
`metatiles`; the shapes feed the height functions; and the markers are where
`Field_PlaceMapMarkers` puts chests and Psynergy Stones. The sixth offset is
called the marker list because that function is the only code that reads the
pointer the loader stores to it, although `MapGridScrollWork`
(`include/map_grid_scroll.h`) still names the field `script`.

[XXX](../../glossary.md#xxx): that the grid is stored as planes to keep long runs together for the
packer is an inference; the bytes show only the layout. XXX: likely the
markers are stored unpacked because only `Field_PlaceMapMarkers` reads them,
once as the map loads; nothing else shows why.

The pictures are drawn from the tables themselves: the haze from
`layout_147`'s header, the shapes from its `shapes.tsv`, the chests from
`layout_110`'s markers and the chest events of `map/crossbone_isle_keys.c`,
and the Colosso runs from the three path tables, point by point. The
placeholder and tile animation pictures stand for any layout: their grass,
water and rock are drawings, not the game's metatiles.

The header's last two bytes (`pad0` and `pad1` in `layout.inc`, `reserved` in
`struct LayoutHeader`) are 0 in every layout of both games, and nothing
reads them: the loader is the only code that reads the header.

