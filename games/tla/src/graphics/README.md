# Graphics

Every picture in the game is an ordinary PNG in this folder: the tiles
Daila is built from, Felix, a Skeleton, a battle's backdrop, the letters of
a message. There are about 3,400 of them. Change one, build, and the game
that comes out has your picture in it.

This page says what a picture can be: its format, its colours, its size,
and how each kind is put together.

## How the console draws a picture

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

A figure is one file, such as `sprite/isaac.png`: a strip one
[picture](../../glossary.md#picture) wide, with 215 pictures of 32 by 32
stacked top to bottom. Beside it, `isaac.tsv` is its
[animations](../../glossary.md#sprite-animation), one a line, each a list
of which picture to show and for how many frames:

```
picture 55 7, picture 30 7, picture 35 7, picture 40 7, picture 45 7, picture 50 7, loop
```

That is a walk. The numbers go up in fives because each pose is drawn
from five directions, and the animation names only the first. The field
picks the one that matches the way the figure faces, and mirrors it for
the directions nobody drew. So an animation is written once and works
facing any way.

A party member's sprite is named for the character whose number it
shares; another takes the name The Broken Seal gives the same picture.
One with neither keeps its row's number, as `spr_009.png` does.

Every figure the field or a battle can draw has a row in the sprite table,
`sprite/sprites.tsv`: its size, a scale it is drawn at, its sheet and its
animations. There are 716, and the Japanese edition has a table of its
own beside it.

## Monsters

A monster is a sprite like any other, with its pictures kept in
`monster/`. `monster/skeleton.png` is a strip of twelve pictures, 64 by 64
each. They are six poses, each drawn twice: once from the front and once
from behind, because the battle's camera swings round. An animation names
the first of a pair and the battle shows whichever side the camera is on.

Three things worth knowing if you want to make one:

- **Size.** 64 by 64 is the most one sprite can be. A bigger monster is two
  sprites, a left half and a right half drawn side by side, each with a
  strip of its own, as `dragon_left.png` and `dragon_right.png` are. Small
  ones fit 32 by 32. A figure's row in the sprite table also has a scale:
  most monsters are drawn at three quarters of their pictures' size, and a
  few at full size.
- **Colours.** A monster's strip indexes the 256-colour list. The ones in
  the game use between 39 and 157 colours.
- **Sharing.** There are 103 strips, and the sprite table has 105 monster
  rows drawn from them.

## Tilesets

A town is not painted as one picture. It is built from a
[tileset](../../glossary.md#tileset): a palette of 14 short lists of 16
colours, and sheets of 512 tiles each. The [map](../../glossary.md#map)
says which tile goes where ([Maps](../map/README.md#cells)).

A map does not name a tileset as a whole. Each palette and each sheet is a
resource of its own, so maps borrow freely. The cheapest borrowing is the
same tiles under new colours: `tileset/palettes/` holds 72 palettes with no
tiles of their own.

## Battle backgrounds

A battle's backdrop is one picture of 256 by 120 pixels in 128 colours,
stored row by row, not as tiles. There are 82 in `battle_backgrounds/`,
picked by where the party stood. They use 128 colours and not 256 so that
two can be loaded at once, which is how the credits fade one into the
next.

## Effects, fonts and the rest

- `battle_effects/` holds the flashes and blasts of a battle. An effect's
  pictures are often different sizes, so each is a PNG of its own
  ([Battle effects](../battle_effect/README.md)). `summon/` holds the
  pictures the summons are drawn from.
- `font/` holds the letters: one sheet for each language, a letter to a 16
  by 16 square, with a table of widths beside it so that an `i` takes less
  room than a `W`. The staff credits have a font of their own, in
  `credits/`.
- `ui/` is the windows' borders, icons and cursors, `title/` the Title's
  pictures and `ending/` the ending's. A picture with words in it has one
  file for each language that needs its own.

## How the game finds a picture

When the game is built, each PNG is split into its colours and its pixels,
and the pixels are usually packed. A picture that one part of the game
always uses, such as the font, is linked straight into that part. A picture
chosen while the game runs, such as a backdrop or a monster, is a numbered
resource ([The Game Pak](../gamepak/README.md#the-resource-directory-and-table-order)).
