# Maps

![The whole 128 by 128 cell grid of Goma Cave's map at half size, each cell drawn from its block: the ground layer's window at the top left holds the cave floor and its pool, the middle layer's below it the rock walls around a dark floor, and the overhead layer's at the top right only the glows of the torches, and the rest of the grid is empty.](https://alchemy.pascalpixel.com/guides/map/grid.png)

This folder holds the map data, as editable tables: 182 layered maps for the
towns, dungeons and interiors, the world map's ground and default cells, and
the opponents' recorded paths in the Colosso finals. It holds no code. Each
scene's row of `gField_SceneTable` ([`../field/scenes.s`](../field/scenes.s))
gives a map number, which picks a row of `gMap_LayeredScenes`
([`../field/layered.tsv`](../field/layered.tsv)): the map's header, palette,
three tile sheets and layer data. `Map_LoadLayeredScene`
([`../field/field/map_layers.c`](../field/field/map_layers.c)) loads it, and
[Field](../field/README.md) describes how the engine draws and scrolls it.

## Files

| Path | What it holds |
| --- | --- |
| `map_000/` to `map_179/` | one layered map each: its `map.s` and the tables it places |
| `gondowan_passage/`, `venus_entrance/` | two more layered maps, Gondowan Passage and Venus Lighthouse's entrance, built like the others |
| `world/blocks.tsv` | the world map's ground (`Field_PerspectiveDataA`) |
| `default/` | the world map's default cells, tile animation and block attributes (`Field_DefaultMapCells` and the tables after it) |
| `colosso/` | the opponents' recorded paths, with the Colosso lettering placed after them |
| `map.s` | places `world/` and `default/` |
| `map.inc` | the `map_header`, `map_layer` and `map_block` macros every `map.s` uses |

The map numbers are the order of the directories, not numbers the game gives
its maps.

## Words

- A **cell** is a 16 by 16 pixel square, one 32-bit word once loaded
  (`struct MapCell` in [`map.h`](../../include/map.h)).
- A **block** (a metatile) is the picture of a cell: 2 by 2 of the 8-pixel
  tiles, stored as the four BG map entries that draw them, top left, top
  right, bottom left, bottom right.
- A **shape** is the form of the ground in a cell: a terrain kind and three
  height samples.
- A **collision code** says what standing on, pushing against or searching a
  cell does (`enum MapCellCode`).
- A **layer** is one of the map's three backgrounds, each a window onto the
  one cell grid.
- A **plane** is one value per cell over the whole 128 by 128 grid.
- A **marker** is a cell where a chest or a Psynergy Stone can stand.

## Cells

A loaded cell is one word. Its low halfword holds the block number in its low
12 bits and four flag bits above, the layer and priority an object standing
there takes; its third byte is the collision code, and its fourth the shape's
number in the map's shape table.

A shape is four bytes, a row of `shapes.tsv`. The low four bits of the first
pick one of the 16 terrain kinds (flat ground, slopes, steps, ground shaped
through three heights and others), and the other three are height samples in
steps of 8 pixels, which the kind's height function interpolates
(`Map_GetTerrainHeight`, through `gMap_TerrainHeightFunctions`).

The collision code is what walls and events are keyed on. `MAP_CELL_WALL`
blocks movement whatever the heights. The codes from `MAP_CELL_EVENT_FIRST`
to `MAP_CELL_EVENT_LAST` belong to the scene: a touch event fires when the
leader steps onto or pushes against a cell with its code, and a search event
when the player presses A facing one. The codes above them mean the same on
every map: `MAP_CELL_ENCOUNTER_GROUND_1` and `_2` for ground with encounters
of its own, `MAP_CELL_JAR` to `MAP_CELL_WELL` for search spots with shared
lines, `MAP_CELL_HAZARD` for the Lamakan Desert's sand, and `MAP_CELL_LEDGE`,
`MAP_CELL_CLIMB` and `MAP_CELL_JUMP_FORWARD`, which `Field_RunPushTileEvent`
acts on when the leader pushes against them.

A wall need not be coded. `Map_CheckObjectStep` lets an object step at most
8 pixels up or 12 down, so ground that rises sharply stops the leader as
surely as `MAP_CELL_WALL`. Goma Cave has no wall-coded cells at all: a rim of
ground 32 pixels high runs along the foot of each cliff, some of it cut along
a cell's diagonal.

![One screen of Goma Cave, and beside it the same screen with the ground of the leader's layer drawn over it at its place on the map: orange where the ground stands 32 pixels above the floor, the rims, some cut along a cell's diagonal, that stop the leader at the foot of each cliff; white lines where the height changes; yellow outlines on the two doorway cells with the scene's own codes. The cave has no wall-coded cells; its walls are all height.](https://alchemy.pascalpixel.com/guides/map/cells.png)

Collision and height belong to a layer: code that asks about the ground
reads the cell in the window of the layer the object follows, so an object
stands in one layer at a time ([Field](../field/README.md#the-ground-under-an-object)).

## A map's tables

Each layered map keeps its tables in its directory:

| File | What it is | Built as |
| --- | --- | --- |
| `map.s` | the header, three layers and six offsets, then the parts; in 129 maps a tile set's palette and tile sheets follow (from `games/tbs/src/graphics/tileset`) | assembled |
| `metatiles.tsv` | the blocks; a block's number is its row, from 0 | `metatiles.delta0.lz`, `.delta1.lz` or `.delta2.lz` |
| `shapes.tsv` | the shapes | `shapes.table.lz` |
| `grid.tsv` | the three planes, in order | `grid.parts.lz` |
| `grid_collision.tsv`, `grid_shape.tsv`, `grid_metatile.tsv` | each cell's collision code, shape and block | parts of the grid |
| `animation.tsv` | the tile animation script | `animation.script.lz` |
| `blend.tsv` | the blend script | `blend.script.lz` |
| `markers.tsv` | the markers | `markers.table` |
| `*_ja.tsv`, `*_de.tsv`, `*_es.tsv` and so on | an edition's own version of a table | chosen in `map.s` |
| `*_patch.tsv` | bytes Camelot changed after packing | read by the packer |

A plane's first line describes it: `cell:u8	grid:128x128	fill:0	at:2,3`
gives the size of each value, the grid, the value of every cell the table
leaves out, and the cell its first row starts at, column then row. The rows
are hexadecimal, one row of the grid per line.

### The header

The header is one `map_header` line and three `map_layer` lines in `map.s`,
read as `struct LayeredSceneHeader`. It gives the camera's bounds in 8-pixel
tiles, each layer's priority (0 leaves the layer off) and the char block its
tiles come from, and for each layer its origin in 8-pixel tiles, its parallax
(in sixteenths of the view's motion), its drift (sixteenths of a pixel a
frame) and its wrap.

Six offsets from the header's start follow: the blocks, the shapes, the cell
grid, the tile animation, the blend script and the markers. An offset of 0
means the map has none of that part.

## Loading a map

`Map_LoadLayeredScene` decodes each part into a fixed buffer in EWRAM: the
blocks into `gMapBlocks`, the shapes into `gMapCollision`, the grid into
`Ram_MapCellBuffer`, and the two scripts into the space after the shapes. The
markers are not decoded: the loader keeps a pointer to them in the map work.

The grid is stored as three whole planes: collision codes, then shapes, then
blocks (16 KB, 16 KB and 32 KB, since the block plane has a halfword per
cell). `TileMap_ConvertBuffer` then runs `Tile_ConvertMap`, an ARM routine
copied into RAM, which weaves them into 128 by 128 cell words, 64 KB from the
start of `Ram_MapCellBuffer`. The first two planes are saved to a temporary
buffer first, since the cell words overwrite them.

![Goma Cave's grid as Tile_ConvertMap leaves it, taken back apart into its three planes, each cropped to the three layers' windows at three pixels a cell: the collision codes, nearly all open ground with a few event and wall cells; the shapes, outlining the rock rims; and the blocks, the ground and middle layers' windows full of different blocks and the overhead layer's nearly empty. Beside them four by three cells around the leader with each cell's word written over it in hex, the shape in its top byte and the block in its low bits](https://alchemy.pascalpixel.com/guides/map/grid-planes.png)

`gMapCellBuffer` itself is 32 KB, so the cells run on through
`gActorSpriteSlots` and `gDecodeBuffer`, and neither can hold anything else
while a map is loaded. Only code that reloads the cells afterwards writes
there: a battle, after which the scene's map is loaded again; the link lobby,
which leaves the partner's party in `gActorSpriteSlots` for the link battle
that follows; and the world map's still view, which saves `BG1`'s tiles to
`gDecodeBuffer` and draws into the cells, which
`Map_LoadDefaultCellsAndUpdateBlock` then puts back. The debug sprite viewer
also loads sprites there. The tile animation only reads from
`Ram_DecodeBuffer`, at offsets that land past it. [System](../system/README.md) has the
EWRAM layout.

### One grid, three layers

All three layers are windows onto the one grid. Each layer's origin picks
the cell its window starts at (half its tile position, since a cell is 2 by 2
tiles), so a map lays its layers out side by side or one above another:
`map_000`'s start at tiles 0, 60 and 120 across, and Venus Lighthouse's
entrance puts them at (0, 0), (64, 0) and (0, 68).

The ground layer shows on `BG3`, the middle layer on `BG2` and the overhead
layer on `BG1`; [Field](../field/README.md#layered-maps-on-screen) explains
how their priorities put actors between them.

### Placeholder blocks

A cell whose block is 4,095, the highest 12 bits can hold, is a placeholder.
As the map is converted, `Tile_ConvertMap` gives each placeholder a fresh
block number, counting up from 0 in grid order. The ROM also keeps an unused
C version of that numbering, `TileMap_NumberPlaceholderBlocks`.

The placeholder is how the grid writes a block's first use. Each map's block
table is ordered by where its blocks first appear in the grid, and the grid
writes that first appearance as 4,095 and every later one by number, so a map
has exactly as many placeholders as blocks (`map_003` has 1,331 of each).
Every map follows the rule but Venus Lighthouse's entrance, whose grid names
170 cells' blocks before their placeholders; those cells still get the right
blocks, since the numbering depends only on the placeholders.

### Chests and Psynergy Stones

The marker list is (column, row, id) triples, a cell and the trigger of a
scene event, ended by 255, 255. When a scene loads, `Field_PlaceMapMarkers`
([`../object/object/markers.c`](../object/object/markers.c)) places an
object at each marker whose id (100 to 239) the scene has an event for, up to
`MARKER_MAX` (10): a chest for a chest event, shown open once its flag is set
and gone once its mimic has been beaten, and a Psynergy Stone for a search
event that gives one, until it is taken. A marker the scene has no event for
takes no slot. 76 maps place `map_000/markers.table`, which holds only the end
marker.

### Tile animation and blend scripts

Both scripts are runs of halfwords, run each frame as tasks
(`field/map_animation.c`). The tile animation copies new tiles over old ones
on a timer, for the parts of a map that move by themselves. In its table,
`channel n` (`0xfd00 + n`) starts channel n of the 16 at the next halfword.
Each `frame` is four values: the source tile, how many tiles, the destination
tile and the frames to wait. `jump n` (`0xfe00 + n`) goes to halfword 2n of
the channel's script, `stop` (`0xfeff`) halts the channel, and `end`
(`0xffff`) loops it to its start. A source tile from 1,536 on is copied from
RAM at `Ram_DecodeBuffer` plus its offset, which lands in `gMapLayerData`, and
a lower one from elsewhere in video memory; the line is 512 for a map whose
animations copy 64-byte tiles (`MapState.wideTiles`).

![Vale where Garet's sister stands, beside the 512 tiles of its ground layer's character block in grey, 32 to a row: every few frames one group of tiles in the block's lower half is rewritten by the tile animation and outlined in red, while the rest stay as they are](https://alchemy.pascalpixel.com/guides/map/tile-animation.gif)

The blend script changes how the layers blend over time. `control` sets
`REG_BLDCNT`; `level` sets `REG_BLDALPHA`, or `REG_BLDY` when the control's
mode is not alpha blending, and waits its second value in frames. `jump`,
`stop` and `end` act as in the tile animation.

## How the build makes a map

`tools/ags-graphics` turns each table into the bytes the game reads, choosing
what to do from the name of the file it is asked to write,
`NAME.FORM[.CODEC]`, made from `NAME.tsv` (the `Makefile`'s asset rules). A
map uses these forms:

- `table`: the shapes and markers, one record per row, each column's type
  giving its size.
- `plane`: a grid plane, written out whole, row by row, with `fill` in every
  cell the table leaves out.
- `parts`: the grid, the encodings of the files it lists one after another.
- `delta0`, `delta1`, `delta2`: the blocks, halfwords behind a method byte
  (`enum MapBlockPacking`) that `TileMap_DecodeStagedBuffer` undoes. Method 0
  stores them as they are, 2 XORs each with the one before, and 1 does the
  same with all the high bytes stored before all the low bytes. Each `map.s`
  names the method its stream uses.
- `script`: the tile animation and blend scripts above.

The codec `lz` then packs each stream as Camelot's packer did, and the bytes
must come out identical. Where the original stream is followed by stray bytes
the packer left in its buffer, `map.s` places them as literal bytes with a
`PACKER:` comment naming what wrote them. Where Camelot changed a stream after
packing it, a `_patch.tsv` lists each changed byte: its position, the byte
the packer saw, the byte that shipped, and the proof. The table holds what
shipped, and the patch gives the packer the byte it packed, so the packer
makes the same choices and the stream still decodes to the edited map. Only
Gondowan Passage's grid and Venus Lighthouse's entrance's grid, with its
Spanish and French versions, have patches.

An edition's own version of a table (`grid_es.tsv`, `markers_ja.tsv`) is
chosen in `map.s` with `.ifdef TBS_EDITION_*`, and editions that agree share
one file: Venus Lighthouse's entrance has a French grid and a Spanish one, and
the Italian edition uses the Spanish. A map whose whole data differs by
edition has a block of its `map.s` for each. A map that uses another map's
table names that map's file.

## The world map's data

The world map's ground, `world/blocks.tsv` (`Field_PerspectiveDataA`), is one
grid of 32-bit values in hexadecimal, 512 across and 320 down. The `blocks`
form cuts it into 640 squares of 16 by 16 values and packs each separately
behind a table of offsets; a square written all as `-` would be left out with
an offset of 0, and the world map leaves none out. The game unpacks only the
2 by 2 squares around the party, in each of its two ground layers
(`Map_UpdateCurrentTileBlock`, `Map_WriteLayerCellTile`).

The tables in `default/` are the world map's too.
`Map_InitializePerspectiveScene` loads the default cells, block attributes and
tile animation when the world map scene starts, and
`Map_LoadDefaultCellsAndUpdateBlock` puts the cells back after the world map's
still view ([Field](../field/README.md#the-world-map-view)).

## The Colosso paths

In each Colosso final the opponent replays a recorded path. `colosso/` holds
three, each 7,200 points of ground in pixels, the size of the race's path
buffer (`ColossoRaceWork.path`), with a row of two zeros where the run ends.
The tables call the columns x and y; the race code reads each pair as x and
z. The Azart match runs path A, the Satrage match path B and the Navampa match
path C (`Colosso_StartOpponent`, `ColosseumSatrageMatch_StartOpponentPath`).

Each frame `Colosso_UpdatePathOpponent` steps the opponent to the next point,
mirrored across the finish when asked, turning at most a sixteenth of a turn,
and at the end walks it to the finish. Its recording mode writes down where
an actor stands every frame and then plays it back with the opponent. The
Colosso lettering, which differs by edition, is placed after the paths.

## Design

The grid is stored as whole planes rather than as cell words, which keeps
each plane's long runs of one value together for the packer. XXX: an
inference; the bytes show only the layout.

The markers are the one part stored unpacked, and the loader reads them in
place rather than copying them. XXX: likely because only
`Field_PlaceMapMarkers` reads them, once as the scene loads; nothing else
shows why.

One grid serves all three layers, so every map takes the same 64 KB of cells,
and a layer's window can sit anywhere in the grid.

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 blocks a map: a cell's block number has 12 bits, and 4,095 is the
  placeholder.
- 256 shapes a map, since a cell names its shape in one byte.
- 16 tile animation channels (`struct MapState`).
- `MARKER_MAX` (10) chests and Psynergy Stones placed on a map.

## Evidence and open questions

The game keeps no names for any of this, so each part is named for what the
code does with it: the blocks become BG map entries, so they are `metatiles`;
the shapes feed the height functions; and the markers are where
`Field_PlaceMapMarkers` puts chests and Psynergy Stones. The sixth offset is
called the marker list because that function is the only code that reads the
pointer the loader stores to it, although `MapScrollWork`
(`include/map_scroll.h`) still names the field `script`.

The header's last two bytes (`pad0` and `pad1` in `map.inc`, `reserved` in
`struct LayeredSceneHeader`) are 0 in every map of both games, and nothing
reads them: the loader is the only code that reads the header.
