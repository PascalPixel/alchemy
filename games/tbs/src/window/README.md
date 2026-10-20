# Windows

Nearly everything you read in The Broken Seal comes to you in a window: a
villager's line, a battle message, the save screen, the name entry keyboard.
The windows are framed boxes drawn into one background layer that the whole
game shares. The code here draws them, decodes the game's compressed
messages, types them out a few letters a frame, and moves the little sprites
that ride along with a window: letters, icons, portraits and the bobbing
arrow at the end of a page.

Many of the screens made of windows live here too: the row of command icons,
the option and system menus, the party status window, the save file menu,
name entry, the status screen, the battle menus and the debug menus. The
[Menus](../menu/README.md) and [Shops](../shop/README.md) guides cover the
other screens built from the same windows.

## Files

| File | What it holds |
| --- | --- |
| `window.c` | the guide to the window object: it includes `window_internal.h`, then the parts in `window/` in ROM order |
| `battle_command.c` | the guide to the menu object, from the party status window to the debug menus: it includes `battle_command_internal.h`, then the parts in `battle_command/` in ROM order |
| `glyph_arm.c` | the text canvas renderer, compiled as ARM code and copied to RAM to run |
| `articles.c` | each edition's articles and possessives, which the decoder writes before names |
| `scales.c` | sprite scales, motion curves and the window colour wheel |
| `text_speed.c` | the message speed tables |
| `windata.s`, [`keyboard.s`](keyboard.s) | glyph widths, text colours, the party status columns, the name entry keyboards |
| [`cursor.s`](cursor.s), `cursor_frame.s`, `fixed_block.s`, `slider.s`, `slidpal.s` | sprite tiles and palettes, built from the PNGs beside them or under `games/tbs/src/menu` and `games/tbs/src/graphics` |

In the ROM, the window code and the menu code are each one long object. Both
are too big to read as a single file, so `window.c` and `battle_command.c`
are split here into smaller files, cut where one family of functions ends and
the next begins. The split is just for reading: Camelot's own files aren't
known. The comment at the top of `window.c` or `battle_command.c` says where
to start, and the second line of every smaller file says what's in it, so
`head -2 window/*.c battle_command/*.c` gives you the whole map. Each of those
files includes its object's private header, so it makes sense opened on its
own, but it's only ever compiled through `window.c` or `battle_command.c`.
The link order is in `games/tbs/objects.tsv`, from `glyph_arm`.

The unit headers are in `games/tbs/include/unit/window/`. Its
[`window.h`](../../include/unit/window/window.h) also holds what both objects
use: the UI tilemap's tiles (`UI_TILE_`), the window flags (`UI_WINDOW_`),
sprite motions (`UI_SPRITE_`), message argument kinds (`MSG_ARG_`) and the
DMA control words (`DMA_COPY32` and its kin).

## The pieces of a window system

All of the window system's state lives in one **work block**, `struct
UiRenderWork`, which sits in heap slot `HEAP_SLOT_WINDOW` (one of the numbered
blocks of memory the game hands out to its systems) and is reached as
`UI_WORK` or `gWindowWork[0]`. In it are the tilemap, the window slots, the
three message channels, the 64 render outputs and the decoded text.

Each box you see on screen is a **window**, a `struct UiWindow` slot: a
framed rectangle of the tilemap. It zooms open over 8 frames and closed over
4, unless it's `UI_WINDOW_INSTANT`. Its `x`, `y`, `width` and `height` count
8-pixel tiles on the 30 by 20 tile screen (240 by 160 pixels), while offsets
inside a window are pixels from its interior's top left.

A window with `UI_WINDOW_TEXT_CANVAS` is a **text canvas**, which draws its
letters into bitmap tiles (`UI_CANVAS_VRAM`) through `glyph_arm.c`. Other
windows show each character as a 16 by 16 sprite, or as a tile of the
tilemap.

The game's text is stored as numbered **messages**, each written in the
code as its linker symbol (`MsgGameSaved`). Before it can be shown, a
message is turned into **decoded text** in the ring of `work->entries`,
explained below, and then one of three **channels** (`struct
UiChannelSlot`) types that text into a window a few characters a frame,
running its control codes (`TEXT_`) as it goes: pages, pauses, colours and
the speaker's voice blips.

The sprites a window carries, its letters, icons and portraits, are its
**render outputs** (`struct RenderOutput`), kept in a list per window.
`UiWork_AnimateSpriteSlots` moves each one every frame by its `UI_SPRITE_`
motion and queues it for drawing. Their pictures sit in **slots** of the VRAM
cache (VRAM being the GBA's video memory): a picture is loaded into a free
slot (`Resource_FindFreeEntry`, `VramCache_Load`) and `Resource_ResetEntry`
frees it again.

Text drawn as tiles of the tilemap is tracked by the **tile attributes**,
which record which text tiles the tilemap still shows, so that
`UiText_RenderStringTiles` can reuse the others.

You'll also meet fields named `unk` plus their offset, such as `unk10`:
those are the ones whose meaning isn't known yet.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `UiWork_` | the work block and its per-frame updates |
| `UiWindow_` | a window's rectangle of the tilemap: frames, fills, dividers, tiles |
| `UiText_` | messages and text: decoding, measuring and drawing |
| `RenderOutput_` | the render output pool and a window's output list |
| `UiIcon_`, `ItemIcon_`, `UiGlyph_` | icons built into sprite tiles |
| `SelectionScreen_`, `Menu_` | the selection row, and the menus built from command icons |
| `OptionMenu_`, `SystemMenu_`, `WindowColor_`, `PaletteGlow_` | the option and system menus and the window colours |
| `SaveState_`, `SaveMenu_` | save files and the save file menu |
| `NameEntry_`, `PartyTalkMenu_`, `DjinnMenu_`, `SummonMenu_`, `ItemList_`, `BattleMenu_`, `BattleTarget_` | the screens they name |
| `UiTimedNotice_` | the notice that shows an area's name near the top of the screen for 90 frames |
| `DebugMenu_` | the debug menus |
| `Resource_`, `RenderResource_`, `Graphics_`, `Vram_` | things the engine names elsewhere: VRAM cache entries, pictures and video registers |

The `Ui_` functions that are left have no family yet, and each is also used
in another folder. Many names here are shared with The Lost Age's twins and
with the far-call tables (`system/far_call/WINDOW.S`), so renaming one means
changing other folders too.

The spellings some sites keep so that they match, and the comment markers,
are explained in [Matching](../MATCHING.md).

## How a message reaches the screen

### From a message number to letters

Showing a message takes three steps, each done by a different piece of code,
so that the slow part happens once and the typing costs little each frame.

**Decoding.** `UiText_BuildRenderEntries` ([`window/decode.c`](window/decode.c))
turns a message into decoded text all at once. Messages are stored compressed
as Huffman codes, where common symbols get short runs of bits, and the game
keeps a separate tree for each previous symbol: the code just decoded picks
the tree for the next (`Text_MessageContexts`), so common pairs of letters
cost few bits. The routine that walks the trees is ARM code; it's copied into
heap slot 50 for the decode and released at the end (the C form,
`UiText_DecodeSymbol`, is in the ROM but unused).

As it decodes, it writes out everything the message only refers to: names,
numbers, and item and ability names from the argument table
(`UiWork_PushValueSlot`, `MSG_ARG_`), and the articles, possessives and
endings each language needs (`articles.c`). Style codes (colours, pauses,
page breaks) are kept with their operands for the channel to run.

**The ring.** Decoded text goes into `work->entries`, a ring of 16-bit codes
indexed with a mask (`RENDER_ENTRY_MASK`), so that counting past the end
wraps round to the start. Each message is written after the last one, ends
with a 0, and is known only by the index it starts at, which
`UiText_BuildRenderEntries` returns (asking for message -1 gives back the
last one's start). Nothing reserves a message's place in the ring: it stays
readable until enough later decoding wraps round onto it. That's what lets
several windows show text at once without any allocation. A message is cut
off at the ring's length.

The Japanese release has a ring of 256 codes and 12 windows; every other
edition has 512 codes and 8 windows (`TBS_PATCH_1`, with the two layouts in
`include/tbs_edition.h`). Giving up four windows saves 144 bytes of the 512
the longer ring costs, so the work block grows by 368 bytes, from 4,492 to
4,860. XXX: the likely reason the ring was doubled is the longer European
text; the bytes show only the change.

**Typing.** A channel (`struct UiChannelSlot`) types decoded text into one
window. It holds the window, the pen position in 1/256 pixels, the index of
the next code, countdowns for pauses and for the wait after each character,
and the current style. `UiText_QueueRenderEntries` takes a free channel and
returns `NULL` when all three are busy, so at most three messages type at any
moment; `UiText_StartBattleMessage` also reuses a channel whose window has
finished, carrying on below its last line.

Typing gets spread over frames by the task system. `UiWork_ProcessAll`, the
window system's scheduled routine, runs once every frame: windows first (opening
and closing), then the channels, then the sprites the windows carry, while the
code that showed the message waits. Outside battle a channel types as many
characters a frame as the player's message speed allows (`text_speed.c`): one
at Slow and Normal and ten at Fast, with Slow also waiting four frames after
each character while no button is held. A space or a control code ends a
frame's share. In battle it's 8 a frame, or 13 after a frame that took two or
more screen refreshes (VBlanks). The speaker's voice blip, one of four sounds,
plays every few characters.

### Windows and their pictures

Every window is a framed rectangle of one shared tilemap, 32 tiles wide and 20
tall (`work->tilemap`, copied to `UI_TILEMAP_VRAM`), so all the windows share
one background and one set of tiles. A window's text is drawn in one of three
ways, chosen by its flags: into the bitmap tiles of the text canvas
(`UI_WINDOW_TEXT_CANVAS`, drawn by `glyph_arm.c` from RAM), as one 16 by 16
sprite a character, or as tiles of the tilemap itself.

The canvas holds 120 tiles, exactly the battle message window's text area:
the bottom four rows of the screen, all 30 columns (that window is opened
with no frame, as a bare canvas). So it serves one window at a time. Labels
and icons are sprites, from the render outputs and the sprite VRAM cache.
Text drawn as tiles takes them from tiles 0x80 to 0xff, 128 of them, or
256 when the work block's `alt` adds 0x200 to 0x27f, and
`work->tileAttributes` remembers which of those the tilemap still shows, so
that redrawing a string can reuse the tiles it already has.

While Mind Read shows someone's thoughts, the work block's `mode` is set:
every window corner becomes the one round tile, the text is drawn in colour 8
with no shadow, and in Japanese and English the message window is made 2
tiles wider.

### Window sizes and message text

A window's place and size are counted in tiles: `UiWindow_Create(x, y, width,
height, flags)` opens one at tile (x, y), width by height tiles, in the first
free window slot. A window narrower or shorter than 2 tiles, or wider or
taller than 30, draws no frame, and nothing checks that a window stays within
the tilemap's 20 rows.

Where the editions lay a screen out differently, such as a window widened for
longer words, the value is a named macro chosen in one `#if` block, at the top
of the private header or of the part that uses it. The battle summon list,
for one, is 17 tiles wide in every edition after the Japanese and 14 tiles in
Japanese, where it starts three tiles further right, so both reach the
screen's right edge (`SUMMON_MENU_WINDOW_WIDTH`, `SUMMON_MENU_WINDOW_X`).

The messages themselves are in the PO catalogs in `games/tbs/text`, one per
language. In `en.po`, message 23 reads `Your game was saved.{end_wait}`. The
name in braces is a control code, one of the `MSG_` codes in
`window/decode.c`: `{end_wait}` is `MSG_END_WAIT`, which waits for a button
and then closes the window. The code calls that message `MsgGameSaved`, the
linker symbol made from its English text. A message window sizes itself to its
text (`UiWindow_FitOnScreen`), measuring it and moving the window so that it
fits on the screen.

## Limits

- The screen is 30 by 20 tiles; a window narrower or shorter than 2 tiles, or
  wider or taller than 30, draws no frame.
- Three message channels and 64 render outputs (`struct UiRenderWork`).
- Twelve windows in Japanese and eight in the other editions.
- Eight message arguments at a time; another is dropped
  (`UiWork_PushValueSlot`).
- The text canvas holds 120 tiles from tile 0x128 (`UI_CANVAS_VRAM`): the
  bottom four rows of the screen, all 30 columns, where the battle message
  window draws.
- Text drawn as tiles has 128 tiles, or 256 with `alt` set.

## Evidence and open questions

The names come from what each piece does on screen: windows, channels and
render outputs from how their structs are used, the control codes from what
the decoder does with each, and the screens from the English release's text
they show. The message names are linker symbols made from their English
text.

Still unknown:

- XXX: why there are three channels, and why the international editions gave
  up four windows when they doubled the ring.
- XXX: what a message longer than the ring was meant to do: the decoder stops
  after one ring's length, and its terminator then lands on the message's own
  start.
- `unk` fields remain in `struct UiRenderWork` around the argument tables.
