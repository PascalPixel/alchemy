# Layouts

A town or dungeon [layout](../../glossary.md#layout) is one big square grid of
16-pixel [cells](../../glossary.md#cell), 128 on a side, and each of its three
layers is a rectangle cut from that grid; the layout's header says where each
one starts, and the rest of the grid goes unused.

The `src/map` folder holds this data as editable tables, and no code: 182
layouts for the towns, dungeons and interiors, and two more sets of data kept
beside them because they are packed and placed the same way: the world map's
ground and the opponents' recorded runs in the Colosso finals. A
[map](../../glossary.md#map) names its layout, [Field](../field/README.md)
draws and scrolls it, and the sections below say how each part is stored,
built and loaded.

## Cells

![Isaac walks to the edge of a ledge in Bilibin Cave's entrance and, with Down held, jumps down to the ground below in a puff of dust](https://alchemy.pascalpixel.com/guides/layout/ledge.gif)

A cell is one 16 by 16 pixel square of a layout, and it says three things:
what is drawn there, what standing on it does, and how its ground is shaped.
The ledge in the recording is all three: a picture of a ledge, cells that say
"jump down when pushed", and ground 32 pixels higher than the ground below.

**What is drawn.** A [metatile](../../glossary.md#metatile) is the picture of
one cell, 2 by 2 of the console's 8-pixel tiles. A cell names its picture by
number, a row of the layout's table of pictures.

**What standing on it does.** The [metatile
behavior](../../glossary.md#metatile-behavior) is a number that walls and
[map events](../../glossary.md#map-event) are keyed on. One number is a wall
whatever the heights. Most belong to the map: a [coord
event](../../glossary.md#coord-event) runs when the leader steps onto or
pushes against a cell with that number, and a [bg
event](../../glossary.md#bg-event) when the player presses A facing one, so
the same number can be a door in one town and a chest in another
([Maps](../scene/README.md#which-map-events-a-cell-starts)). The rest mean the
same on every map: ground with random battles of its own, the jars, barrels,
wooden boxes, graves, books and wells anyone can search, the Lamakan Desert's
burning sand, and the ledges, climbing spots and gaps the leader jumps when
pushing against them.

**How the ground is shaped.** A cell's shape is flat ground, a slope, a step
or one of the other 16 kinds, with three heights in steps of 8 pixels, and
the height anywhere in the cell is worked out from them. An object can step
at most 8 pixels up or 12 down, so a sharp rise or drop stops the leader as
surely as a wall. Most of the game's walls are made this way: Goma Cave has no
wall cells at all, only a rim of ground 32 pixels high along the foot of each
cliff, some of it cut along a cell's diagonal.

Behavior and height belong to a layer. Each layer is a window onto the grid
([below](#one-grid-three-layers)), and an object follows the ground of one
layer, which a map's code can change, so the three layers can have different
walls and heights ([Field](../field/README.md#how-high-a-step-can-go)).

A loaded cell is `struct MapGridCell`, one 32-bit word: 12 bits
pick the metatile, four flag bits nothing reads ([below](#evidence)), a byte
the behavior and a byte the shape. The behaviors are
`enum MetatileBehavior`:

| Name | Value | Meaning |
| --- | --- | --- |
| (open ground) | 0 | nothing happens |
| `MB_EVENT_FIRST` to `MB_EVENT_LAST` | 1 to 239 | the map's own coord events, [warps](../../glossary.md#warp) and bg events |
| `MB_ENCOUNTER_GROUND_1`, `_2` | 240, 241 | ground with random battles of its own |
| `MB_JAR` to `MB_WELL` | 242 to 247 | search spots with lines every map shares: a jar, a barrel, a wooden box, a grave, books, a well |
| `MB_DESERT_SAND` | 250 | ground that hurts the [party](../../glossary.md#the-party) over time: the Lamakan Desert's sand |
| `MB_LEDGE` | 252 | jumped down with Down held |
| `MB_CLIMB` | 253 | climbed up or down with Up or Down held |
| `MB_JUMP_FORWARD` | 254 | jumped across |
| `MB_WALL` | 255 | a wall, whatever the heights |

## A layout's tables

Everything that makes one town or dungeon layout, from the pictures of its
squares to where its chests stand, is kept as a handful of plain tables in the
layout's own folder. Each table holds one part of the layout, and the layout's
`map.s` puts the parts together behind its header.

| File | What it is |
| --- | --- |
| `map.s` | the header, then the parts; in 129 layouts a tileset's palette and tiles follow (from `games/tbs/src/graphics/tileset`) |
| `metatiles.tsv` | the pictures of the cells; a picture's number is its row, from 0 |
| `shapes.tsv` | the shapes of the ground |
| `map.txt` | the grid: each cell's picture, behavior and shape |
| `tileset_anims.tsv` | the tileset animation script |
| `blend.tsv` | the blend [script](../../glossary.md#script) |
| `treasure_spots.tsv` | where chests and Psynergy Stones can stand |
| `*_ja.tsv`, `map_de.txt`, `*_es.tsv` and so on | an [edition](../../glossary.md#editions)'s own version of a table |
| `*_patch.tsv` | bytes [Camelot](../../glossary.md#camelot) changed after packing |

`map.txt` holds one line of the grid per row, each cell written
`metatile:behavior:shape` in hexadecimal, a part left off the end being 0
(`4fff:ff`, `20`). Its first line, `grid:128x128	at:2,3	fill:1:0:0`, gives
the grid's size, the cell its first written row starts at (column, then row),
and the cell every square the rows leave out holds.

### The header

![Isaac stands in the north of Suhalla Desert while a haze of sand grains streams across the screen, then walks south through it](https://alchemy.pascalpixel.com/guides/layout/haze.gif)

In Suhalla Desert a haze of sand streams across the screen faster than the
ground moves, and wraps round as it goes. The layout's header is what sets
that up: for each layer it says how fast the layer follows the camera, whether
it drifts by itself and where it wraps, as well as how far the camera may go.

A layer that follows the camera at the camera's own pace looks as far away as
the ground; one that moves faster looks nearer. The north of Suhalla Desert
(`SceneId_SuhallaDesertNorth`, whose row in the map table names the layout in
`map_147`) moves its ground and middle layers with the camera and its
overhead haze a quarter faster, drifts the haze about 7.6 pixels right and 2.2
up the screen every [frame](../../glossary.md#frame), and wraps it every 256 pixels.
The south of the desert (`map_148`) drifts its haze the same way. The haze
is there from the start, but the desert's own code keeps it invisible until
the first storm rises and then fades it in, which is why the recording shows the
desert as it is once a storm has risen. The only other drifting layer is the ground of the Tolbi ship's
deck (`map_103`), which drifts down the screen by a pixel and a half a frame and wraps only every 2,048 pixels.

Six offsets follow the header: where the pictures, the shapes, the grid, the
tileset animation, the blend script and the treasure spots start. An offset of
0 means the layout has none of that part.

The header is one `map_header` line and three `map_layer`
lines in `map.s`, read as `struct MapHeader` and `struct MapLayer`.

| Field | Unit | Meaning |
| --- | --- | --- |
| `cameraBounds[4]` | 8-pixel tiles | how far the camera may go: left, top, right and bottom |
| `priority[3]` | | each layer's priority, 1 to 3; 0 leaves the layer off |
| `layers[n].x`, `.y` | 8-pixel tiles | where the layer's window starts in the grid |
| `layers[n].scrollX`, `.scrollY` | sixteenths of the camera's motion | how fast the layer follows the camera |
| `layers[n].speedX`, `.speedY` | sixteenths of a pixel a frame | the layer's drift |
| `layers[n].periodX`, `.periodY` | 8-pixel tiles, less one | where the layer wraps: `0x1f` wraps every 32 tiles |

## How the build makes a layout

The tables are for people to read and edit; the game wants packed bytes. The
build turns each table into the bytes the game reads and packs them exactly as
Camelot's packer did, so the result is the same, byte for byte, as the
cartridge.

The name of the file the build asks for says what to make, and
`tools/ags-graphics` makes it: the grid becomes three packed planes, one value
a cell for the whole grid; the pictures of the cells are stored as the
differences from one value to the next, which pack better; and each stream is
then packed in the format the game unpacks. Where [the
original](../../glossary.md#the-original) stream is followed by stray bytes
the packer left in its buffer, `map.s` places them as literal bytes with a
`PACKER:` comment naming what wrote them.

Where Camelot changed a stream after packing it, a `_patch.tsv` lists each
changed byte. The table holds what shipped, and the patch gives the packer
the byte it packed, so the packer makes the same choices and the stream still
unpacks to the edited layout. The first row of Gondowan Passage's grid patch:

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

An edition's own version of a table (`grid_es.tsv`, `treasure_spots_ja.tsv`)
is chosen in `map.s` with `.ifdef` on the edition's name
(`TBS_EDITION_ES`), and editions that agree share one file: Venus Lighthouse's
entrance has a French grid and a Spanish one, and the Italian edition uses the
Spanish. A layout whose whole data differs by edition has a block of its
`map.s` for each. A layout that uses another layout's table names that
layout's file.

The forms are named in the `Makefile`'s asset rules, and
`enum MetatilePacking` names the ways the pictures are stored, which
`TileMap_DecodeStagedBuffer` undoes.

## Loading a layout

When the party enters a town or dungeon, the map's layers and
[sprites](../../glossary.md#sprite-and-oam) are hidden while its layout is
unpacked into working memory. Most of what is unpacked is the grid, which is
stored as three separate planes and put together into one word per cell as it
loads, so that everything about a cell can be read in one go.

The cells take 64 KB, twice the room set aside for them, so they run on over
two other buffers, and neither can hold anything else while a layout is
loaded. Only code that loads the cells again afterwards writes there: a
battle, after which the map's layout is loaded again; the Battle Arena, which
leaves the partner's party there for the link battle that follows; the world
map's still picture; and the debug sprite viewer.

Each map's row of `gSceneHeaders`
([`field/scene_headers.s`](../field/scene_headers.s)) gives a layout number,
which picks a row of `gMaps` ([`field/maps.c`](../field/maps.c)):
the layout's header, palette and tiles. `MapGrid_LoadMap` unpacks each
part, and `Tile_ConvertMapGrid` puts the three planes together into the cell
words in `gMapGridCellBuffer`. [System](../system/README.md) has the memory's
layout.

| Plane, in stored order | Size | Values |
| --- | --- | --- |
| metatile behaviors | 16 KB | a byte a cell |
| shapes | 16 KB | a byte a cell |
| metatiles | 32 KB | a halfword a cell |
| the cell words they make | 64 KB | a word a cell |

### One grid, three layers

![Vale's screen in front of the Sanctum taken apart: the ground, the middle layer, the sprites and the overhead layer spread out one behind another, then close up into the picture the player sees](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

A town's three layers are drawn one over another, but in memory they are side
by side: all three are windows onto the one grid. In Goma Cave's entrance room
the ground layer, the cave floor and its pool, starts at cell 0, 0; the middle
layer, the rock walls, at cell 0, 34, below it; and the overhead layer, only
the glow of the torches, at cell 50, 0, to its right. Each layer's place in
the header picks the cell its window starts at, so a layout lays its layers
out side by side or one above another: `map_000`'s start at tiles 0, 60 and
120 across, and Venus Lighthouse's entrance puts them at (0, 0), (64, 0) and
(0, 68). A window has no size of its own: the camera's bounds stop the view,
so a layer shows the cells from its start out as far as the camera can go.

The ground layer shows on `BG3`, the middle layer on `BG2` and the overhead
layer on `BG1`; [Field](../field/README.md#towns-and-dungeons-on-screen)
explains how their priorities put
[object events](../../glossary.md#object-event) between them.

### Placeholder metatiles

A layout's grid doesn't store the number of every cell's picture from the
start. The first time a picture appears in the grid, the cell holds a
placeholder instead, and the game hands out picture numbers in order as it
loads the layout. Each layout's table of pictures is ordered by where they
first appear, so a layout has exactly as many placeholders as pictures
(`map_003` has 1,331 of each). Every layout follows the rule but Venus
Lighthouse's entrance, whose grid names 170 cells' pictures before their
placeholders; those cells still get the right pictures, since the numbering
depends only on the placeholders.

A placeholder is metatile 4,095, the highest 12 bits can hold,
and `Tile_ConvertMapGrid` numbers them as it puts the cells together; the ROM
also keeps an unused C version, `TileMap_NumberPlaceholderMetatiles`.

### Chests and Psynergy Stones

![Isaac opens a chest in Sol Sanctum: the Small Jewel rises over him as the treasure fanfare plays](https://alchemy.pascalpixel.com/guides/front/chest.gif)

![In Sol Sanctum, Isaac searches the Psynergy Stone in front of him: it rises over his head, "Isaac got a Psynergy Stone.", "The party's PP is fully restored!", and "The Psynergy Stone disappeared..."](https://alchemy.pascalpixel.com/guides/layout/psynergy-stone.gif)

Several maps can share one layout, and each puts its own chests in it. The
layout lists the spots where a chest or a
[Psynergy](../../glossary.md#psynergy-and-pp) Stone can stand, each with the
event that would place one there, and a map places one only where it has that
event. A chest is shown open once it has been opened and is gone once its
mimic has been beaten; a Psynergy Stone stands until it is taken, and taking
it fills every member's PP.

Crossbone Isle's keys room and its Bane room share `map_110`, whose eight
treasure spots carry the [triggers](../../glossary.md#trigger) 100 to 107: the
keys room has chest events for 100 to 103, and the Bane room for 104 to 107.
The Bane room moves the Demon Axe's chest out of the way until the stone
pillar carrying it reaches the ladder, since its treasure spot's cell lies
past the room's east wall.

The treasure spot list is (column, row, trigger) rows ended by 255, 255, and
`Field_PlaceTreasureSpots` places an object at each spot whose trigger the map
has an event for, up to `TREASURE_SPOT_MAX` (10); a spot the map has no event
for takes none of the ten. 76 layouts use `map_000`'s list, which holds
only the end row. Opening a chest and taking a stone are the map's search
events ([Maps](../scene/README.md#how-a-map-event-is-chosen)).

### Tileset animation and blend scripts

![Isaac stands by the pool below Vale's waterfall while the falling water streams down and the spray at its foot churns](https://alchemy.pascalpixel.com/guides/layout/waterfall.gif)

Some parts of a map move by themselves without anything on the map moving:
the game simply swaps the pictures of a few tiles on a timer, as with Vale's
waterfall. A layout's tileset animation script says which pictures to copy
over which tiles and how long to wait between them; its blend script changes,
in the same way, how the layers mix. Suhalla Desert's north shows the usual
shape: one of its channels copies four rows of four tiles from one of four
stored pictures, waits 15 frames, takes the next picture, and starts again
after the fourth. Up to 16 channels run side by side.

Both scripts are lists of numbers, run each frame as
[tasks](../../glossary.md#task): a channel's start, then frames of four
values each (the tile to copy from, how many tiles, the tile to copy to and
the frames to wait), then a jump, a stop or a loop. A blend script's steps set
how the layers blend and wait.

`TilesetAnim_Update` and `BlendScript_Run` run the scripts,
which have no names for their words; the names below are the tables'.

| Word | Value | Meaning |
| --- | --- | --- |
| `channel n` | `0xfd00 + n` | channel n starts at the next value; with bit 7 set it starts paused |
| `frame` | four values below `0xfd00` | source tile, tile count, destination tile, frames to wait |
| `jump n` | `0xfe00 + n` | continue at value 2n of the channel's script |
| `stop` | `0xfeff` | halt the channel |
| `end` | `0xffff` | loop back to the channel's start |
| a source tile from RAM | 1,536 on (512 for 256-colour tiles) | copied from unpacked tiles in RAM rather than from video memory |

## The world map's data

The world map is far too big to unpack at once, so its ground is cut into
squares that are packed one by one, and the game unpacks only the few around
the party, bringing in new ones as the party walks.

The world is 16 by 16 squares of 512 pixels. The ground is stored as two
sets of squares, one for the lower ground and one for the mountains, woods and
towns drawn over it, and each position starts on the square of its own number.
Each set holds 64 squares more than there are positions, alternates: a world map
script can put one in a position, and the snowfield's graphics swap six.

`world/blocks.tsv` is the ground, one value for each 32-pixel
cell of the world map, 512 across and 320 down, which the build cuts into the
640 squares; `WorldMap_UpdateCurrentTileBlock` unpacks the 2 by 2 squares
around the party and `WorldMap_PreloadCurrentTileBlock` brings in at most one
new square a frame. The tables in `default/` are the world map's too: its
default cells, which come back after its still picture
([Field](../field/README.md#the-world-map-in-perspective)).

| Part | Size |
| --- | --- |
| the world | 16 by 16 squares of 512 pixels |
| a value | one 32-pixel cell of the world map |
| squares | 640, each packed on its own, 320 for each of the two grounds: 256 positions and 64 alternates |
| unpacked at once | the 2 by 2 squares around the party, in each ground |

## The Colosso paths

![The first Colosso final begins: "The 1st Match" scrolls across, a count of 3, 2, 1 and GO, and while Isaac stands at the start the gauge at the top shows Azart's marker moving on towards the finish](https://alchemy.pascalpixel.com/guides/map/colosso-race.gif)

In each Colosso final Isaac races an opponent through an obstacle course to
the arena, and the gauge at the top of the screen shows how near each is to
the finish. The opponent isn't steered by any rule: it replays a run that was
recorded once, one point of ground a frame. In the first final Azart runs east
of the finish, on a mirror of Isaac's course, so he is seen only as he comes
into the arena.

`colosso/` holds the three runs, one for each final. The Azart match runs path
A, the Satrage match path B and the Navampa match path C. Each frame the
opponent steps to the next point, mirrored across the finish where the match asks for it, turning at most
a sixteenth of a turn, and at the end walks to the finish. The race code can
also record a run, writing down where someone stands every frame and then
playing it back as an opponent.

`Colosso_StartOpponent` starts a run and
`Colosso_UpdatePathOpponent` steps it, from `ColossoRaceWork.path`, 7,200
points long; each table ends with a row of two zeros, names its columns x and y where the race reads x and z, and the Colosso
lettering, which differs by edition, is placed after the paths.

| Table | Match | Points |
| --- | --- | --- |
| `opponent_path_a.tsv` | Azart | 3,918 |
| `opponent_path_b.tsv` | Satrage | 4,380 |
| `opponent_path_c.tsv` | Navampa | 5,048 |

## Design

The grid is stored as whole planes rather than as cell words, which keeps
each plane's long runs of one value together for the packer: a behavior
plane is mostly runs of open ground and walls, as `map_003`'s is.

The treasure spots are the one part stored unpacked, and the loader reads them
in place rather than copying them: only `Field_PlaceTreasureSpots` reads them,
once as the map loads.

One grid serves all three layers, so every layout takes the same 64 KB of
cells, and a layer's window can sit anywhere in the grid.

The layout numbers are the order of the directories, not numbers the game
gives its layouts: a map's row in `gSceneHeaders` names a row of `gMaps`,
and that row names the layout's resources.

## Limits

- 128 by 128 cells, for all three layers together.
- 4,095 metatiles a layout: a cell's metatile number has 12 bits, and 4,095
  is the placeholder.
- 256 shapes a layout, since a cell names its shape in one byte.
- 16 tileset animation channels (`struct MapGridScrollAnimView`).
- `TREASURE_SPOT_MAX` (10) chests and Psynergy Stones placed on a map.

## Words used here

- A **shape** is the form of the ground in a cell: a terrain kind and three
  height samples.
- A **layer** is one of the layout's three backgrounds, each a window onto the
  one cell grid.
- A **plane** is one value per cell over the whole 128 by 128 grid.
- A **treasure spot** is a cell where a chest or a Psynergy Stone can stand.

## Evidence

The game keeps no names for any of this, so each part is named for what the
code does with it: the 16-pixel pictures become BG map entries, so they are
`metatiles`; the shapes feed the height functions; and the treasure spots are where
`Field_PlaceTreasureSpots` puts chests and Psynergy Stones. The sixth offset is
called the treasure spot list because that function is the only code that reads the
pointer the loader stores to it (`MapGridScrollWork.markers`).

The haze's layout is found from the map table: `SceneId_SuhallaDesertNorth`'s
row of `gSceneHeaders` gives layout 151, whose row of `gMaps` names the
resource the directory's `area` line gives to `Field_Map147`; the south's
row leads the same way to `Field_Map148`, and the Tolbi ship deck's to
`Field_Map103`. These three are the only layouts whose headers drift a
layer.

The header's last two bytes (`pad0` and `pad1` in `map.inc`, `reserved` in
`struct MapHeader`) are 0 in every layout of both games, and nothing
reads them: the loader is the only code that reads the header.

The four bits above a cell's metatile number are set all over the game's
grids: of the cells the layouts write out, bit 15 is set in about 59,000 and
bit 14 in about 33,000, in more than 150 layouts, and bits 12 and 13 in about
1,200 and 500, in some 25. Nothing gives them a meaning. `Tile_ConvertMapGrid`
carries them through when it numbers the placeholder metatiles, and every
other reader, from the scroll code that draws the metatiles to the height and
behavior lookups, takes the metatile number's 12 bits or the cell's other
bytes alone, so a layout drawn with the bits cleared looks the same. Only
the unused `MapGrid_GetCellHighFlags` and Crossbone Isle's stepping stones,
which copy them along with the rest of a cell, name them.
