# Maps

A map is the ground a place is drawn on and walked over. Each is a folder
here, such as `vale/` for Vale, and the heart of it is three grids you
can open, one number a metatile.

## Metatiles

![Isaac walks behind a house in Vale: the roof hides him, and he comes out on its other side](https://alchemy.pascalpixel.com/guides/world/behind-roof.gif)

A map is a grid of [metatiles](../../../../docs/tbs/glossary.md#metatile), 16 pixels square. A
metatile is three numbers, each in a grid of its own:

- **What is drawn.** `grid_picture.tsv` names a
  [metatile tiles](../../../../docs/tbs/glossary.md#metatile-tiles), four tiles two by two,
  from the map's own list in `metatile_tiles.tsv`.
- **What standing there does.** `grid_behavior.tsv` holds the
  [metatile behavior](../../../../docs/tbs/glossary.md#metatile-behavior). Zero is plain ground.
  The numbers from 240 up mean the same everywhere: a wall, a ledge, a
  ladder, a jar to search. The rest mean whatever the scene's cue
  table says ([Scenes](../scene/README.md#which-cues-a-metatile-starts)).
- **How the ground is shaped.** `grid_shape.tsv` picks a shape from
  `shapes.tsv`: how high the ground is at each pixel of the metatile. Flat
  ground, slopes, steps and bumps are all shapes, and walking is decided by
  comparing heights.

A metatile can also say who is in front. `grid_actors.tsv` lists the metatiles
that change the layer or the drawing priority of whoever stands there,
which is how Isaac goes behind a roof by walking, with no code involved.

## One grid, three layers

![Vale's screen in front of the Sanctum taken apart: the ground, the walls, the people and the roofs spread out one behind another, then close up into the picture the player sees](https://alchemy.pascalpixel.com/guides/field/layers-3d.gif)

On screen a town is three layers, one over another. In the file they lie
side by side in the same grid: the ground in one block of columns, the
walls in the next, the roofs in a third. `layers.tsv` has a row for each,
which says where it begins.

A layer can also drift by itself and scroll at its own rate, which is how
a haze of sand hangs in the air in front of a desert.

## Chests and Psynergy Stones

A chest is not painted into the map, or every scene sharing the map would
have it. The map only lists the metatiles where treasure could stand, in
`spots.tsv`. When a
scene starts, the field engine looks in the scene's cues for each spot's
behavior number and puts a chest there only if it finds a row.

## Tileset animation and blend scripts

![A spot in Vale, with the tiles its ground is built from laid out in grey beside it: watch for the red outline, which marks each group of tiles at the moment the game repaints it](https://alchemy.pascalpixel.com/guides/map/tile-animation.gif)

Stand still in Vale and water keeps moving, though no metatile changes. The
game repaints a tile on a timer, and every metatile that uses the tile changes
with it. A map's list of these is in `tileset_anims.tsv`.

A blend script does the same for transparency: a list of how strongly one
layer mixes into the others, and for how long.

## The World Map's data

The World Map is four times a town's grid in each direction, too big to
unpack whole. Its ground is cut into squares of 512 pixels, each packed
separately, and only the four around the party are kept unpacked. As the
party walks, the next square is unpacked, never more than one in a tick.

## Loading a map

### Finding a scene's map

The scene table gives each scene a row of a second table, `MapsWork`. That
row names the map's grid and the palette and tile sheets it is drawn
with, each a resource of its own
([Graphics](../graphics/README.md#tilesets)). Many scenes share a map, and
many maps share tiles.

A folder is named for the scene that uses it, or for the place when
several scenes of one place share it. The empty layout of
the title, the File Screen and the World Map, the first row `MapsWork` counts
from, is `empty_map/`; the two that no place or scene names keep their
resource number.

When the party arrives, the grid, the metatile tiles, the shapes and the
tiles are unpacked into memory, where the field engine reads them
directly.

The grid comes packed as three planes: the metatile tiles, the behaviors
and the shapes. As it is unpacked, a small function copied into fast RAM
weaves them into one word a metatile, the metatile tiles in the low half and the
behavior and shape in the high.

## Where the files are

Each folder holds the three grids, `grid_actors.tsv` and
`metatile_tiles.tsv`, with `shapes.tsv`, `spots.tsv`, `tileset_anims.tsv` and
`blend.tsv` where it has them. Where a language changes a map, its own
tables sit beside them, such as `grid_picture_es.tsv`. `header.tsv`,
`layers.tsv`, `contents.tsv` and `tilesets.tsv` put the map together and
name the pictures it reads. [`maps.tsv`](maps.tsv) lists the folders in
the order the Game Pak holds them, and [`resources.s`](resources.s) is the
map table, what each map loads. The code that reads them is in
[Field code](../field/README.md).
