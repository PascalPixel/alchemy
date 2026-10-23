# Layouts

A town or dungeon [layout](../../glossary.md#layout) is one big square grid of 16-pixel [cells](../../glossary.md#cell), 128 on
a side, and each of its three layers is a rectangle cut from that grid; the
layout's header says where each one starts, and the rest of the grid goes
unused.

The `src/layout` folder holds this data as editable tables, and no code:
182 layouts for the towns, dungeons and interiors, and two more sets of
data kept beside them because they are packed and placed the same way: the
world map's ground and the opponents' recorded paths in the Colosso finals. A [map](../../glossary.md#map) names its layout,
[Field](../field/README.md) draws and scrolls it, and the sections below say
how each part is stored, built and loaded.

## Cells

A cell is one 16 by 16 pixel square of a layout. Once loaded it is one
32-bit word that says three things: what is drawn there, what standing on
it does, and how its ground is shaped.

A cell is `struct MapGridCell` in [`map_grid.h`](../../include/map_grid.h):

| Bits of the cell word | Field | Meaning |
| --- | --- | --- |
| 0 to 11 | `metatileAndFlags` | the metatile drawn in the cell |
| 12 to 15 | `metatileAndFlags` | four flag bits ([below](#evidence-and-open-questions)) |
| 16 to 23 | `behavior` | the metatile behavior |
| 24 to 31 | `shape` | the shape, a row of the layout's shape table |

**What is drawn.** A [metatile](../../glossary.md#metatile) is the picture of one cell, 2 by 2 of the
console's 8-pixel tiles, stored as the four BG map entries that draw them
(top left, top right, bottom left, bottom right). A cell names its metatile
by number, a row of the layout's metatile table. Of the four bits above the
number, the field engine reads none: only an unused helper,
`MapGrid_GetCellHighFlags`, and Crossbone Isle's stepping stones, which copy
them with the cell, touch them.

**What standing on it does.** The [metatile behavior](../../glossary.md#metatile-behavior) is what walls and
[map events](../../glossary.md#map-event) are keyed on. `MB_WALL` blocks movement whatever the heights.
The behaviors from `MB_EVENT_FIRST` to `MB_EVENT_LAST` belong to the map:
a [coord event](../../glossary.md#coord-event) runs when the leader steps onto or pushes against a cell
with its behavior, and a [bg event](../../glossary.md#bg-event) when the player presses A facing one
([Maps](../map/README.md#which-map-events-a-cell-starts)). The behaviors above them mean
the same on every map: `MB_ENCOUNTER_GROUND_1` and `_2` for ground with
random battles of its own, `MB_JAR` to `MB_WELL` for search spots with
shared lines, `MB_DESERT_SAND` for the Lamakan Desert's sand, and
`MB_LEDGE`, `MB_CLIMB` and `MB_JUMP_FORWARD`, which `Field_RunPushCellEvent`
acts on when the leader pushes against them.

**How the ground is shaped.** A shape is four bytes, a row of `shapes.tsv`.
The low four bits of the first pick one of the 16 terrain kinds (flat
ground, slopes, steps, ground shaped through three heights and others), and
the other three are height samples in steps of 8 pixels, which the kind's
height function interpolates (`MapGrid_GetTerrainHeight`, through
`gMapGrid_TerrainHeightFunctions`). `MapGrid_CheckCollision` lets an object
step at most 8 pixels up or 12 down, so a sharp rise or drop stops the
leader as surely as `MB_WALL`. Most of the game's walls are made this way:
Goma Cave has no `MB_WALL` cells at all, only a rim of ground 32 pixels high
along the foot of each cliff, some of it cut along a cell's diagonal.

Behavior and height belong to a layer. Each layer is a window onto the grid
([below](#one-grid-three-layers)), and an object follows one layer, the one
its `layer` names, which a map's code sets. Code that asks about the
ground at an object's position reads the cell at that position in its
layer's window, so the three layers can have different walls and heights
([Field](../field/README.md#how-high-a-step-can-go)).

The behaviors are `enum MetatileBehavior`.

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
| `MB_JUMP_FORWARD_UNREACHED` | 249 | jumps forward like `MB_JUMP_FORWARD`, but never reaches `Field_RunPushCellEvent` |
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
| `layout.s` | the header, three layers and six offsets, then the parts; in 129 layouts a tileset's palette and tiles follow (from `games/tbs/src/graphics/tileset`) | assembled |
| `metatiles.tsv` | the metatiles; a metatile's number is its row, from 0 | `metatiles.delta0.lz`, `.delta1.lz` or `.delta2.lz` |
| `shapes.tsv` | the shapes | `shapes.table.lz` |
| `map.txt` | the grid: each cell's metatile, metatile behavior and shape | `map.grid.lz` |
| `tileset_anims.tsv` | the tileset animation script | `tileset_anims.script.lz` |
| `blend.tsv` | the blend [script](../../glossary.md#script) | `blend.script.lz` |
| `treasure_spots.tsv` | the treasure spots | `treasure_spots.table` |
| `*_ja.tsv`, `map_de.txt`, `*_es.tsv` and so on | an [edition](../../glossary.md#editions)'s own version of a table | chosen in `layout.s` |
| `*_patch.tsv` | bytes [Camelot](../../glossary.md#camelot) changed after packing | read by the packer |

`map.txt` holds one line of the grid per row, each cell written
`metatile:behavior:shape` in hexadecimal, a part left off the end being 0
(`4fff:ff`, `20`). Its first line, `grid:128x128	at:2,3	fill:1:0:0`, gives
the grid's size, the cell its first written row starts at (column, then
row), and the cell every square the rows leave out holds. The game reads the
grid as three planes one after another: the behaviors, the shapes, then the
metatiles.

### The header

In the north of Suhalla Desert a haze of sand streams across the screen
faster than the ground moves, and wraps round as it goes. The layout's header is
what sets that up: for each layer it says how fast the layer follows the
camera, whether it drifts by itself and where it wraps, as well as how far
the camera may go.

A layer that follows the camera at the camera's own pace looks as far away
as the ground; one that moves faster looks nearer. Suhalla Desert's north
(`layout_147`) moves its ground and middle layers with the camera and its
overhead haze a quarter faster, drifts the haze by -121 and 35 sixteenths of
a pixel a [frame](../../glossary.md#frame), and wraps it every 32 tiles, 256 pixels.

The header is one `layout_header` line and three `layout_layer` lines in `layout.s`,
read as `struct LayoutHeader`. It gives the camera's bounds in 8-pixel
tiles, each layer's priority (0 leaves the layer off) and the char block its
tiles come from, and for each layer its origin in 8-pixel tiles, its
parallax (in sixteenths of the view's motion), its drift (sixteenths of a pixel a frame) and its wrap.

Six offsets from the header's start follow: the metatiles, the shapes, the cell
grid, the tileset animation, the blend script and the treasure spots. An offset of 0
means the layout has none of that part.

`struct LayoutHeader` and `struct LayoutLayer` are defined in
`field/field/map_grid.c`, and the `layout_header` and `layout_layer` macros that
write them in `layout.inc`.

| Field | Unit | Meaning |
| --- | --- | --- |
| `cameraBounds[4]` | 8-pixel tiles | the camera's bounds: left, top, right and bottom |
| `priority[3]` | | each layer's BG priority, 1 to 3; 0 leaves the layer off |
| `charBase[3]` | | each layer's BG char block |
| `reserved` | | 0 in every layout; nothing reads it |
| `layers[n].x`, `.y` | 8-pixel tiles | the layer's origin |
| `layers[n].scrollX`, `.scrollY` | sixteenths of the view's motion | the layer's parallax |
| `layers[n].speedX`, `.speedY` | sixteenths of a pixel a frame | the layer's drift |
| `layers[n].periodX`, `.periodY` | 8-pixel tiles, less one | where the layer wraps, as a mask: `0x1f` wraps every 32 tiles |
| `metatiles` | bytes from the header | the metatiles |
| `shapes` | bytes from the header | the shapes |
| `cells` | bytes from the header | the cell grid |
| `animation` | bytes from the header | the tileset animation script, or 0 for none |
| `blend` | bytes from the header | the blend script, or 0 for none |
| `markers` | bytes from the header | the treasure spot list, read in place |

## How the build makes a layout

The tables are for people to read and edit; the game wants packed bytes. The
build turns each table into the bytes the game reads and packs them exactly as
Camelot's packer did, so the result is the same, byte for byte, as the
cartridge.

The name of the file the build asks for says what to make:
`NAME.FORM[.CODEC]`, from `NAME.tsv`. `tools/ags-graphics` makes it (the
`Makefile`'s asset rules), and a layout uses these forms:

- `table`: the shapes and treasure spots, one entry per row, each column's type
  giving its size.
- `plane`: a grid plane, written out whole, row by row, with `fill` in every
  cell the table leaves out.
- `parts`: the grid, the encodings of the files it lists one after another.
- `delta0`, `delta1`, `delta2`: the metatiles, halfwords behind a method byte
  (`enum MetatilePacking`, in the table below) that
  `TileMap_DecodeStagedBuffer` undoes. Each `layout.s` names the method its
  stream uses.
- `script`: the tileset animation and blend scripts below.

The codec `lz` then packs each stream in the format the game's
`Decompress_Type01` unpacks, and the bytes must come out identical.
Where [the original](../../glossary.md#the-original) stream is followed by stray bytes the packer left in its
buffer, `layout.s` places them as literal bytes with a `PACKER:` comment naming
what wrote them. Where Camelot changed a stream after packing it, a
`_patch.tsv` lists each changed byte. The table holds what shipped, and the
patch gives the packer the byte it packed, so the packer makes the same
choices and the stream still decodes to the edited layout. The first row of
Gondowan Passage's grid patch:

```text
position  pack  ship  proof
65472     0x02  0x01  literal: 254 of 255 stand-ins reproduce the stream; this is the lowest
```

The byte at that position shipped as `0x01`. The packer must have seen
something else there, since `0x01` would pack differently; any of 254 values
gives the shipped stream, and the patch names the lowest, `0x02`. The proof
column says how the packed byte was found, and a later row that the packer
copied from an earlier one says `copy of` that row's position. Only Gondowan
Passage's grid and Venus Lighthouse's entrance's grid, with its Spanish and
French versions, have patches.

An edition's own version of a table (`grid_es.tsv`, `treasure_spots_ja.tsv`) is
chosen in `layout.s` with `.ifdef` on the edition's name (`TBS_EDITION_ES`),
and editions that agree share
one file: Venus Lighthouse's entrance has a French grid and a Spanish one, and
the Italian edition uses the Spanish. A layout whose whole data differs by
edition has a block of its `layout.s` for each. A layout that uses another
layout's table names that layout's file.

`enum MetatilePacking` is in `field/field_internal.h`.

| Name | Value | Form | Meaning |
| --- | --- | --- | --- |
| `METATILE_PACKING_RAW` | 0 | `delta0` | the metatiles as they are |
| `METATILE_PACKING_SPLIT_DELTA` | 1 | `delta1` | each value XORed with the one before, all the high bytes stored before all the low bytes |
| `METATILE_PACKING_DELTA` | 2 | `delta2` | each value XORed with the one before |

The two delta methods store each halfword XORed with the one before it, the
first with 0, so a run of similar values packs well. Split delta then
stores all the high bytes before all the low bytes. Two halfwords
`0x0102` and `0x0105` become `0x0102` and `0x0007`, and are stored as:

```text
METATILE_PACKING_DELTA         02 01 07 00    each halfword, low byte first
METATILE_PACKING_SPLIT_DELTA   01 00 02 07    the high bytes, then the low bytes
```

## Loading a layout

When the party enters a town or dungeon, the map's layers and [sprites](../../glossary.md#sprite-and-oam) are
hidden while its layout is unpacked into working memory. Most of what is
unpacked is the grid, which is stored in three separate pieces and put together into one
word per cell as it loads, so that everything about a cell can be read in
one go.

Each map's row of `gMapHeaders` ([`field/map_headers.s`](../field/map_headers.s))
gives a layout number, which picks a row of `gLayouts`
([`field/layouts.c`](../field/layouts.c)): the layout's header, palette,
its tileset's three sheets of tiles and layer data. `MapGrid_LoadLayout`
([`field/field/map_grid.c`](../field/field/map_grid.c)) decodes each part
into a fixed buffer in [EWRAM](../../glossary.md#iwram-and-ewram): the metatiles into `gMetatiles`, the
shapes into `gMapGridShapes`, the grid into `Ram_MapGridCellBuffer`, and
the two scripts into the space after the shapes. The treasure spots are not
decoded: the loader keeps a pointer to them in the map grid work. A `Ram_`
name is the same buffer as the `g` name after it (`Ram_MapGridCellBuffer` is
`gMapGridCellBuffer`), written where the code reaches the buffer by its fixed
address.

The grid is stored as three whole planes, in the order of the table below.
`TileMap_ConvertBuffer` then runs `Tile_ConvertMapGrid`, a routine in the
processor's fast 32-bit ARM instructions copied into RAM, which weaves them
into 128 by 128 cell words, 64 KB from the start of
`Ram_MapGridCellBuffer`, and numbers the placeholder metatiles as it goes
([Placeholder metatiles](#placeholder-metatiles)). The first two planes are
saved to a temporary buffer first, since the cell words overwrite them.

`gMapGridCellBuffer` itself is 32 KB, so the cells run on through
`gActorSpriteSlots` and `gDecodeBuffer`, and neither can hold anything else
while a layout is loaded. Only code that reloads the cells afterwards writes
there:

- a battle, after which the map's layout is loaded again;
- the Battle Arena, which leaves the partner's party in `gActorSpriteSlots`
  for the link battle that follows;
- the world map's still view, which saves `BG1`'s tiles to `gDecodeBuffer`
  and draws into the cells, which `WorldMap_LoadDefaultCellsAndUpdateBlock`
  then puts back;
- the debug sprite viewer, which loads sprites there.

The tileset animation only reads from `Ram_DecodeBuffer`, at offsets that land
past it. [System](../system/README.md) has the EWRAM layout.

| Plane, in stored order | Size | Values |
| --- | --- | --- |
| metatile behaviors | 16 KB | a byte a cell |
| shapes | 16 KB | a byte a cell |
| metatiles | 32 KB | a halfword a cell |
| the woven cell words | 64 KB | a word a cell, from the start of `Ram_MapGridCellBuffer` |

### One grid, three layers

All three layers are windows onto the one grid. In Goma Cave's entrance room
the ground layer, the cave floor and its pool, starts at cell 0, 0; the middle
layer, the rock walls, at cell 0, 34, below it; and the overhead layer, only
the glow of the torches, at cell 50, 0, to its right. Each layer's origin in
the header picks the cell its window starts at (half its tile position, since a cell is 2 by 2
tiles), so a layout lays its layers out side by side or one above
another: `layout_000`'s start at tiles 0, 60 and 120 across, and Venus Lighthouse's
entrance puts them at (0, 0), (64, 0) and (0, 68). A window has no size of
its own: the camera's bounds in the header stop the view, so a layer shows
the cells from its origin out as far as the camera can go.

The ground layer shows on `BG3`, the middle layer on `BG2` and the overhead
layer on `BG1`; [Field](../field/README.md#towns-and-dungeons-on-screen) explains
how their priorities put [object events](../../glossary.md#object-event) between them.

### Placeholder metatiles

A layout's grid doesn't store the number of every cell's picture from the
start. The first time a picture appears in the grid, the cell holds a
placeholder instead, and the game hands out metatile numbers in order as it
loads the layout.

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

CrossboneIsleKeys and CrossboneIsleBane share `layout_110`, whose eight treasure spots
carry the [triggers](../../glossary.md#trigger) 100 to 107: CrossboneIsleKeys has chest events for 100 to
103, and CrossboneIsleBane for 104 to 107 (`map/crossbone_isle_keys.c`).
CrossboneIsleBane moves the Demon Axe's chest to 0, 0 until the stone pillar
carrying it reaches the ladder, since its treasure spot's cell lies past the room's
east wall.

The treasure spot list is (column, row, id) triples, a cell and the trigger of a
map event, ended by 255, 255. When a map loads, `Field_PlaceTreasureSpots`
([`overworld/overworld/markers.c`](../overworld/overworld/markers.c)) places an
object at each treasure spot whose id (100 to 239) the map has an event for, up to
`TREASURE_SPOT_MAX` (10): a chest for a chest event, shown open once its flag is set
and gone once its mimic has been beaten, and a Psynergy Stone for a search
event that gives one, until it is taken. A treasure spot the map has no event for
takes none of the ten. 76 layouts place `layout_000/treasure_spots.table`, which holds only the end
row.

| Name | Value | Meaning |
| --- | --- | --- |
| a treasure spot's id | 100 to 239 | the trigger of the map event that places it |
| the end row | column 255, row 255 | ends the list |
| `TREASURE_SPOT_MAX` | 10 | treasure spots placed on a map (`include/unit/overworld/overworld.h`) |

### Tileset animation and blend scripts

Some parts of a map move by themselves without anything on the map moving:
the game simply swaps the pictures of a few tiles on a timer. A
layout's tileset animation script says which pictures to copy over which tiles and
how long to wait between them; its blend script changes, in the same way, how
the layers mix.

Suhalla Desert's north shows the usual shape: its channel 0 copies four rows
of four tiles from one of four stored pictures, waits 15 frames, takes the
next picture, and jumps back to the first after the fourth.

Both scripts are runs of halfwords, run each frame as [tasks](../../glossary.md#task)
([`field/field/tile_animation.c`](../field/field/tile_animation.c)). In the
tileset animation's table, `channel n` (`0xfd00 + n`) starts channel n of the 16 at the next halfword; only the low
four bits pick the channel, and a channel word with bit 7 set starts it
paused.
Each `frame` is four values: the source tile, how many tiles, the destination
tile and the frames to wait. `jump n` (`0xfe00 + n`) goes to halfword 2n of
the channel's script, `stop` (`0xfeff`) halts the channel, and `end`
(`0xffff`) loops it to its start. A source tile below a threshold is copied from
elsewhere in video memory, and one from the threshold on from RAM, at
`Ram_DecodeBuffer` plus the tile's offset, which lands in
`gMapGridLayerData`. The threshold is tile 1,536 for 16-colour tiles, 32
bytes each, and tile 512 for a layout whose animations copy 256-colour
tiles, 64 bytes each (`MapGridState.wideTiles`).

The blend script changes how the layers blend over time. `control` sets
`REG_BLDCNT`; `level` sets `REG_BLDALPHA`, or `REG_BLDY` when the control's
mode is not alpha blending, and waits its second value in frames. `jump`,
`stop` and `end` act as in the tileset animation.

The words are tested in `TilesetAnim_Update` and `BlendScript_Run`,
which have no names for them; the names are the tables' (`tileset_anims.tsv`,
`blend.tsv`).

| Word | Value | Meaning |
| --- | --- | --- |
| `channel n` | `0xfd00 + n` | the tileset animation's channel n starts at the next halfword |
| `frame` | four values below `0xfd00` | source tile, tile count, destination tile, frames to wait |
| `jump n` | `0xfe00 + n` | continue at halfword 2n of the channel's script |
| `stop` | `0xfeff` | halt the channel |
| `end` | `0xffff` | loop back to the channel's start |
| a source tile from RAM | 1,536 on (512 for 256-colour tiles) | copied from `Ram_DecodeBuffer` plus its offset, rather than from video memory |

## The world map's data

The world map is far too big to unpack at once, so its ground is cut into
squares that are packed one by one, and the game unpacks only the few around
the party, bringing in new ones as the party walks.

The world is 16 by 16 squares of 512 pixels, and a table in the world map's
work (`WorldMapView.tileIds`) says which stored square shows at each of
them. `WorldMap_UpdateCurrentTileBlock` unpacks the 2 by 2 squares whose top left
holds the point 256 pixels left of the party and 320 above, in each of the
two ground layers (`WorldMap_WriteLayerCellTile`), and
`WorldMap_PreloadCurrentTileBlock` brings in at most one new square a frame as the
party walks.

The ground's data, `world/blocks.tsv` (`Field_PerspectiveDataA`), is one grid
of 32-bit values in hexadecimal, one value for each 32-pixel cell of the
world map, 512 across and 320 down. The `blocks` form cuts it into squares of
16 by 16 values, 512 pixels a side, and packs each separately behind a table
of offsets; a square written all as `-` would be left out with an offset of
0, and the world map leaves none out. The count works out as:

- 512 / 16 = 32 squares across and 320 / 16 = 20 down, 640 in all;
- the first 320 are the lower ground layer's and the next 320 the upper's;
- each layer has 320 stored squares for the world's 256 positions.

Each position starts on the stored square of its own number, 0 to 255. The
other 64 are alternates: a world map script puts one in a position
(`WorldMap_SetWindowCellTile`), and the snowfield's graphics set swaps six
squares (`WorldMap_LoadGraphics`).

The tables in `default/` are the world map's too.
`WorldMap_InitializePerspective` loads the default cells, block attributes and
tileset animation when the world map starts, and
`WorldMap_LoadDefaultCellsAndUpdateBlock` puts the cells back after the world map's
still view ([Field](../field/README.md#the-world-map-in-perspective)).

| Part | Size |
| --- | --- |
| the world | 16 by 16 squares of 512 pixels |
| the ground's grid | 512 by 320 values, 32 bits each |
| a value | one 32-pixel cell of the world map |
| a square | 16 by 16 values, 512 pixels a side |
| squares | 640, each packed on its own, 320 for each ground layer: 256 positions and 64 alternates |
| unpacked at once | the 2 by 2 squares around the party, in each of two ground layers |

## The Colosso paths

In each Colosso final Isaac races an opponent through an obstacle course. The
opponent isn't steered by any rule: it replays a run that was recorded once,
one point of ground a frame.

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

The grid is stored as whole planes rather than as cell words. [XXX](../../glossary.md#xxx): likely
to keep each plane's long runs of one value together for the packer,
because a behavior plane is mostly runs of open ground and walls, as
`layout_003`'s is; the bytes show only the layout.

The treasure spots are the one part stored unpacked, and the loader reads them in
place rather than copying them. XXX: likely because only
`Field_PlaceTreasureSpots` reads them, once as the map loads.

One grid serves all three layers, so every layout takes the same 64 KB of cells,
and a layer's window can sit anywhere in the grid.

The layout numbers are the order of the directories, not numbers the game
gives its layouts.

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 metatiles a layout: a cell's metatile number has 12 bits, and 4,095
  is the placeholder.
- 256 shapes a layout, since a cell names its shape in one byte.
- 16 tileset animation channels (`struct MapGridState`).
- `TREASURE_SPOT_MAX` (10) chests and Psynergy Stones placed on a map.

## Words used here

- A **shape** is the form of the ground in a cell: a terrain kind and three
  height samples.
- A **layer** is one of the layout's three backgrounds, each a window onto the
  one cell grid.
- A **plane** is one value per cell over the whole 128 by 128 grid.
- A **treasure spot** is a cell where a chest or a Psynergy Stone can stand.

## Evidence and open questions

The game keeps no names for any of this, so each part is named for what the
code does with it: the 16-pixel pictures become BG map entries, so they are
`metatiles`; the shapes feed the height functions; and the treasure spots are where
`Field_PlaceTreasureSpots` puts chests and Psynergy Stones. The sixth offset is
called the treasure spot list because that function is the only code that reads the
pointer the loader stores to it, although `MapGridScrollWork`
(`include/map_grid_scroll.h`) still names the field `script`.

The pictures are drawn from the tables themselves: the haze from
`layout_147`'s header, the shapes from its `shapes.tsv`, the chests from
`layout_110`'s treasure spots and the chest events of `map/crossbone_isle_keys.c`,
and the Colosso runs from the three path tables, point by point. The
placeholder and tileset animation pictures stand for any layout: their grass,
water and rock are drawings, not the game's metatiles.

The header's last two bytes (`pad0` and `pad1` in `layout.inc`, `reserved` in
`struct LayoutHeader`) are 0 in every layout of both games, and nothing
reads them: the loader is the only code that reads the header.

The [Design](#design) section gives two reasons only as likely: why the
grid is stored as planes and why the treasure spots are not packed.

- XXX (no evidence): why the grid is stored as planes and the treasure spots
  unpacked; only Camelot's tools would show what the packing was chosen for.
- XXX: what the four flag bits above a cell's metatile number mean; the
  field engine never reads them, so a map whose cells set them, rendered
  with each bit cleared, would settle it.

