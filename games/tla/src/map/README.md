# Maps

A map is the ground a place is drawn on and walked over. Each is a folder
here, such as `alhafra/` for Alhafra, and the heart of it is four grids
you can open, one number a metatile.

## Metatiles

![Felix walks east along the grass above the house on Daila's rise: the top of its roof hides him to the shoulders, and he walks back out to the west](https://alchemy.pascalpixel.com/tla/guides/world/behind-roof.gif)

A map is a grid of [metatiles](../../../../docs/tla/glossary.md#metatile), 16 pixels square, 128
by 128 of them. Each metatile says four things, each in a grid of its own:

- **What is drawn.** `grid_picture.tsv` names the metatile's
  [metatile tiles](../../../../docs/tla/glossary.md#metatile-tiles), four tiles two by two,
  from the map's own list in `metatile_tiles.tsv`. `grid_actors.tsv` lists the
  metatiles that change the layer or the drawing priority of whoever stands
  there, which is how Felix goes behind a roof by walking, with no code
  involved.
- **What standing there does.** `grid_behavior.tsv` holds the
  [metatile behavior](../../../../docs/tla/glossary.md#metatile-behavior). Zero is plain ground.
  A few numbers mean the same everywhere: 255 is a wall, and 242 to 247
  are a jar, a barrel or the like to search. The numbers from 1 to 230 mean
  whatever the scene's cue table says
  ([Scenes](../scene/README.md#which-cues-a-metatile-starts)).
- **What else is true of it.** `grid_shape.tsv` holds the flags, a byte: water
  to wade through, a ledge to hop down, a ladder to climb, the second of
  the scene's two battle zones.
- **How the ground is shaped.** `shape_grid.tsv` picks a shape from
  `shapes.tsv`: a kind and up to three heights, which say how high the
  ground is at each pixel of the metatile. Flat ground, slopes, steps and bumps
  are all shapes, and walking is decided by comparing heights.

## One grid, three layers

![Daila's screen taken apart: the ground, the walls and roofs, the people and the window layer spread one behind another, then closed up](https://alchemy.pascalpixel.com/tla/guides/field/layers-3d.gif)

On screen a town is three layers, one over another. In the file they lie
side by side in the same grid, each in a block of its own. `layers.tsv`
has a row for each layer, which says where it begins.

A layer can also drift by itself and scroll at its own rate, and the same
row says by how much.

## Chests and Psynergy Stones

A chest is not painted into the map, or every scene sharing the map would
have it. The map only lists numbered points, in `points.tsv`. The
scene item table in [the field's folder](../field/README.md) gives each
scene its treasure: what kind it is, which point it stands on, the flag
that says it has been opened, and what is inside. When a scene starts, the
field puts out each one whose flag is not yet set, at most 16. One kind of
chest is put out either way, open once its flag is set. Other kinds hide an item in a jar or a barrel, out of sight.

## Tileset animation and blend scripts

![A spot in Daila beside the tiles of its layer the tile animation repaints, laid out in grey, each outlined in red as it is repainted (the ponds' water)](https://alchemy.pascalpixel.com/tla/guides/map/tile-animation.gif)

Stand still in Daila and the picture still moves in places, though no metatile
changes. The
game repaints a tile on a timer, and every metatile that uses the tile changes
with it. A map's list of these is in `animation.tsv`, and a map that has
none of its own may borrow another's.

A blend script, `blend.tsv`, does the same for transparency: a list of how
strongly one layer mixes into the others, and for how long.

## The World Map's data

The World Map is too big to unpack whole. Its ground is cut into squares
of 512 pixels, 32 by 32 of them, each packed separately, and only the four
around the party are kept unpacked. As the party sails or walks, the next
square is unpacked, never more than one in a frame. Its squares are in
`world/blocks.tsv`.

## Loading a map

### Finding a scene's map

The scene table gives each scene a row of a second table, `MapsWork`, which
has 332. That row names the map and the palette and four tile sheets it
is drawn with, each a resource of its own
([Graphics](../graphics/README.md#tilesets)). Many scenes share a map, and
many maps share tiles.

A folder is named for the scene that uses it, as The Broken Seal's are:
the first in the scene table, when several share it. Daila is row 9 of
`MapsWork` and folder `daila_flooded/`, for the scene `DailaFlooded`; a room keeps its
scene's number until the scene has a better name. Two are named
otherwise: `empty_map/`, the empty map the scene table gives the title,
the World Map, the credits and the debug level room, and `map_290/`,
which no scene names and so keeps its number.

When the party arrives, the grid, the metatile tiles, the shapes, the tile
animation, the blend script and the scene's own part of the map are
unpacked into memory, where the field reads them directly. The palette
and the tiles come too, unless they are already there.

## Where the files are

Each folder holds the four grids, `grid_actors.tsv`, `metatile_tiles.tsv` and
`shapes.tsv`, with `animation.tsv` and `blend.tsv` where it has them. The
scene's part is three tables: `points.tsv`, `entrances.tsv` for where the
party comes in, and `bounds.tsv` for the camera's bounds. `header.tsv`,
`layers.tsv`, `contents.tsv` and `tilesets.tsv` put the map together, and
[`maps.tsv`](maps.tsv) lists the folders in the Game Pak's order. Where
the Japanese edition's map differs, its own table sits beside it, such as
`grid_picture_ja.tsv`.
`world/` is the World Map's ground, and `default/` holds the metatiles the
World Map starts from and a few other tables. The code that reads them is in
[Field code](../field/README.md).
