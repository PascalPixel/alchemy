# Windows

When someone in a town speaks, a framed box opens at the top of the screen
and their words appear in it a few letters at a time, with a little voice blip
as they go. Every box of text in the game works this way: what people say, the
menus, the shop counters, the battle commands. The window code draws those
boxes, unpacks the game's stored text, types it out over several
[frames](../../glossary.md#frame) and moves the small pictures a box carries:
letters, icons, portraits and the page arrow.

It also builds the screens made directly from windows: the field menu's row of
icons, the Pause menu and its settings menu, the file list that saves the game,
the name entry screen, the [party](../../glossary.md#the-party) status window, the
battle menus and the debug menus.
[Menus, windows and shops](../../menus.md#windows) gives the whole layer in
brief; the field menu's four screens are in [Menus](../menu/README.md), and
the shop counters in [Shops](../shop/README.md).

## The work block

Everything the window system keeps between frames lives in one block of
memory, set up with each [scene](../../glossary.md#scene) and with each battle,
and gone when the next begins: the windows themselves, the three text
printers that type into them, the small pictures they carry, the unpacked
text waiting to be typed and the blanks to fill in it.

In the code the block is `struct WindowWork`, borrowed from the
[heap](../../glossary.md#heap-and-pools) under the
[heap slot](../../glossary.md#heap-slot) `HEAP_SLOT_WINDOW` and reached as
`WINDOW_WORK`; the struct's comments describe each part. The Japanese
[edition](../../glossary.md#editions) has more window slots and a smaller text
buffer than the others, so the block is a different size there, but the code
that reads it is the same.

| Name | Value | Meaning |
| --- | --- | --- |
| `WINDOW_WORK_SIZE` | 4,492 bytes in Japanese, 4,860 in the others | the work block's size |
| `WINDOW_COUNT` | 12 in Japanese, 8 in the others | windows open at once |
| `WINDOW_SPRITE_COUNT` | 64 | small pictures the windows carry |
| `MESSAGE_ARGUMENT_COUNT` | 8 | blanks a message can fill |

## Windows

![The field menu in Xian opens on the Item screen: six windows zoom open from their centres at once, and when the player presses B they shrink away and the row of icons is back.](https://alchemy.pascalpixel.com/guides/window/windows-zoom.gif)

A window grows out of its centre over a few frames when it opens and shrinks
back when it closes. The code that opened it waits until it is full size
before writing in it. Its frame and blue filling are tiles of one
[background layer](../../glossary.md#background-layer), in front of the three
the scene uses; what it carries (letters, portraits, the hand) are sprites riding
on it.

Windows are placed on a grid of 8-pixel tiles, 30 across and 20 down, the size
of the screen. A closing window still holds its place until it has shrunk
away, so a box that is closing still counts against the limit on windows
open at once. A message
window sizes itself to its text, and moves so that it stays on screen.

`Window_Create` opens a window and `Window_Close` starts closing it;
`Window_UpdateZooms` grows and shrinks them each frame, and
`Window_FitOnScreen` sizes a window to a message.

| Name | Value | Meaning |
| --- | --- | --- |
| `WINDOW_OPEN_FRAMES` | 8 | frames a window takes to open |
| `WINDOW_CLOSE_FRAMES` | 4 | frames a window takes to close |

## Decoding a message

![Isaac asks Bilibin's innkeeper for a room: `Welcome to our inn.`, then `It's 21 coins for a room. Would you like to stay?` with the price filled in, a Yes and No choice and the coins window; he answers No and the innkeeper says `Please come again.`](https://alchemy.pascalpixel.com/guides/window/message-blanks.gif)

The cartridge stores the game's text squeezed, so a message is unpacked whole
before it is typed. A message can hold blanks, such as the 21 coins above or a
party member's or an item's name, and they are filled in as it is unpacked, with
"a" or "an" and the other small words each language puts in front of a name.
All the slow work happens here, once, so typing it afterwards costs almost
nothing a frame.

The squeezing is Huffman coding: common letters take fewer bits than rare
ones, and the code for a letter depends on the letter before it, which makes
the text smaller still. Marks such as "wait for a button here" or "switch to
red" travel inside the text as control codes, written in braces in the
catalogs (`{end_wait}` at the end of `Your game was saved.{end_wait}`), and
come out of the unpacking unchanged for the typing to act on.

The text is in the PO catalogs under `games/tbs/text`, one per language; the
code names a message by its linker symbol (`MsgGameSaved`). A caller fills the
blanks with `MessageArgument_Push`, then `Message_Decode`
([`window/decode.c`](window/decode.c)) unpacks the message into the text
buffer; the bit-by-bit work is the handwritten `Text_DecodeSymbol`.

## The text buffer

Unpacked messages wait in the text buffer until a text printer has typed them.

The buffer is a ring: each message is written after the one before, and when
writing reaches the end it carries on from the start, over whatever was there.
Nothing has to be set aside or given back, so several windows can show text
at once, and a message stays readable until enough later text has run over
it. No message in the game is long enough to run over itself
([Evidence](#evidence)). `Message_Decode` returns where in the ring a message
starts, and that position is all a text printer needs.

| Name | Value | Meaning |
| --- | --- | --- |
| `TEXT_BUFFER_MASK` | 511, or 255 in Japanese | the ring holds one more letter code than this |

## Typing

![Isaac talks to Garet's sister in Vale: a message window opens at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

A text printer puts a message into its window a little each frame, with the
speaker's voice blip every few letters. At a page break it shows the arrow
and waits for a button; then the window is cleared, refitted to the next
page's text and typing goes on. At the end the window stays open or, with the
closing mark, waits for a button and closes.

The window system runs once a frame as a [task](../../glossary.md#task): it
grows and shrinks the windows, lets each of the three text printers type its
share, and moves the window sprites. `Message_OpenWindow` (`window/frames.c`)
is the whole path for one message, and shows the order every caller follows:

```c
entry = Message_Decode(message, 1);
Window_FitOnScreen(entry, &x, &y, &width, &height, bounds, 0);
window = Window_Create(x, y, width, height, flags);
TextPrinter_Add(window, entry, 0, 0, bounds, 0);
```

`TextPrinter_Step` types a printer's share each frame and acts on the control
codes, whose meanings are listed with them in `window_internal.h` (`TEXT_`).

### How fast the letters come

![Garet's sister's line typed twice from the same moment: first at the slow message speed, a letter at a time with a pause after each, then at the fast speed, where it comes about a word at a time.](https://alchemy.pascalpixel.com/guides/window/message-speed.gif)

The player chooses the message speed in the settings menu. At the slow speed one
letter comes a frame with a pause of 4 frames after it, which holding a button
skips; at the normal speed one a frame; at the fast speed up to ten, though a
space ends the frame's share, so the text comes about a word at a time. In
battle it is faster still, 8 letters a frame, and more after a frame that ran
late, so the battle keeps moving.

Letters go on in 16-pixel cells, and in the international editions two narrow
letters that fit in one cell share it, so "Yo" appears on the very first
frame.

The three speeds are three small tables in `message_speed.c`
(`gMessageSpeed_Steps`, `gMessageSpeed_Delays`, `gMessageSpeed_VoiceGaps`).

| Speed | Letters a frame | Frames after each letter | Letters between blips |
| --- | --- | --- | --- |
| slow | 1 | 4 | 1 |
| normal | 1 | 0 | 3 |
| fast | 10 | 0 | 10 |

## Drawing text

![Isaac casts Mind Read on Garet's sister in Vale: the field tints red, then gold and green, as her thought `Gee, I wish I could see new lands, too.` is typed into a round-cornered window beside her portrait.](https://alchemy.pascalpixel.com/abilities/mind-read.gif)

A window's letters go on in one of three ways. A message typed on the field
puts each letter cell on as a sprite riding on the window, which is what lets
letters bob or shake. A battle message is painted into one shared picture, the
text canvas, across the bottom of the screen, and since there is one canvas
only one such window shows at a time. Labels drawn all at once, such as menu
text and the area's name, become tiles of the window layer itself. Under Mind
Read the window gets round corners and the text another colour.

The canvas is drawn by `glyph_arm.c`, labels by `Text_DrawMessageAsTiles`, and
the sprites are moved each frame by `WindowSprite_UpdateAll`.

| Way | Used for | Line spacing |
| --- | --- | --- |
| sprites | a message typed on the field | 15 pixels |
| text canvas | a battle message (`WINDOW_FLAG_TEXT_CANVAS`) | 13 pixels |
| window tiles | text drawn all at once | 15 pixels |

## The area name

![The party arrives in Bilibin from the world map: as the screen fades in, a small window with `Bilibin` appears near the top of the screen, stays a moment and goes.](https://alchemy.pascalpixel.com/guides/window/area-name.gif)

Arriving in a new area, the player sees its name in a small window near the
top of the screen for a second and a half. Opening the field menu hides it at
once.

`AreaNameWindow_Show` draws the name and `AreaNameWindow_Update` closes it when its
time is up.

| Name | Value | Meaning |
| --- | --- | --- |
| `AREA_NAME_WINDOW_FRAMES` | 90 | frames the area's name shows |

## The Pause menu

![Isaac presses Start in Vale: a window titled Pause offers `Save your quest`, `Sleep` and `Change settings`, each with an icon that lights up as the hand moves down to it, and B closes it.](https://alchemy.pascalpixel.com/guides/window/pause-menu.gif)

Start on the field opens the Pause menu: Save your quest, Sleep and Change
settings. Sleep says to press L and R to resume and then stops the console
until they are pressed. Backing out of the file choice or the settings menu
brings the Pause menu back. In debug mode three more entries follow.

The field calls `PauseMenu_Run` (`window/screens/debug.c`), which runs the
choice: `Save_ToChosenFile`, the sleep message or `SettingsMenu_Run`.

### Saving

![Isaac presses Start in Vale and chooses `Save your quest`: a window titled Save lists three files, all `(No saved data)`; the first is chosen, `Saving...` shows, and then `Your game was saved.`](https://alchemy.pascalpixel.com/guides/window/save.gif)

The game keeps three save files in the cartridge's backup memory. Saving asks
which file, with the one last used first, and asks `OK to save over this
file?` when it already holds a game; then the saved jingle plays and the game
is written. Along with the game it writes a short summary for the file list
(the leader, level, play time and Djinn found) and where the scene's people
stand. A file is 8 KB, written as two halves of 4 KB.

`Save_ToChosenFile` (`window/screens/save.c`) does all of it; its file list,
`FileList_SelectFile`, is the same one the title's Continue, Copy and Erase
use.

| Name | Value | Meaning |
| --- | --- | --- |
| `SAVE_FILE_COUNT` | 3 | save files |
| `SAVE_RECORD_BYTES` | 4 KB | one of a file's two halves |

### The settings menu

![Isaac opens Change settings from the Pause menu: the hand turns the window colour slider, and every window turns from blue to green; it lowers the brightness, the windows darken; B drops the changes, the windows are blue again and the Pause menu returns.](https://alchemy.pascalpixel.com/guides/window/options.gif)

Change settings sets the windows' colour and brightness, the message speed,
whether people speak with voice blips, and auto-sleep. The windows change
colour as the player moves the slider, out of 24 hues and 15 steps of
brightness; Select steps through preset colours. A or Start keeps the
settings, and B puts the old ones back, which is why the menu works on a copy
of them until then.

`SettingsMenu_Run` (`window/window/options.c`) runs it, and `WindowColor_Apply`
recolours every window from a hue and a brightness.

## The name entry screen

![A new game with no save file: a keyboard window comes up under Isaac's portrait and the name `Isaac`; B takes off two letters, the hand moves to `a` and then `c` and puts them back, Start jumps to End, and A ends the name.](https://alchemy.pascalpixel.com/guides/window/naming-screen.gif)

A new game opens on the name entry screen with the leader's name already in it.
The player moves the hand over a keyboard: A puts a letter on, B or Del takes
the last one off, and End (or Start, which jumps there) keeps the name. A
name has at most five letters, and an empty one keeps the old name. Two hidden
button codes on the File Screen add more names to enter: the first Garet, Ivan
and Mia, the second Felix, Jenna and Sheba as well.

`NameEntry_EditMemberName` (`window/screens/name_entry.c`) runs it; the main
menu (`menu/file_screen.c`) calls it for each name.

| Name | Value | Meaning |
| --- | --- | --- |
| `NAME_MAX_LETTERS` | 5 | letters in a name |

## Design

[Menus, windows and shops](../../menus.md) explains why messages are unpacked
ahead of typing and why every window shares one background layer. The two
routines that work pixel by pixel or bit by bit, the canvas painter and the
Huffman decoder, are copied into the console's fast internal memory and run
from there in ARM code, the processor's full 32-bit instructions: that memory
fetches such an instruction in one read, where the cartridge takes two. The
rest runs from the cartridge as Thumb code. The work block's two maps are
one struct whose array sizes come from `tbs_edition.h`, so the code that reads
it is the same in every edition.

Many of the window functions' names are shared with
[The Lost Age](../../glossary.md#the-two-games)'s copies of these files and
with the [far-call](../../glossary.md#far-call) table `system/far_call/WINDOW.S`,
so renaming one changes other folders too.

## Limits

- The screen is 30 by 20 tiles; a frame narrower or shorter than 2 tiles, or
  wider or taller than 30, is not drawn, and nothing stops a window running
  off the bottom.
- Twelve windows at once in Japanese and eight in the international editions.
- Three text printers and 64 window sprites.
- Eight blanks at a time; a ninth is dropped.
- The text buffer holds 256 letter codes in Japanese and 512 in the others.
- The text canvas holds 120 tiles, one canvas window at a time.
- Text drawn as tiles has 128 tiles, or 256 while the field menu's screens or
  the battle's Djinn menu are open.
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
- **file list**: the list of the three save files (`FileList_SelectFile`).
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
