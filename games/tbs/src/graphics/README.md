# Graphics

The images the game shows are kept here as ordinary PNGs anyone can open and
edit: the battle backgrounds, the monsters, the fonts, the
[map](../../glossary.md#map) tiles, the logos and the Title. Every build packs
them into the forms the game reads. The folder also holds the code of the two
screens that are mostly images, the title intro and the credits, and the
handwritten decoders that unpack tiles and text for the rest of the game,
beside a few tables of the German and Italian message text.
[Battle effects](../battle_effect/README.md) load their sheets from
`battle_effects/`, and each map in [Maps](../map/README.md) places its tiles
from `tileset/`.

## How an image is built and placed

The console draws each image from a palette of 16 or 256 colours, and each
pixel is a number into it; a 16-colour image also carries a palette number
saying which of sixteen palettes it uses. The PNGs here are stored the same way, as indexed images
with their own colour list, so an image editor can open them while the game
sees exactly the colours it shipped with.

Each build packs every image into the colours and pixels the game reads,
usually compressed, and places the result on the cartridge under the name the
code uses, most of it after the code, at the end of the cartridge. The placing is a short assembly file in each folder:
`battle_bg/battle_bg.s` places `Battle_GrasslandBackdrop` from
`grassland.gbapal` and the packed `grassland.bitmap.d7`. An image the game asks
for by number is also a row of the
[resource directory](../cartridge/README.md#the-resource-directory-and-table-order):
the grassland is asked for that way both by a battle on open ground and by the
credits. Some packed images are followed by a few bytes
[Camelot](../../glossary.md#camelot)'s packing tool left behind, kept beside
the image with a `PACKER:` note.

A `.tsv` beside an image holds its packing options, such as a font's first
letter. A sprite's `.tsv` in `sprite/` also holds its animations, one line
each, written as the commands the field reads: `picture 55 6` shows
[picture](../../glossary.md#picture) 55 for 6 frames, and `loop`, `play 1` or
`hold` says what follows the last picture: start again, switch to animation 1,
or stay on it.

## The intro

The title intro is the short scene that plays after the logos, before the File
Screen.

![The title intro: the clouds and the mountain scroll past the sun and its lens flares, ending on the Golden Sun logo](https://alchemy.pascalpixel.com/story/title.gif)

At power-on the Nintendo logo shows with its jingle, then the Camelot logo for
two seconds. If the cartridge holds a save, the intro follows: the clouds and
the mountain scroll down while the sun and its lens flares rise along the
mountain's slope, then the screen flashes white into the Golden Sun logo, where
"Press Start" pulses for up to a minute. If nothing is pressed, the screen
fades out and the game starts again from the logos. With no save the logos
lead straight to the File Screen. After the prologue the same intro plays
without the prompt, holds the logo five seconds and goes on to Vale. A or
Start cuts each part short.

The sense of depth comes from two
[background layers](../../glossary.md#background-layer) scrolling at
different speeds: the clouds at the back move a pixel every third frame, the
mountain in front every other frame. The sun and its lens flares are
sprites, placed by a VBlank task, `Title_UpdateIntroSprites`, which alternates
two sets every other VBlank so twice as many show.

The Title is a [scene](../../glossary.md#scene) like any other: its
[overlay](../../glossary.md#overlay)'s entry, `Title_Run` in
[Menus](../menu/README.md), chooses by the entrance it was started with whether
to show the logos and the intro, the intro after the prologue, the credits
after the ship sails or the Golden Sun logo after the ending. The screens are
drawn by `Title_ShowNintendoLogo`, `Title_ShowCamelotLogo` and
`Title_ShowIntro`, in the title [bank](../../glossary.md#bank).

### The title intro's own palette fade

The intro fades in by changing colours, not the image: every one of the 512
colours on screen, backgrounds and sprites alike, takes an equal step each
frame from a dark red towards its own colour, so they all arrive together
after 60 frames. Each colour is kept as three components, red, green and blue,
with ten bits of fraction below each, so a small change spread over many
frames still moves; on the last frame the fade copies the target colours
outright. Each frame the new colours are packed into one of two pages, while
the other page waits to reach the screen at the next VBlank.

The fade can also head for a single colour or a filtered palette, but the game
uses it only here. It is a [task](../../glossary.md#task),
`TitlePaletteFade_Update`, set up by `TitlePaletteFade_Initialize` and started
by `TitlePaletteFade_Start`.
Scenes fade through the general palette fade instead, `PaletteFade_Update` in
[color.c](../overworld/overworld/color.c), which the field sequence starts for
every scene and which covers 224 background and 224 sprite colours.

## The credits

The credits are the staff names that roll over the game's battle backgrounds
after the ending, as the Lemurian ship sails off.

![The staff names scroll up over a picture of Vale's houses in the rain, which fades into a stone hall lit by torches while the names keep moving.](https://alchemy.pascalpixel.com/guides/graphics/credits.gif)

The backgrounds crossfade one into the next: stormy Vale, then Sol Sanctum's
hall, then the grassland, 32 in all, and last a fade to black, after which the
game goes on to the File Screen. `Credits_Run` runs them; the title overlay
calls it through the credits bank's [far-call](../../glossary.md#far-call)
table.

### Two battle backgrounds in one background memory

A crossfade needs both images in memory at once, so the credits split the
background tile memory and colours into two halves: one shows while the next
image loads into the other, the screen blends across in 64 frames, about a
second, and the halves swap roles. A battle background uses 128 colours,
exactly half of what the background layers can show, so each half takes one
image with its own 128 colours. An image loaded into the second half has its
colour numbers moved up by 128 as it is unpacked, so the same image works in
either. Both images drift right a pixel every fourth VBlank, and since each is
exactly as wide as its layer, it wraps round without a seam.
`Credits_LoadBackground` loads each image of `gCredits_Backgrounds`.

### The scrolling names

The list of names is far longer than the screen, and drawing it all at once
would take more tile memory than the console can spare. Instead the names live
in a ring of 32 rows of tiles, 16 on screen and 16 hidden: as the text scrolls
up, each new line is drawn into the hidden half of the ring, over a line that
left the top long before, so the credits' memory never grows.

The names are sprites, not a background: six sprites side by side make each
line of 192 pixels. They move up a pixel every fourth VBlank, so a new line
arrives every 32 VBlanks, about half a second. Opaque strips of sprites at the
top and bottom of the screen hide the rows that are only partly on screen. Each
letter is drawn with a shadow a pixel down and to the right. The original game
forgets to give back one line's buffer at the very end
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)), which does no harm,
since the [heap](../../glossary.md#heap-and-pools) is emptied before the File
Screen starts. Each line is drawn by `Credits_DrawEnteringLine` and
`Credits_DrawLine`, set up by `Credits_InitTextSprites`; the text is
`gCredits_Lines`.

### Taking over the screen

For their length the credits run the screen themselves: they clear the task
table and add their own tasks, stop the system's usual copy of the sprite list
and send their own, and switch off the debug displays and the idle sleep. When
they end they reload the windows' tiles and turn the sprite copy back on, as
the next screen expects. The title intro does the same with its tasks and
sprites.

The credits keep their state (the sprite buffer's address, the scroll, the last
line drawn and an end-of-text latch) in four words of memory that belong to the
flash chip's code, which isn't running while the credits roll and sets each
word again before it reads it. [The original](../../glossary.md#the-original)
reaches the words in a way the C language leaves undefined (`// UB:`), and
`UBFIX` gives them proper types.

## The handwritten decoders

The routines that unpack tiles, sprites and text everywhere else are
assembly, because each does something the game's compiler never produces:
saving registers without the return address, a bit reader that carries a bit
through the carry flag (`rrx`), a jump computed into a table of command
routines, or `r11` used as an ordinary register. Each file opens with the
instructions that prove it, and each is linked into the bank of the code that
calls it.

| Routine | What it unpacks | Called from |
| --- | --- | --- |
| `Text_DecodeSymbol` | one symbol of packed message text | the windows |
| `Tile_Decompress4bpp`, `Tile_ExpandMasked`, `Tile_ExpandOpaque` | window images and icons | the windows |
| `Render_DecodePicture` | a sprite animation's picture into its sprite | the field's sprite drawing |
| `TileMap_DrawRows` | the world map's still picture, from its scaled layer | the world map |
| `Tile_ConvertMapGrid` | a map's three decoded planes into cells | the map grid, as a map loads |
| `BitDecoder_DecodeImage` | a battle background | the battle |
| `Credits_DecodeBackground` | a battle background, its colours moved into a half | the credits |

Each is written in the processor's 32-bit instructions, which run slowly over
the cartridge's 16-bit bus, so its caller copies it into RAM first, a
[heap slot](../../glossary.md#heap-slot) or the heap
([Field](../field/README.md#routines-run-from-ram)). For
the two battle background decoders the copy is needed: they rewrite their own
table of commands before they start, which code in ROM cannot do.
`text/decode_symbol.s` also holds `Message_FindArm`, which finds where a
message starts in the packed text for the windows.

## Limits

- A credits image has at most 128 colours and 256 by 120 pixels: half the
  background colours, and 30 KB of its half's 32 KB of tile memory.
- A credits line is 192 pixels wide and always centred (the drawing routine
  can also align left or right, but nothing asks it to).
- The credits last about 3 minutes 8 seconds, besides the time taken loading.

## Words used here

- **component**: one of a colour's three five-bit levels, red, green or blue,
  as the title intro's fade keeps it, with a fraction below it.
- **half**: one of the two parts the credits split the background tile memory
  and colours into (`alternate` in the code).
- **tile ring**: the credits' 32 rows of text tiles, 16 on screen at a time,
  reused as the text scrolls.
