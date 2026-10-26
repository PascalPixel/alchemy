# Graphics

The grassland behind a battle, the monsters, the fonts, the
[map](../../glossary.md#map) tiles and the title screen are kept here as
ordinary picture files anyone can open and edit, and every build packs them
into the forms the game reads. The folder also holds the code of the two
screens that are mostly pictures, the title intro and the credits, the
handwritten decoders that unpack tiles and animation
[frames](../../glossary.md#frame) for the rest of the game, and a few helpers
of the German and Italian message text that sit beside them on the cartridge.
[Battle animations](../battle_anim/README.md) load their sheets from
`battle_anims/`, and each [layout](../../glossary.md#layout) in
[Layouts](../map/README.md) places its tiles from `tileset/`.

## How a picture is built and placed

The console draws with a small list of colours per picture, and each pixel is
a number into that list. The PNG files here are stored the same way, as
indexed pictures with their own colour list, so an image editor can open them
while the game sees exactly the colours it shipped with.

Each build packs every picture into the colours and pixels the game reads,
usually compressed, and places the result on the cartridge under the name the
code uses. A picture the game asks for by number is also a row of the
resource directory
([System](../system/README.md#the-resource-directory-and-table-order)): the
grassland behind a battle is asked for that way, both by the battle and by
the credits. Some packed pictures are followed by a few bytes
[Camelot](../../glossary.md#camelot)'s packing tool left behind, which are
kept beside the picture as they are.

The placing is a short assembly file, usually beside its picture: the
grassland is `battle_bg/grassland.s`, which places `Battle_GrasslandBackdrop`.
A `.tsv` beside a picture holds its packing options, such as a font's first
letter.

## The palette fade

![The title intro after the prologue: from black, the purple clouds, the mountain and the sun's lens flares brighten together until the picture has its full colours](https://alchemy.pascalpixel.com/guides/graphics/title-fade.gif)

The title intro fades in by changing colours, not the picture: every one of
the 512 colours on screen, backgrounds and sprites alike, takes an equal step
each frame from near-black towards its own colour, so they all arrive
together. Each colour is three levels, red, green and blue, and each level
keeps a fraction beside it, so that a level which must rise by 7 over 60
frames still arrives exactly. Every frame the new colours are handed to the
screen in the next blank. The fade can also start from or head for one colour
everywhere, or a filtered version of the picture, such as grey or sepia, but
this game uses it only once: the intro's 60-frame fade from a dark red.

The fade is a [task](../../glossary.md#task), `TitlePaletteFade_Update`, set
up by `TitlePaletteFade_Initialize` and started by `TitlePaletteFade_Start`.

## The title sequence

![The title intro: the clouds and the mountain scroll past the sun and its lens flares, ending on the title picture](https://alchemy.pascalpixel.com/story/title.gif)

After the logos, the camera seems to rise past a mountain towards the sun
until the Golden Sun title fills the screen. The sense of depth comes from
layers that scroll at different speeds: the clouds at the back slower than
the mountain in front, with the sun tucked behind the mountain and its lens
flares laid over everything, both blended with what is behind them.

At power-on the Nintendo logo shows, then the Camelot logo for two seconds.
With a save file, the intro then scrolls the pictures while the sun rises
along the slope, and flashes white into the title picture, where "Press
Start" pulses for up to a minute; if nothing is pressed, the game starts
again from the logos. With no save file the logos lead straight to the main
menu. After the prologue the same intro plays without the prompt. A or Start
cuts each part short.

The title's [overlay](../../glossary.md#overlay), `Title_Run` in
[Menus](../menu/README.md), decides what runs; the pictures are drawn by
`Title_ShowNintendoLogo`, `Title_ShowCamelotLogo` and `Title_ShowIntro`, here
in the title [bank](../../glossary.md#bank).

## Two battle backgrounds in one background memory

![The staff names scroll up over a picture of Vale's houses in the rain, which fades into a stone hall lit by torches while the names keep moving.](https://alchemy.pascalpixel.com/guides/graphics/credits.gif)

While the staff names roll, the picture behind them fades from one battle
background to the next: stormy Vale, then Sol Sanctum's hall, then the
grassland and on. A fade between two pictures needs both in memory at once,
so the credits split the background's tile memory and colours into two
halves: one shows while the next picture loads into the other, the screen
blends across, and the halves swap roles.

The credits roll at the end of the game, as the Lemurian ship sails off with
the ending music playing, and afterwards the game goes on to the main menu.
They show 32 battle backgrounds, each crossfading into the next over about
a second, and end by fading to black. A battle background uses 128 colours,
exactly half of what the background layers can show, so each half of memory
takes one picture with its own 128 colours; a picture loaded into the second
half has its colour numbers moved up by 128 as it is unpacked, so the same
picture works in either. Both pictures also drift right a pixel every fourth
frame, and since each is exactly as wide as the layer, it wraps round without
a seam.

The credits are `Credits_Run`, which loads each picture with
`Credits_LoadBackground`; the pictures are listed in `gCredits_Backgrounds`.

## The scrolling names

![The staff names scroll steadily up the screen over Sol Sanctum's torch-lit stone hall, each line appearing at the bottom edge and vanishing at the top](https://alchemy.pascalpixel.com/guides/graphics/credits-names.gif)

The list of names is far longer than the screen, and drawing it all at once
would take more tile memory than the console can spare. Instead the names live
in a ring of 32 rows, 16 on screen and 16 hidden: as the text scrolls up, each
new line is drawn into the hidden row just below the screen, over a line that
left the top long before, so the credits' memory never grows.

The names are sprites, not a background: six sprites side by side make each
line of 192 pixels. They move up a pixel every fourth frame, so a new line
arrives every 32 frames. Opaque strips at the top and bottom of the screen
hide the rows that are only partly on screen. Each letter is drawn with a
shadow a pixel down and to the right. The original game forgets
to give back one line's memory at the very end
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix)), which does no harm,
since all memory is cleared before the next map.

Each line is drawn by `Credits_DrawEnteringLine` and `Credits_DrawLine`, set
up by `Credits_InitTextSprites`; the text is `gCredits_Lines`.

## Running outside the engine's frame

For their length the credits run the screen themselves: they set aside the
engine's usual task list and its copy of the sprite list, send their own
sprites, and switch off the debug displays and the idle sleep. When they end
they put the display back as the next screen expects it.

The few numbers they keep live in four cells of memory borrowed from the save
code, which isn't running while the credits roll and sets each cell again
before it uses it. [The original](../../glossary.md#the-original) reaches the
cells in a way the C language leaves undefined (`// UB:`), and `UBFIX` gives
them proper types. `Credits_InitTextSprites` names the four cells.

## Design

Only two parts of the folder have banks of their own: the credits are the
credits bank and the title intro the title bank ([Source](../README.md)).
Everything else here is linked beside the code that uses it.
`Message_FindArm` carries the text window's prefix, though it sits beside
`Text_DecodeSymbol` in `text/decode_symbol.s`.

## How it fits with the rest of the game

The title intro and the credits are main-image code the title overlay calls
through their banks' [far-call](../../glossary.md#far-call) tables. The
decoders are called by the code that loads pictures everywhere else, many
from a copy in a [heap slot](../../glossary.md#heap-slot). The pictures
belong to the code that loads them: battle animations, maps, layouts, windows
and menus.

## Limits

- A credits picture has at most 128 colours and 256 by 120 pixels: half the
  background colours, and 30 KB of its half's 32 KB of tile memory.
- A credits line is 192 pixels wide, drawn centred.
- The palette fade covers all 512 background and sprite colours.
- The credits last about 3 minutes 8 seconds, besides the time taken loading.

## Words used here

- **component**: one of a colour's three five-bit levels, red, green or blue,
  as the palette fade keeps it, with a fraction beside it.
- **half**: one of the two parts the credits split the background tile memory
  and colours into (`alternate` in the code).
- **tile ring**: the credits' 32 rows of text tiles, 16 on screen at a time,
  reused as the text scrolls.

## Evidence

The pictures are named for what they show, from the maps and battle
animations that load them, and the decoders for the format each unpacks,
proven in their headers. The credits' and the title's functions are named for
what they draw.
