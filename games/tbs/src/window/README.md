# Windows

When someone in a town speaks, a framed box opens at the top of the screen
and their words appear in it a few letters at a time, with a little voice blip
as they go. Every box of text in the game works this way: what people say, the
menus, the shop counters, the battle commands. The window code draws those
boxes, unpacks the game's stored text, types it out over several
[frames](../../glossary.md#frame) and moves the small pictures a box carries:
letters, icons, portraits and the page arrow.

It also builds the screens made directly from windows: the field menu's row of
icons, the pause menu and its option menu, the file menu that saves the game,
the naming screen, the [party](../../glossary.md#the-party) status window, the
battle menus and the debug menus.
[Menus, windows and shops](../../menus.md#windows) gives the whole layer in
brief; the field menu's four screens are in [Menus](../menu/README.md), and
the shop counters in [Shops](../shop/README.md).

## The work block

Everything the window system keeps between frames lives in one block of
memory, set up with each [map](../../glossary.md#map) and gone at the next.

The block is `struct WindowWork`, borrowed from the
[heap](../../glossary.md#heap-and-pools) under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_WINDOW` by
`WindowSystem_InitField` for the field and `WindowSystem_InitBattle` for
battle, and reached as `WINDOW_WORK`. In order it holds:

- `tilemap`: the windows' tilemap, 32 by 20 tiles, copied to video memory
  during [VBlank](../../glossary.md#frame) in 4-row bands marked in `dirty`;
- `windows`: the window slots, `WINDOW_COUNT` of them;
- `printers`: the three text printers;
- `sprites` and `freeSprites`: the 64 window sprites and the list of free ones;
- `textTileState`, `nextTextTile` and `extraTextTiles`: the text tiles'
  bookkeeping ([Drawing text](#drawing-text));
- `mindRead`, `battleMode` and the current text style;
- `textBuffer`: the text buffer, a ring of unpacked text;
- `argumentValues` and `argumentKinds`: the eight message arguments;
- `portraitPictures` and `portraitSprites`: the two portrait slots;
- `voiceSound` and the flags for halting speech.

The Japanese [edition](../../glossary.md#editions) has 12 window slots and a
ring of 256 codes, the others 8 slots and 512 codes, so the block grows from
4,492 bytes to 4,860; `include/tbs_edition.h` gives every field's offset in
each layout and `STATIC_ASSERT`s in `include/window.h` check the struct
against them. `battleMode` is 0 on the field, 1 in battle (typing is faster)
and 2 while `BattleMessage_Open` unpacks a battle message.

| Name | Value | Meaning |
| --- | --- | --- |
| `HEAP_SLOT_WINDOW` | 15 | the heap slot that holds the work block |
| `WINDOW_WORK_SIZE` | 4,492 bytes in Japanese, 4,860 in the others | the work block's size |
| `WINDOW_COUNT` | 12 in Japanese, 8 in the others | window slots |
| `WINDOW_SPRITE_COUNT` | 64 | window sprites |
| `MESSAGE_ARGUMENT_COUNT` | 8 | message arguments at a time |

## Windows

![The field menu in Xian opens on the Item screen: six windows zoom open from their centres at once, and when the player presses B they shrink away and the row of icons is back.](https://alchemy.pascalpixel.com/guides/window/windows-zoom.gif)

A window grows out of its centre over a few frames when it opens and shrinks
back when it closes. The code that opened it waits until it is full size
before writing in it. Its frame and blue filling are tiles of the one
[background layer](../../glossary.md#background-layer) in front of the map's
three; what it carries (letters, portraits, the hand) are sprites riding on
it.

`Window_Create(x, y, width, height, flags)` takes the first free slot, or
returns `NULL` when none is free, and places the window at tile (x, y). The
tilemap is 32 tiles wide because the console's background layers are, but the
screen shows only 30 of them, so windows are placed on a screen of 30 by 20
tiles. Unless the window has `WINDOW_FLAG_INSTANT`, `Window_UpdateZooms` draws
its frame at (8 − frame) / 8 of its size about its centre over
`WINDOW_OPEN_FRAMES`, and shrinks it over `WINDOW_CLOSE_FRAMES`; the slot is
freed only once it has closed, so a closing window still counts against the 8.
`Window_Close(window, instant)` starts the close and returns at once.
`Window_DrawFrame` draws nothing for a width or height below 2 or above 30,
and nothing checks that a window stays within the tilemap's 20 rows.

A message window sizes itself to its text: `Window_FitOnScreen`
(`window/measure.c`) measures the message and moves the window so it fits.
Where the editions lay a screen out differently, the value is a named macro
chosen in one `#if` block at the top of the private header or the part, as
the battle [summon](../../glossary.md#summons) list's `SUMMON_MENU_WINDOW_X`
and `SUMMON_MENU_WINDOW_WIDTH` are.

| Name | Value | Meaning |
| --- | --- | --- |
| `WINDOW_FLAG_OPEN` | `0x1` | the slot is in use |
| `WINDOW_FLAG_INSTANT` | `0x2` | opens and closes at once, with no zoom |
| `WINDOW_FLAG_TEXT_CANVAS` | `0x8` | text is drawn into the text canvas ([Drawing text](#drawing-text)) |
| `WINDOW_FLAG_ANCHOR_BOTTOM` | `0x80` | when a new page resizes the window, its bottom edge stays put |
| `WINDOW_FLAG_FIXED_X` | `0x100` | when a new page resizes the window, its left edge stays put |
| `WINDOW_OPEN_FRAMES` | 8 | frames a window takes to open |
| `WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to close |

## Decoding a message

![Isaac asks Bilibin's innkeeper for a room: `Welcome to our inn.`, then `It's 21 coins for a room. Would you like to stay?` with the price filled in, a Yes and No choice and the coins window; he answers No and the innkeeper says `Please come again.`](https://alchemy.pascalpixel.com/guides/window/message-blanks.gif)

The cartridge stores the game's text squeezed, so a message is unpacked whole
before it is typed. A message can hold blanks, such as the 21 coins above or a
member's or an item's name, and they are filled in as it is unpacked, with
"a" or "an" and the other small words each language puts in front of a name.
All the slow work happens here, once, so typing it afterwards costs almost
nothing a frame.

The text is in the PO catalogs under `games/tbs/text`, one per language, which
the build's `ags-text` packs, with a header naming each message; the code
writes a message by that linker symbol (`MsgGameSaved`). `Message_Decode`
([`window/decode.c`](window/decode.c)) unpacks a whole message into the text
buffer. Messages are Huffman-coded, with a separate tree for each previous
letter (`Text_MessageContexts`); the routine that walks the trees,
`Text_DecodeSymbol`, is handwritten ARM code in
`graphics/text/decode_symbol.s`, copied into heap slot `TEXT_WORK_BLOCK` (50)
for the decode. A blank takes its value from the argument table, which callers
fill with `MessageArgument_Push(value, kind)`; the articles and endings come
from `articles.c`. The inn's price is `MessageArgument_Push(price,
MESSAGE_ARGUMENT_NUMBER)` before its line.

Control codes are written in braces in the catalogs (`{end_wait}` at the end
of `Your game was saved.{end_wait}`) and copied into the buffer unchanged for
the text printer. An operand is stored one above its value.

| Name | Value | Kept as |
| --- | --- | --- |
| `MESSAGE_END` | 0 | `TEXT_END`: the window stays open |
| `MESSAGE_PAGE_BREAK` | 1 | `TEXT_PAGE` |
| `MESSAGE_END_WAIT` | 2 | `TEXT_WAIT_KEY`: a button closes the window |
| `MESSAGE_LINE_BREAK` | 3 | `TEXT_NEWLINE` |
| `MESSAGE_TEXT_COLOR` | 8 | `TEXT_COLOR`, with its operand |
| `MESSAGE_LOCALIZED_WORD` | 13 | German: a counted noun; Italian: a gender ending; Spanish and French: the plural suffix |
| `MESSAGE_PROTAGONIST_NAME` | 16 | the letters of the leader's name |

## The text buffer

Unpacked messages wait in a ring, the text buffer, until a text printer has
typed them.

Each message is written after the one before, ends with 0 and is known only by
the index it starts at, which `Message_Decode` returns. When writing reaches
the end of the ring it carries on from the start, over whatever was there, so
several windows can show text at once with nothing set aside, and a message
stays readable until enough later text has run over it. The index wraps
through `TEXT_BUFFER_MASK` (511, or 255 in Japanese); no message in the game
is longer than the ring ([Evidence](#evidence)).

## Typing

![Isaac talks to Garet's sister in Vale: a message window opens at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

A text printer puts a message into its window a little each frame, with the
speaker's voice blip every few letters. At a page break it shows the arrow
and waits for a button; then the window is cleared, refitted to the next
page's text and typing goes on. At the end the window stays open or, with the
closing mark, waits for a button and closes.

A text printer (`struct TextPrinter`) holds its window, the pen position in
1/256 pixels, the index of its next code, its countdowns and its style.
`TextPrinter_Add` takes one of the three, or returns `NULL` when all are busy.
`WindowSystem_Update`, the window system's [task](../../glossary.md#task), runs
once a frame: the windows' zoom, then `TextPrinter_Step` for each printer, then
the window sprites. `Message_OpenWindow` (`window/frames.c`) is the whole path
for one message, and a caller then waits:

```c
entry = Message_Decode(message, 1);
Window_FitOnScreen(entry, &x, &y, &width, &height, bounds, 0);
window = Window_Create(x, y, width, height, flags);
TextPrinter_Add(window, entry, 0, 0, bounds, 0);

while (TextPrinter_AllFinishedFar() == 0)
  WaitFrames(1);
```

The codes a printer acts on are in `window_internal.h`; from `TEXT_SPACE` (32)
up they are letters.

| Name | Value | Meaning |
| --- | --- | --- |
| `TEXT_END` | 0 | the text ends; the window stays open |
| `TEXT_PAGE` | 1 | wait for a button, then clear the window for a new page |
| `TEXT_WAIT_KEY` | 2 | wait for a button, then close the window |
| `TEXT_NEWLINE` | 3 | a new line |
| `TEXT_PAUSE_LONG` | 4 | a 60-frame pause |
| `TEXT_PAUSE_SHORT` | 5 | a 20-frame pause a button can cut short |
| `TEXT_PAUSE_LONGEST` | 6 | a 120-frame pause a button can cut short |
| `TEXT_COLOR`, `TEXT_OUTLINE`, `TEXT_LINE_SPACING` | 8, 9, 10 | the next code is the colour, the style or the line spacing |

### How fast the letters come

![Garet's sister's line typed twice from the same moment: first at the slow message speed, a letter at a time with a pause after each, then at the fast speed, where it comes about a word at a time.](https://alchemy.pascalpixel.com/guides/window/message-speed.gif)

The player chooses the message speed in the option menu. At the slow speed one
letter comes a frame with a pause of 4 frames after it, which holding a button
skips; at the normal speed one a frame; at the fast speed up to ten, though a
space ends the frame's share, so the text comes about a word at a time. In
battle it is faster still, 8 letters a frame, so the battle keeps moving.

`TextPrinter_Step` reads the share from `gMessageSpeed_Steps` and the pause
from `gMessageSpeed_Delays`, both indexed by `gSaveBlock.messageSpeed`
(`message_speed.c`). A letter is a 16-pixel sprite cell, and in the
international editions two narrow letters whose widths add up to 15 pixels or
less share one cell, so "Yo" appears on the first frame. In battle the share
is 3 + 5 × min(VBlanks the last frame took, 2): 8 on time, 13 after a late
frame.

| Name | Value, slow to fast | Meaning |
| --- | --- | --- |
| `gMessageSpeed_Steps` | 1, 1, 10 | letters typed each frame |
| `gMessageSpeed_Delays` | 4, 0, 0 | frames waited after each letter while no button is held |
| `gMessageSpeed_VoiceGaps` | 1, 3, 10 | letters between two voice blips |

## Drawing text

![Isaac casts Mind Read on Garet's sister in Vale: the field tints red, then gold and green, as her thought `Gee, I wish I could see new lands, too.` is typed into a round-cornered window beside her portrait.](https://alchemy.pascalpixel.com/abilities/mind-read.gif)

A window's letters go on in one of three ways. A message typed on the field
puts each letter cell on as a sprite riding on the window, which is what lets
letters bob or shake. A battle message is painted into one shared picture, the
text canvas, across the bottom of the screen. Labels drawn all at once, such
as menu text and the area's name, become tiles of the window layer itself.
Under Mind Read the window gets round corners and the text another colour.

The canvas (`WINDOW_FLAG_TEXT_CANVAS`) is 120 tiles from
`TEXT_CANVAS_FIRST_TILE`, drawn by `glyph_arm.c`; there is one, so only one
canvas window shows at a time. Text drawn as tiles (`Text_DrawMessageAsTiles`)
takes 128 tiles, and 128 more (`extraTextTiles`, set by `Text_UseExtraTiles`)
while the field menu's screens or the battle's Djinn menu are open. Each
window keeps a list of its window sprites (`struct WindowSprite`), moved every
frame by `WindowSprite_UpdateAll` and returned to the pool when it closes;
their pictures hold entries of the VRAM cache until `VramCache_FreeEntry`.
While `mindRead` is set every corner is `WINDOW_TILE_FRAME_ROUND_CORNER` and
the fitted window is 2 tiles wider.

| Way | Chosen by | Where it draws | Line spacing |
| --- | --- | --- | --- |
| sprites | a typed message without the canvas flag | one 16 by 16 window sprite a cell | 15 pixels |
| text canvas | `WINDOW_FLAG_TEXT_CANVAS` | 120 tiles from `TEXT_CANVAS_FIRST_TILE` (`0x128`) | 13 pixels |
| tilemap tiles | text drawn all at once | tiles `0x80` to `0xFF`, and `0x200` to `0x27F` with `extraTextTiles` | 15 pixels |

## The area name

![The party arrives in Bilibin from the world map: as the screen fades in, a small window with `Bilibin` appears near the top of the screen, stays a moment and goes.](https://alchemy.pascalpixel.com/guides/window/area-name.gif)

Arriving in a new area, the player sees its name in a small window near the
top of the screen for a second and a half.

`MapNamePopup_Show` draws the name, from `MapSec_GetNameIndexFar`, as tiles in
a window centred across the screen, opened with `WINDOW_FLAG_INSTANT`. It keeps
the window and a countdown in the field work (`gFieldWork`), and
`MapNamePopup_Update`, a main-loop task, closes it when the countdown ends.
Opening the field menu hides it at once (`MapNamePopup_HideFar`).

| Name | Value | Meaning |
| --- | --- | --- |
| `MAP_NAME_POPUP_FRAMES` | 90 | frames the area's name shows |

## The pause menu

![Isaac presses Start in Vale: a window titled Pause offers `Save your quest`, `Sleep` and `Change settings`, each with an icon that lights up as the hand moves down to it, and B closes it.](https://alchemy.pascalpixel.com/guides/window/pause-menu.gif)

Start on the field opens the pause menu: Save your quest, Sleep and Change
settings. Sleep says to press L and R to resume and then stops the console
until they are pressed. Backing out of the file choice or the option menu
brings the pause menu back.

The field's `Map_Run` calls `PauseMenu_Run` (`window/screens/debug.c`), which
reads the choice from `PauseMenu_Choose` and runs `Save_ToChosenFile`, the
sleep message (setting `gSleepRequested`) or `OptionMenu_Run`. The menu and
the option menu share one heap slot, `HEAP_SLOT_OPTION_MENU`. In debug mode
three more entries follow (`PAUSE_MENU_DEBUG_ROWS`).

| Name | Value | Meaning |
| --- | --- | --- |
| `PAUSE_MENU_SAVE_QUEST` | 0 | Save your quest |
| `PAUSE_MENU_SLEEP` | 1 | Sleep |
| `PAUSE_MENU_CHANGE_SETTINGS` | 2 | Change settings |

### Saving

![Isaac presses Start in Vale and chooses `Save your quest`: a window titled Save lists three files, all `(No saved data)`; the first is chosen, `Saving...` shows, and then `Your game was saved.`](https://alchemy.pascalpixel.com/guides/window/save.gif)

The game keeps three save files in the cartridge's backup memory. Saving asks
which file, with the one last used first, and asks `OK to save over this
file?` when it already holds a game; then the saved jingle plays and the game
is written.

`Save_ToChosenFile` (`window/screens/save.c`) runs the file menu
(`FileMenu_SelectFile`, also used by the title's Continue, Copy and Erase),
builds the file's summary (`Save_BuildFileSummary`: level, play time, the
Djinn found), records where the map's people stand
(`Save_CaptureObjectEventsFar`) and writes the 8 KB save buffer as two 4 KB
records, `file` and `file + SAVE_FILE_COUNT`. A failed write shows
`Failed to save.`; with no backup memory it shows
`Unable to detect backup memory.`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SAVE_FILE_COUNT` | 3 | save files |
| `SAVE_RECORD_BYTES` | 4 KB | one of a file's two records |

### The option menu

![Isaac opens Change settings from the pause menu: the hand turns the window colour slider, and every window turns from blue to green; it lowers the brightness, the windows darken; B drops the changes, the windows are blue again and the pause menu returns.](https://alchemy.pascalpixel.com/guides/window/options.gif)

Change settings sets the windows' colour and brightness, the message speed,
whether people speak with voice blips, and auto-sleep. The windows change
colour as the player moves the slider; A or Start keeps the settings, B puts
the old ones back.

`OptionMenu_Run` (`window/window/options.c`) copies the settings into its work
block so that cancelling can drop them, and calls `WindowColor_Apply` as the
slider moves, which rewrites the window colours of `BG` palette 15 from a hue
and brightness. Select cycles through preset colours. Every setting wraps at
its ends.

| Row | Values |
| --- | --- |
| window colour | 24 hues |
| window brightness | 15 steps, 7 neutral |
| message speed | 3 ([How fast the letters come](#how-fast-the-letters-come)) |
| speech | on or off |
| auto-sleep | on or off |

## The naming screen

![A new game with no save file: a keyboard window comes up under Isaac's portrait and the name `Isaac`; B takes off two letters, the hand moves to `a` and then `c` and puts them back, Start jumps to End, and A ends the name.](https://alchemy.pascalpixel.com/guides/window/naming-screen.gif)

A new game opens on the naming screen with the leader's name already in it.
The player moves the hand over a keyboard: A puts a letter on, B or Del takes
the last one off, and End (or Start, which jumps there) keeps the name. A
name has at most five letters, and an empty one keeps the old name.

`NamingScreen_EditMemberName(member)` (`window/screens/name_entry.c`) runs it
and writes the name over the member's only on End; B on an empty name returns
-1. The main menu's new game (`menu/main_menu.c`) names Isaac alone; two
hidden button codes there add more names to enter, the first Garet, Ivan and
Mia, the second Felix, Jenna and Sheba as well.

| Name | Value | Meaning |
| --- | --- | --- |
| `NAME_MAX_LETTERS` | 5 | letters in a name |

## Design

[Menus, windows and shops](../../menus.md) explains why messages are unpacked
ahead of typing and why every window shares one tilemap. The two routines that
work per pixel or per bit, the canvas renderer and the tree walker, are ARM
code (the processor's full 32-bit instructions) copied into RAM, likely for
speed, since ARM code fetched from the cartridge's 16-bit bus is slow; the
rest is Thumb code run from ROM. The work block's two layouts are one struct
whose array sizes come from `tbs_edition.h`, so the code that reads it is the
same in every edition.

Many of the window functions' names are shared with
[The Lost Age](../../glossary.md#the-two-games)'s copies of these files and
with the [far-call](../../glossary.md#far-call) table `system/far_call/WINDOW.S`,
so renaming one changes other folders too.

## Limits

- The screen is 30 by 20 tiles; a frame narrower or shorter than 2 tiles, or
  wider or taller than 30, is not drawn.
- Twelve window slots in Japanese and eight in the international editions.
- Three text printers and 64 window sprites.
- Eight message arguments at a time; a ninth is dropped
  (`MessageArgument_Push`).
- The text buffer holds 256 codes in Japanese and 512 in the others.
- The text canvas holds 120 tiles, one canvas window at a time.
- Text drawn as tiles has 128 tiles, or 256 with `extraTextTiles` set.
- Three save files; a name has five letters.

## Words used here

- **work block**: `struct WindowWork`, heap slot `HEAP_SLOT_WINDOW`, reached
  as `WINDOW_WORK`. It holds all of the window system's state.
- **window**: a `struct Window` slot, one framed rectangle of the shared
  tilemap.
- **text canvas**: a window with `WINDOW_FLAG_TEXT_CANVAS`, whose text is drawn
  into bitmap tiles rather than placed as sprites or tilemap tiles.
- **text buffer**: the ring `work->textBuffer` of unpacked 16-bit codes.
- **text printer**: a `struct TextPrinter`, which types unpacked text into one
  window and runs its control codes (`TEXT_`).
- **window sprite**: a `struct WindowSprite`, one sprite a window carries.
- **file menu**: the list of the three save files (`FileMenu_SelectFile`).
- `unk` plus an offset, such as `unk10`: a field whose meaning is not known.

## Evidence

Windows, text printers and window sprites are named from how their structs
are used, the control codes from what the decoder and the printer do with each,
and the screens from the English release's text they show. Messages are
named by their linker symbols, made from their English text. That "Yo" is
typed on the first frame follows from the English glyph widths
(`graphics/font/text.tsv`) and the pairing rule in `TextPrinter_Step`, and
agrees with the recording of the window opening
(`guides/window/window-open.png`).

A message longer than the ring would be cut, its terminator landing on its own
start, but no message is that long. Counting each control code and line break
as one code, before the arguments are filled in, the longest message of each
edition's text is the Lucky Dice rules, at 200 of the Japanese ring's 256
codes and at most 446 (Italian) of the others' 512. The two bytes before the
argument table, `padding` in `struct WindowWork`, align `argumentValues` to a
word in every edition, and nothing reads them. The halfword after
`lineSpacing`, `unusedAfterLineSpacing`, is set to 1 whenever `Text_ResetStyle`
resets the text style and read by nothing: the text printer has no field to
copy into it (`TextPrinter_ApplyStyle` copies only the colour, outline and
line spacing), no text control code writes it, and the glyph code, all C,
reads the style through its named fields.
