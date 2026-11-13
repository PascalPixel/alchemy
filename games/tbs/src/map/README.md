# Maps

A map is the ground a place is drawn on and walked over. Each is a folder
here, such as `vale/` for Vale, and the heart of it is one text file
you can open, `map.txt`.

## Cells

![Isaac walks behind a house in Vale: the roof hides him, and he comes out on its other side](https://alchemy.pascalpixel.com/guides/world/behind-roof.gif)

A map is a grid of [cells](../../glossary.md#cell), 16 pixels square. In
`map.txt` a cell is up to three numbers, written `2e:100:3`:

- **What is drawn.** The first number names a
  [cell picture](../../glossary.md#cell-picture), four tiles two by two,
  from the map's own list in `cell_pictures.tsv`.
- **What standing there does.** The second is the
  [cell behavior](../../glossary.md#cell-behavior). Zero is plain ground.
  The numbers from 240 up mean the same everywhere: a wall, a ledge, a
  ladder, a jar to search. The rest mean whatever the scene's trigger
  table says ([Scenes](../scene/README.md#which-triggers-a-cell-starts)).
- **How the ground is shaped.** The third picks a shape from
  `shapes.tsv`: how high the ground is at each pixel of the cell. Flat
  ground, slopes, steps and bumps are all shapes, and walking is decided by
  comparing heights.

A cell can also say who is in front. A digit before the picture number
changes the drawing priority of whoever stands there, which is how Isaac
goes behind a roof by walking, with no code involved.

## One grid, three layers

![Vale's screen in front of the Sanctum taken apart: the ground, the walls, the people and the roofs spread out one behind another, then close up into the picture the player sees](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

On screen a town is three layers, one over another. In the file they lie
side by side in the same grid: the ground in one block of columns, the
walls in the next, the roofs in a third. The map's first line says where
each begins.

A layer can also drift by itself and scroll at its own rate, which is how
a haze of sand hangs in the air in front of a desert.

## Chests and Psynergy Stones

A chest is not painted into the map, or every scene sharing the map would
have it. The map only lists the cells where treasure could stand. When a
scene starts, the field looks in the scene's triggers for each spot's
behavior number and puts a chest there only if it finds a row.

## Tileset animation and blend scripts

![A spot in Vale, with the tiles its ground is built from laid out in grey beside it: watch for the red outline, which marks each group of tiles at the moment the game repaints it](https://alchemy.pascalpixel.com/guides/map/tile-animation.gif)

Stand still in Vale and water keeps moving, though no cell changes. The
game repaints a tile on a timer, and every cell that uses the tile changes
with it. A map's list of these is in `tileset_anims.tsv`.

A blend script does the same for transparency: a list of how strongly one
layer mixes into the others, and for how long.

## The World Map's data

The World Map is four times a town's grid in each direction, too big to
unpack whole. Its ground is cut into squares of 512 pixels, each packed
separately, and only the four around the party are kept unpacked. As the
party walks, the next square is unpacked, never more than one in a frame.

## Loading a map

### Finding a scene's map

The scene table gives each scene a row of a second table, `gMaps`. That
row names the map's grid and the palette and tile sheets it is drawn
with, each a resource of its own
([Graphics](../graphics/README.md#tilesets)). Many scenes share a map, and
many maps share tiles.

A folder is named for the scene that uses it, or for the place when
several scenes of one place share it. The three that no one
place or scene names keep their resource number, such as `map_001/`, the
first row `gMaps` counts from.

When the party arrives, the grid, the cell pictures, the shapes and the
tiles are unpacked into memory, where the field reads them directly.

The grid comes packed as three planes: the cell pictures, the behaviors
and the shapes. As it is unpacked, a small routine copied into fast RAM
weaves them into one word a cell, the picture in the low half and the
behavior and shape in the high.

## Where the files are

Each folder holds `map.txt` (the grid), `cell_pictures.tsv`,
`shapes.tsv` and `tileset_anims.tsv`. Where a language changes a map,
its own grid sits beside it, such as `map_es.txt`. A folder's `map.inc`
writes the map's header and the pictures it reads, and `maps.s` gathers
them in the order the Game Pak holds them; `resources.c` lists each
layout's resources by map number. The code that reads them is in
[Field code](../field/README.md).
