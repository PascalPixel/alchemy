# Windows

The window code draws the framed boxes that carry the game's text, decodes
its compressed messages, types them into those boxes over several frames and
moves the sprites a window carries: letters, icons, portraits and the page
arrow. It also holds the screens built directly from windows: the command
icon rows, the option and system menus, the party status window, the save
file menu, name entry, the status screen, the battle menus and the debug
menus. [Menus, windows and shops](../../menus.md#windows) gives the outline
of windows and messages; this page gives the work block's layout and the
detail behind each step. The field menu's four screens are in
[Menus](../menu/README.md), the counters in [Shops](../shop/README.md).

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
motions (`UI_SPRITE_`), message argument kinds (`MSG_ARG_`) and the text
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
  window and runs its control codes (`TEXT_`).
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
| `Resource_`, `RenderResource_`, `Graphics_`, `GraphicsPalette_`, `Vram_` | things the engine names elsewhere: VRAM cache entries, pictures, palettes and video registers |

The `Ui_` functions that remain have no family yet. Many of these names are
shared with The Lost Age's twins and with the far-call table
`system/far_call/WINDOW.S`, so renaming one changes other folders too.
[Matching](../MATCHING.md) explains the spellings some sites keep and the
comment markers.

## The work block

`struct UiRenderWork` is allocated in heap slot `HEAP_SLOT_WINDOW` by
`UiWork_InitializeWithResourceCounters` for the field and by
`UiWork_Initialize` for battle, and lasts until the next scene change. In
order it holds:

- `tilemap`: the windows' tilemap, 32 by 20 tiles, uploaded in 4-row bands
  marked in `dirty`;
- `windows`: the window slots, `WINDOW_COUNT` of them;
- `channels`: the three message channels;
- `outputs` and `freeOutputs`: the pool of `UI_OUTPUT_COUNT` (64) render
  outputs and the list of those no window holds;
- `tileAttributes`, `tileCursor` and `alt`: the text tiles' bookkeeping;
- `mode`, `menuState` and the current channel's style, copied in to draw;
- `entries`: the decoded-text ring;
- `values` and `names`: the eight message arguments and their `MSG_ARG_`
  kinds;
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

## Windows

`UiWindow_Create(x, y, width, height, flags)` takes the first free slot and
places the window at tile (x, y), `width` by `height` tiles, on the 30 by 20
tile screen; offsets inside a window are pixels from its interior's top left.
Unless the window has `UI_WINDOW_INSTANT`, `UiWork_ProcessDirectWork` zooms it
open over `UI_WINDOW_OPEN_FRAMES` (8) and closed over `UI_WINDOW_CLOSE_FRAMES`
(4); the slot is freed only once the close has finished.

![Ten frames of a message window opening over Vale, left to right and down: a thin bar appears at the top of the screen and grows taller and wider frame by frame to its full size, and then the first letters, `You`, and Garet's sister's portrait follow.](https://alchemy.pascalpixel.com/guides/window/window-open.png)

`UiWindow_DrawFrame` writes the frame into the tilemap and draws nothing for a
width or height below 2 or above 30. Nothing checks that a window stays within
the tilemap's 20 rows; the caller keeps it there. While `mode` is set every
corner is the one round tile (`UI_TILE_FRAME_ROUND_CORNER`), flipped to fit.

A message window sizes itself to its text: `UiWindow_FitOnScreen`
(`window/measure.c`) measures the message and moves the window so that it
fits on the screen. While `mode` is set the fitted window is 2 tiles wider;
which of the measuring routines add those tiles differs by edition, and in
Japanese and English the text also starts 8 pixels further in.

Where the editions lay a screen out differently, the value is a named macro
chosen in one `#if` block at the top of the private header or of the part.
The battle summon list, for one, is `SUMMON_MENU_WINDOW_WIDTH` 17 tiles wide
at `SUMMON_MENU_WINDOW_X` 13 outside Japan, and 14 tiles wide at 16 in
Japanese, so both end at the screen's right edge.

## Decoding a message

The game's text is stored as numbered messages, written in the code by their
linker symbols (`MsgGameSaved`); their text is in the PO catalogs under
`games/tbs/text`, one per language. `UiText_BuildRenderEntries`
([`window/decode.c`](window/decode.c)) decodes a whole message at once into
the ring.

Messages are Huffman-coded with a separate tree for each previous symbol
(`Text_MessageContexts`), so the code just decoded chooses the tree for the
next. The routine that walks the trees, `Text_DecodeSymbol`, is handwritten
ARM in `graphics/text/decode_symbol.s`; the decoder copies it into heap slot
`TEXT_WORK_BLOCK` for the call, runs it from there one symbol at a time and
releases the slot at the end.

The decoder writes out everything the message refers to. An argument code
takes its value from the argument table, which callers fill with
`UiWork_PushValueSlot(value, kind)`: a name, a number, or an item or ability
name, by its `MSG_ARG_` kind. Each language's articles, possessives and
endings come from `articles.c`, chosen by the article class an item or name
carries. Style codes (colours, pauses, page breaks) are kept with their
operands for the channel to run. A control code in the catalogs is written in
braces: `{end_wait}`, at the end of `Your game was saved.{end_wait}`, is
`MSG_END_WAIT`, kept as `TEXT_WAIT_KEY`, which waits for a button and closes
the window.

While a battle message is decoded `menuState` is 2, and the Japanese decoder
then sets names off with spaces and style codes and doubles each space.

## The decoded-text ring

`work->entries` is a ring of 16-bit codes indexed through `RENDER_ENTRY_MASK`.
A message is written at `work->count`, ends with 0, and is known only by the
index it starts at, which `UiText_BuildRenderEntries` returns; asking for
message -1 returns the last message's start again. No space is reserved, so
later decoding that wraps round overwrites the message.
A message longer than the ring is cut at the ring's length.

## Typing

A channel holds its window, the pen position in 1/256 pixels, the index of
its next code, the countdowns for pauses and for the wait after each
character, and its style. `UiText_QueueRenderEntries` takes a free channel and
returns `NULL` when all three are busy; the channel stays taken until its
window closes. `UiText_StartBattleMessage` also reuses a channel whose window
has finished, continuing below its last line and scrolling once both lines
are used.

`UiWork_ProcessAll` is the window system's task. Both setups schedule it in
the main loop's pass, and it runs every frame: the windows' zoom
(`UiWork_ProcessDirectWork`), then the channels, then the render outputs
(`UiWork_AnimateSpriteSlots`). Code that shows a message waits on its window
while the task types it.

`UiWork_StepChannelScript` types one frame's share of a channel. Outside
battle the share is `gUiText_SpeedSteps` for the player's message speed: one
character at the slow and normal speeds, ten at the fast. At the slow speed
the channel also waits `gUiText_SpeedDelays` 4 frames after each character
while no button is held, and a space or a control code ends a frame's share
at any speed. In battle (`menuState` nonzero) the share is 3 characters plus 5
for each VBlank the last frame took, counting at most two: 8 for a frame on
time and 13 after a late one. The speaker's voice blip plays every
`gUiText_SpeedCueGaps` characters (1, 3 or 10), and is one of four sounds
from `speakerVoiceSound`, chosen by the character's code.

![Isaac talks to Garet's sister in Vale: a message window zooms open at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out with a bobbing arrow at its end, the window narrows to fit `I wish I could go with you.`, and it closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

## Drawing text

All windows share the one tilemap, so they share one background and one set
of tiles. A window's text is drawn in one of three ways, chosen by its flags.

A text canvas draws into bitmap tiles at `UI_CANVAS_VRAM`, through
`glyph_arm.c`. The canvas is 120 tiles from
`UI_CANVAS_FIRST_TILE`, enough for the interior of a full-width window six
tiles tall, and its tiles run down columns. Every canvas window maps the same
tiles, so one is shown at a time. Canvas lines are 13 pixels apart, and from
the second line on the canvas scrolls up instead.

Other windows show each character as a 16 by 16 sprite, a render output, or
as tiles of the tilemap. Text drawn as tiles takes tiles `0x80` to `0xFF`, and
`0x200` to `0x27F` as well while `alt` is set; their lines are 15 pixels
apart. A tile's attribute has bit 0 once allocated and bit 1 while the
tilemap shows it, so `UiText_RenderStringTiles` can reuse the tiles a string
already has and take only free ones.

While `mode` is set the text is drawn in colour 8 with no shadow, and a
button press latched before a page's wait is dropped.

![Isaac casts Mind Read on Garet's sister in Vale: the field tints red, then gold and green, as her thought `Gee, I wish I could see new lands, too.` is typed into a round-cornered window beside her portrait.](https://alchemy.pascalpixel.com/abilities/mind-read.gif)

## Render outputs

A render output is one sprite a window carries: a letter, an icon, a portrait
or the page arrow. Each window keeps a list of its outputs, taken from the
pool's free list and returned when the window closes.
`UiWork_AnimateSpriteSlots` moves each output every frame by its `UI_SPRITE_`
motion and queues it for drawing. An output's picture occupies an entry of the
VRAM cache, loaded into a free one (`Resource_FindFreeEntry`,
`VramCache_Load`) and freed by `Resource_ResetEntry`. The two portrait slots
record which picture each shows (`resourceIds`, 999 when free) and its cache
entry.

## The area name

`UiTimedNotice_Create` shows the area's name, from
`Scene_GetAreaNameIndexFar`, in a window centred across the screen and in its
top 10 rows, opened at once. It keeps the window and a countdown of
`UI_AREA_NOTICE_FRAMES` (90) in the event work, and `UiTimedNotice_Tick`
closes it when the countdown ends.

## Design

[Menus, windows and shops](../../menus.md) explains why messages are decoded
ahead of typing and why every window shares one tilemap. Within the window
code, the two routines that run per pixel or per bit, the canvas renderer and
the tree walker, are ARM code copied into RAM, while everything else is Thumb
code run from ROM. XXX: likely for speed, because ARM code fetched from the
cartridge's 16-bit bus is slow; nothing in the code says so. The work block's
two layouts are one struct whose array sizes come from `tbs_edition.h`, so the
code that reads it is the same in every edition. XXX: the likely reason the
ring was doubled is the longer European text; the bytes show only the change.

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

## Evidence and open questions

Windows, channels and render outputs are named from how their structs are
used, the control codes from what the decoder and the channel do with each,
and the screens from the English release's text they show. Messages are
named by their linker symbols, made from their English text.

- XXX: why there are three channels, and why the international editions gave
  up four windows when they doubled the ring.
- XXX: what a message longer than the ring was meant to do: the decoder stops
  after one ring's length, and its terminator then lands on the message's own
  start.
- XXX: `unknownBeforeValues` in `struct UiRenderWork`, the bytes before the
  argument table, which the TBS code never names.
- XXX (picture): no picture yet takes the window layer apart: the shared tilemap drawn
  alone beside the canvas tiles and the render outputs of one screen, which
  `alchemy-devtools render` could show from a tbs-en build.
