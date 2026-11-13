# Maps

A map is the ground a place is drawn on and walked over. Each is a folder
here, such as `alhafra/` for Alhafra, and the heart of it is a text grid
you can open, `map.txt`, one cell per square.

## Cells

A map is a grid of [cells](../../glossary.md#cell), 16 pixels square, 128
by 128 of them. Each cell says four things. The first three are written
together in `map.txt`, as `picture:behavior:flags`, and the fourth has a
grid of its own:

- **What is drawn.** The picture names a
  [cell picture](../../glossary.md#cell-picture), four tiles two by two,
  from the map's own list in `cell_pictures.tsv`. It is written in hex, and
  its top digit holds the cell's layer and drawing priority, which is how
  Felix goes behind a roof by walking, with no code involved.
- **What standing there does.** The behavior is the
  [cell behavior](../../glossary.md#cell-behavior). Zero is plain ground.
  A few numbers mean the same everywhere: 255 is a wall, and 242 to 247
  are a jar, a barrel or the like to search. The numbers from 1 to 230 mean
  whatever the scene's trigger table says
  ([Scenes](../scene/README.md#which-triggers-a-cell-starts)).
- **What else is true of it.** The flags are a byte: water
  to wade through, a ledge to hop down, a ladder to climb, the second of
  the scene's two battle zones.
- **How the ground is shaped.** `shape_grid.tsv` picks a shape from
  `shapes.tsv`: a kind and up to three heights, which say how high the
  ground is at each pixel of the cell. Flat ground, slopes, steps and bumps
  are all shapes, and walking is decided by comparing heights.

## One grid, three layers

On screen a town is three layers, one over another. In the file they lie
side by side in the same grid, each in a block of its own. The map's
`map.inc` opens with a header and one `map_layer` line for each layer, which
says where it begins.

A layer can also drift by itself and scroll at its own rate, and the same
line says by how much.

## Chests and Psynergy Stones

A chest is not painted into the map, or every scene sharing the map would
have it. The map only lists numbered points, in `scene.tsv`. The
scene item table in [the field's folder](../field/README.md) gives each
scene its treasure: what kind it is, which point it stands on, the flag
that says it has been opened, and what is inside. When a scene starts, the
field puts out each one whose flag is not yet set, at most 16. One kind of
chest is put out either way, open once its flag is set. Other kinds hide an item in a jar or a barrel, out of sight.

## Tileset animation and blend scripts

Stand still in Daila and the picture still moves in places, though no cell
changes. The
game repaints a tile on a timer, and every cell that uses the tile changes
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

The scene table gives each scene a row of a second table, `gMaps`, which
has 332. That row names the map and the palette and four tile sheets it
is drawn with, each a resource of its own
([Graphics](../graphics/README.md#tilesets)). Many scenes share a map, and
many maps share tiles.

A folder is named for the one place that uses it, as its scene or the
scene's code names it, when no other map shares that name. The rest, the
rooms of a place with several maps and a map that several places share,
keep their number among the maps' resources, where each map is followed
by the tile sheets it brings, so that number rarely matches its row in
`gMaps`. Daila is row 9 and folder `map_008/`.

When the party arrives, the grid, the cell pictures, the shapes, the tile
animation, the blend script and the scene's own part of the map are
unpacked into memory, where the field reads them directly. The palette
and the tiles come too, unless they are already there.

## Where the files are

Each folder holds `map.inc`, which puts the map together and
which [`maps.s`](maps.s) includes in the Game Pak's order, and its grids
and tables: `map.txt`, `cell_pictures.tsv`, `shape_grid.tsv`,
`shapes.tsv`, `animation.tsv` and `blend.tsv` where it has them. The
scene's part is `scene.tsv`, three tables one after another: the points,
where the party comes in, and the camera's bounds. Where the Japanese
edition's map differs, its own file sits beside it, such as `map_ja.txt`.
`world/` is the World Map's ground, and `default/` holds the cells the
World Map starts from and a few other tables. The code that reads them is in
[Field code](../field/README.md).
