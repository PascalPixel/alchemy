# Windows

When someone in a town speaks, a framed box zooms open at the top of the
screen and their words appear in it a few letters at a time, with a little
voice blip as they go. Every box of text in the game works this way: the
messages people say, the menus, the shop counters, the battle commands.
The window code draws those boxes, unpacks the game's stored messages, types
them out over several [frames](../../glossary.md#frame) and moves the small pictures a box carries:
letters, icons, portraits and the page arrow.

It also builds the screens that are made directly from windows: the rows of
command icons, the option and pause menus, the [party](../../glossary.md#the-party) status window, the
file menu, the naming screen, the summary screen, the battle menus and the
debug menus. [Menus, windows and shops](../../menus.md#windows) gives the
whole layer in brief. The field menu's four screens are in
[Menus](../menu/README.md), and the shop counters in
[Shops](../shop/README.md).

## The work block

Everything the window system keeps between frames lives in one block of
memory, set up with each [map](../../glossary.md#map) and gone at the next.

The block holds the picture of the screen that every window is drawn into,
a slot for each window that can be open, three text printers (one per
message being typed), the pool of small [sprites](../../glossary.md#sprite-and-oam) the windows carry, the text
buffer of unpacked messages and the blanks a message fills in. Because all of it is in
one place, the code that reads it is the same in every [edition](../../glossary.md#editions) even though
the Japanese edition has more window slots and a shorter ring.

In the code the block is `struct WindowWork`, borrowed from the [heap](../../glossary.md#heap-and-pools) under
the [heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_WINDOW` by `WindowSystem_InitField`
for the field and by `WindowSystem_InitBattle` for battle; it lasts until the next
map change and is reached as `WINDOW_WORK`. In order it holds:

- `tilemap`: the windows' tilemap, 32 by 20 tiles, uploaded to video memory
  during [VBlank](../../glossary.md#frame) in 4-row bands marked in `dirty`;
- `windows`: the window slots, `WINDOW_COUNT` of them;
- `printers`: the three text printers;
- `sprites` and `freeSprites`: the pool of `WINDOW_SPRITE_COUNT` (64) window
  sprites, the sprites windows carry, and the list of those no window holds;
- `textTileState`, `nextTextTile` and `extraTextTiles`: the text tiles'
  bookkeeping
  ([Drawing text](#drawing-text));
- `mindRead`, `battleMode` and the current printer's style, copied in to draw;
- `textBuffer`: the text buffer, a ring of decoded text;
- `argumentValues` and `argumentKinds`: the eight message arguments and their
  `MESSAGE_ARGUMENT_` kinds;
- `portraitPictures` and `portraitSprites`: the two portrait slots;
- `voiceSound` and the flags for halting speech.

Its size differs by edition. The Japanese edition has 12 window slots and a
ring of 256 codes; the others have 8 slots and 512 codes. A window slot is 36
bytes and a code 2, so dropping four slots saves 144 bytes while the longer
ring costs 512, and the block grows from `WINDOW_WORK_SIZE` 4,492 bytes to
4,860. `include/tbs_edition.h` gives every field's offset in each layout, and
`STATIC_ASSERT`s in `include/window.h` check the struct against them.

Despite its name, `battleMode` says where the windows are, not what a menu
is doing:

| `battleMode` | Set by | Meaning |
| --- | --- | --- |
| 0 | the field setup, which clears the block | on the field |
| 1 | `WindowSystem_InitBattle` | in battle: typing is faster, and cleared rows near the bottom keep the battle's tile |
| 2 | `BattleMessage_Open`, while it decodes | a battle message is being decoded |

`mindRead` is set while Mind Read, the Psynergy that shows what a person is
thinking, shows someone's thoughts ([Drawing text](#drawing-text)).

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_WINDOW` | 15 | the heap slot that holds the work block |
| `WINDOW_WORK_SIZE` | `0x118C` (4,492) in Japanese, `0x12FC` (4,860) in the others | the work block's size in bytes |
| `WINDOW_COUNT` | 12 in Japanese, 8 in the others | window slots |
| `WINDOW_SPRITE_COUNT` | 64 | window sprites in the pool |
| `TEXT_BUFFER_MASK` | `0xFF` in Japanese, `0x1FF` in the others | the text buffer's index mask: 256 or 512 codes |
| `MESSAGE_ARGUMENT_COUNT` | 8 | message arguments at a time |

## Windows

A window is a framed rectangle drawn into the shared picture of the screen.
It zooms open from its centre, stays while it is needed, and zooms shut.

Opening a window takes the first free slot. Unless it is asked to appear at
once, the window then grows out from its centre over 8 frames, a bigger
frame drawn each frame, and the code that opened it waits until it is full
size before putting anything in it. Closing works the other way, over 4
frames, and the slot stays taken until the window has gone, so a closing
window still counts against the 8 slots.

The frame and the blue filling are tiles of the windows' tilemap, which the
console shows on the one [background layer](../../glossary.md#background-layer) in front of the map's three. A
message's letters, its portrait and its page arrow are sprites riding on the
window, in front of everything. In a message in Vale, the map's grass and
cliffs are on `BG3`, its trees and bushes on `BG2` and a roof and a tree top
on `BG1`, with the people as sprites level with `BG2`; the window's frame and
filling are on `BG0`, the one layer every window shares.

A message window sizes itself to its text: it is measured first, then
placed so that it fits on the screen.

In the code, `Window_Create(x, y, width, height, flags)` takes the first
free slot, or returns `NULL` when none is free, and places the window at tile
(x, y), `width` by `height` tiles. The tilemap is 32 tiles wide because the
console's background layers are, but the screen's 240 pixels show only 30 of
them, so windows are placed on a screen of 30 by 20 tiles. Offsets inside a
window are pixels from its interior's top left. Unless the window has
`WINDOW_FLAG_INSTANT`, `Window_UpdateZooms` zooms it open over
`WINDOW_OPEN_FRAMES` (8), drawing its frame at (8 − frame) / 8 of its size
about its centre, and closed over `WINDOW_CLOSE_FRAMES` (4); the slot is
freed only once the close has finished. `Window_Close(window, instant)`
starts closing that one window and returns without waiting.

`Window_DrawFrame` writes the frame into the tilemap and draws nothing for a
width or height below 2 or above 30. Nothing checks that a window stays within
the tilemap's 20 rows; the caller keeps it there. While `mindRead` is set every
corner is the one round tile (`WINDOW_TILE_FRAME_ROUND_CORNER`), flipped to fit.

`Window_FitOnScreen` (`window/measure.c`) measures a message and moves its
window so that it fits on the screen. While `mindRead` is set the fitted window
is 2 tiles wider; which of the measuring routines add those tiles differs by
edition, and in Japanese and English the text also starts 8 pixels further
in.

Where the editions lay a screen out differently, the value is a named macro
chosen in one `#if` block at the top of the private header or of the part.
The battle [summon](../../glossary.md#summons) list, for one, is `SUMMON_MENU_WINDOW_WIDTH` 17 tiles wide
at `SUMMON_MENU_WINDOW_X` 13 in the international editions, and 14 tiles wide at 16 in
Japanese, so both end at the screen's right edge.

The window flags (`struct Window.flags`) are in
[`window.h`](../../include/unit/window/window.h), the zoom's lengths in
`window_internal.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `WINDOW_FLAG_OPEN` | `0x1` | the window slot is in use |
| `WINDOW_FLAG_INSTANT` | `0x2` | opens and closes at once, with no zoom |
| `WINDOW_FLAG_TEXT_CANVAS` | `0x8` | text is drawn into the text canvas ([Drawing text](#drawing-text)) |
| `WINDOW_FLAG_FRAMED_CANVAS` | `0x20` | the canvas has a frame and a filled background |
| `WINDOW_FLAG_OUTLINED_TEXT` | `0x40` | the canvas text gets an outline |
| `WINDOW_FLAG_ANCHOR_BOTTOM` | `0x80` | when a new page resizes the window, its bottom edge stays put |
| `WINDOW_FLAG_FIXED_X` | `0x100` | when a new page resizes the window, its left edge stays put |
| `WINDOW_OPEN_FRAMES` | 8 | frames a window takes to zoom open |
| `WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to zoom closed |
| `SUMMON_MENU_WINDOW_X`, `SUMMON_MENU_WINDOW_WIDTH` | 13 and 17 tiles; 16 and 14 in Japanese | the battle summon list's place and width |

## Decoding a message

The cartridge stores the game's text squeezed, so before a message can be
typed it is unpacked, whole, into plain letter codes.

Each stored letter is a short run of bits, and which run stands for which
letter depends on the letter before it, so the pairs that are common in the
game's text cost very little room. A message can also hold blanks: a party
member's name, a number, the name of an item or a [Psynergy](../../glossary.md#psynergy-and-pp), each filled in
as the message is unpacked, with "a" or "an" and the other small words each
language puts in front of a name. The marks that say where a line or a page
ends, where to pause and which colour to use are kept as they are, for the
text printer to act on later. All the slow work happens here, once, so typing the
message afterwards costs almost nothing a frame.

The game's text is stored as numbered messages. Their text is in the PO
catalogs under `games/tbs/text`, one per language, which the build's
`ags-text` turns into the packed messages and a header naming each one; the
code writes a message by that linker symbol (`MsgGameSaved`). `Message_Decode`
([`window/decode.c`](window/decode.c)) decodes a whole message at once into
the text buffer.

Messages are Huffman-coded: common symbols get short codes and rare ones
long codes, read one bit at a time down a tree of choices. There is a
separate tree for each previous symbol (`Text_MessageContexts`), so the code
just decoded chooses the tree for the next. The routine that walks the
trees, `Text_DecodeSymbol`, is handwritten in the processor's 32-bit ARM
instructions in `graphics/text/decode_symbol.s`; the decoder copies it into heap slot
`TEXT_WORK_BLOCK` (50), which the heap takes from IWRAM when it has room,
runs it from there one symbol at a time and releases the slot at the end.

An argument code takes its value from the argument table, which callers fill
with `MessageArgument_Push(value, kind)`: a name, a number, or an item or
ability name, by its `MESSAGE_ARGUMENT_` kind. Each language's articles,
possessives and endings come from `articles.c`, chosen by the article class
an item or name carries. Style codes (colours, pauses, page breaks) are
copied into the text buffer unchanged, with their operands, for the text printer to run. A control code in the catalogs
is written in braces: `{end_wait}`, at the end of
`Your game was saved.{end_wait}`, is `MESSAGE_END_WAIT`, kept as
`TEXT_WAIT_KEY`, which waits for a button and closes the window. Garet's
sister's line in Vale is `MsgValeYouGuysAreSoLuckyGaret`, whose name blank
is `{actor_name:2}`.

While a battle message is decoded `battleMode` is 2, and the Japanese decoder
then sets names off with spaces and style codes and doubles each space.

The stored control codes are an enum in `window/decode.c`; an operand is
stored one above its value.

| Name | Value | Kept as |
| --- | --- | --- |
| `MESSAGE_END` | 0 | `TEXT_END`: the window stays open |
| `MESSAGE_PAGE_BREAK` | 1 | `TEXT_PAGE` |
| `MESSAGE_END_WAIT` | 2 | `TEXT_WAIT_KEY`: a button closes the window |
| `MESSAGE_LINE_BREAK` | 3 | `TEXT_NEWLINE` |
| `MESSAGE_TEXT_COLOR` | 8 | `TEXT_COLOR`, with its operand |
| `MESSAGE_TEXT_EFFECT` | 9 | `TEXT_OUTLINE`, with its operand: the style of sprite text |
| `MESSAGE_LINE_SPACING` | 10 | `TEXT_LINE_SPACING`, with its operand |
| `MESSAGE_LOCALIZED_WORD` | 13 | German: a counted noun; Italian: a gender ending; Spanish and French: the plural suffix |
| `MESSAGE_WINDOW_EFFECT` | 15 | `TEXT_WINDOW_STATE`, with its operand: the window effect to run |
| `MESSAGE_PROTAGONIST_NAME` | 16 | the letters of the leader's name (`gSaveBlock.leaderLocalId`) |

## The text buffer

Unpacked messages wait in a ring, the text buffer, until a text printer has
typed them.

Each message is written after the one before it, ends with an end mark, and
is known only by where it starts. When the writing reaches the end of the
ring it carries on from the beginning, over whatever was there, so several
windows can show text at once without anyone setting room aside, and a
message stays readable until enough later text has been unpacked to run over
it.

`work->textBuffer` is a ring of 16-bit codes whose index wraps through
`TEXT_BUFFER_MASK`, the ring's size less one. A message is written at `work->textBufferEnd`, ends with 0, and
is known only by the index it starts at, which `Message_Decode`
returns; asking for message -1 returns the last message's start again. A
message longer than the ring is cut at the ring's length; none in the game is
([Evidence and open questions](#evidence-and-open-questions)).

## Typing

A text printer types an unpacked message into its window a
little each frame and acts on the marks it meets: a new line, a new page, a
pause, a colour.

![Isaac talks to Garet's sister in Vale: a message window zooms open at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

Each frame the text printer puts on one more letter cell. A cell is a sprite 16 pixels wide, and in the
international editions two narrow letters that fit in one cell together are typed
as one, so "Yo" appears on the first frame. How many cells come a frame is
the player's message speed: at the slow speed one, followed by a pause of 4
frames that holding a button skips; at the normal speed one; at the fast
speed up to ten, though a space or a mark ends the frame's share, so the
text comes about a word at a time. Battle text is typed faster still, 8
cells a frame, and 13 after a frame that ran late, so a battle keeps moving.
Every few cells the speaker's voice blip plays.

At a page break the text printer shows the arrow and waits for a button; then the
window is cleared, refitted to the next page's text, and typing goes on. At
the end the window either stays open or, with the closing mark, waits for a
button and closes.

In the code a text printer (`struct TextPrinter`) holds its window, the pen
position in 1/256 pixels, the index of its next code, the countdowns for
pauses and for the wait after each character, and its style.
`TextPrinter_Add` takes a free text printer and returns `NULL` when all
three are busy; the printer stays taken until its window closes.
`BattleMessage_Continue` also reuses a text printer whose window has finished,
continuing below its last line and scrolling once both lines are used.

`WindowSystem_Update` is the window system's [task](../../glossary.md#task). Both setups add it in
the main loop's pass, and it runs every frame: the windows' zoom
(`Window_UpdateZooms`), then the text printers, then the window sprites
(`WindowSprite_UpdateAll`). Code that shows a message waits on its window
while the task types it. `Message_OpenWindow` (`window/frames.c`) is the
whole path for one message, and a caller then waits a frame at a time
(simplified, with the shop's wait from `Shop_ShowKeeperMessage`):

```c
entry = Message_Decode(message, 1);   // decode into the ring
Window_FitOnScreen(entry, &x, &y, &width, &height, bounds, 0);
window = Window_Create(x, y, width, height, flags);  // NULL: no free slot
TextPrinter_Add(window, entry, 0, 0, bounds, 0); // take a text printer

while (TextPrinter_AllFinishedFar() == 0)   // every printer has typed its text
  WaitFrames(1);
```

`TextPrinter_Step` types one frame's share of a text printer. Outside
battle the share is `gMessageSpeed_Steps` for the player's message speed, and
a space or a control code ends it; after each character the printer waits
`gMessageSpeed_Delays` frames while no button is held. Two characters whose
glyph widths add up to 15 pixels or less are drawn as one pair, the second in
the code's high byte (14 in Spanish with an outline), except in a text canvas
and in Japanese. In battle (`battleMode` nonzero) the share grows with how
many VBlanks the last frame took (`gLagFramesShown`, 1 for a frame on time):

```
share = 3 + 5 × min(VBlanks, 2)    → 8 on time, 13 after a late frame
``` The speaker's voice blip plays every
`gMessageSpeed_VoiceGaps` characters, and is one of four sounds from
`voiceSound`, chosen by the character's code.

The speed tables are in `message_speed.c`, indexed by
`gSaveBlock.messageSpeed`, 0 to 2 from slowest to fastest; the codes a
text printer runs are in `window_internal.h`, and codes from `TEXT_SPACE` up are
characters.

| Name | Value | Meaning |
| --- | --- | --- |
| `gMessageSpeed_Steps` | 1, 1, 10 | characters typed each frame |
| `gMessageSpeed_Delays` | 4, 0, 0 | frames waited after each character while no button is held |
| `gMessageSpeed_VoiceGaps` | 1, 3, 10 | characters between two voice blips |
| `TEXT_END` | 0 | the text ends; the window stays open |
| `TEXT_PAGE` | 1 | wait for a button, then clear the window for a new page |
| `TEXT_WAIT_KEY` | 2 | wait for a button, then close the window |
| `TEXT_NEWLINE` | 3 | a new line |
| `TEXT_PAUSE_LONG` | 4 | a 60-frame pause |
| `TEXT_PAUSE_SHORT` | 5 | a 20-frame pause a button can cut short |
| `TEXT_PAUSE_LONGEST` | 6 | a 120-frame pause a button can cut short |
| `TEXT_COLORS_RESET` | 7 | colour 15, style 0 and line spacing 10 again |
| `TEXT_COLOR`, `TEXT_OUTLINE`, `TEXT_LINE_SPACING` | 8, 9, 10 | the next entry is the colour, the style or the line spacing |
| `TEXT_WINDOW_STATE` | 15 | the next entry is the window effect to run |
| `TEXT_END_ALT` | 30 | ends the text as `TEXT_END` does |
| `TEXT_SPACE` | 32 | the first character code |

## Drawing text

A window's frame is always tiles of the windows' tilemap, but its letters go
on in one of three ways, depending on what the window is for.

A message typed on the field puts each cell on as a sprite riding on the
window, which is what lets letters bob or shake. A battle message is painted
into one shared picture, the text canvas, across the bottom rows of the
screen; there is one canvas, so only one such window shows at a time. Labels
drawn all at once, such as menu text and the area's name, become tiles of the
tilemap itself, from a set of tiles kept for text.

![Isaac casts Mind Read on Garet's sister in Vale: the field tints red, then gold and green, as her thought `Gee, I wish I could see new lands, too.` is typed into a round-cornered window beside her portrait.](https://alchemy.pascalpixel.com/abilities/mind-read.gif)

While `mindRead` is set, as under Mind Read, the window has round corners, the
text is drawn in colour 8 with no shadow, and a button press latched before
a page's wait is dropped.

The canvas (`WINDOW_FLAG_TEXT_CANVAS`) draws into bitmap tiles at
`TEXT_CANVAS_VRAM`, through `glyph_arm.c`. It is 120 tiles from
`TEXT_CANVAS_FIRST_TILE`, enough for the interior of a full-width window six
tiles tall, and its tiles run down columns; `BattleMessage_Open` opens
it as a frameless window over the bottom rows for battle messages. Every canvas window maps the
same tiles. Canvas lines are 13 pixels apart, and from the second line on the
canvas scrolls up instead.

Text drawn as tiles (`Text_DrawMessage`, `Text_DrawMessageAsTiles`)
takes tiles `0x80` to `0xFF`, and `0x200` to `0x27F` as well while
`extraTextTiles` is set. `Text_UseExtraTiles` sets it while the field menu's Djinn, Item,
Psynergy and summary screens and the battle's Djinn menu are open, which need
more text tiles, and clears it as they close. Its lines are 15 pixels apart. A tile's attribute has bit 0 once
allocated and bit 1 while the tilemap shows it, so `Text_DrawStringAsTiles`
can reuse the tiles a string already has and take only free ones.

| Way | Chosen by | Where it draws | Line spacing |
| --- | --- | --- | --- |
| text canvas | `WINDOW_FLAG_TEXT_CANVAS` | 120 tiles from `TEXT_CANVAS_FIRST_TILE` (`0x128`), at `TEXT_CANVAS_VRAM` (`0x06002500`) | 13 pixels |
| sprites | a typed message without the canvas flag | one 16 by 16 window sprite a cell: a character, or two narrow ones in the international editions | 15 pixels |
| tilemap tiles | text drawn all at once | tiles `0x80` to `0xFF`, and `0x200` to `0x27F` with `extraTextTiles` set | 15 pixels |

## Window sprites

A window sprite is one sprite a window carries: a cell of text, an icon, a
portrait or the page arrow. It moves with its window and goes when the
window closes.

Each window keeps a list of its sprites, taken from the pool's free list and
returned when the window closes. `WindowSprite_UpdateAll` moves each
sprite every frame by its `WINDOW_SPRITE_MOTION_` motion and queues it for drawing. A
window sprite's picture occupies an entry of the VRAM cache, loaded into a free one
(`VramCache_FindFreeEntry`, `VramCache_Load`) and freed by
`VramCache_FreeEntry`. The two portrait slots record which picture each shows
(`portraitPictures`, 999 when free) and its cache entry. In Japanese, a character
that finds no free window sprite closes its window.

## The area name

Arriving in a new area, the player sees its name in a small window near the
top of the screen for a moment.

`MapNamePopup_Show` shows the area's name, from
`MapSec_GetNameIndexFar`, drawn as tiles in a window centred across the
screen and in its top 10 rows, opened with `WINDOW_FLAG_INSTANT`. It keeps the
window and a countdown of `MAP_NAME_POPUP_FRAMES` (90) in two fields of the event work
(`gFieldWork`, the running map's state), and
`MapNamePopup_Update`, a main-loop task, closes it when the countdown ends.

| Name | Value | Meaning |
| --- | --- | --- |
| `MAP_NAME_POPUP_FRAMES` | 90 | frames the area's name shows |

## Design

[Menus, windows and shops](../../menus.md) explains why messages are decoded
ahead of typing and why every window shares one tilemap. Within the window
code, the two routines that run per pixel or per bit, the canvas renderer and
the tree walker, are ARM code (the processor's full 32-bit instructions)
copied into RAM, while everything else is Thumb code (its compact 16-bit
form) run from ROM; why is an open question (below). The work block's two
layouts are one struct whose array sizes come from `tbs_edition.h`, so the
code that reads it is the same in every edition.

## Limits

- The screen is 30 by 20 tiles; a frame narrower or shorter than 2 tiles, or
  wider or taller than 30, is not drawn.
- Twelve window slots in Japanese and eight in the international editions.
- Three text printers and 64 window sprites.
- Eight message arguments at a time (`MESSAGE_ARGUMENT_COUNT`); a ninth is
  dropped (`MessageArgument_Push`).
- The text buffer holds 256 codes in Japanese and 512 in the others.
- The text canvas holds 120 tiles, one canvas window at a time.
- Text drawn as tiles has 128 tiles, or 256 with `extraTextTiles` set.

## Files

| File | What it holds |
| --- | --- |
| `window.c` | the window object, written as parts in `window/`: windows, text printers, the decoder, window sprites, icons, the selection row and the option and pause menus |
| `screens.c` | the screens built on the window object, written as parts in `screens/`, from the party status window to the debug menus |
| `glyph_arm.c` | the text canvas renderer, compiled as ARM code and copied to RAM to run |
| `articles.c` | each edition's articles and possessives, the kana marks and the sprite motion curves |
| `scales.c` | sprite scales, motion curves and the lookup tables of the window and menu code |
| `message_speed.c` | the three message speed tables |
| `windata.s` | glyph widths, kana mark ranges, the party and [Djinni](../../glossary.md#djinn) sprites, the growing bar and the cursor stops |
| [`keyboard.s`](keyboard.s) | the naming screen's keyboard tilemaps |
| [`cursor.s`](cursor.s), `cursor_frame.s`, `fixed_block.s`, `slider.s`, `slidpal.s` | sprite tiles and palettes, built from the PNGs beside them or under `../menu` and `../graphics` |

Both objects are written as parts: `window.c` and `screens.c` include
their private headers (`window_internal.h`, `screens_internal.h`) and
then their parts in ROM order, and their opening comments say where to start.
`head -3 window/*.c screens/*.c` lists what each part holds. Link order
is in `games/tbs/objects.tsv`, from `glyph_arm`.

The prototypes are in `games/tbs/include/unit/window/`; its
[`window.h`](../../include/unit/window/window.h) also holds what both objects
use: the tilemap's tiles (`WINDOW_TILE_`), the window flags (`WINDOW_FLAG_`), sprite
motions (`WINDOW_SPRITE_MOTION_`), message argument kinds (`MESSAGE_ARGUMENT_`) and the text
canvas's place in VRAM. The work block's layout, `struct WindowWork`, is in
[`include/window.h`](../../include/window.h), and its offsets for each edition
in `include/tbs_edition.h`.

## Words used here

- **work block**: `struct WindowWork`, heap slot `HEAP_SLOT_WINDOW`, reached
  as `WINDOW_WORK`. It holds all of the window system's state.
- **window**: a `struct Window` slot, one framed rectangle of the shared
  tilemap.
- **text canvas**: a window with `WINDOW_FLAG_TEXT_CANVAS`, whose text is drawn
  into bitmap tiles rather than placed as sprites or tilemap tiles.
- **decoded text**: a message after decoding, as 16-bit codes in the text
  buffer, the ring `work->textBuffer`.
- **text printer**: a `struct TextPrinter`, which types decoded text into one
  window and runs its control codes (`TEXT_`).
- **window sprite**: a `struct WindowSprite`, one sprite a window carries.
- **text tile state**: `work->textTileState`, one byte per text tile saying
  whether it is allocated and whether the tilemap still shows it.
- `unk` plus an offset, such as `unk10`: a field whose meaning is not known.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `WindowSystem_` | setting up the window system for the field or for battle, and its per-frame updates |
| `Window_`, `Win_` | a window's rectangle of the tilemap: frames, fills, dividers, tiles |
| `Text_`, `Message_`, `MessageArgument_`, `TextPrinter_` | messages and text: decoding, measuring, drawing and typing |
| `WindowSprite_` | the window sprite pool and a window's sprite list |
| `MenuArrow_` | the menus' arrow sprites |
| `Icon_`, `ItemIcon_`, `CommandIcon_`, `Portrait_` | icons and portraits built into sprite tiles |
| `SelectionScreen_`, `Menu_` | the selection row, and the menus built from command icons |
| `OptionMenu_`, `PauseMenu_`, `WindowColor_` | the option and pause menus and the window colours |
| `Save_`, `FileMenu_` | save files and the file menu |
| `NamingScreen_`, `PartyStatus_`, `ColossoMenu_`, `BattleDjinnMenu_`, `BattleSummonMenu_`, `BattleItemMenu_`, `BattlePsynergyMenu_`, `BattleTarget_` | the screens they name |
| `Party_` | the joined-party messages and the party's Psynergy list |
| `MapNamePopup_` | the area name shown near the top of the screen on arrival |
| `DebugMapSelect_`, `DebugFlagEditor_`, `DebugBrowser_`, `Debug_` | the debug menus |
| `IconMenu_` | the icon menus' pulsing icon and their position table |
| `Link_` | the link countdown window and its page arrows |
| `Color_` | scaling a colour's channels |
| `Resource_`, `Graphics_`, `Vram_`, `VramCache_` | things the engine names elsewhere: VRAM cache entries, pictures, palettes and video registers |
| `FieldMessage_` | showing a message window in the field: at an object, centred, or at a window position |
| `Glyph_` | drawing a font glyph with its outline into a window |
| `BattleCommandRow_` | the row of battle commands a member chooses from |
| `BattleSummaryScreen_` | the battle's status screen of a unit, stepping through the members with L and R |
| `YesNoMenu_` | the yes-or-no menu |
| `Heading_` | the 64 by 16 headings of the menus (Save, Continue, Copy, Erase, Battle, Link, Pause, Setup), loaded and bobbing |
| `RenderResource_` | loading one sprite tile of the heading pictures into the VRAM cache |

Many of these names are
shared with [The Lost Age](../../glossary.md#the-two-games)'s copies of these files and with the [far-call](../../glossary.md#far-call) table
`system/far_call/WINDOW.S`, so renaming one changes other folders too.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## Evidence and open questions

Windows, text printers and window sprites are named from how their structs
are used, the control codes from what the decoder and the printer do with each,
and the screens from the English release's text they show. Messages are
named by their linker symbols, made from their English text. That "Yo" is
typed on the first frame follows from the English glyph widths
(`graphics/font/text.tsv`) and the pairing rule in `TextPrinter_Step`, and
agrees with a recording of the window opening
(`guides/window/window-open.png`), whose first typed frame shows it.

[XXX](../../glossary.md#xxx): likely the canvas renderer and the tree walker are ARM code run from
RAM for speed, because ARM code fetched from the cartridge's 16-bit bus is
slow; nothing in the code says so. XXX: the likely reason the text buffer was
doubled in the international editions is their longer text; the bytes show only the
change.

A message longer than the ring would be cut, its terminator landing on its own
start, but no message is that long. Counting each control code and line break
as one code, before the arguments are filled in, the longest message of each
edition's text is the Lucky Dice rules, at 200 of the Japanese ring's 256
codes and at most 446 (Italian) of the others' 512. The two bytes before the
argument table, `padding` in `struct WindowWork`, align `argumentValues` to a word
in every edition, and nothing reads them.

- XXX (no evidence): why there are three text printers, and why the international
  editions gave up four windows when they doubled the ring; the code shows
  only the counts each edition uses, and only Camelot's sources could say how
  they were chosen.
- XXX: what `WindowWork.unknownAfterLineSpacing` holds; `Text_ResetStyle`
  sets it to 1 beside the text style and no C reads it, so a read in the
  glyph assembly or a text control code that changes it would settle it.
