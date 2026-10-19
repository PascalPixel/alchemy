# Windows

This folder holds The Broken Seal's window system and the screens built on it:
the windows drawn into a shared BG tilemap, the message channels that type
text into them, the message decoder, the sprites windows carry and the icons,
and the screens made of windows: the selection row, the option and system
menus, the party status window, the save file menu, name entry, the status
screen, the battle menus and the debug menus.

See also: [Menus](../menu/README.md) and [Shops](../shop/README.md), the
screens built on these windows.

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

Each guide's comment says where to start, and each part's second line says
what it holds: `head -2 window/*.c battle_command/*.c` lists them. A part
includes its object's private header, so it resolves when opened on its own,
but it is compiled only through its guide. The ROM shows each of the two
objects as one; the parts are ours, cut where one family ends and the next
begins. Link order is in `games/tbs/objects.tsv`, from `glyph_arm`.

The unit headers are in `games/tbs/include/unit/window/`. Its
[`window.h`](../../include/unit/window/window.h) also holds what both objects
use: the UI tilemap's tiles (`UI_TILE_`), the window flags (`UI_WINDOW_`),
sprite motions (`UI_SPRITE_`), message argument kinds (`MSG_ARG_`) and the
DMA control words (`DMA_COPY32` and its kin).

## Words

- **work block**: `struct UiRenderWork`, heap slot `HEAP_SLOT_WINDOW`, reached
  as `UI_WORK` or `gWindowWork[0]`. It holds the tilemap, the window slots,
  the three message channels, the 64 render outputs and the decoded text.
- **window**: a `struct UiWindow` slot, a framed rectangle of the tilemap. It
  zooms open over 8 frames and closed over 4 unless it is `UI_WINDOW_INSTANT`.
  Its `x`, `y`, `width` and `height` are tiles on the 30 by 20 tile screen;
  offsets inside a window are pixels from its interior's top left.
- **text canvas**: a window with `UI_WINDOW_TEXT_CANVAS` draws its text into
  bitmap tiles (`UI_CANVAS_VRAM`) through `glyph_arm.c`; other windows show
  each character as a 16x16 sprite, or as a tile of the tilemap.
- **channel**: one of three `struct UiChannelSlot`, which types decoded text
  into a window a few characters a frame and runs its control codes
  (`TEXT_`): pages, pauses, colours and the speaker's voice blips.
- **message** and **decoded text**: a message is a number, written as its
  linker symbol (`MsgGameSaved`); decoded text is what it becomes in the ring
  of `work->entries` (see the design below).
- **render output**: a `struct RenderOutput`, a sprite in a window's output
  list (a character, an icon, a portrait); `UiWork_AnimateSpriteSlots` moves
  and queues each one every frame by its `UI_SPRITE_` motion.
- **slot**: a sprite tile slot of the VRAM cache, which a picture is loaded
  into (`Resource_FindFreeEntry`, `VramCache_Load`) and which
  `Resource_ResetEntry` frees.
- **tile attributes**: which text tiles the tilemap still shows, so that
  `UiText_RenderStringTiles` can reuse the others.
- `unk<offset>`, such as `unk10`: a field whose meaning is not known yet.

## Prefixes

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
| `Resource_`, `RenderResource_`, `Graphics_`, `Vram_` | things the engine names elsewhere: VRAM cache entries, pictures and video registers |

The `Ui_` functions left have no family yet, and each is also used in another
folder. Many names here are shared with The Lost Age's twins and with the
far-call tables (`system/far_call/WINDOW.S`), so a rename crosses folders.

The spellings some sites keep to match, and the comment markers, are in
[Matching](../MATCHING.md).

## Design

### From a message number to letters on the screen

Showing a message takes three steps, each owned by a different piece, so
that the slow part happens once and the typing costs little each frame.

**Decoding.** `UiText_BuildRenderEntries` ([`window/decode.c`](window/decode.c))
turns a message into decoded text all at once. Messages are Huffman coded
with one tree per previous symbol: the code just decoded picks the tree for
the next (`Text_MessageContexts`), so common pairs of letters cost few bits.
The routine that walks the trees is ARM code; it is copied into heap slot 50
for the decode and released at the end (the C form, `UiText_DecodeSymbol`,
is in the ROM but unused). As it decodes, it writes out what the message only
refers to: names, numbers, item and ability names from the argument table
(`UiWork_PushValueSlot`, `MSG_ARG_`), and the articles, possessives and
endings each language needs (`articles.c`). Style codes
(colours, pauses, page breaks) are kept with their operands for the channel.

**The ring.** Decoded text goes into `work->entries`, a ring of 16-bit codes
indexed with a mask (`RENDER_ENTRY_MASK`). Each message is written after the
last one, ends with a 0, and is known only by the index it starts at, which
`UiText_BuildRenderEntries` returns (and `-1` asks for the last one again).
Nothing owns a message's place in the ring: it stays readable until enough
later decoding wraps round onto it. That is what lets several windows show
text at once without any allocation. A message is cut off at the ring's
length. The Japanese release has a ring of 256 codes and 12 windows; every
other edition has 512 codes and 8 windows (`TBS_PATCH_1` in
`include/tbs_edition.h`), the same work block traded differently. XXX: that
the ring was doubled for the longer European text is the likely reason; the
bytes show only the change.

**Typing.** A channel (`struct UiChannelSlot`) types decoded text into one
window: it holds the window, the pen position in 1/256 pixels, the index of
the next code, a countdown to the next character and the current style.
`UiText_QueueRenderEntries` takes the first free channel and returns `NULL`
when all three are busy, so at most three messages type at any moment;
`UiText_StartBattleMessage` also reuses a channel whose window has finished,
continuing below its last line. Typing is spread over frames because it is
a task: `UiWork_ProcessAll`, the window system's scheduled routine, runs
every frame, windows first (opening and closing), then the channels, then
the sprites the windows carry, while the code that showed the message waits.
XXX: why three channels is not known.

### Windows and their pictures

A window is a framed rectangle of one shared tile map, 32 tiles wide and 20
tall (`work->tilemap`, copied to `UI_TILEMAP_VRAM`), so every window shares
one background and one set of tiles. Its text is drawn in one of three ways,
chosen by flags: into bitmap tiles of the text canvas
(`UI_WINDOW_TEXT_CANVAS`, drawn by `glyph_arm.c` from RAM), as one 16 by 16
sprite a character, or as tiles of the tile map itself. The canvas holds 120
tiles, the interior of one full-width window six tiles tall, so it serves one
window at a time; labels and icons are sprites, from the render outputs and
the sprite VRAM cache.
`work->tileAttributes` remembers which text tiles the tile map still shows,
so that redrawing a string can reuse the tiles it already has.

## Evidence and open questions

The names come from what each piece does on screen: windows, channels and
render outputs from their structs' use, the control codes from what the
decoder does with each, the screens from the English release's text they
show. The message names are linker symbols made from their English text.

Still unknown:

- XXX: why there are three channels, and why the patch traded four windows
  for a longer ring.
- XXX: what a message longer than the ring was meant to do: the decoder
  stops after one ring's length, and its terminator then lands on the
  message's own start.
- `unk` fields remain in `struct UiRenderWork` around the argument tables.

## How to change a window or a message

- A window's place and size are tiles: `UiWindow_Create(x, y, width, height,
  flags)`; a window narrower or shorter than 2 tiles, or wider or taller than
  30, draws no frame.
- A value that differs by edition, such as a window widened for longer words,
  is a named macro chosen in one `#if` block, at the top of the private header
  or of the part that uses it; change that edition's branch.
- Messages are in the PO catalogs in `games/tbs/text`. Their control codes are
  the `MSG_` codes in `window/decode.c`, and a message window sizes itself to
  its text (`UiWindow_FitOnScreen`).

## Limits

- The screen is 30 by 20 tiles; a window narrower or shorter than 2 tiles,
  or wider or taller than 30, draws no frame.
- Three message channels and 64 render outputs (`struct UiRenderWork`).
- The text canvas holds 120 tiles from tile 0x128 (`UI_CANVAS_VRAM`): the
  interior of a full-width window six tiles tall.
