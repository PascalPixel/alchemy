# Graphics

Every picture in the game is an ordinary PNG in this folder: the tiles Vale
is built from, Isaac, a Skeleton, a battle's backdrop, the letters of a
message. There are about 2,100 of them. Change one, build, and the game
that comes out has your picture in it.

This page says what a picture can be: its format, its colours, its size,
and how each kind is put together.

## How the console draws a picture

![The title intro fades in: every colour on screen, the sky, the mountain and the sun's flares, brightens from a dark red towards its own colour](https://alchemy.pascalpixel.com/guides/graphics/title-fade.gif)

The console never stores a colour per pixel. A picture is a grid of
numbers, and a separate list, the palette, says which colour each number
stands for. That is why a fade or a recolour never touches a picture: it
only changes the list.

The limits that follow from the hardware:

- **Colours.** 256 for everything on the background layers and another 256
  for all the sprites on screen. A colour has 32 levels each of red, green
  and blue. The build refuses a PNG colour the console cannot show, so one
  never quietly comes out as another.
- **Two depths.** A rich picture indexes the whole list of 256. A simple
  one uses 16 colours, one of sixteen short lists, chosen by a
  [palette number](../../glossary.md#palette-number).
- **Tiles.** Pixels are stored as 8 by 8 squares. A background layer is a
  grid of tile numbers, so one square of grass can cover a field.
- **Sprites.** A sprite is a block of tiles the console can put anywhere.
  The largest is 64 by 64 pixels.

Every PNG here is the indexed kind, carrying its own palette, so an image
editor shows it as the game does.

## Sprites and their animations

Isaac is one file, `sprite/isaac.png`: a strip one
[picture](../../glossary.md#picture) wide, with 210 pictures of 32 by 32
stacked top to bottom. Beside it, `isaac.tsv` is his
[animations](../../glossary.md#sprite-animation), one a line, each a list
of which picture to show and for how many frames:

```
picture 55 6, picture 30 6, picture 35 6, picture 40 6, picture 45 6, picture 50 6, loop
```

That is his walk. The numbers go up in fives because each pose is drawn
from five directions, and the animation names only the first. The field
picks the one that matches the way he faces, and mirrors it for the
directions nobody drew. So an animation is written once and works facing
any way.

Every figure the field or a battle can draw has a row in the sprite table,
`sprite/sprites.tsv`: its size, a scale it is drawn at, its sheet and its
animations. There are 367.

## Monsters

![A battle opens on one Skeleton: watch its picture change as the camera swings round to the party's side, and again as it rushes in to attack](https://alchemy.pascalpixel.com/guides/graphics/monster.gif)

A monster is a sprite like Isaac, with its pictures kept in `monster/`.
`monster/skeleton.png` is a strip of twelve pictures, 64 by 64 each. They
are six poses, each drawn twice: once from the front and once from behind,
because the battle's camera swings round. An animation names the first of
a pair and the battle shows whichever side the camera is on.

Three things worth knowing if you want to make one:

- **Size.** 64 by 64 is the most one sprite can be. The Fusion Dragon is
  bigger because it is two sprites, a left half and a right half drawn 64
  pixels apart, each with a strip of its own. A figure's row in the sprite
  table also has a scale: most monsters are drawn at three quarters of
  their pictures' size, and the scale can go above full size too.
- **Colours.** A monster's strip indexes the 256-colour list. The ones in
  the game use between 39 and 151 colours.
- **Sharing.** 54 strips serve 172 enemies. The Skeleton, the Bone Fighter
  and the Skull Warrior are one set of drawings under three palettes.

## Tilesets

![The title intro taken apart: the clouds, the mountain and the sun pulled apart and spread one behind another, then put back together into the picture the player sees](https://alchemy.pascalpixel.com/guides/graphics/title-intro-layers-3d.gif)

A town is not painted as one picture. It is built from a
[tileset](../../glossary.md#tileset): a palette of 14 short lists of 16
colours, and sheets of 512 tiles each. The [map](../../glossary.md#map)
says which tile goes where ([Maps](../map/README.md#cells)).

A map does not name a tileset as a whole. It names a palette and its sheets
one by one, so maps borrow freely: the game's 182 maps are drawn from 69
folders. The cheapest borrowing is the same tiles under new colours. The
walls of Mercury Lighthouse appear in four places under nine palettes.

## Battle backgrounds

A battle's backdrop is one picture of 256 by 120 pixels in 128 colours,
stored row by row, not as tiles, on the layer the console can zoom. There
are 34 in `battle_backgrounds/`, picked by where the party stood. They use
128 colours and not 256 so that two can be on screen at once, which is how
the credits melt one into the next.

## Effects, fonts and the rest

- `battle_effects/` holds the flashes and blasts of a battle. An effect's
  pictures are often different sizes, so each is a PNG of its own
  ([Battle effects](../battle_effect/README.md)).
- `font/` holds the letters: one sheet for each language, a letter to a 16
  by 16 square, with a table of widths beside it so that an `i` takes less
  room than a `W`.
- `ui/` is the windows' borders, icons and cursors, and `title/` the
  Title's pictures. A picture with words in it has one file for each
  language that needs its own.

## How the game finds a picture

When the game is built, each PNG is split into its colours and its pixels,
and the pixels are usually packed. A picture that one part of the game
always uses, such as the font, is linked straight into that part. A picture
chosen while the game runs, such as a backdrop or a monster, is a numbered
resource ([The Game Pak](../gamepak/README.md#the-resource-directory-and-table-order)).
