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
`battle_anims/`, and each [layout](../../glossary.md#layout)'s `layout.s` in
[Layouts](../layout/README.md) places its tileset from `tileset/`.

## How a picture is built and placed

The console draws with a small list of colours per picture, and each pixel is
a number into that list. The PNG files here are stored the same way, as
indexed pictures with their own colour list, so an image editor can open them
while the game sees exactly the colours it shipped with.

The build packs each PNG, with the `.tsv` of packing options beside it where
there is one (a font's first glyph and its rows), into its colours and pixels,
usually compressed, and a short assembly file places the result on the
cartridge under the label the code uses. That `.s` usually sits beside the
picture; the tilesets are placed by the maps' `map.s` files and the pictures
in `window/` by [Windows](../window/README.md) and [Menus](../menu/README.md).
A picture the game asks for by number is also a row of the resource directory
([System](../system/README.md#the-resource-directory-and-table-order)): the
grassland battle background is `Battle_GrasslandBackdrop`, placed by
`battle_bg/grassland.s`, and `ResourceId_GrasslandBackdrop`, which
`BattleBackground_SelectForEnvironment` and the credits ask for. Some packed
pictures are followed by a few bytes [Camelot](../../glossary.md#camelot)'s
packing tool left behind; the `.s` keeps them beside the picture as literal
bytes marked `@ PACKER:`.

## The palette fade

![The title intro after the prologue: from black, the purple clouds, the mountain and the sun's lens flares brighten together until the picture has its full colours](https://alchemy.pascalpixel.com/guides/graphics/title-fade.gif)

The title intro fades in by changing colours, not the picture: every one of
the 512 colours on screen, backgrounds and sprites alike, takes an equal step
each frame from near-black towards its own colour, so they all arrive
together. The start and the target can be one colour everywhere, the
picture's own colours or a filtered version of them, such as grey or sepia.

`TitlePaletteFade_Initialize` saves both palettes as starting components in
the fade's [heap](../../glossary.md#heap-and-pools) block and adds
`TitlePaletteFade_Update` as a task. `TitlePaletteFade_Start` divides each
component's distance to the target by the number of frames, keeping the
remainder in its low ten bits. Each frame the update adds the steps to all
1,536 components, packs them into colours and queues both palettes for the
next VBlank, the pause after the screen's last line. The queue holds a
pointer to the colours until then, so the update packs each frame into the
spare one of two copies. `TitlePaletteFade_BuildColors` builds what a fade
starts from or heads for: one colour, a whole-palette filter, a tint, blend or
split, or a copy.

In [The Broken Seal](../../glossary.md#the-two-games) only `Title_ShowIntro`
uses the fade, 60 frames from `RGB(2, 0, 0)`, a dark red, to the picture's
colours; `TitlePaletteFade_Release`, `TitlePaletteFade_SetTarget` and
`TitlePaletteFade_SetBackdrop` are [`UNUSED`](../../glossary.md#unused).

| Name | Value | Meaning |
| --- | --- | --- |
| `PALETTES_ALL` | 0 | a setter changes both palettes |
| `PALETTES_BG` | 1 | the background palette only |
| `PALETTES_OBJECTS` | 2 | the sprite palette only |

## The title sequence

![The title intro: the clouds and the mountain scroll past the sun and its lens flares, ending on the title picture](https://alchemy.pascalpixel.com/story/title.gif)

After the logos, the camera seems to rise past a mountain towards the sun
until the Golden Sun title fills the screen. The sense of depth comes from
layers that scroll at different speeds: the clouds at the back slower than
the mountain in front, with the sun tucked behind the mountain and its lens
flares laid over everything.

The title [overlay](../../glossary.md#overlay), `Title_Run` in
[Menus](../menu/README.md), decides what runs; the code is here, in the title
[bank](../../glossary.md#bank). At power-on `Title_ShowNintendoLogo` shows the
Nintendo logo and `Title_ShowCamelotLogo` the Camelot logo for two seconds,
cycling its four frames of tiles. With a save file, `Title_ShowIntro` then
scrolls the backgrounds while the sprites rise along the slope until frame 278
and flashes white into the title picture, where "Press Start" pulses for up to
a minute; if nothing is pressed `Title_Run` starts again from the logos. With
no save file the logos lead straight to the main menu. After the prologue the
same intro plays without the prompt. A or Start cuts each part short.

`BG1`, the purple clouds round an opening of blue sky, scrolls a pixel every
third tick and `BG2`, the mountain, every other one, each copying the next band
of its picture in as a new row of tiles comes into view. The sun is a sprite
whose priority puts it behind the mountain, the lens flares are sprites in
front of everything, and both are blended over what is behind them.

## Two battle backgrounds in one background memory

![The staff names scroll up over a picture of Vale's houses in the rain, which fades into a stone hall lit by torches while the names keep moving.](https://alchemy.pascalpixel.com/guides/graphics/credits.gif)

While the staff names roll, the picture behind them fades from one battle
background to the next: stormy Vale, then Sol Sanctum's hall, then the
grassland and on. A fade between two pictures needs both in memory at once,
so the credits split the background's tile memory and colours into two
halves: one shows while the next picture loads into the other, the screen
blends across, and the halves swap roles.

The credits roll at the end of the game: the ending sends the party to the
title screen's [warp destination](../../glossary.md#warp-destination)
`TITLE_FROM_LEMURIAN_SHIP` with `MUS_ENDING` playing, `Title_Run` calls them
through the credits bank's [far-call](../../glossary.md#far-call) table as
`Import_CreditsRun`, and afterwards the game goes on to the main menu. They
show the 32 battle backgrounds of `gCredits_Backgrounds`, each crossfading
into the next; the last entry, 0, fades to black.

`BG3` draws from tiles in half 0 with colours 0 to 127, and `BG2` from tiles
in half 1 with colours 128 to 255. A battle background has 128 colours, and
`Credits_LoadBackground` adds 128 to every pixel it unpacks into half 1, so
the same picture works in either. `Credits_Run` loads each picture into the
half not showing and steps the blend between `BG2` and `BG3` over 16 steps of
4 frames. `Credits_DecodeBackground` runs from a copy of the decoder in the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_RAM_CODE_TEMP`, and the
colours reach palette RAM through the write queue at the next VBlank. Every
fourth frame a VBlank [task](../../glossary.md#task) moves both pictures a
pixel right; a picture is 256 pixels wide, as wide as its map, so it wraps
without a seam.

| Half | Background | Tile memory | Colours | Pixel offset |
| --- | --- | --- | --- | --- |
| 0 | `BG3` | `0x06000000`, 32 KB | 0 to 127 | 0 |
| 1 | `BG2` | `0x06008000`, 32 KB | 128 to 255 | 128 |

## The scrolling names

![The staff names scroll steadily up the screen over Sol Sanctum's torch-lit stone hall, each line appearing at the bottom edge and vanishing at the top](https://alchemy.pascalpixel.com/guides/graphics/credits-names.gif)

The list of names is far longer than the screen, and drawing it all at once
would take more tile memory than the console can spare. Instead the names live
in a ring of 32 rows, 16 on screen and 16 hidden: as the text scrolls up, each
new line is drawn into the hidden row just below the screen, over a line that
left the top long before, so the credits' memory never grows.

The names are sprites, not a background: six 32 by 8 sprites across each
192-pixel line, drawing from the ring. They move up a pixel every fourth
frame, so a new line arrives every 32 frames, and each time the scroll enters
a new line `Credits_DrawEnteringLine` draws the next line of text into the
ring row `CREDITS_VISIBLE_ROWS` below the top. Opaque border sprites over the
top 16 and bottom 24 pixels hide the rows only partly on screen; they come
first in OAM, and the hardware draws a lower entry over a higher one.
`Credits_DrawLine` draws each glyph in colour 15 with a colour 1 shadow a pixel
down and to the right. It takes its line buffer before checking for the end of
the text, so the last call leaks the buffer
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), fixed behind `BUGFIX`).

| Name | Value | Meaning |
| --- | --- | --- |
| `CREDITS_LINE_PIXELS` | 192 | a line's width |
| `CREDITS_COLUMNS` | 6 | 32-pixel sprites across a line |
| `CREDITS_LINE_TILES` | 24 | a line's 4bpp tiles |
| `CREDITS_RING_ROWS` | 32 | tile rows in the ring |
| `CREDITS_VISIBLE_ROWS` | 16 | rows on screen at a time |
| `CREDITS_BUFFER_BYTES` | `0x900` | a line's raster: nine rows of 256 one-byte pixels, the ninth for the shadow |
| `CREDITS_BORDER_SPRITES` | 24 | OAM entries 0 to 23, the opaque strips above and below the text |
| `CREDITS_OAM_ENTRIES` | 128 | the whole OAM: the borders, 96 text sprites and 8 parked off screen |

## Running outside the engine's frame

For their length the credits run the screen themselves: they reset the task
table, stop the VBlank's copy of the sprite list (`gOamCopyEnabled`) and send
their own 128 sprites from a 1 KB buffer taken from the heap's EWRAM pool and
never released (the heap is emptied before the next map). They also clear
`gCpuLoadDisplayEnabled`, `gDebugTextEnabled` and `gIdleSleepEnabled`. On
return `BG2`, `BG3` and blending are off, the window graphics are reloaded and
the OAM copy is back on; the three tasks they added keep running.

Their few numbers live in four cells borrowed from the flash library, which
isn't running while the credits roll and sets each cell before it reads it
(`IdentifyFlash` writes `gFlash`, `gFlash_Handler0` and `gFlash_Handler3` before
each use of the save, and each program routine sets
`gFlashNumRemainingBytes`). [The original](../../glossary.md#the-original)
reaches them through casts of the flash library's declarations, undefined
behaviour (`// UB:`); `UBFIX` gives them the credits' own types.

| Flash library cell | Credits' name | Holds |
| --- | --- | --- |
| `gFlashNumRemainingBytes` | `SCROLL_OBJECTS` | the sprite buffer's pointer |
| `gFlash_Handler3` | | the scroll position, in pixels |
| `gFlash` | `CREDITS_LAST_LINE_POSITION` | the position of the last line drawn |
| `gFlash_Handler0` | `CREDITS_END_OF_TEXT` | the end-of-text latch |

## Design

Only two parts of the folder have banks of their own: the credits are the
credits bank and the title intro the title bank ([Source](../README.md)).
Everything else here is linked beside the code that uses it.
`Message_FindArm` carries the text window's prefix, though it sits beside
`Text_DecodeSymbol` in `text/decode_symbol.s`.

## How it fits with the rest of the game

The title intro and the credits are main-image code the title overlay calls
through their banks' far-call tables. The decoders are called by the code
that loads pictures everywhere else, many from a copy in a heap slot
(`RamCode_LoadSlot`). The pictures belong to the code that loads them: battle
animations, maps, layouts, windows and menus.

## Limits

- A credits picture has at most 128 colours and 480 tiles, 256 by 120 pixels:
  half the background palette, and 30 KB of its half's 32 KB of tile memory,
  the picture's map taking the rest.
- A credits line is 192 pixels wide, six 32-pixel sprites, drawn centred.
- The palette fade covers all 512 background and sprite colours.
- The credits' waits come to 11,223 frames, about 3 minutes 8 seconds,
  besides the loads: a 300-frame lead, then for each of the 33 entries a
  64-frame fade and a 267-frame hold.

## Words used here

- **component**: one of a colour's three five-bit levels, red, green or blue,
  as the palette fade keeps it: in the top bits of a halfword, shifted up ten
  bits, so the ten bits below hold the fraction a fade step leaves.
- **half**: one of the two parts the credits split the background tile memory
  and palette into (`alternate` in the code).
- **tile ring**: the credits' 32 rows of text tiles, 16 on screen at a time,
  reused as the text scrolls.

## Evidence and open questions

The pictures are named for what they show, from the maps and battle
animations that load them, and the decoders for the format each unpacks,
proven in their headers. The credits' and the title's functions are named for
what they draw.

- XXX (no evidence): why the credits use the flash library's four cells
  rather than globals of their own; the ROM's code shows only that both reach
  the same addresses, no other code uses them, and only Camelot's link map or
  source could say whether the sharing was meant.
