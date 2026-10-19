# Maps

This folder holds The Broken Seal's maps as editable tables: the layered
maps of towns, dungeons and interiors, the world map's ground blocks, the
default map, and the Colosso rivals' recorded paths. There is no code
here. The field engine loads the maps (`Map_LoadLayeredScene`,
[`../field/field/map_layers.c`](../field/field/map_layers.c)); the scene
table says which scene uses which map.

See also: [Field](../field/README.md), which loads and draws the maps;
[Field objects](../object/README.md), which places the markers; and
[Scenes](../scene/README.md).

## Files

| Path | What it holds |
| --- | --- |
| `map_NNN/` | one layered map, 180 of them: `map.s` and the tables it places |
| `gondowan_passage/`, `venus_entrance/` | two layered maps named for their place, built like the others |
| `world/blocks.tsv` | the world map's ground blocks (`Field_PerspectiveDataA`) |
| `default/` | the default cells, tile animation and block attributes (`Field_DefaultMapCells` and its kin) |
| `colosso/` | the rivals' recorded paths in the Colosso finals, and the Colosso lettering placed after them |
| `map.s` | places `world/` and `default/` |
| `map.inc` | the `map_header`, `map_layer` and `map_block` macros every `map.s` uses |

The folder has 1,527 tracked files: 1,342 tables (`.tsv`), 184 assembly files
(`.s`) and `map.inc`. A built tree has about 780 more, the encoded form of
each table, written beside it and ignored by git (2,308 in all).

A layered map's directory holds:

| File | What it is | Built as |
| --- | --- | --- |
| `map.s` | the header, the three layers and six offsets, then the blocks; and the tile sheets and palette of its tile set after them | assembled |
| `metatiles.tsv` | the blocks: four tile entries each, top left, top right, bottom left, bottom right | `metatiles.delta0.lz`, `.delta1.lz` or `.delta2.lz` |
| `shapes.tsv` | the ground shapes: a kind and three height samples each | `shapes.table.lz` |
| `grid.tsv` | the cell grid's three planes, in order | `grid.parts.lz` |
| `grid_collision.tsv`, `grid_shape.tsv`, `grid_metatile.tsv` | the planes: each cell's collision code, shape and block | parts of the grid |
| `animation.tsv` | the tile animation channels | `animation.script.lz` |
| `blend.tsv` | the blend script | `blend.script.lz` |
| `markers.tsv` | the cells where chests and Psynergy stones stand | `markers.table` |
| `*_ja.tsv`, `*_de.tsv`, `*_es.tsv` and so on | an edition's own version of a table | chosen in `map.s` |
| `*_patch.tsv` | the bytes Camelot changed after packing | read by the packer |

## Words

- **cell**: one square of 16 by 16 pixels, a 32-bit word in the decoded map
  (`struct MapCell`): its block and flags in the low halfword, its collision
  code in the third byte and its shape in the fourth.
- **block** (a metatile): 2 by 2 tiles, four BG map entries. A cell's low
  12 bits choose its block.
- **shape**: a row of `shapes.tsv`, four bytes: a terrain kind in the low
  four bits of the first, then the ground's height samples, which the kind's
  height function interpolates (`Map_GetTerrainHeight`).
- **collision code**: 0xff is a wall; scenes' touch and search events are
  keyed on the code under the leader.
- **layer**: one of the map's three backgrounds, a window into the cell grid.
- **plane**: one byte or halfword per cell over the 128 by 128 grid, as a
  table: `cell:u8	grid:128x128	fill:0	at:2,3` gives the cell type, the
  grid, the value of cells the table leaves out and the cell its first row
  starts at.

## Design

### The layered format

A map's header (`map_header` and three `map_layer` lines, `struct
LayeredSceneHeader` in the loader) holds the camera's bounds in 8-pixel
tiles, each layer's priority and tile block, and for each layer its origin,
its parallax (sixteenths of the view's motion), its drift (sixteenths of a
pixel a frame) and its wrap. Six offsets follow, from the header to its
blocks: the blocks, the shapes, the cell grid, the tile animation, the blend
script and the markers. An offset of 0 means the map has none.

`Map_LoadLayeredScene` decodes each into a fixed buffer: the blocks into
`gMapBlocks`, the shapes into `gMapCollision`, the grid into
`Ram_MapCellBuffer`, and the two scripts into the space after the shapes.
The grid is stored as three planes one after another (collision codes, then
shapes, then blocks: 16 KB, 16 KB and 32 KB), which keeps each plane's long
runs of one value together for the packer (XXX: an inference; the bytes show
only the layout). `Tile_ConvertMap` then interleaves them into
cell words, 128 by 128 of them, 64 KB from 0x02010000. That is more than
`gMapCellBuffer`'s 32 KB: the cells run on through `gActorSpriteSlots` and
`gDecodeBuffer`, which the field therefore cannot use for anything else
while a map is loaded. A battle reuses that memory, and the scene's map is
loaded again when the party returns.

The three layers share that one grid. Each layer's origin picks the cell its
window starts at, so a map lays its layers out side by side or one above
another in the grid: map 000's three layers start at tiles 0, 60 and 120
across, Venus Lighthouse's entrance at (0, 0), (64, 0) and (0, 68). Layer 0
shows on BG3, layer 1 on BG2 and layer 2 on BG1, and BG0 stays the windows'.
A layer's priority byte is also its switch: a layer with priority 0 is not
shown. Most maps use 3, 2 and 1; nine give 0 to their second and third
layers and show only the first.

Collision and height are per layer too. Code that asks about the ground
passes a layer (an object's `terrainId`), reads the cell at that point in
that layer's window, and looks its shape up in `gMapCollision`; with no map
work, it reads the buffer from its start. So each layer has its own walls
and heights, and an object belongs to one layer at a time.

A cell whose block is 0xfff is a placeholder. As the map is converted, each
placeholder is given a block number of its own, counting up from 0 in map
order. XXX: what fills those blocks is not known.

### How the build encodes a map

Every table is encoded by `tools/ags-graphics`, which reads the form and the
codec from the name of the file it is asked to write (`NAME.FORM[.CODEC]`
from `NAME.tsv`, the `Makefile`'s asset rules). The forms a map uses:

- `table`: typed records, one row per record, each column's type giving its
  width (`shapes`, `markers`).
- `plane`: a whole grid, written row by row; a table names only the
  rectangle that holds data, and the rest of the grid takes its `fill`.
- `parts`: the listed files' encodings, one after another (the three planes
  of `grid.tsv`).
- `script`: animation and blend commands as halfwords: `channel n` and
  `jump n` carry their operand in a prefixed halfword (0xfd00, 0xfe00),
  `stop` and `end` are 0xfeff and 0xffff, and `frame`, `control` and `level`
  are their operands.
- `delta0`, `delta1`, `delta2`: the blocks as halfwords behind a method byte,
  which `Tilemap_DecodeStagedBuffer` undoes: 0 stored as they are, 2 each
  XORed with the one before, and 1 the same with all the high bytes first and
  then all the low bytes. Each map's `map.s` names the method its stream
  uses, so the choice is data, not a guess the build makes.

The codec `lz` then packs the result as Camelot's packer did, and the bytes
must come out identical. Two kinds of record keep them so. Where the original
stream is followed by a few stray bytes, which the packer left from its
buffer, `map.s` places them as literal bytes with a `PACKER:` comment that
says what wrote them. Where Camelot changed a stream after packing it, a
`_patch.tsv` beside the table lists each changed byte: its position, the byte
the packer saw, the byte that shipped, and the proof. The table holds what
shipped, and the patch gives the packer the byte it packed, so the packer
makes the same choices and the stream decodes to the edited map.

An edition's own version of a table (`grid_es.tsv`, `markers_ja.tsv`) is
chosen in `map.s` with `.ifdef TBS_EDITION_*`; editions that agree share one
file, and a map that uses another map's table names that map's file (many
`map.s` place `map_000/markers.table`, which is empty).

## How to change a map

- To move a wall or a step, edit the cell in `grid_collision.tsv` and the
  shape in `grid_shape.tsv`; to change what is drawn, the block in
  `grid_metatile.tsv`. The planes are in hexadecimal, one row of the grid per
  line, starting at the plane's `at` cell.
- A new block is a row of `metatiles.tsv`; its row number is the block
  number cells use.
- Any change makes that edition differ from the original, so it no longer
  compares OK.

## Limits

- 128 by 128 cells for all three layers together.
- 4,095 blocks a map (12 bits), and 0xfff is the placeholder.
- 16 tile animation channels (`struct MapScrollWork`).

## Evidence and open questions

The names `metatiles`, `shapes`, `grid` and `markers` come from what the
loader and the code that reads each block do with it: the blocks become BG map
entries, the shapes feed the height functions, the markers place chests and
Psynergy stones (`Field_PlaceMapMarkers`). The map numbers are the
directory's.

Still unknown:

- XXX: the loader names the sixth offset `script` (`struct
  LayeredSceneHeader`, `MapScrollWork.script`), while the marker code reads
  the same pointer as `markers` (`struct MarkerMap`) and the data is a marker
  list of x, z and an event id. The loader's names cross folders, so they
  wait for a rename of their own.
- XXX: what the placeholder blocks are filled with.
- XXX: what the header's last two bytes (`pad0`, `pad1`; `unk0a` in the
  loader) mean; every map has them 0.
- XXX: the cell map fills 64 KB through `gActorSpriteSlots` and
  `gDecodeBuffer`, yet the tile animation copies tiles from `Ram_DecodeBuffer`
  plus their number (from 0x600 on, which lands past it, in
  `gMapLayerData`); which code may use those two buffers while a map is
  loaded has not been traced.
