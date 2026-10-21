# Graphics

Most of The Broken Seal's pictures live here, from monsters and battle
backdrops to fonts and map tiles, along with the code for the screens that are
mostly pictures.

Those screens are the title intro, with the Nintendo and Camelot logos before
it and the palette fade it uses, and the end credits. Next to them are the
handwritten decoders that unpack tiles and animation frames for the rest of
the game. Everything else here is data: the
[Battle effects](../effect/README.md) load their sheets from `battle_fx/`, and
each map's `map.s` in [Maps](../map/README.md) places its tile set from
`tileset/`.

## How a picture gets into the ROM

Each picture is an indexed PNG, one whose pixels are numbers into its own
palette, which is how the GBA draws them too. The build packs the PNG, with the
`.tsv` beside it where there is one, into the form the game reads, and an `.s`
file places the result in the ROM under the label the code knows it by.
Usually that `.s` sits right beside the picture. The tile sets in `tileset/`
are placed by the maps' `map.s` files instead, and the pictures in `window/`
by `.s` files of the [Windows](../window/README.md) and
[Menus](../menu/README.md) code.

Some packed pictures are followed by a few stray bytes that Camelot's packing
tool left behind, and the original kept them. The `.s` keeps them too, as
literal bytes marked `@ PACKER:`, beside the picture they follow.

A picture an effect or a scene loads by number, rather than by label, is an
entry of the resource directory (`games/tbs/src/system/resource/directory.s`).
The unit headers for the C files are in `games/tbs/include/unit/graphics/`,
and the link order is in `games/tbs/objects.tsv`.

## Files

| File | What it holds |
| --- | --- |
| `title/intro.c` | the title intro, the Nintendo and Camelot logos, and the palette fade (`PaletteFade_`) |
| `credits/credits.c` | the credits: the backdrops that slide and crossfade, and the scrolling text |
| `text/numwords.c` | the counted nouns the German message decoder writes after a number |
| `text/genders.s` | the grammatical gender of each item name, for the German and Italian editions |
| `text/decode_symbol.s`, [`render/decode.s`](render/decode.s), `tile/*.s` | handwritten ARM and Thumb decoders, copiers and a dot product, each proven in its header |
| `battle_fx/` | the battle effects' sheets and masks (`BattleFx_`), loaded by resource id |
| `ui/` | window tiles, the small font, icons and menu sprites |
| `font/` | the main text font, in each language's version (`gUiText_Glyphs`) |
| `world_map/` | the world map's tiles, palettes and the story frames |
| `tileset/` | the maps' tile sets and their palettes |
| `window/` | the name entry keyboards, the cursor frame and the main font's glyph widths |
| `shop/` | the eight frames of the shop cursor, `Shop_CursorFrameA` to `Shop_CursorFrameH` (resource `ResourceId_ShopCursorFrames`) |
| `title/`, `render/labels.s` | the title screen, the logos and the "Press Start" lettering, the "To be continued" lettering, and the text headings |
| `display/credits.s` | the credits font, with its glyph widths in `display/glyph_widths.h` |
| `backdrop/`, `monster/`, `sprite/`, `colosso/`, `lamakan/`, `lucky_dice/`, `slots/`, `sol_sanctum_statue/` | the pictures of the place or screen they name |
| `animref*.s` | animation slot tables, whose entries point back at their own table |

## Colours, halves and the tile ring

The GBA keeps a colour as three five-bit levels, red, green and blue, each
from 0 to 31. The palette fade works on those levels one by one and calls each
a **component**. It keeps a component in the top bits of a halfword (a 16-bit
value), the level shifted up by ten bits, and works out each frame's step by
dividing the distance to the target by the number of frames, so the ten bits
below the level hold the fraction a step leaves over. It steps all 1536 components of the 512
background and sprite colours, and its `paletteHalf` picks what to fade: 0
both, 1 only the background colours, 2 only the sprites'.

A **half** is one of the two parts the credits split the background tile
memory and the background palette into (`alternate` in the code). Half 0 is
the first 32 KB of video memory (VRAM) with colours 0 to 127, and half 1 the
next 32 KB with colours 128 to 255.

The **tile ring** is the credits' 32 rows of text tiles, sixteen of them on
screen at a time, used over and over as the text scrolls.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Title_` | the title intro and the logos |
| `PaletteFade_`, `Palette_` | the palette fade, and `Palette_Transform`, which builds its colours |
| `DisplayScroll_` | the credits |
| `Graphics_` | tile memory and palettes, as the engine names them elsewhere |
| `Tile_`, `TileMap_`, `BitDecoder_`, `Render_`, `Text_` | the handwritten decoders |

A few routines here carry another folder's prefix. The two ARM routines in
`tile/dot_product.s` are `BattleEffect_DotProduct`, which nothing calls, and
`BattleEffect_Expand`, which only the credits use, and
`UiText_LookupMessageArm` sits beside `Text_DecodeSymbol`.

## The title and the logos

At power-on, `Title_ShowSplashScreen` shows the Nintendo logo with its jingle
and `Title_ShowAnimatedSplash` the Camelot logo for two seconds, cycling its
four frames of tiles; A or Start cuts either of them short. Then, when a save
file exists, `Title_ShowIntro` plays the intro: the backgrounds scroll down and
the sprites rise along the slope until frame 278 (A or Start skips ahead), and
the screen flashes white into the title picture, where "Press Start" pulses
for up to a minute. The same intro plays after the prologue without the
prompt, and then the picture stays five seconds. Which of these runs is
decided by the title overlay, `Title_Run` in [Menus](../menu/README.md).

The intro fades in from black over 60 frames through the palette fade, which
moves every colour's components a step at a time towards a target.
`PaletteFade_Init` saves both palettes, 512 colours, in the colour buffer's
heap block and schedules `PaletteFade_Update`. `PaletteFade_Start` works out
how far each component has to move each frame, and each frame the update adds
those steps, packs the components back into colours and queues both palettes
to be copied at the next VBlank, the short pause between frames when the
screen isn't being drawn. The queue keeps a pointer to the colours until then,
so the update packs each frame into the spare one of two copies, and this
frame's packing can't overwrite the colours the queue still has to send.

`Palette_Transform` builds the colours a fade starts from or heads for: one
colour everywhere, the whole-palette filters (plain, grey, sepia and the
others), the tint, blend and split modes that carry a colour, or a copy. Only
`Title_ShowIntro` uses the fade in ☀️; `PaletteFade_Release`,
`PaletteFade_SetTarget` and `PaletteFade_SetBackdrop` are kept, unused.

## How the credits are drawn

At the end of the game, once the Lemurian ship sets sail, the staff credits
roll over a slideshow of battle backdrops (`gDisplayScroll_SlideResources`),
from the storm over Vale to a violet sky, each one fading into the next until
the screen goes black.
Afterwards the file select screen offers to save the cleared game.
`Title_Run` reaches the credits as `ScrollFar_Entry00`, at the title scene's
entrance 9, with `MUS_067` playing. They run a little over three minutes: a
300-frame lead, then 33 slides, each a 64-frame fade and a 267-frame hold,
11223 frames in all besides the loading.

### Two backdrops on screen at once

A crossfade needs both pictures on screen at the same time, so the credits
split the 256-colour background tile memory and the background palette into
two halves. `BG3` draws from tiles in the first half with colours 0 to 127, and `BG2`
from tiles in the second with colours 128 to 255. A backdrop has 128 colours,
and `Graphics_LoadCharacterBlockAndPalette` adds 128 to every pixel it unpacks
into the second half, so the same picture works in either.

`DisplayScroll_RunSlideshow` loads each backdrop into the half that isn't
showing, then steps the blend weights between `BG2` and `BG3` over 16 steps of 4
frames, and holds. The halves swap roles from one backdrop to the next, so a
backdrop is never loaded while it's being shown. The colours go to palette RAM
through the I/O write queue at the next VBlank, and the tiles are unpacked by
`BattleEffect_Expand`, copied into heap slot 49 for each load and released
again. Every fourth frame both backdrops drift one pixel to the right; a
picture is as wide as its map, 256 pixels, so it wraps around without a seam.

### The scrolling names

The names scroll as sprites rather than as a background, six 32-by-8 sprites
across each 192-pixel line. They move up a pixel every fourth frame, so a new
line arrives every 32 frames, and the sprites draw from a ring of 32 rows of
tiles of which 16 are visible. Each time the scroll crosses into a new line,
`DisplayScroll_RenderEnteringLine` draws the line 16 rows ahead of the top
into the hidden part of the ring. Only lines about to show are ever drawn, and
the tile memory the credits need doesn't grow with their length. Solid sprites
over the top 16 and bottom 24 pixels of the screen hide the rows that are only
partly on.

`DisplayScroll_DrawLine` draws each glyph in colour 15 with a colour 1 shadow
a pixel down and to the right. The original has a small bug here: it takes its
line buffer before checking for the end of the text, so the last call leaks
the buffer. The fix is behind `BUGFIX`.

### Running outside the engine's usual frame

The credits switch most of the engine's usual frame off. They reset the task
table, stop the VBlank's copy of the sprite list to OAM, the hardware's list
of sprites on screen (`gOamCopyEnabled`), and send their own 128 sprites from
a 1 KB buffer that is never freed. They also turn off the CPU load display,
the debug text and the idle sleep that would otherwise come after three idle
minutes. When they finish, `BG2`, `BG3` and blending are off, the window graphics
are reloaded and the OAM copy is back on; the three tasks they scheduled stay
scheduled.

Their state lives in four cells borrowed from the flash library, the code
that saves the game. Nothing is saved while the credits roll, so the cells sit
idle: `gFlashNumRemainingBytes` holds the sprite buffer's
pointer, `gFlash_Handler3` the scroll position, `gFlash` the position of the
last line drawn and `gFlash_Handler0` the end-of-text latch. Borrowing them is
safe because the flash library sets each one before it reads it:
`IdentifyFlash`, which runs before each use of the save, writes `gFlash`,
`gFlash_Handler0` and `gFlash_Handler3`, and each program routine sets
`gFlashNumRemainingBytes`. The original reaches them through casts of the
flash library's declarations, which is undefined behaviour; `UBFIX` gives the
cells the credits' own types.

## Limits

- A credits backdrop has at most 128 colours and 480 tiles, 256 by 120
  pixels: half of the background palette, and 30 KB of its half's 32 KB of
  tile memory, the backdrop's map taking the rest.
- A credits line is 192 pixels wide, six 32-pixel sprites, and is drawn
  centred.
- The palette fade covers all 512 background and sprite colours.

## Evidence and open questions

The pictures are named for what they show, from the scenes and effects that
load them, and the decoders for the format each one unpacks, proven in their
headers. The credits' and the title's functions are named for what they draw.
The `// BUG:` and `// UB:` notes in the credits code, and the `BUGFIX` and
`UBFIX` switches behind their fixes, work as they do everywhere in the source,
described in [Matching](../MATCHING.md).

Nobody knows yet why the credits borrow flash-library cells for their state
instead of having globals of their own.
