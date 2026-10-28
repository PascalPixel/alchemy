# Maps

Every town, dungeon and [room](../../glossary.md#room) the party walks through
is drawn from a [map](../../glossary.md#map): a square grid of 16-pixel
[cells](../../glossary.md#cell), 128 on a side, that holds all three of its
layers. Each layer is a window onto that grid, starting at a cell the map's
header gives, and the cells no layer reaches go unused.

182 maps, the World Map's ground and the Colosso paths sit here as editable
tables, with no code. A [scene](../../glossary.md#scene) names its map, and
[the field bank](../field/README.md) draws and scrolls it.

## Cells

A cell is one 16 by 16 pixel square of a map, and it says three things:
what is drawn there, what standing on it does, and how its ground is shaped.
A ledge is all three: a picture of a ledge, cells that say "jump down when
pushed", and ground 32 pixels higher than the ground below.

What is drawn is the cell's [cell picture](../../glossary.md#cell-picture),
2 by 2 of the console's 8-pixel tiles. A cell names its cell picture by
number, a row of the map's table of cell pictures.

What standing on it does is its [cell behavior](../../glossary.md#cell-behavior),
a number that walls and [triggers](../../glossary.md#trigger) are keyed on.
One number is a wall whatever the heights. Most belong to the scene: a
[step trigger](../../glossary.md#step-trigger) runs when the leader steps onto
or pushes against a cell with that number, and an
[examine trigger](../../glossary.md#examine-trigger) when the player presses A
facing one, so the same number can be a door in one town and a chest in
another ([Scenes](../scene/README.md#which-triggers-a-cell-starts)). The rest
mean the same in every scene: ground with random battles of its own, the jars,
barrels, wooden boxes, graves, books and wells anyone can search, the Lamakan
Desert's burning sand, and the ledges, climbing spots and gaps the leader jumps
when pushing against them.

How the ground is shaped is the cell's shape: one of 16 kinds
(`enum TerrainKind`): flat ground, slopes, steps, bends and others, with three
height samples in 8-pixel units, from which the height anywhere in the cell is
worked out. How far up or down an actor can step from one height to the next
is the field bank's rule
([How high a step can go](../field/README.md#how-high-a-step-can-go)).

Behavior and height belong to a layer. Each layer is a window onto the grid
([below](#one-grid-three-layers)), and an actor follows the ground of one
layer, so the three layers can have different walls and heights. A scene's
code can move an actor to another layer, and so can a cell: an actor standing
on a cell whose layer bits are set moves to that cell's layer.

A loaded cell is `struct MapGridCell`, one 32-bit word: 12 bits pick the cell
picture, 2 bits the layer and 2 the sprite priority an actor standing there
takes (0 for no change), a byte the behavior and a byte the shape. The
behaviors are `enum CellBehavior`:

| Name | Value | Meaning |
| --- | --- | --- |
| `CELL_BEHAVIOR_NORMAL` | 0 | open ground: nothing happens |
| `CELL_BEHAVIOR_EVENT_FIRST` to `CELL_BEHAVIOR_EVENT_LAST` | 1 to 239 | the scene's own step triggers, [exits](../../glossary.md#exit) and examine triggers |
| `CELL_BEHAVIOR_ENCOUNTER_GROUND_1`, `_2` | 240, 241 | ground with random battles of its own |
| `CELL_BEHAVIOR_JAR` to `CELL_BEHAVIOR_WELL` | 242 to 247 | search spots with lines every scene shares: a jar, a barrel, a wooden box, a grave, books, a well |
| `CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED` | 249 | jumps forward like `CELL_BEHAVIOR_JUMP_FORWARD`, but walking never passes it to the push code; a leader who lands on one jumps again |
| `CELL_BEHAVIOR_DESERT_SAND` | 250 | ground that hurts the [party](../../glossary.md#the-party) over time: the Lamakan Desert's sand |
| `CELL_BEHAVIOR_LEDGE` | 252 | jumped down with Down held |
| `CELL_BEHAVIOR_CLIMB` | 253 | climbed up or down with Up or Down held |
| `CELL_BEHAVIOR_JUMP_FORWARD` | 254 | jumped across |
| `CELL_BEHAVIOR_WALL` | 255 | a wall, whatever the heights |

## A map's tables

Everything that makes one town or dungeon map, from its cell pictures to where
its chests stand, is kept as a handful of plain tables in the map's own
folder. Each table holds one piece of the map, and the map's `map.s` puts them
together behind its header.

| File | What it is |
| --- | --- |
| `map.s` | the header, then each table's packed stream, and the treasure spot list as `treasure_spot` lines ended by `treasure_spots_end`; in 129 maps a tileset's palette and tiles follow (from `games/tbs/src/graphics/tileset`) |
| `cell_pictures.tsv` | the cell pictures; a cell picture's number is its row, from 0 |
| `shapes.tsv` | the shapes of the ground |
| `map.txt` | the grid: each cell's cell picture, behavior and shape |
| `tileset_anims.tsv` | the tileset animation script |
| `blend.tsv` | the blend script |
| `map_es.txt`, `cell_pictures_ja.tsv`, `shapes_es.tsv` and so on | an [edition](../../glossary.md#editions)'s own version of a table |
| `*_patch.tsv` | bytes [Camelot](../../glossary.md#camelot) changed after packing |

`map.txt` holds one line of the grid per row, each cell written
`picture:behavior:shape` in hexadecimal, a field left off the end being 0
(`4fff:ff`, `20`). Its first line gives the grid's size, the cell its first
written row starts at (column, then row), and the cell every cell the rows
leave out holds; Vale Plaza's (`map_003`) is `grid:128x128	at:0,0	fill:2b:0:0`.
Every map's rows start at 0,0.

### The header

The header says, for each layer, where its window starts in the grid, how fast
it follows the camera, whether it drifts by itself and where it wraps, and,
for the whole map, how far the camera may go. A layer that follows the camera
at the camera's own pace looks as far away as the ground; one that moves
faster looks nearer. In the north of Suhalla Desert (`map_147`) the overhead
layer is a haze of sand that follows the camera a quarter faster than the
ground, drifts across the screen by itself and wraps every 256 pixels. Only
three maps drift a layer: the north and south of Suhalla Desert and the Tolbi
ship's deck ([below](#evidence)).

Six offsets follow the header: where the cell pictures, the shapes, the grid,
the tileset animation, the blend script and the treasure spots start. The
tileset animation and blend offsets are 0 in a map that has none; the other
four are always present.

The header is one `map_header` line and three `map_layer` lines in `map.s`,
read as `struct MapHeader` and `struct MapLayer`. The last column gives the
Suhalla haze's values.

| Field | Unit | Meaning | The Suhalla haze |
| --- | --- | --- | --- |
| `cameraBounds[4]` | 8-pixel tiles | how far the camera may go: left, top, right and bottom | |
| `priority[3]` | | each layer's priority, 1 to 3; 0 leaves the layer off | |
| `charBase[3]` | | each layer's character block | |
| `reserved` | | always 0, and never read | |
| `layers[n].x`, `.y` | 8-pixel tiles | where the layer's window starts in the grid | 128, 0 |
| `layers[n].scrollX`, `.scrollY` | sixteenths of the camera's motion | how fast the layer follows the camera | 20, 20: a quarter faster |
| `layers[n].speedX`, `.speedY` | sixteenths of a pixel a frame | the layer's drift | -121, 35: about 7.6 pixels right and 2.2 up a [frame](../../glossary.md#frame) |
| `layers[n].periodX`, `.periodY` | 8-pixel tiles, less one | where the layer wraps | `0x1f`: every 32 tiles |

## How the build makes a map

The tables are for people to read and edit; the game wants packed bytes. The
build packs each table into the stream the game unpacks, choosing what
Camelot's packer chose.

The name of the file the build asks for names the form to make, and
`tools/ags-graphics` makes it. `map.txt` becomes `map.grid.lz`: the grid as
three packed planes, one value a cell for the whole grid. `cell_pictures.tsv`
becomes `cell_pictures.delta1.lz`: the cell pictures stored with each value
XORed with the one before, which packs better, in the form the file name
gives. Where [the original](../../glossary.md#the-original) stream is followed
by stray bytes the packer left in its buffer, `map.s` places them as literal
bytes with a `PACKER:` comment naming what wrote them.

Where Camelot changed a stream after packing it, a `_patch.tsv` lists each
changed byte. The table holds what shipped, and the patch gives the packer
the byte it packed, so the packer makes the same choices and the stream still
unpacks to the edited map. The first row of Gondowan Passage's grid patch:

```text
position  pack  ship  proof
65472     0x02  0x01  literal: 254 of 255 stand-ins reproduce the stream; this is the lowest
```

The byte at that position shipped as `0x01`. The packer must have seen
something else there, since `0x01` would pack differently; any of 254 values
gives the shipped stream, and the patch names the lowest, `0x02`. The proof
column says how the packed byte was found, and a later row that the packer
copied from an earlier one says `copy of` that row's position. Only Gondowan
Passage's grid and Venus Lighthouse Entry's grid, with its Spanish and French
versions, have patches.

An edition's own version of a table (`map_es.txt`, `cell_pictures_ja.tsv`)
is chosen in `map.s` with `.ifdef` on the edition's name
(`TBS_EDITION_ES`), and editions that agree share one file: Venus Lighthouse
Entry has a French grid and a Spanish one, and the Italian edition uses the
Spanish. A map whose whole data differs by edition has a block of its
`map.s` for each. A map that uses another map's table names that
map's file.

The forms are named in the `Makefile`'s asset rules, and
`enum CellPicturePacking` names the ways the cell pictures are stored, which
`TileMap_DecodeStagedBuffer` undoes.

## Loading a map

When the party enters a town or dungeon, the map's layers and
[sprites](../../glossary.md#sprite-and-oam) are hidden while its map is
unpacked into working memory. Most of what is unpacked is the grid, which is
stored as three separate planes and put together into one word per cell as it
loads, so that everything about a cell can be read in one go.

The cells take 64 KB, twice the room set aside for them, so they run on over
two other buffers, and neither can hold anything else while a map is
loaded. Only code that loads the cells again afterwards writes there: a
battle, after which the scene's map is loaded again; the Battle Arena, which
leaves the partner's party there for the link battle that follows; the World
Map's still picture; and the debug sprite viewer.

Each scene's row of `gScenes` ([`field/scenes.s`](../field/scenes.s)) names a
row of `gMaps` ([`field/maps.c`](../field/maps.c)), which gives the map's
header, palette and tiles. A map's folder is named after its header's label in
the resource directory
([`cartridge/resource/directory.s`](../cartridge/resource/directory.s)), not
after its row of `gMaps`, so the two numbers differ: the north of Suhalla
Desert is row 151 of `gMaps` and folder `map_147`, and Vale is row 3 and
folder `map_002`.

`MapGrid_LoadMap` unpacks each stream, and `TileMap_ConvertBuffer` puts the
three planes together into the cell words in `gMapGridCellBuffer`.
[Cartridge and memory](../cartridge/README.md) has the memory's map.

| Plane, in stored order | Size | Values |
| --- | --- | --- |
| cell behaviors | 16 KB | a byte a cell |
| shapes | 16 KB | a byte a cell |
| cell pictures | 32 KB | a halfword a cell |
| the cell words they make | 64 KB | a word a cell |

### One grid, three layers

![Vale's screen in front of the Sanctum taken apart: the ground, the middle layer, the sprites and the overhead layer spread out one behind another, then close up into the picture the player sees](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

A town's three layers are drawn one over another, but in memory they are side
by side: all three are windows onto the one grid. Each layer's place in the
header, in 8-pixel tiles, picks the cell its window starts at, so a map lays
its layers out side by side or one above another. In Vale in the prologue
(`map_000`) the ground, middle and overhead layers start at tiles (0, 0),
(60, 0) and (120, 0), all in a row; in the Goma Cave room with two pillars and
a stump (`map_022`) at (0, 0), (0, 68) and (100, 0); and in Venus Lighthouse
Entry at (0, 0), (64, 0) and (0, 68). A window has no size of its own: the
camera's bounds stop the view, so a layer shows the cells from its start out
as far as the camera can go.

The ground layer shows on `BG3`, the middle layer on `BG2` and the overhead
layer on `BG1`; [the field bank](../field/README.md#towns-and-dungeons-on-screen)
explains how their priorities put [actors](../../glossary.md#actor) between
them.

### Placeholder cell pictures

A map's grid doesn't store the number of every cell's cell picture from the
start. The first time a cell picture appears in the grid, the cell holds a
placeholder instead, and the game hands out cell picture numbers in order as
it loads the map. Each map's table of cell pictures is ordered by where they
first appear, so a map has exactly as many placeholders as cell pictures
(`map_003` has 1,331 of each). Every map follows the rule but Venus
Lighthouse Entry, whose grid names 170 cells' cell pictures before their
placeholders; those cells still get the right cell pictures, since the
numbering depends only on the placeholders.

A placeholder is cell picture 4,095, the highest 12 bits can hold.
`Tile_ConvertMapGrid`, an assembly routine `TileMap_ConvertBuffer` copies to
RAM and runs, numbers them as it puts the cells together; an unused C
routine, `TileMap_NumberPlaceholderCellPictures`, does the numbering alone.

### Chests and Psynergy Stones

Several scenes can share one map, and each puts its own chests in it. The
map lists the spots where a chest or a
[Psynergy](../../glossary.md#psynergy-and-pp) Stone can stand, each with the
subject of the trigger that would place one there (100 to 239), and a scene
places one only where it has a trigger with that subject. A chest is shown
open once it has been opened and is gone once its mimic has been beaten; a
Psynergy Stone stands until it is taken, and taking it fills every party
member's PP.

Crossbone Isle's keys room and its Bane room share `map_110`, whose eight
treasure spots carry the trigger subjects 100 to 107: the keys room has chest
triggers for 100 to 103, and the Bane room for 104 to 107. The Demon Axe's
chest rides on the Bane room's second stone pillar: the room moves spot 107
out of the way as it starts (`TreasureSpot_SetPosition`), then sets its
position to follow the pillar (`TreasureSpot_SetActorPosition`) until the
pillar reaches the ladder.

The treasure spot list is (column, row, subject) rows ended by 255, 255, and
`Field_PlaceTreasureSpots` places an actor at each spot whose subject the
scene has a trigger for, up to `TREASURE_SPOT_MAX` (10); a spot the scene has
no trigger for takes none of the ten. Every map has its own list, and 76 of
them hold only the end row. Opening a chest and taking a stone are the scene's
examine triggers ([Scenes](../scene/README.md#how-a-trigger-is-chosen)).

### Tileset animation and blend scripts

![A spot in Vale beside the 512 tiles of its ground layer's character block in grey, 32 to a row: every few frames one group of tiles in the block's lower half is rewritten by the tile animation and outlined in red, while the rest stay as they are](https://alchemy.pascalpixel.com/guides/map/tile-animation.gif)

Some parts of a map move without anything on the map moving: the game swaps
the pictures of a few tiles on a timer, as with Vale's waterfall. A map's
tileset animation script says which tiles to copy over which and how long to
wait between them; its blend script changes, in the same way, how the layers
mix. Up to 16 tileset animations run side by side, each run each frame as a
[task](../../glossary.md#task).

A tileset animation is a list of numbers: its start (the `channel` line), then
groups of four values (the tile to copy from, how many tiles, the tile to copy
to and the frames to wait), then a jump, a stop or a loop. Each `picture` line
copies one run of tiles, so Suhalla Desert's north needs four such lines for
each of the four 32 by 32 pictures its first animation shows: it copies the
four rows of a picture, waits 15 frames, takes the next, and starts again
after the fourth.

The blend script has its own commands: `control` sets how the layers blend
(`REG_BLDCNT`), and `level` sets the blend's strength (`REG_BLDALPHA`, or
`REG_BLDY` when the layers are not alpha blended) and waits a number of
frames; it ends with the same jump, stop and loop.

`TilesetAnim_Update` and `BlendScript_Run` run the scripts, which have no
names for their words; the names below are the tileset animation tables'.

| Word | Value | Meaning |
| --- | --- | --- |
| `channel n` | `0xfd00 + n` | tileset animation n starts at the next value; with bit 7 set it starts paused |
| `picture` | four values below `0xfd00` | source tile, tile count, destination tile, frames to wait |
| `jump n` | `0xfe00 + n` | continue at value 2n of the animation's script |
| `stop` | `0xfeff` | halt the animation |
| `end` | `0xffff` | loop back to the animation's start |
| a source tile from RAM | 1,536 on (512 for 256-colour tiles) | copied from unpacked tiles in RAM rather than from video memory |

## The World Map's data

The World Map is far too big to unpack at once, so its ground is cut into
squares that are packed one by one, and the game unpacks only the few around
the party, bringing in new ones as the party walks. The code calls a square a
world map tile, and the 2 by 2 squares around the party its tile block.

The world is 16 by 16 squares of 512 pixels. The ground is stored as two
sets of squares, one for the lower ground and one for the mountains, woods and
towns drawn over it. Each ground has 320 squares: the 256 the world starts
with, one per position, and 64 alternates that a World Map event can put in a
position (`WorldMap_SetWindowCellTile`).

`world/blocks.tsv` is the ground, one value for each 32-pixel cell of the
World Map, 512 across and 320 down, which the build cuts into the 640 squares,
each packed on its own. `WorldMap_UpdateCurrentTileBlock` unpacks the 2 by 2
squares around the party in each ground, and
`WorldMap_PreloadCurrentTileBlock` brings in at most one new square a frame.
The tables in `default/` are the World Map's too: its default cells, which
come back after its still picture
([the field bank](../field/README.md#the-world-map-in-perspective)).

## The Colosso paths

In each Colosso final Isaac races an opponent to the arena, and no rule
steers the opponent: it replays a run recorded once, one point a frame, which
`Colosso_UpdatePathOpponent` steps through from `ColossoRaceWork.path` (room
for 7,200 points). `colosso/` holds the three runs, each ending with a row of
two zeros.

| Table | Match | Points |
| --- | --- | --- |
| `opponent_path_a.tsv` | Azart | 3,918 |
| `opponent_path_b.tsv` | Satrage | 4,380 |
| `opponent_path_c.tsv` | Navampa | 5,048 |

## Design

The grid is stored as three whole planes rather than as cell words, and the
treasure spot list is the one table stored unpacked, which the loader leaves
in place for `Field_PlaceTreasureSpots` to read.

One grid serves all three layers, so every map takes the same 64 KB of
cells, and a layer's window can sit anywhere in the grid.

## How it fits with the rest of the game

When a scene starts, `FieldSequence_Run` loads the map its row of `gScenes`
names through `MapGrid_LoadMap`, then places the treasure spots
(`Field_PlaceTreasureSpots`) for the scene's triggers
([Scenes](../scene/README.md)). From then on
[the field bank](../field/README.md) draws and scrolls the three layers and
reads each cell's behavior and height as actors move. A battle reuses the
cells' memory, so when the party comes back the scene starts again and its map
is loaded again. Where the cells, shapes and cell pictures sit in memory is in
[Cartridge and memory](../cartridge/README.md).

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 cell pictures a map: a cell's cell picture number has 12 bits, and
  4,095 is the placeholder.
- 256 shapes a map, since a cell names its shape in one byte.
- 16 tileset animations (`struct TilesetAnim`).
- `TREASURE_SPOT_MAX` (10) chests and Psynergy Stones placed in a scene.

## Words used here

- A **shape** is the form of the ground in a cell: a terrain kind and three
  height samples.
- A **layer** is one of the map's three backgrounds, each a window onto the
  one cell grid.
- A **plane** is one value per cell over the whole 128 by 128 grid.
- A **treasure spot** is a cell where a chest or a Psynergy Stone can stand.

## Evidence

The game keeps no names for any of this, so each piece is named for what the
code does with it: the 16-pixel pictures are what each cell shows, so they
are cell pictures; the shapes feed the height functions; and the treasure
spots are where `Field_PlaceTreasureSpots` puts chests and Psynergy Stones.
The sixth offset is called the treasure spot list because that function is the
only code that reads the pointer the loader stores to it
(`MapGridScrollWork.markers`).

The haze's map is found from the scene table: `SceneId_SuhallaDesertNorth`'s
row of `gScenes` gives row 151 of `gMaps`, folder `map_147`; the south's row
leads the same way to `map_148`, and the Tolbi ship deck's to `map_103`, whose
ground drifts down the screen by a pixel and a half a frame and wraps only
every 2,048 pixels.

The last two bytes of the `map_header` line (`pad0` and `pad1` in `map.inc`,
`reserved` in `struct MapHeader`) are 0 in every map of both games, and
nothing reads them: the loader is the only code that reads the header.

The layer and priority bits of a cell are read through
`struct ActorCameraTile` by `ActorSystem_PlaceSprites`, which moves an actor
to the layer they give and, for an actor with `ACTOR_PRIORITY_AUTOMATIC`, sets
its sprite's priority.
