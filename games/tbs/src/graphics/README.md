# Graphics

The grassy field behind a battle on the world map, the monsters, the fonts,
the [map](../../glossary.md#map) tiles and the title screen are all kept here as ordinary picture
files that anyone can open and edit. Every build packs them into the forms the
game reads, so the same files serve someone browsing the source and the
cartridge. The folder also holds the code of the two screens that are mostly
pictures, the title intro and the credits, the handwritten decoders that
unpack tiles and animation [frames](../../glossary.md#frame) for the rest of the game, and a few
helpers of the German and Italian message text that sit beside them in the
cartridge. The
[Battle animations](../battle_anim/README.md) load their sheets from `battle_anims/`, and
each [layout](../../glossary.md#layout)'s `layout.s` in [Layouts](../layout/README.md) places its tileset from
`tileset/`.

## How a picture is built and placed

Every picture is an indexed PNG that the build packs into the form the game
reads, placed by an `.s` file under the label the code uses.

The console draws with a small list of colours per picture, and each pixel is
a number into that list. The PNG files here are stored the same way, as
indexed pictures with their own colour list, so an ordinary image editor can
open them while the game sees exactly the colours it shipped with. Pictures
the game asks for by number rather than by name also get a row in the
resource directory
([System](../system/README.md#the-resource-directory-and-table-order)).

In the code, the build packs the PNG, with the `.tsv` of packing options
beside it where there is one (a font's first glyph and its rows, for
instance), into the colour list and the pixels, usually compressed, and a short
assembly file (`.s`) places the result on the cartridge under the label the
code uses. That `.s` usually sits beside the picture; the tilesets in
`tileset/` are placed by the maps' `map.s` files, and the pictures in
`window/` by `.s` files of [Windows](../window/README.md) and
[Menus](../menu/README.md). A picture loaded by number rather than by label is
an entry of the resource directory (`system/resource/directory.s`): the
grassland battle background is `Battle_GrasslandBackdrop`, placed by
`battle_bg/grassland.s`, and `ResourceId_GrasslandBackdrop` in the directory,
which `BattleBackground_SelectForEnvironment` and the credits ask for.

Some packed pictures are followed by a few bytes [Camelot](../../glossary.md#camelot)'s packing tool left
behind. The rebuild must reproduce them to match the cartridge, so the `.s`
keeps them beside the picture they follow, as literal bytes marked
`@ PACKER:`.

## The palette fade

The palette fade moves every background and [sprite](../../glossary.md#sprite-and-oam) colour a step a frame
towards a target, as when the title intro fades in from black.

A fade changes the colours, not the picture: every one of the 512 colours on
screen, background and sprite alike, moves an equal step each frame from
where it starts towards where it should end, so they all arrive together. The
start and the target can be one colour everywhere, the picture's own colours
or a filtered version of them, such as grey or sepia.

`TitlePaletteFade_Initialize` saves both palettes, 512 colours, as the starting
components in the palette fade's [heap](../../glossary.md#heap-and-pools) block and adds
`TitlePaletteFade_Update` as a task. `TitlePaletteFade_Start` divides each component's distance
to the target by the number of frames, keeping the remainder in the
component's low ten bits. Each frame the update adds the steps to all 1,536
components, packs them into colours and queues both palettes for the next
VBlank, the pause after the screen's last line when the hardware can be
changed without showing half a change. The queue holds a pointer to the colours until then, so the update
packs each frame into the spare one of two copies, and a frame's packing
never overwrites colours the queue has yet to send. A `selectedPalettes` of
`PALETTES_ALL`, `PALETTES_BG` or `PALETTES_OBJECTS` chooses which
palettes a setter changes.

`TitlePaletteFade_BuildColors` builds the colours a fade starts from or heads for: one
colour everywhere, a whole-palette filter (plain, grey, sepia and the
others), a tint, blend or split carrying a colour, or a copy. In [The Broken Seal](../../glossary.md#the-two-games)
only `Title_ShowIntro` uses the fade, for its 60-frame fade from near-black
(`RGB(2, 0, 0)`, a dark red) to the picture's own colours;
`TitlePaletteFade_Release`, `TitlePaletteFade_SetTarget` and `TitlePaletteFade_SetBackdrop`
are [`UNUSED`](../../glossary.md#unused).

The palette selections are named in `include/graphics/palbuf.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `PALETTES_ALL` | 0 | a setter changes both palettes |
| `PALETTES_BG` | 1 | the background palette only |
| `PALETTES_OBJECTS` | 2 | the sprite palette only |

## The title sequence

The title sequence is the logos at power-on, then the intro that scrolls
into the title picture and waits for Start.

![The title intro: the clouds and the mountain scroll past the sun and its lens flares, ending on the title picture](https://alchemy.pascalpixel.com/story/title.gif)

The title [overlay](../../glossary.md#overlay), `Title_Run` in [Menus](../menu/README.md), decides what
runs; the code is here, in the title [bank](../../glossary.md#bank). At power-on
`Title_ShowNintendoLogo` shows the Nintendo logo with its fanfare and
`Title_ShowCamelotLogo` the Camelot logo for two seconds, cycling its four
frames of tiles. When a save file exists, `Title_ShowIntro` then scrolls the
backgrounds down while the sprites rise along the slope until frame 278, and
flashes white into the title picture, where "Press Start" pulses for up to a
minute; if nothing is pressed `Title_Run` starts again from the logos. With
no save file, the logos lead straight to the main menu. After the
prologue the same intro plays without the prompt and holds the picture five
seconds. A or Start cuts each part short.

The intro's sense of depth comes from layers that scroll at different
speeds. Of the backgrounds, `BG1` at the back scrolls a pixel every third
tick and `BG2` in front every other one, each copying the next band of its
picture in as a new row of tiles comes into view. `BG1` holds the purple
clouds around an opening of blue sky and `BG2` the mountain. The sun is a
sprite whose priority puts it behind the mountain, the lens flares are sprites
in front of everything, and the sun and the flares are blended over what is
behind them.

## Two battle backgrounds in one background memory

The credits fade from one battle background to the next, and to do that they
split the background's memory in two.

While the staff names roll, the picture behind them fades from one battle
background to the next: stormy Vale, then Sol Sanctum's stone hall, then the
grassland, and so on. A fade between two pictures needs both in memory at
once, so the credits split the background's tile memory and colours into two
halves. One half shows while the next picture is loaded into the other, the
screen blends across, and the halves swap roles.

The credits roll over 32 battle backgrounds
(`gCredits_Backgrounds`), each crossfading into the next, and its
last entry, 0, fades the screen to black. They run at the end of the game:

1. the ending sends the party to the title screen's [warp destination](../../glossary.md#warp-destination)
   `TITLE_FROM_LEMURIAN_SHIP`, with `MUS_ENDING` playing;
2. `Title_Run` calls the credits through the credits bank's [far-call](../../glossary.md#far-call) table,
   as `CreditsFar_Entry00`;
3. after them the game goes on to `MapId_MainMenu`, the main menu.

The two halves split the 256-colour background tile memory and the
background palette. `BG3` draws from tiles in half 0 with colours 0 to 127,
and `BG2` from tiles in half 1 with colours 128 to 255. A battle background has 128
colours, and `Credits_LoadBackground` adds 128 to every pixel
it unpacks into half 1, so the same picture works in either.

`Credits_Run` loads each picture into the half that isn't
showing, the first into half 1, steps the blend weights between `BG2` and
`BG3` over 16 steps of 4 frames, and holds. The halves alternate from one
picture to the next, so a picture is never loaded while it is shown.
`Credits_DecodeBackground` unpacks the tiles, running from a copy of the decoder
loaded into the [heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_RAM_CODE_TEMP` and released after each
load, and the colours reach palette RAM through the I/O
write queue at the next VBlank. Every fourth frame a VBlank [task](../../glossary.md#task) moves both
pictures a pixel right; a picture is as wide as its map, 256 pixels, so it
wraps without a seam.

| Half | Background | Tile memory | Colours | Pixel offset |
| --- | --- | --- | --- | --- |
| 0 | `BG3` | `0x06000000`, 32 KB | 0 to 127 | 0 |
| 1 | `BG2` | `0x06008000`, 32 KB | 128 to 255 | 128 |

## The scrolling names

The staff names are drawn a line at a time into a small ring of tiles, just
before each line comes into view.

The list of names is far longer than the screen, and drawing all of it at
once would take more tile memory than the console has to spare. Instead the
names live in a ring of 32 rows: 16 on screen, 16 hidden. As the text scrolls
up, each new line is drawn into the hidden row just below the screen, over a
line that left the top long ago, and the ring wraps round. The credits'
memory never grows, however many names there are.

The names are sprites, not a background: six 32-by-8 sprites across each
192-pixel line, drawing from the tile ring. They move up a pixel every
fourth frame, so a new line arrives every 32 frames. Each time the scroll
enters a new line, `Credits_DrawEnteringLine` draws the next line of
text into the ring row `CREDITS_VISIBLE_ROWS` below the top visible row, the
first hidden one. Opaque border sprites over the top 16 and bottom 24 pixels
hide the rows that are only partly on screen; they come first in OAM, and the
hardware draws a lower entry over a higher one, so they cover the text.

![The staff names scroll up over a picture of Vale's houses in the rain, which fades into a stone hall lit by torches while the names keep moving.](https://alchemy.pascalpixel.com/guides/graphics/credits.gif)

`Credits_DrawLine` draws each glyph in colour 15 with a colour 1
shadow a pixel down and to the right. It takes its line buffer before
checking for the end of the text, so the last call leaks the buffer
([`// BUG:`](../../glossary.md#bug-bugfix-and-ubfix), fixed behind `BUGFIX`).

The sizes are macros at the top of `credits/credits.c`.

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

The credits switch most of the engine's frame off and keep their state in
four cells borrowed from the flash library.

For their length the credits run the screen themselves: they send their own
sprite list each frame instead of the engine's, and they keep the few numbers
they need (where the scroll is, which line was drawn last) in four variables
that belong to the save code, which is not running while the credits roll.

The credits reset the task table, stop the VBlank's copy of the sprite list
(`gOamCopyEnabled`) and send their own 128 sprites from a 1 KB buffer taken
from the heap's EWRAM pool and never released; the heap is emptied
before the next map, so the buffer goes then. They also clear
`gCpuLoadDisplayEnabled`, `gDebugTextEnabled` and `gIdleSleepEnabled`. On
return `BG2`, `BG3` and blending are off, the window graphics are reloaded and
the OAM copy is back on; the three tasks they added keep running.

Their state lives in four cells borrowed from the flash library:
`gFlashNumRemainingBytes` holds the sprite buffer's pointer,
`gFlash_Handler3` the scroll position in pixels, `gFlash` the position of
the last line drawn and `gFlash_Handler0` the end-of-text latch. The borrow
is safe because the flash library sets each cell before it reads it:
`IdentifyFlash`, which runs before each use of the save, writes `gFlash`,
`gFlash_Handler0` and `gFlash_Handler3`, and each program routine sets
`gFlashNumRemainingBytes`. [The original](../../glossary.md#the-original) reaches the cells through casts of
the flash library's declarations, which is undefined behaviour (`// UB:`);
`UBFIX` gives them the credits' own types.

| Flash library cell | Credits' name | Holds |
| --- | --- | --- |
| `gFlashNumRemainingBytes` | `SCROLL_OBJECTS` | the sprite buffer's pointer |
| `gFlash_Handler3` | | the scroll position, in pixels |
| `gFlash` | `CREDITS_LAST_LINE_POSITION` | the position of the last line drawn |
| `gFlash_Handler0` | `CREDITS_END_OF_TEXT` | the end-of-text latch |

## How it fits with the rest of the game

The title intro and the credits are main-image code the title overlay
calls through their banks' [far-call](../../glossary.md#far-call) tables. The decoders are called by the
code that loads pictures everywhere else, many of them from a copy in a heap
slot (`RamCode_LoadSlot`), as `Credits_DecodeBackground` is. The pictures belong to
the code that loads them: battle animations, maps, layouts, windows and menus.

## Limits

- A credits picture has at most 128 colours and 480 tiles, 256 by 120
  pixels: half the background palette, and 30 KB of its half's 32 KB of
  tile memory, the picture's map taking the rest.
- A credits line is 192 pixels wide, six 32-pixel sprites, drawn centred.
- The palette fade covers all 512 background and sprite colours.
- The credits' waits come to 11,223 frames, about 3 minutes 8 seconds,
  besides the loads: a 300-frame lead, then for each of the 33
  entries a 64-frame fade and a 267-frame hold.

## Files

| File | What it holds |
| --- | --- |
| `title/intro.c` | the title intro, the Nintendo and Camelot logos, and the palette fade |
| `credits/credits.c` | the credits: the battle backgrounds that slide and crossfade, and the scrolling text |
| `text/numwords.c` | the counted nouns the German message decoder writes after a number |
| `text/genders.s` | the grammatical gender of each item name, for the German and Italian [editions](../../glossary.md#editions) |
| `text/decode_symbol.s`, [`render/decode.s`](render/decode.s), `tile/*.s` | handwritten ARM and Thumb decoders, copiers and a dot product, each proven in its header |
| `battle_anims/` | the battle animations' sheets and masks, loaded by resource id |
| `ui/` | window tiles, the small font, icons and menu sprites |
| `font/` | the main text font in each language's version (`gFont_Glyphs`) |
| `world_map/` | the world map's tiles and palettes and the story frames |
| `tileset/` | the maps' tilesets and their palettes |
| `window/` | the naming screen's keyboards, the cursor frame and the main font's glyph widths |
| `shop/` | the eight frames of the shop cursor, `MenuCursor_FrameA` to `MenuCursor_FrameH` (`ResourceId_MenuCursorFrames`) |
| `title/` | the title picture, the logos, the "Press Start" prompt and the "To be continued" lettering |
| `render/labels.s` | the 64-by-16 text headings, each two 32-by-16 sprites |
| `credits/font.s` | the credits font, with its glyph widths in `credits/glyph_widths.h` |
| `battle_bg/`, `monster/`, `sprite/`, `colosso/`, `lamakan/`, `lucky_dice/`, `lucky_wheels/`, `sol_sanctum_statue/` | the pictures of the map or screen they name |
| `animref*.s` | animation slot tables, whose entries point back at their own table |

The unit headers are in `games/tbs/include/unit/graphics/`. The credits are
the credits bank and the title intro the title bank
([Source](../README.md)); everything else here is linked where its users
are, as `games/tbs/objects.tsv` lists.

## Words used here

- **component**: one of a colour's three five-bit levels, red, green or
  blue, as the palette fade keeps it: in the top bits of a halfword, shifted
  up ten bits, so the ten bits below hold the fraction a fade step leaves.
- **half**: one of the two parts the credits split the background tile
  memory and palette into (`alternate` in the code). Half 0 is the first
  32 KB of VRAM with colours 0 to 127, half 1 the next 32 KB with colours
  128 to 255.
- **tile ring**: the credits' 32 rows of text tiles, 16 on screen at a time,
  reused as the text scrolls.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Title_` | the title intro and the logos |
| `PaletteFade_`, `Palette_` | the palette fade, and `TitlePaletteFade_BuildColors`, which builds its colours |
| `Credits_` | the credits |
| `Graphics_` | tile memory and palettes, as the engine names them elsewhere |
| `Tile_`, `TileMap_`, `BitDecoder_`, `Render_`, `Text_` | the handwritten decoders, among them `Credits_DecodeBackground`, which unpacks the credits' pictures |
| `Vector_` | `Vector_DotProduct`, a 16.16 dot product in `tile/dot_product.s` that nothing calls |

`Message_FindArm` carries the text window's prefix, though it sits
beside `Text_DecodeSymbol` in `text/decode_symbol.s`.

## Evidence and open questions

The pictures are named for what they show, from the maps and battle animations
that load them, and the decoders for the format each unpacks, proven in their
headers. The credits' and the title's functions are named for what they
draw.

- XXX (no evidence): why the credits use the flash library's four cells
  rather than globals of their own; the ROM's code shows only that both reach
  the same addresses, no other code uses them, and only Camelot's link map or
  source could say whether the sharing was meant.
