# Windows and menus

This folder holds The Broken Seal's window system and the screens built on it:
the windows drawn into a shared BG tilemap, the message channels that type
text into them, the message decoder, the sprites windows carry and the icons,
and the screens made of windows: the selection row, the option and system
menus, the party status window, the save file menu, name entry, the status
screen, the battle menus and the debug menus.

## Files

| File | What it holds |
| --- | --- |
| `window.c` | the guide to the window object: it includes `window_internal.h`, then the parts in `window/` in ROM order |
| `battle_command.c` | the guide to the menu object, from the party status window to the debug menus: it includes `battle_command_internal.h`, then the parts in `battle_command/` in ROM order |
| `glyph_arm.c` | the text canvas renderer, compiled as ARM code and copied to RAM to run |
| `articles.c` | each edition's articles and possessives, which the decoder writes before names |
| `scales.c` | sprite scales, motion curves and the window colour wheel |
| `text_speed.c` | the message speed tables |
| `windata.s`, `keyboard.s` | glyph widths, text colours, the party status columns, the name entry keyboards |
| `cursor.s`, `cursor_frame.s`, `fixed_block.s`, `slider.s`, `slidpal.s` | sprite tiles and palettes, built from the PNGs beside them or under `games/tbs/src/menu` and `games/tbs/src/graphics` |

Each guide's comment says where to start, and each part's second line says
what it holds: `head -2 window/*.c battle_command/*.c` lists them. A part
includes its object's private header, so it resolves when opened on its own,
but it is compiled only through its guide. The ROM shows each of the two
objects as one; the parts are ours, cut where one family ends and the next
begins. Link order is in `games/tbs/objects.tsv`, from `glyph_arm`.

The unit headers are in `games/tbs/include/unit/window/`. Its `window.h` also
holds what both objects use: the UI tilemap's tiles (`UI_TILE_`), the window
flags (`UI_WINDOW_`), sprite motions (`UI_SPRITE_`), message argument kinds
(`MSG_ARG_`) and the DMA control words (`DMA_COPY32` and its kin).

## Words used here

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
  linker symbol (`MsgGameSaved`). `UiText_BuildRenderEntries` decodes its
  Huffman-coded script into `work->entries`, a ring of 16-bit codes, writing
  out names, numbers and articles from the argument table
  (`UiWork_PushValueSlot`, `MSG_ARG_`).
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

## Spellings

Each site keeps the form it matches with. These pairs are one idea and can
compile differently:

- The work block as `UI_WORK` or as `gWindowWork[0]`, and a neighbouring heap
  block as `gWindowWork[n]`.
- A tilemap cell as `work->tilemap + y * 32 + x`, or as a byte offset added to
  `(u32)work->tilemap`.
- Palette RAM through `BG_PLTT_COLOR`, which is volatile, or through a plain
  pointer to it such as `(u16 *)&BG_PLTT_COLOR(14, 0)`.
- A function whose result nothing reads keeps its `s32` return where `void`
  changes its epilogue; `UBFIX` gives the `void` form.

## Changing a window or a message

- A window's place and size are tiles: `UiWindow_Create(x, y, width, height,
  flags)`; a window narrower or shorter than 2 tiles, or wider or taller than
  30, draws no frame.
- A value that differs by edition, such as a window widened for longer words,
  is a named macro chosen in one `#if` block, at the top of the private header
  or of the part that uses it; change that edition's branch.
- Messages are in the PO catalogs in `games/tbs/text`. Their control codes are
  the `MSG_` codes in `window/decode.c`, and a message window sizes itself to
  its text (`UiWindow_FitOnScreen`).
