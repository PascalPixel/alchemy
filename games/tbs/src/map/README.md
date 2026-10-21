# Maps

Every town, dungeon and house the party walks through stands on a map: a grid
of 16-pixel squares, each saying what's drawn there, what shape the ground
has and whether the party can walk on it. All of The Broken Seal's maps are here as
tables you can read: 182 layered maps for the towns, dungeons and interiors,
the world map's ground, and the paths the rivals run in the Colosso finals.

There's no code here. The field engine loads and draws the maps
(`Map_LoadLayeredScene`, in
[`../field/field/map_layers.c`](../field/field/map_layers.c)), and each scene
says which map it stands on: its row of the scene table (`gField_SceneTable`,
in [`../field/scenes.s`](../field/scenes.s)) gives a map number, and that
number picks a row of `gMap_LayeredScenes`
([`../field/layered.tsv`](../field/layered.tsv)), which names the map's
header, palette, three tile sheets and layer data.

The [Field](../field/README.md) guide covers the engine that loads and draws
the maps, [Field objects](../object/README.md) the code that places the chests
and Psynergy Stones a map marks, and [Scenes](../scene/README.md) the places
themselves.

## Files

| Path | What it holds |
| --- | --- |
| `map_NNN/` | one layered map, 180 of them: its `map.s` and the tables it places |
| `gondowan_passage/`, `venus_entrance/` | two more layered maps, Gondowan Passage and Venus Lighthouse's entrance, named for their places and built like the others |
| `world/blocks.tsv` | the world map's ground (`Field_PerspectiveDataA`) |
| `default/` | the default cells, tile animation and block attributes the world map loads (`Field_DefaultMapCells` and its kin) |
| `colosso/` | the rivals' recorded paths in the Colosso finals, with the Colosso lettering placed after them |
| `map.s` | places `world/` and `default/` |
| `map.inc` | the `map_header`, `map_layer` and `map_block` macros every `map.s` uses |

Almost all of it is tables: 1,342 `.tsv` files, next to 184 `.s` files and
`map.inc`, 1,527 in all besides the README. When you build, the encoded form
of each table is written beside it, about 780 files more that git ignores, so
a built tree holds 2,308 files here.

## Cells, blocks and shapes

A map is cut into **cells**, squares of 16 by 16 pixels, and each cell says
three things: which picture is drawn there, what shape the ground has, and
its collision code. Once the map is loaded, a cell is one 32-bit word, four
bytes (`struct MapCell` in `include/map.h`). Its first two bytes, the low
halfword, hold the block number in their low 12 bits and four flag bits above
it; the third byte is the collision code and the fourth the shape.

A **block** (a metatile, in ROM-hacking terms) is the picture: 2 by 2 of the
GBA's 8-pixel tiles, stored as the four background map entries that draw
them, top left, top right, bottom left and bottom right.

A **shape** is the form of the ground in a cell: a row of `shapes.tsv`, four
bytes. The low four bits of the first byte pick one of sixteen terrain kinds
(flat ground, slopes, steps, ground shaped through three heights, and a few
more), and the other three bytes are height samples, in steps of eight
pixels, which the kind's height function fills in between
(`Map_GetTerrainHeight`, through `gMap_TerrainHeightFunctions`).

The **collision code** says what standing on the cell means, and
`enum MapCellCode` in [`map.h`](../../include/map.h) names each one. `MAP_CELL_WALL` is a
wall. The codes from `MAP_CELL_EVENT_FIRST` to `MAP_CELL_EVENT_LAST`, 1 to
239, belong to the scene: a scene's touch event fires when the leader steps
onto or pushes against a cell with its code, and its search event when the
player presses A facing one. The codes above them mean the same thing on
every map: ground where battles can start, the jar, barrel, wooden box,
grave, books and well that every map searches with the same lines, the
desert's hazardous sand, and the ledges, climbing spots and jumps that
`Field_RunPushTileEvent` acts on when the leader pushes against them.

![One screen of Goma Cave, and beside it the same screen with its ground cells drawn over it, each raised by its height as the actors standing on it are: white edges where the ground steps up or down run along the rock rims around the dark floor, the cells of the two doorways that lead on are yellow with the scene's own codes, and a wall cell at the right edge is red.](https://alchemy.pascalpixel.com/guides/map/cells.png)

A **layer** is one of the map's three backgrounds, each a window onto the
cell grid. A **plane** is one byte, or one halfword (two bytes), per cell
over the whole 128 by 128 grid, written as a table whose first line describes
it: `cell:u8	grid:128x128	fill:0	at:2,3` gives the size of each value, the
grid, the value of every cell the table leaves out, and the cell its first
row starts at, column then row.

## Reading a map's tables

Each layered map keeps its tables together in one directory:

| File | What it is | Built as |
| --- | --- | --- |
| `map.s` | the header, the three layers and six offsets, then the blocks; in 129 of the 182 maps, a tile set's palette and tile sheets follow (built from `games/tbs/src/graphics/tileset`) | assembled |
| `metatiles.tsv` | the blocks: four tile entries each, top left, top right, bottom left, bottom right | `metatiles.delta0.lz`, `.delta1.lz` or `.delta2.lz` |
| `shapes.tsv` | the ground shapes: a kind and three height samples each | `shapes.table.lz` |
| `grid.tsv` | the cell grid's three planes, in order | `grid.parts.lz` |
| `grid_collision.tsv`, `grid_shape.tsv`, `grid_metatile.tsv` | the planes: each cell's collision code, shape and block | parts of the grid |
| `animation.tsv` | the tile animation channels | `animation.script.lz` |
| `blend.tsv` | the blend script | `blend.script.lz` |
| `markers.tsv` | the cells where chests and Psynergy Stones stand | `markers.table` |
| `*_ja.tsv`, `*_de.tsv`, `*_es.tsv` and so on | an edition's own version of a table | chosen in `map.s` |
| `*_patch.tsv` | the bytes Camelot changed after packing | read by the packer |

The three planes are where a map's layout lives. Walls and steps are a cell's
collision code in `grid_collision.tsv` together with its shape in
`grid_shape.tsv`, and what's drawn there is its block in `grid_metatile.tsv`.
All three are written in hexadecimal, one row of the grid per line, starting
at the plane's `at` cell. A block's number is simply its row in
`metatiles.tsv`, counting from 0, and that's the number the cells use.

## How a layered map works

### The header

A map starts with a small header, written in `map.s` as one `map_header` line
and three `map_layer` lines (the loader reads it as `struct
LayeredSceneHeader`). It gives the camera's bounds in 8-pixel tiles, each
layer's priority and the tile block (char block) its tiles come from, and for
each layer its origin, its parallax (how far it moves, in sixteenths of the
view's motion), its drift (sixteenths of a pixel a frame, for a layer that
keeps moving while the camera stands still) and its wrap.

Six offsets follow, each counted from the header's start: the blocks, the
shapes, the cell grid, the tile animation, the blend script and the markers.
The tile animation copies new tiles over old ones on a timer, for the parts
of a map that move by themselves, and the blend script changes how the layers
blend over time. An offset of 0 means the map has none of that part.

### Loading a map into memory

`Map_LoadLayeredScene` unpacks each part into a fixed buffer in the GBA's
work RAM: the blocks into `gMapBlocks`, the shapes into `gMapCollision`, the
grid into `Ram_MapCellBuffer`, and the two scripts into the space after the
shapes. The markers are the exception: they stay where they are, and the
loader keeps a pointer to them.

The grid is stored as three planes, one after another: collision codes, then
shapes, then blocks (16 KB, 16 KB and 32 KB, since the block plane has a
halfword per cell, the block number with its flag bits, and the others a
byte). Keeping each plane whole keeps its long runs
of one value together for the packer (XXX: an inference; the bytes show only
the layout). `Tile_ConvertMap`, run from a copy in RAM, then weaves them
together into cell words, 128 by 128 of them: 64 KB from the start of `Ram_MapCellBuffer`.

That's more than `gMapCellBuffer`'s own 32 KB. The cells run on through
`gActorSpriteSlots` and `gDecodeBuffer`, so the field can't use those two for
anything else while a map is loaded. A battle reuses that memory, and the
scene's map is loaded again when the party returns.

### One grid, three layers

The GBA draws a screen from up to four background layers stacked on top of
each other, and a layered map uses three of them. They all share the one
grid. Each layer's origin picks the cell its window starts at, so a map lays
its layers out side by side or one above another in the grid: map 000's three
layers start at tiles 0, 60 and 120 across, and Venus Lighthouse's entrance
puts its layers at (0, 0), (64, 0) and (0, 68).

![The whole 128 by 128 cell grid of Goma Cave's map at half size, each cell drawn from its block: the ground layer's window at the top left holds the cave floor and its pool, the middle layer's below it the rock walls around a dark floor, and the overhead layer's at the top right only the glows of the torches, and the rest of the grid is empty.](https://alchemy.pascalpixel.com/guides/map/grid.png)

The ground layer (`MAP_LAYER_GROUND`) shows on `BG3`, the middle layer on
`BG2` and the overhead layer on `BG1`, and `BG0` stays free for the windows. A layer's priority byte is also its switch: a layer with
priority 0 isn't shown. Most maps use 3, 2 and 1; nine give 0 to their second
and third layers and show only the first.

### Ground belongs to a layer

Collision and height belong to a layer too. Code that asks about the ground
passes a layer (an object's `terrainId`), reads the cell at that point in that
layer's window, and looks its shape up in `gMapCollision`; before the map work
exists, it reads the grid from its start. So walls and heights differ from
one layer to the next, and an object stands in one layer at a time.

### Chests and Psynergy Stones

The sixth part of a map, its marker list, says where chests and Psynergy
Stones can stand. It's a run of (column, row, id) triples, a cell and the id
of a scene event, ended by 255, 255. When a scene loads,
`Field_PlaceMapMarkers` walks the list and places an object at each marker
whose id (100 to 239) the scene has an event for, up to ten of them. A chest
event gets a chest, shown open once its flag is set and gone once
its mimic has been beaten; a search event that gives a Psynergy Stone gets the
stone, until it's taken. A marker the scene has no event for takes
no slot.

### Placeholder blocks

A cell whose block is 4,095, the highest a cell can name, is a placeholder. As the map is
converted, each placeholder is given a fresh block number, counting up
from 0 in map order; the ROM also keeps an unused C version of that numbering,
`Tilemap_NumberPlaceholderBlocks`. XXX: what fills those blocks isn't known.

## How the build makes a map

Every table here is turned into the bytes the game reads by
`tools/ags-graphics`, which works out what to do from the name of the file
it's asked to write: `NAME.FORM[.CODEC]`, made from `NAME.tsv` (the
`Makefile`'s asset rules). A map needs only a handful of these forms.

The shapes and the markers are `table`s: one record per row, and each
column's type says how many bytes it takes. A grid plane is a `plane`, written
out whole, row by row; since a table names only the rectangle that holds
data, every other cell gets the table's `fill`. `grid.tsv` itself is `parts`,
which just puts the encodings of the files it lists one after another, here
the three planes.

The blocks are written as `delta0`, `delta1` or `delta2`: halfwords behind a
byte that says how they were stored, which `Tilemap_DecodeStagedBuffer`
undoes. With method 0 they're stored as they are. Method 2 XORs each with the
one before it, and method 1 does the same but stores all the high bytes first
and then all the low bytes. Each map's `map.s` names the method its stream
uses, so the build never has to guess.

The tile animation and blend scripts are `script`s, a run of halfwords.
`channel n` and `jump n` are a prefix plus n (`0xfd00 + n` and `0xfe00 + n`),
`stop` is `0xfeff` and `end` is `0xffff`, and `frame`, `control` and `level` are
written as their values alone, with no command word in front.

In a tile animation script, `channel n` starts channel n right after it, each
`frame` is four values (the source tile, how many tiles, the destination tile
and the frames to wait), `jump n` goes to halfword 2n of the channel's script,
`stop` halts the channel and `end` loops it back to its start. A source tile
numbered 1,536 or more is copied from RAM and a lower one from elsewhere in
video memory (the line is 512 when a map's animations copy 64-byte tiles).
In a blend script, `control` sets the blend control register (`BLDCNT`), and
`level` sets the blend amounts and then waits its second value in frames.

The codec `lz` then packs the result the way Camelot's packer did, and the
bytes have to come out identical. Two kinds of record make sure they do.
Where the original stream is followed by a few stray bytes the packer left in
its buffer, `map.s` places them as literal bytes with a `PACKER:` comment
saying what wrote them. Where Camelot changed a stream after packing it, a
`_patch.tsv` beside the table lists each changed byte: its position, the byte
the packer saw, the byte that shipped, and the proof. The table holds what
shipped and the patch gives the packer the byte it packed, so the packer makes
the same choices and the stream still decodes to the edited map. Only two maps
have patches: Gondowan Passage's grid, and Venus Lighthouse's entrance, whose
grid and its Spanish and French versions each have one.

### Editions

An edition's own version of a table (`grid_es.tsv`, `markers_ja.tsv`) is
chosen in `map.s` with `.ifdef TBS_EDITION_*`. Editions that agree share one
file: Venus Lighthouse's entrance has a French grid and a Spanish one, and the
Italian edition uses the Spanish. A map that uses another map's table names
that map's file, and 76 maps place `map_000/markers.table`, which holds
nothing but the end marker.

## The world map and the Colosso paths

The world map is drawn in perspective, on two background layers the GBA can
scale and rotate, and its data is laid out differently. Its ground,
`world/blocks.tsv` (`Field_PerspectiveDataA`), is one big grid of 32-bit cells
in hexadecimal, 512 across and 320 down. The `blocks` form cuts it into 640
squares of 16 by 16 cells and packs each one separately behind a table of
offsets. (The form lets a square be left out, written all as `-`, with an
offset of 0; the world map leaves none out.) The game unpacks only the squares
it needs, the 2 by 2 around the party in each of its two ground layers
(`Map_UpdateCurrentTileBlock`, `Map_WriteLayerCellTile`).

The tables in `default/` are the world map's too.
`Map_InitializePerspectiveScene` loads the default cells, block attributes and
tile animation when the world map scene starts, and
`Map_LoadDefaultCellsAndUpdateBlock` puts the cells back after the world map's
still view. The world map itself, and that still view, are in the
[Field](../field/README.md) guide.

In the Colosso finals Isaac races a rival over each course, and the rival
doesn't decide anything: it replays a recorded path. `colosso/` holds three of
them, each 7,200 points on the ground in pixels, the size of the buffer the
race keeps a path in, with a row of two zeros where the run ends. The tables
call their two columns x and y; the race code reads each pair as x and z, a
spot on the ground (`struct ColossoRaceWork`). Every frame the
rival steps to the next point, mirrored across the finish (it starts opposite
Isaac), turning at most a sixteenth of a turn at a time, and at the end it
walks to the finish. The River course runs path A, the Wall course path B and
the Log course path C (`Colosso_StartRival`, `ColosseumSatrageMatch_StartRivalPath`).
The race code still has a recording mode, which writes down where an actor
stands every frame and then plays it back with the rival. The Colosso
lettering, which differs by edition, is placed after the paths.

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 blocks a map: a cell's block number has 12 bits (0 to 4,095), and
  4,095 is the placeholder.
- 256 shapes a map, since a cell names its shape in one byte.
- 16 tile animation channels (`struct MapScrollWork`).
- Up to ten chests and Psynergy Stones placed on a map
  (`Field_PlaceMapMarkers`).

## Evidence and open questions

The game keeps no names for any of this, so each part is named for what the
code does with it. The blocks become background map entries, so they're
`metatiles`; the shapes feed the height functions; and the markers are where
`Field_PlaceMapMarkers` puts chests and Psynergy Stones. The sixth offset
needs a word more. The loader doesn't unpack that part: it stores a
pointer to it in the map work (`LayeredSceneHeader.markers`), and the only
code that ever reads the pointer is `Field_PlaceMapMarkers`, which reads
(column, row, id) triples from it. That's why it's called the marker list,
even though `MapScrollWork` (`include/map_scroll.h`) still names the same
field `script`. The map numbers are only the order of
the directories, not numbers the game gives its maps.

Still unknown:

- XXX: what the placeholder blocks are filled with.
- XXX: what the header's last two bytes mean (`pad0` and `pad1` in
  `map.inc`, `unk0a` in the loader, which doesn't read them); every map has
  them 0.
- XXX: the cell map fills 64 KB through `gActorSpriteSlots` and
  `gDecodeBuffer`, yet the tile animation copies tiles from `Ram_DecodeBuffer`
  plus their number (from 1,536 on, which lands past it, in `gMapLayerData`).
  Which code may use those two buffers while a map is loaded hasn't been
  traced.
