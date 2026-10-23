# Windows

![Garet's sister's line in Vale, "You guys are so lucky, Garet.", typed into its window at the normal message speed. Each frame the window gets one more letter cell: a single letter, or two narrow letters that fit together in one cell 16 pixels wide. On the first frame "Yo" appears, on the second "You", on the fourth "You gu", and on the 18th the whole first page; a space and the line break take a frame each. Then an arrow shows and the window waits for a button before the next page. The player's message speed decides how fast the cells come: at the slow speed one cell and then a pause of 4 frames, which is skipped while a button is held; at the normal speed one cell a frame; at the fast speed up to ten a frame, but a space ends the frame's share, so about a word a frame.](https://alchemy.pascalpixel.com/illustrations/window/typing.svg)

When someone in a town speaks, a framed box zooms open at the top of the
screen and their words appear in it a few letters at a time, with a little
voice blip as they go. Every box of text in the game works this way: the
messages people say, the menus, the shop counters, the battle commands.
The window code draws those boxes, unpacks the game's stored messages, types
them out over several frames and moves the small pictures a box carries:
letters, icons, portraits and the page arrow.

It also builds the screens that are made directly from windows: the rows of
command icons, the option and system menus, the party status window, the
save file menu, name entry, the status screen, the battle menus and the
debug menus. [Menus, windows and shops](../../menus.md#windows) gives the
whole layer in brief. The field menu's four screens are in
[Menus](../menu/README.md), and the shop counters in
[Shops](../shop/README.md).

## The work block

Everything the window system keeps between frames lives in one block of
memory, set up with each map and gone at the next.

![Everything the window system keeps is in one block of memory, set up with each map. In it: the windows' tilemap, a picture of the screen 32 tiles wide and 20 tall that every window is drawn into; 8 window slots, 12 in the Japanese edition; 3 typists, each typing one message into one window; 64 sprites that windows carry, such as letters, icons, portraits and the page arrow; the ring of unpacked messages, 512 codes, 256 in Japanese; 8 blanks for names and numbers a message will fill in; and 2 portrait slots.](https://alchemy.pascalpixel.com/illustrations/window/work-block.svg)

The block holds the picture of the screen that every window is drawn into,
a slot for each window that can be open, three typists (one per message
being typed), the pool of small sprites the windows carry, the ring of
unpacked messages and the blanks a message fills in. Because all of it is in
one place, the code that reads it is the same in every edition even though
the Japanese edition has more window slots and a shorter ring.

In the code the block is `struct UiRenderWork`, borrowed from the heap (the
memory the engine lends out to whatever runs) under the numbered slot
`HEAP_SLOT_WINDOW` by `UiWork_InitializeWithResourceCounters` for the field
and by `UiWork_Initialize` for battle; it lasts until the next map change
and is reached as `UI_WORK`. In order it holds:

- `tilemap`: the windows' tilemap, 32 by 20 tiles, uploaded in 4-row bands
  marked in `dirty`;
- `windows`: the window slots, `WINDOW_COUNT` of them;
- `channels`: the three message channels (the typists);
- `outputs` and `freeOutputs`: the pool of `UI_OUTPUT_COUNT` (64) render
  outputs and the list of those no window holds;
- `tileAttributes`, `tileCursor` and `alt`: the text tiles' bookkeeping;
- `mode`, `menuState` and the current channel's style, copied in to draw;
- `entries`: the decoded-text ring;
- `values` and `names`: the eight message arguments and their
  `MESSAGE_ARGUMENT_` kinds;
- `resourceIds` and `outputIds`: the two portrait slots;
- `speakerVoiceSound` and the flags for halting speech.

Its size differs by edition. The Japanese edition has 12 window slots and a
ring of 256 codes; the others have 8 slots and 512 codes. A window slot is 36
bytes and a code 2, so dropping four slots saves 144 bytes while the longer
ring costs 512, and the block grows from `RENDER_WORK_SIZE` 4,492 bytes to
4,860. `include/tbs_edition.h` gives every field's offset in each layout, and
`STATIC_ASSERT`s in `include/window.h` check the struct against them.

`menuState` is 1 after `UiWork_Initialize` and 0 after the field setup, so
code that must behave differently in battle tests it; while a battle message
is decoded it is 2. `mode` is set while Mind Read shows someone's thoughts.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_WINDOW` | 15 | the heap slot that holds the work block |
| `RENDER_WORK_SIZE` | `0x118C` (4,492) in Japanese, `0x12FC` (4,860) in the others | the work block's size in bytes |
| `WINDOW_COUNT` | 12 in Japanese, 8 in the others | window slots |
| `UI_OUTPUT_COUNT` | 64 | render outputs in the pool |
| `RENDER_ENTRY_MASK` | `0xFF` in Japanese, `0x1FF` in the others | the decoded-text ring's index mask: 256 or 512 codes |
| `UI_ARGUMENT_SLOT_COUNT` | 8 | message arguments at a time |

## Windows

A window is a framed rectangle drawn into the shared picture of the screen.
It zooms open from its centre, stays while it is needed, and zooms shut.

![A message window opening at the top of the screen in Vale, seen in five moments: first a thin bar, then a bigger box each frame, growing outwards from its centre, until it is full size; only then does the text begin, "Yo". A window takes 8 frames to open and 4 to close, shrinking back towards its centre, and it keeps its place among the window slots until it has closed. A window can also be asked to open and close at once, with no zoom.](https://alchemy.pascalpixel.com/illustrations/window/zoom.svg)

Opening a window takes the first free slot. Unless it is asked to appear at
once, the window then grows out from its centre over 8 frames, a bigger
frame drawn each frame, and the code that opened it waits until it is full
size before putting anything in it. Closing works the other way, over 4
frames, and the slot stays taken until the window has gone, so a closing
window still counts against the 8 slots.

The frame and the blue filling are tiles of the windows' tilemap, which the
console shows on the one background layer in front of the map's three. What
a window carries, its letters and its portrait, are sprites in front of
everything. Taken apart, a message in Vale shows the split:

![Garet's sister talking in Vale, pulled apart into its layers: the map's ground, middle and overhead layers at the back, the message window's frame on BG0, and the letters of `You guys are so lucky, Garet.`, her portrait and the actors as sprites in front](https://alchemy.pascalpixel.com/guides/window/layers-3d.gif)

![A message's five planes. Garet's sister talking in Vale, pulled apart into the five planes the console stacks to draw it, numbered from back to front: 1, the map's ground layer, BG3; 2, its middle layer, BG2; 3, its overhead layer, BG1; 4, the window layer, BG0, with the message window's frame; 5, the sprites, the message's letters, her portrait and the actors. The window's letters and portrait are sprites, in front of everything.](https://alchemy.pascalpixel.com/illustrations/window/talk-planes.svg)

A message window sizes itself to its text: it is measured first, then
placed so that it fits on the screen.

In the code, `UiWindow_Create(x, y, width, height, flags)` takes the first
free slot and places the window at tile (x, y), `width` by `height` tiles, on
the 30 by 20 tile screen; offsets inside a window are pixels from its
interior's top left. Unless the window has `UI_WINDOW_INSTANT`,
`UiWork_ProcessDirectWork` zooms it open over `UI_WINDOW_OPEN_FRAMES` (8),
drawing its frame at (8 − frame) / 8 of its size about its centre, and closed
over `UI_WINDOW_CLOSE_FRAMES` (4); the slot is freed only once the close has
finished. `UiWork_Finalize` starts the close and returns without waiting.

`UiWindow_DrawFrame` writes the frame into the tilemap and draws nothing for a
width or height below 2 or above 30. Nothing checks that a window stays within
the tilemap's 20 rows; the caller keeps it there. While `mode` is set every
corner is the one round tile (`UI_TILE_FRAME_ROUND_CORNER`), flipped to fit.

`UiWindow_FitOnScreen` (`window/measure.c`) measures a message and moves its
window so that it fits on the screen. While `mode` is set the fitted window
is 2 tiles wider; which of the measuring routines add those tiles differs by
edition, and in Japanese and English the text also starts 8 pixels further
in.

Where the editions lay a screen out differently, the value is a named macro
chosen in one `#if` block at the top of the private header or of the part.
The battle summon list, for one, is `SUMMON_MENU_WINDOW_WIDTH` 17 tiles wide
at `SUMMON_MENU_WINDOW_X` 13 outside Japan, and 14 tiles wide at 16 in
Japanese, so both end at the screen's right edge.

The window flags (`struct UiWindow.flags`) are in
[`window.h`](../../include/unit/window/window.h), the zoom's lengths in
`window_internal.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `UI_WINDOW_OPEN` | `0x1` | the window slot is in use |
| `UI_WINDOW_INSTANT` | `0x2` | opens and closes at once, with no zoom |
| `UI_WINDOW_TEXT_CANVAS` | `0x8` | text is drawn into the canvas of bitmap tiles |
| `UI_WINDOW_FRAMED_CANVAS` | `0x20` | the canvas has a frame and a filled background |
| `UI_WINDOW_OUTLINED_TEXT` | `0x40` | the canvas text gets an outline |
| `UI_WINDOW_ANCHOR_BOTTOM` | `0x80` | when a new page resizes the window, its bottom edge stays put |
| `UI_WINDOW_FIXED_X` | `0x100` | when a new page resizes the window, its left edge stays put |
| `UI_WINDOW_OPEN_FRAMES` | 8 | frames a window takes to zoom open |
| `UI_WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to zoom closed |
| `SUMMON_MENU_WINDOW_X`, `SUMMON_MENU_WINDOW_WIDTH` | 13 and 17 tiles; 16 and 14 in Japanese | the battle summon list's place and width |

## Decoding a message

The cartridge stores the game's text squeezed, so before a message can be
typed it is unpacked, whole, into plain letter codes.

![Garet's sister's message as the cartridge stores it and as it is ready to type. Stored, it is squeezed: each letter is a short run of bits, and the code for a letter depends on the letter before it, so common pairs are cheap. It also holds a blank for a party member's name and marks for a new line, a new page and the end. Before any of it is typed, the whole message is unpacked in one go: the letters come out, Garet's name is written into the blank, and the marks stay as they are. The window then only has to type what is there.](https://alchemy.pascalpixel.com/illustrations/window/decode.svg)

Each stored letter is a short run of bits, and which run stands for which
letter depends on the letter before it, so the pairs that are common in the
game's text cost very little room. A message can also hold blanks: a party
member's name, a number, the name of an item or a Psynergy, each filled in
as the message is unpacked, with "a" or "an" and the other small words each
language puts in front of a name. The marks that say where a line or a page
ends, where to pause and which colour to use are kept as they are, for the
typist to act on later. All the slow work happens here, once, so typing the
message afterwards costs almost nothing a frame.

The game's text is stored as numbered messages, written in the code by their
linker symbols (`MsgGameSaved`); their text is in the PO catalogs under
`games/tbs/text`, one per language. `UiText_BuildRenderEntries`
([`window/decode.c`](window/decode.c)) decodes a whole message at once into
the ring.

Messages are Huffman-coded: common symbols get short codes and rare ones
long codes, read one bit at a time down a tree of choices. There is a
separate tree for each previous symbol (`Text_MessageContexts`), so the code
just decoded chooses the tree for the next. The routine that walks the
trees, `Text_DecodeSymbol`, is handwritten in the processor's 32-bit ARM
instructions in `graphics/text/decode_symbol.s`; the decoder copies it into heap slot
`TEXT_WORK_BLOCK` for the call, runs it from there one symbol at a time and
releases the slot at the end.

An argument code takes its value from the argument table, which callers fill
with `UiWork_PushValueSlot(value, kind)`: a name, a number, or an item or
ability name, by its `MESSAGE_ARGUMENT_` kind. Each language's articles,
possessives and endings come from `articles.c`, chosen by the article class
an item or name carries. Style codes (colours, pauses, page breaks) are kept
with their operands for the channel to run. A control code in the catalogs
is written in braces: `{end_wait}`, at the end of
`Your game was saved.{end_wait}`, is `MESSAGE_END_WAIT`, kept as
`TEXT_WAIT_KEY`, which waits for a button and closes the window. The
message in the picture is `MsgValeYouGuysAreSoLuckyGaret`, whose name blank
is `{actor_name:2}`.

While a battle message is decoded `menuState` is 2, and the Japanese decoder
then sets names off with spaces and style codes and doubles each space.

The stored control codes are an enum in `window/decode.c`; an operand is
stored one above its value.

| Name | Value | Kept as |
| --- | --- | --- |
| `MESSAGE_END` | 0 | `TEXT_END`: the window stays open |
| `MESSAGE_PAGE_BREAK` | 1 | `TEXT_PAGE` |
| `MESSAGE_END_WAIT` | 2 | `TEXT_WAIT_KEY`: a button closes the window |
| `MESSAGE_LINE_BREAK` | 3 | `TEXT_NEWLINE` |
| `MESSAGE_TEXT_COLOR` | 8 | `TEXT_COLOUR`, with its operand |
| `MESSAGE_TEXT_EFFECT` | 9 | `TEXT_OUTLINE`, with its operand: the style of sprite text |
| `MESSAGE_LINE_SPACING` | 10 | `TEXT_LINE_SPACING`, with its operand |
| `MESSAGE_LOCALIZED_WORD` | 13 | German: a counted noun; Italian: a gender ending; Spanish and French: the plural suffix |
| `MESSAGE_WINDOW_EFFECT` | 15 | `TEXT_WINDOW_STATE`, with its operand: the window effect to run |
| `MESSAGE_PROTAGONIST_NAME` | 16 | the name of the unit `gGameState.selectedObjectEvent` names |
| `TEXT_WORK_BLOCK` | 50 | the heap slot the tree walker is copied into |

## The decoded-text ring

Unpacked messages wait in a ring until a typist has typed them.

![Unpacked messages are written into a ring of 512 letter codes, 256 in the Japanese edition, one after another round the ring. Each ends with an end mark and is known only by where it starts. Nothing reserves a message's place: once writing comes round the ring again, the newest message runs on over the oldest. A message stays readable until then.](https://alchemy.pascalpixel.com/illustrations/window/ring.svg)

Each message is written after the one before it, ends with an end mark, and
is known only by where it starts. When the writing reaches the end of the
ring it carries on from the beginning, over whatever was there, so several
windows can show text at once without anyone setting room aside, and a
message stays readable until enough later text has been unpacked to run over
it.

`work->entries` is a ring of 16-bit codes indexed through
`RENDER_ENTRY_MASK`. A message is written at `work->count`, ends with 0, and
is known only by the index it starts at, which `UiText_BuildRenderEntries`
returns; asking for message -1 returns the last message's start again. A
message longer than the ring is cut at the ring's length; none in the game is
([Evidence and open questions](#evidence-and-open-questions)).

## Typing

A typist (a message channel) types an unpacked message into its window a
little each frame and acts on the marks it meets: a new line, a new page, a
pause, a colour.

![Isaac talks to Garet's sister in Vale: a message window zooms open at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

Each frame the typist puts on one more letter cell, as the picture at the top
of this page shows. A cell is a sprite 16 pixels wide, and outside the
Japanese edition two narrow letters that fit in one cell together are typed
as one, so "Yo" appears on the first frame. How many cells come a frame is
the player's message speed: at the slow speed one, followed by a pause of 4
frames that holding a button skips; at the normal speed one; at the fast
speed up to ten, though a space or a mark ends the frame's share, so the
text comes about a word at a time. Battle text is typed faster still, 8
cells a frame, and 13 after a frame that ran late, so a battle keeps moving.
Every few cells the speaker's voice blip plays.

At a page break the typist shows the arrow and waits for a button; then the
window is cleared, refitted to the next page's text, and typing goes on. At
the end the window either stays open or, with the closing mark, waits for a
button and closes.

In the code a channel (`struct UiChannelSlot`) holds its window, the pen
position in 1/256 pixels, the index of its next code, the countdowns for
pauses and for the wait after each character, and its style.
`UiText_QueueRenderEntries` takes a free channel and returns `NULL` when all
three are busy; the channel stays taken until its window closes.
`UiText_StartBattleMessage` also reuses a channel whose window has finished,
continuing below its last line and scrolling once both lines are used.

`UiWork_ProcessAll` is the window system's task. Both setups schedule it in
the main loop's pass, and it runs every frame: the windows' zoom
(`UiWork_ProcessDirectWork`), then the channels, then the render outputs
(`UiWork_AnimateSpriteSlots`). Code that shows a message waits on its window
while the task types it.

`UiWork_StepChannelScript` types one frame's share of a channel. Outside
battle the share is `gUiText_SpeedSteps` for the player's message speed, and
a space or a control code ends it; after each character the channel waits
`gUiText_SpeedDelays` frames while no button is held. Two characters whose
glyph widths add up to 15 pixels or less are drawn as one pair, the second in
the code's high byte (14 in Spanish with an outline), except in a text canvas
and in Japanese. In battle (`menuState` nonzero) the share is 3 characters
plus 5 for each VBlank the last frame took, counting at most two: 8 for a
frame on time and 13 after a late one. The speaker's voice blip plays every
`gUiText_SpeedCueGaps` characters, and is one of four sounds from
`speakerVoiceSound`, chosen by the character's code.

The speed tables are in `text_speed.c`, indexed by
`gGameState.messageSpeed`, 0 to 2 from slowest to fastest; the codes a
channel runs are in `window_internal.h`, and codes from `TEXT_SPACE` up are
characters.

| Name | Value | Meaning |
| --- | --- | --- |
| `gUiText_SpeedSteps` | 1, 1, 10 | characters typed each frame |
| `gUiText_SpeedDelays` | 4, 0, 0 | frames waited after each character while no button is held |
| `gUiText_SpeedCueGaps` | 1, 3, 10 | characters between two voice blips |
| `TEXT_END` | 0 | the text ends; the window stays open |
| `TEXT_PAGE` | 1 | wait for a button, then clear the window for a new page |
| `TEXT_WAIT_KEY` | 2 | wait for a button, then close the window |
| `TEXT_NEWLINE` | 3 | a new line |
| `TEXT_PAUSE_LONG` | 4 | a 60-frame pause |
| `TEXT_PAUSE_SHORT` | 5 | a 20-frame pause a button can cut short |
| `TEXT_PAUSE_LONGEST` | 6 | a 120-frame pause a button can cut short |
| `TEXT_COLOURS_RESET` | 7 | colour 15, style 0 and line spacing 10 again |
| `TEXT_COLOUR`, `TEXT_OUTLINE`, `TEXT_LINE_SPACING` | 8, 9, 10 | the next entry is the colour, the style or the line spacing |
| `TEXT_WINDOW_STATE` | 15 | the next entry is the window effect to run |
| `TEXT_END_ALT` | 30 | ends the text as `TEXT_END` does |
| `TEXT_SPACE` | 32 | the first character code |

## Drawing text

A window's frame is always tiles of the windows' tilemap, but its letters go
on in one of three ways, depending on what the window is for.

![A window's frame and filling are always tiles of the window layer. Its letters go on in one of three ways. A message typed on the field puts each letter cell on as a sprite riding on the window, so the letters can bob and move. A battle message is drawn into one shared picture across the bottom of the screen, which only one window can use at a time. Text drawn all at once, such as menu labels and the area's name, becomes tiles of the window layer itself, taken from a set of 128.](https://alchemy.pascalpixel.com/illustrations/window/text-ways.svg)

A message typed on the field puts each cell on as a sprite riding on the
window, which is what lets letters bob or shake. A battle message is painted
into one shared picture, the text canvas, across the bottom rows of the
screen; there is one canvas, so only one such window shows at a time. Labels
drawn all at once, such as menu text and the area's name, become tiles of the
tilemap itself, from a set of tiles kept for text.

![Isaac casts Mind Read on Garet's sister in Vale: the field tints red, then gold and green, as her thought `Gee, I wish I could see new lands, too.` is typed into a round-cornered window beside her portrait.](https://alchemy.pascalpixel.com/abilities/mind-read.gif)

While `mode` is set, as under Mind Read, the window has round corners, the
text is drawn in colour 8 with no shadow, and a button press latched before
a page's wait is dropped.

The canvas (`UI_WINDOW_TEXT_CANVAS`) draws into bitmap tiles at
`UI_CANVAS_VRAM`, through `glyph_arm.c`. It is 120 tiles from
`UI_CANVAS_FIRST_TILE`, enough for the interior of a full-width window six
tiles tall, and its tiles run down columns; `UiText_PrepareMessageWork` opens
it as a frameless window over the bottom rows for battle messages. Every canvas window maps the
same tiles. Canvas lines are 13 pixels apart, and from the second line on the
canvas scrolls up instead.

Text drawn as tiles (`UiText_DrawResource`, `UiText_DrawCharacterAtOffset`)
takes tiles `0x80` to `0xFF`, and `0x200` to `0x27F` as well while `alt` is
set; its lines are 15 pixels apart. A tile's attribute has bit 0 once
allocated and bit 1 while the tilemap shows it, so `UiText_RenderStringTiles`
can reuse the tiles a string already has and take only free ones.

| Way | Chosen by | Where it draws | Line spacing |
| --- | --- | --- | --- |
| text canvas | `UI_WINDOW_TEXT_CANVAS` | 120 tiles from `UI_CANVAS_FIRST_TILE` (`0x128`), at `UI_CANVAS_VRAM` (`0x06002500`) | 13 pixels |
| sprites | a typed message without the canvas flag | one 16 by 16 render output a cell: a character, or two narrow ones outside Japanese | 15 pixels |
| tilemap tiles | text drawn all at once | tiles `0x80` to `0xFF`, and `0x200` to `0x27F` with `alt` set | 15 pixels |

## Render outputs

A render output is one sprite a window carries: a cell of text, an icon, a
portrait or the page arrow. It moves with its window and goes when the
window closes.

Each window keeps a list of its outputs, taken from the pool's free list and
returned when the window closes. `UiWork_AnimateSpriteSlots` moves each
output every frame by its `UI_SPRITE_` motion and queues it for drawing. An
output's picture occupies an entry of the VRAM cache, loaded into a free one
(`Resource_FindFreeEntry`, `VramCache_Load`) and freed by
`Resource_ResetEntry`. The two portrait slots record which picture each shows
(`resourceIds`, 999 when free) and its cache entry. In Japanese, a character
that finds no free output closes its window.

## The area name

Arriving in a new area, the player sees its name in a small window near the
top of the screen for a moment.

![The party arriving in Bilibin: a small window with the town's name appears at once, centred across the screen and in its top half, with no zoom. It stays for 90 frames, a second and a half, counting down by one each frame, and closes when the count reaches zero. The name comes from the table of area names.](https://alchemy.pascalpixel.com/illustrations/window/area-name.svg)

`UiTimedNotice_Create` shows the area's name, from
`MapSec_GetNameIndexFar`, drawn as tiles in a window centred across the
screen and in its top 10 rows, opened with `UI_WINDOW_INSTANT`. It keeps the
window and a countdown of `UI_AREA_NOTICE_FRAMES` (90) in the event work, and
`UiTimedNotice_Tick`, a main-loop task, closes it when the countdown ends.

| Name | Value | Meaning |
| --- | --- | --- |
| `UI_AREA_NOTICE_FRAMES` | 90 | frames the area's name shows |

## Design

[Menus, windows and shops](../../menus.md) explains why messages are decoded
ahead of typing and why every window shares one tilemap. Within the window
code, the two routines that run per pixel or per bit, the canvas renderer and
the tree walker, are ARM code (the processor's full 32-bit instructions)
copied into RAM, while everything else is Thumb code (its compact 16-bit
form) run from ROM, likely for speed, because ARM code fetched from the
cartridge's 16-bit bus is slow. The work block's two layouts are one struct
whose array sizes come from `tbs_edition.h`, so the code that reads it is the
same in every edition; the ring was likely doubled for the longer European
text.

## Limits

- The screen is 30 by 20 tiles; a frame narrower or shorter than 2 tiles, or
  wider or taller than 30, is not drawn.
- Twelve window slots in Japanese and eight in the other editions.
- Three message channels and 64 render outputs.
- Eight message arguments at a time (`UI_ARGUMENT_SLOT_COUNT`); a ninth is
  dropped (`UiWork_PushValueSlot`).
- The decoded-text ring holds 256 codes in Japanese and 512 in the others.
- The text canvas holds 120 tiles, one canvas window at a time.
- Text drawn as tiles has 128 tiles, or 256 with `alt` set.

## Files

| File | What it holds |
| --- | --- |
| `window.c` | the window object, written as parts in `window/`: windows, message channels, the decoder, render outputs, icons, the selection row and the option and system menus |
| `battle_command.c` | the screens built on the window object, written as parts in `battle_command/`, from the party status window to the debug menus |
| `glyph_arm.c` | the text canvas renderer, compiled as ARM code and copied to RAM to run |
| `articles.c` | each edition's articles and possessives, the kana marks and the sprite motion curves |
| `scales.c` | sprite scales, motion curves and the lookup tables of the window and menu code |
| `text_speed.c` | the three message speed tables |
| `windata.s` | glyph widths, kana mark ranges, the party and Djinni sprites, the growing bar and the cursor stops |
| [`keyboard.s`](keyboard.s) | the name entry screen's keyboard tilemaps |
| [`cursor.s`](cursor.s), `cursor_frame.s`, `fixed_block.s`, `slider.s`, `slidpal.s` | sprite tiles and palettes, built from the PNGs beside them or under `../menu` and `../graphics` |

Both objects are written as parts: `window.c` and `battle_command.c` include
their private headers (`window_internal.h`, `battle_command_internal.h`) and
then their parts in ROM order, and their opening comments say where to start.
`head -3 window/*.c battle_command/*.c` lists what each part holds. Link order
is in `games/tbs/objects.tsv`, from `glyph_arm`.

The prototypes are in `games/tbs/include/unit/window/`; its
[`window.h`](../../include/unit/window/window.h) also holds what both objects
use: the tilemap's tiles (`UI_TILE_`), the window flags (`UI_WINDOW_`), sprite
motions (`UI_SPRITE_`), message argument kinds (`MESSAGE_ARGUMENT_`) and the text
canvas's place in VRAM. The work block's layout, `struct UiRenderWork`, is in
[`include/window.h`](../../include/window.h), and its offsets for each edition
in `include/tbs_edition.h`.

## Words used here

- **work block**: `struct UiRenderWork`, heap slot `HEAP_SLOT_WINDOW`, reached
  as `UI_WORK`. It holds all of the window system's state.
- **window**: a `struct UiWindow` slot, one framed rectangle of the shared
  tilemap.
- **text canvas**: a window with `UI_WINDOW_TEXT_CANVAS`, whose text is drawn
  into bitmap tiles rather than placed as sprites or tilemap tiles.
- **decoded text**: a message after decoding, as 16-bit codes in the ring
  `work->entries`.
- **channel**: a `struct UiChannelSlot`, which types decoded text into one
  window and runs its control codes (`TEXT_`); the pictures call it a typist.
- **render output**: a `struct RenderOutput`, one sprite a window carries.
- **tile attributes**: `work->tileAttributes`, one byte per text tile saying
  whether it is allocated and whether the tilemap still shows it.
- `unk` plus an offset, such as `unk10`: a field whose meaning is not known.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `UiWork_` | the work block and its per-frame updates |
| `UiWindow_` | a window's rectangle of the tilemap: frames, fills, dividers, tiles |
| `UiText_`, `UiTextResource_` | messages and text: decoding, measuring, drawing, and text placed as a resource |
| `RenderOutput_` | the render output pool and a window's output list |
| `UiIcon_`, `ItemIcon_`, `UiGlyph_` | icons built into sprite tiles |
| `SelectionScreen_`, `Menu_` | the selection row, and the menus built from command icons |
| `OptionMenu_`, `SystemMenu_`, `WindowColor_`, `PaletteGlow_` | the option and system menus and the window colours |
| `SaveState_`, `SaveMenu_` | save files and the save file menu |
| `NameEntry_`, `PartyStatus_`, `PartyTalkMenu_`, `DjinnMenu_`, `SummonMenu_`, `ItemList_`, `BattleMenu_`, `BattleTarget_` | the screens they name |
| `Party_` | the joined-party messages and the party's Psynergy list |
| `UiTimedNotice_` | the area name shown near the top of the screen on arrival |
| `DebugMenu_` | the debug menus |
| `BattleLayout_` | the party status window's highlighted columns while commands are chosen |
| `AffineEffect_`, `ResourceMenu_` | the command icon menus' pulsing icon and their position table |
| `Link_` | the link countdown window and its page arrows |
| `Color_` | scaling a colour's channels |
| `UiRender_` | an empty function nothing calls |
| `Resource_`, `RenderResource_`, `Graphics_`, `GraphicsPalette_`, `Vram_` | things the engine names elsewhere: VRAM cache entries, pictures, palettes and video registers |

The `Ui_` functions that remain have no family yet. Many of these names are
shared with The Lost Age's twins and with the far-call table
`system/far_call/WINDOW.S`, so renaming one changes other folders too.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## Evidence and open questions

Windows, channels and render outputs are named from how their structs are
used, the control codes from what the decoder and the channel do with each,
and the screens from the English release's text they show. Messages are
named by their linker symbols, made from their English text. The typing
picture's frames come from the English glyph widths (`graphics/font/text.tsv`)
and the pairing rule in `UiWork_StepChannelScript`, and agree with a
recording of the window opening (`guides/window/window-open.png`), whose
first typed frame shows "Yo".

XXX: likely the canvas renderer and the tree walker are ARM code run from
RAM for speed, because ARM code fetched from the cartridge's 16-bit bus is
slow; nothing in the code says so. XXX: the likely reason the ring was
doubled outside Japan is the longer European text; the bytes show only the
change.

A message longer than the ring would be cut, its terminator landing on its own
start, but no message is that long. Counting each control code and line break
as one code, before the arguments are filled in, the longest message of each
edition's text is the Lucky Dice rules, at 200 of the Japanese ring's 256
codes and at most 446 (Italian) of the others' 512. The two bytes before the
argument table, `padding` in `struct UiRenderWork`, align `values` to a word
in every edition, and nothing reads them.

- XXX (no evidence): why there are three channels, and why the international
  editions gave up four windows when they doubled the ring; the code shows
  only the counts each edition uses, and only Camelot's sources could say how
  they were chosen.
