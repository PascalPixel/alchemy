#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/object/object.h"
#include "unit/window/glyph_arm.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "battle_work.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "battle_party.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "battle_type.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resource_menu.h"
#include "battle_summon.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "display_transition.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/scales.h"

// OAM attribute bits a sprite output is created with: the size field of
// attribute 1 (bits 30-31 of the first word). A square sprite of size 1 is
// 16x16 pixels, of size 2 32x32.
#define OUTPUT_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32 0x80000000

// The two portrait slots of the UI work, and the palette the first one uses
// (the second uses the next).
#define PORTRAIT_SLOT_COUNT 2
#define PORTRAIT_PALETTE 14
#define PORTRAIT_SLOT_EMPTY 999

// Creates a sprite output for the VRAM cache slot `slot` and attaches it to
// `window`, placed at (offsetX, offsetY) pixels from the window's inner
// corner (one tile in from its top-left border). `oamFlags` holds the OAM
// shape and size bits. The sprite draws on layer 255 until the caller changes
// it. Returns the output, or NULL with the VRAM slot released when all
// outputs are in use.
struct RenderOutput *RenderOutput_Create(s32 slot, s32 oamFlags,
                                         struct UiWindow *window, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct RenderOutput *output;
  s32 y;

  output = RenderOutput_AcquireFree();
  if (output == NULL) {
    Resource_ResetEntry(slot);
    return NULL;
  }

  // Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y.
  x = offsetX + (window->x * 8) + 8;
  y = offsetY + (window->y * 8) + 8;
  x &= 0x1ff;
  y &= 0xff;
  *(u32 *)&output->oam = (x << 16) | y | oamFlags;
  // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
  ((u32 *)&output->oam)[1] =
      ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
  output->sentinel = 0xff;
  output->next = NULL;
  output->x = x;
  output->y = y;
  output->index = slot;
  output->kind = 1;
  output->active = 1;
  RenderOutput_AppendToList(window, output);
  return output;
}

// Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
// it as a 16x16 sprite at (x, y) in `window`. Returns NULL when it cannot be
// loaded.
struct RenderOutput *RenderOutput_CreateFromResource(s32 mode, s32 resource,
                                                     struct UiWindow *window,
                                                     s32 x, s32 y) {
  s32 slot;

  slot = Resource_LoadByMode(mode, resource);
  if (slot < 0) {
    return NULL;
  }
  return RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
}

// Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
// slot and shows it as a 16x16 sprite at (x, y) in `window`. Returns NULL
// when no slot is free.
struct RenderOutput *RenderOutput_CreateLoaded(s32 icon, s32 layers,
                                               struct UiWindow *window, s32 x,
                                               s32 y) {
  s32 slot;
  struct RenderOutput *output;

  slot = Resource_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    UiIcon_CopyResourceToSlot(icon, layers, slot);
    output = RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return output;
}

// Builds a 16x16 icon from frame pattern 1 with overlay pattern `icon` drawn
// over it, and shows it at (x, y) in `window` on draw layer 251. Returns NULL
// when no VRAM slot is free.
struct RenderOutput *RenderOutput_CreateWithTransform(s32 icon,
                                                      struct UiWindow *window,
                                                      s32 x, s32 y) {
  s32 slot;
  s32 unusedTile;
  struct RenderOutput *output;

  slot = Resource_FindFreeEntry();
  if (slot == VRAM_CACHE_ENTRY_COUNT) {
    return NULL;
  }
  Ui_BuildPairedPatternsToSlot(icon, 1, &slot, &unusedTile, 1);
  output = RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  output->sentinel = 251;
  return output;
}

// Loads the pattern at the start of gRenderResource_PairSourceTable into a
// free VRAM slot and shows it as a 16x16 sprite at (x, y) in `window`.
// `tableEntry` is passed on to the loader, which loads that same pattern for
// every value. Returns NULL when no slot is free.
struct RenderOutput *RenderOutput_CreateFromTable(s32 tableEntry,
                                                  struct UiWindow *window,
                                                  s32 x, s32 y) {
  s32 slot;
  struct RenderOutput *output;

  slot = Resource_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    RenderResource_LoadTableEntry(tableEntry, 0, slot);
    output = RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return output;
}

// Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
// slot `side` (0 or 1, or any other value for the first free one; the second
// is tried first). Each slot has its own palette, 14 or 15. Once game flag 32
// is set, characters 0 and 1 show portraits 18 and 19 instead. `unused` is
// passed on to the loader, which ignores it. Returns the sprite, or NULL when
// the character has no portrait or both slots are taken.
struct RenderOutput *CreateSideObject(s32 character, s32 unused, s32 side,
                                      struct UiWindow *window, s32 x, s32 y) {
  struct UiRenderWork *work = UI_WORK;
  struct RenderOutput *portrait = NULL;
  s32 slot;
  s32 tile;
  s32 icon;
  s32 palette;

  if (GameFlag_TestFar(32) != 0) {
    if (character == 0)
      character = 18;
    if (character == 1)
      character = 19;
  }

  icon = Localization_LookupEntryId(character);
  if (icon == -1)
    return portrait;

  if ((u32)side > 1) {
    side = 1;
    if (work->resourceIds[1] != PORTRAIT_SLOT_EMPTY) {
      side = 0;
      if (work->resourceIds[0] != PORTRAIT_SLOT_EMPTY)
        return portrait;
    }
  }

  palette = PORTRAIT_PALETTE + side;
  UiGlyph_LoadEntryWithPalette(icon, unused, &slot, &tile, palette, 0);
  portrait = RenderOutput_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
  if (portrait != NULL) {
    portrait->oam.palette = palette;
    portrait->kind = 2;
  }

  work->resourceIds[side] = icon;
  work->outputIds[side] = slot;
  return portrait;
}

// Reloads the portrait image of `character` into portrait slot `side` (0 or
// 1), keeping its sprite: used to change the picture of a portrait already on
// screen. With any other `side`, the slot already showing that character's
// portrait is used, and nothing happens when none does. `unused` is passed on
// to the loader, which ignores it.
void Ui_LoadCharacterEntryForSlot(u32 side, s32 character, s32 unused) {
  s32 tile;
  s32 slot;
  u32 icon;
  struct UiRenderWork *work;

  work = UI_WORK;

  if (GameFlag_TestFar(32) != 0) {
    if (character == 0)
      character = 18;
    if (character == 1)
      character = 19;
  }

  icon = Localization_LookupEntryId(character);
  if (icon != -1U) {
    if (side > 1U) {
      if (work->resourceIds[1] == icon) {
        side = 1;
      } else if (work->resourceIds[0] == icon) {
        side = 0;
      } else {
        return;
      }
    }
    slot = work->outputIds[side];
    UiGlyph_LoadEntryWithPalette(icon, unused, &slot, &tile,
                                 PORTRAIT_PALETTE + side, 1);
  }
}

// Switches the sprite output `out` to animation `mode` from its first step.
// Does nothing when `out` is NULL.
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode) {
  if (out != NULL) {
    out->active = mode;
    out->animStep = 0;
  }
}

// Sets the draw layer of the sprite output `output` to 255 - `priority`, so a
// higher priority draws on a lower layer. Does nothing when `output` is NULL.
void RenderOutput_SetInvertedSentinel(void *output, s32 priority) {
  if (output != NULL)
    ((struct RenderOutput *)output)->sentinel = ~priority;
}

typedef void (*WindowSpanRoutine)(u16 *spans, u16 *tilemap);

// Fills `spans` with the horizontal extent of the windows on each of the 20
// tile rows, read from the UI tilemap by the ARM routine Tile_BuildMetatiles
// copied into RAM. With no UI work, every row is filled with 0xe0e0 (no
// window).
void UiWindow_FillFromScene(void *spans) {
  void *tilemap = gWindowWork[0];

  if (tilemap == NULL) {
    Dma_Fill16(0xe0e0, spans, 0x140);
  } else {
    WindowSpanRoutine routine;

    RamCode_Load(routine, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
    routine(spans, tilemap);
    MemFree(routine);
  }
}

// Fills the top-left `width` x `height` tiles of the 32-tile-wide BG screen
// block at 0x06002000 with the tilemap entry `value`. The first two
// parameters are ignored.
void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width,
                                  u32 height, s32 value) {
  u32 row = 0;
  s16 *dst = (s16 *)0x06002000;

  if (row < height) {
    do {
      u32 column = 0;
      if (column < width) {
        do {
          column++;
          *dst = value;
          dst++;
        } while (column < width);
      }
      row++;
      dst += 32 - width;
    } while (row < height);
  }
}

// The party status window: one column of six tiles per party member (name,
// HP and, when shown, PP with a bar under each), and in battle an optional
// five-tile column of standby Djinn counts on its left. It sits at the top
// right of the screen. Its flags:
#define PARTY_STATUS_SHOW_PP 1     // add the PP row
#define PARTY_STATUS_SHOW_DJINN 2  // add the Djinn column (needs the PP row)
#define PARTY_STATUS_CLOSED 9      // erase the window
#define PARTY_STATUS_KEEP_FLAGS -1 // redraw with the flags it has
#define PARTY_STATUS_COLUMN_WIDTH 6
#define PARTY_STATUS_DJINN_WIDTH 5

// The heap block the party status window's layout lives in.
#define PARTY_STATUS_BLOCK 16

// The address of pixel row 7 - `up` of BG tile `tile` (4bpp, 32 bytes a tile,
// 4 bytes a row): the status bars are drawn from a tile's bottom row up.
#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

// Works out where the party status window goes for `flags`: one column per
// party member (those in battle when a battle is on), plus the Djinn column,
// right-aligned on the 30-tile screen row at the top. It is four tiles high
// outside battle and three in battle, plus one with the PP row.
void UiWindow_BuildLayoutBounds(s32 flags) {
  // The layout and the UI work are neighbouring work slots.
  void **slot = (void **)&gPartyStatusWork;
  struct PartyStatusWindow *layout = *slot;
  struct UiRenderWork *work = slot[-1];
  s32 height = 4;
  s32 members;
  s32 width;
  s32 x;

  if (work->menuState != 0) {
    members = BattleParty_PrepareActiveOwnersFar(0);
    height = 3;
  } else {
    members = Party_CountActiveOwnersFar();
  }
  if (flags & PARTY_STATUS_SHOW_PP)
    height++;
  else
    flags &= ~PARTY_STATUS_SHOW_DJINN;

  members *= PARTY_STATUS_COLUMN_WIDTH;
  width = members + 1;
  if (flags & PARTY_STATUS_SHOW_DJINN)
    width += PARTY_STATUS_DJINN_WIDTH;

  x = 30;
  x -= width;
  layout->x = x;
  layout->y = 0;
  layout->width = width;
  layout->height = height;
  layout->flags = flags;
}

// Opens the party status window with `flags` (see PARTY_STATUS_*) in a new
// heap block and draws it. The UI is held busy while it is drawn.
void UiWindow_CreateWithLayoutBounds(s32 flags) {
  s32 zero;
  struct PartyStatusWindow *layout;
  s8 *busy;

  layout = Runtime_AllocateBlock(PARTY_STATUS_BLOCK, sizeof(*layout));
  busy = (s8 *)&UI_WORK->menuBusy;
  zero = 0;
  *busy = 1;
  UiWindow_BuildLayoutBounds(flags);
  layout->window =
      UiWindow_Create(layout->x, layout->y, layout->width, layout->height, 6);
  PartyStatusDraw(flags);
  *busy = zero;
}

// Draws the vertical lines between the members' columns of the party status
// `window`, with their top and bottom joints, at the offsets listed in
// gUiWindow_PartyColumnOffsets (shifted right past the Djinn column, whose own
// line comes first in the list). In battle it also draws the window's bottom
// edge.
void UiWindow_DrawColumnBorders(struct UiWindow *window, u32 flags) {
  struct UiRenderWork *work = UI_WORK;
  s32 first = 1;
  s32 shift = 0;
  u32 lastColumn = window->width - 1;
  s32 i;
  u32 rows = window->height;
  u32 column;
  u32 row;
  u16 *cell;

  if ((flags & PARTY_STATUS_SHOW_PP) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags & PARTY_STATUS_SHOW_DJINN) {
    shift = PARTY_STATUS_DJINN_WIDTH;
    first = 0;
  }
  i = first;
  while (gUiWindow_PartyColumnOffsets[i] >= 0) {
    column = gUiWindow_PartyColumnOffsets[i] + shift;
    if (column < lastColumn) {
      for (row = 0; row != rows; row++) {
        cell = (u16 *)(((window->y + row) * 32 + (window->x + column)) * 2 +
                       (u32)work->tilemap);
        if (row == 0)
          *cell = 0xf018; // joint with the top edge
        else if (row == rows - 1)
          *cell = 0xf019; // joint with the bottom edge
        else
          *cell = 0xf00f; // line
      }
    }
    i++;
  }
  if (work->menuState) {
    cell = work->tilemap + (window->y + window->height - 1) * 32 + window->x;
    column = 1;
    *cell++ = 0xf080;
    while (column < lastColumn) {
      column++;
      *cell++ = 0xf081;
    }
    *cell = 0xf082;
  }
  work->dirty = 1;
}

// Recolours the five bar tiles at (x, y) in `window` to show a bar `length`
// pixels long (0-40). The bar tiles are drawn in colour 14 with a colour 1
// shadow; each pixel left of the length becomes colour 8 (shadow 13), the
// rest colour 2 (shadow 12). Only the bottom row of each tile changes for an
// empty bar, the bottom three otherwise. Outside battle it first loads the
// basic colours into BG palette 14, its colour 14 taken from colour 4 of
// palette 15.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                 s32 length) {
#else
s32 UiWindow_DrawStatusBarTiles(struct UiWindow *window, s32 x, s32 y,
                                s32 length) {
#endif
  s32 i;
  struct UiRenderWork *work = UI_WORK;
  s32 fullLength = length;
  s32 row;
  s32 column;
  u32 tile;
  u32 bar;
  u32 shadow;
  u32 pixels;
  u32 recoloured;

  if (work->menuState == 0) {
    Dma_Set((const void *)Palette_GetBasicObjColors(),
            (void *)&BG_PLTT_COLOR(14, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
  }
  x += window->x;
  y += window->y;
  for (i = 0; i < 5; i++, x++) {
    // This tile's eight pixels: filled up to `length`, empty after.
    tile = work->tilemap[y * 32 + x];
    bar = 0x22222222;
    shadow = 0xcccccccc;
    tile &= 0x3ff;
    if (length > 7) {
      bar = 0x88888888;
      shadow = 0xdddddddd;
    } else if (length >= 0) {
      bar <<= length * 4;
      bar |= 0x88888888U >> (32 - length * 4);
      shadow <<= length * 4;
      shadow |= 0xddddddddU >> (32 - length * 4);
    }
    for (row = 0; row <= (fullLength != 0 ? 2 : 0); row++) {
      pixels = *TILE_ROW_FROM_BOTTOM(tile, row);
      recoloured = 0;
      for (column = 0; column <= 7; column++) {
        u32 colour = pixels & 15;

        if (colour == 14)
          recoloured |= bar & (15U << (column * 4));
        else if (colour == 1)
          recoloured |= shadow & (15U << (column * 4));
        else
          recoloured |= colour << (column * 4);
        pixels >>= 4;
      }
      *TILE_ROW_FROM_BOTTOM(tile, row) = recoloured;
    }
    length -= 8;
  }
}

// Draws the party status window with `flags` (see PARTY_STATUS_*): each
// member's name, HP and optionally PP with their bars, and in battle the
// standby Djinn of each element. The HP is drawn in text colour 2 at zero and
// colour 4 at a quarter or less. When the flags change, the window is erased
// and laid out again first.
void PartyStatusDraw(s32 flags) {
  // The layout, the UI work and the battle work are neighbouring work slots.
  void **slot = (void **)gPartyStatusWork;
  struct PartyStatusWindow *layout = slot[0];
  struct BattleWork *battle = slot[-7];
  struct UiWindow *window = layout->window;
  struct UiRenderWork *work = slot[-1];
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  // List the members shown: those fighting in battle, the party otherwise.
  if (work->menuState != 0) {
    count = BattleParty_PrepareActiveOwnersFar(0);
    y = -1;
    for (i = 0; i < count; i++) {
      members[i] = battle->partyUnits[i];
      if (members[i] == 255)
        break;
    }
  } else {
    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++)
      members[i] = gGameState.activeOwners[i];
    members[i] = 255;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (work->menuState == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    return;
  }

  // Redraw the frame, laying it out again when the flags changed.
  work->menuBusy = 1;
  if (layout->flags == flags) {
    RenderOutput_RedrawSavedRect(window);
    UiWindow_DrawColumnBorders(window, flags);
  } else {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    UiWindow_BuildLayoutBounds(flags);
    window->width = layout->width;
    window->height = layout->height;
    window->x = layout->x;
    UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
    UiWindow_DrawColumnBorders(window, flags);
  }

  // Each member's column: name, HP and PP, each bar 40 pixels at full.
  if (flags & PARTY_STATUS_SHOW_DJINN)
    x = PARTY_STATUS_DJINN_WIDTH;
  for (i = 0; i != count; i++) {
    struct BattleUnit *unit = UnitGetFar(members[i]);
    s32 hp = unit->hp;
    s32 maxHp = unit->maxHp;
    s32 barLength;
    s32 current;

    if (hp == 0)
      UiWork_SetParamNibble(2);
    else if (hp <= maxHp / 4)
      UiWork_SetParamNibble(4);
    else
      UiWork_SetParamNibble(15);
    work->level = 14;
    if (work->menuState != 0)
      work->level = 5;
    UiText_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->level = 15;
    UiText_DrawStringAtOffset(unit->name, window,
                              (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    UiWork_SetParamNibble(15);
    if (unit->maxHp != 0) {
      // A member with any HP left shows at least one pixel of bar.
      current = unit->hp;
      barLength = ((current * 40) / unit->maxHp);
      if (barLength == 0 && current != 0)
        barLength = 1;
      UiWindow_DrawStatusBarTiles(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1,
                                  y + 2, barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      work->level = 14;
      if (work->menuState != 0)
        work->level = 5;
      UiText_DrawPrefixedNumberAtOffset(unit->pp, window,
                                        (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                        y * 8 + 16, 1);
      if (unit->maxPp != 0) {
        current = unit->pp;
        barLength = ((current * 40) / unit->maxPp);
        if (barLength == 0 && current != 0)
          barLength = 1;
        UiWindow_DrawStatusBarTiles(
            window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3, barLength);
      }
    }
  }
  work->level = 15;

  // The Djinn column: each element's icon and its count of standby Djinn.
  if (work->menuState != 0 && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    BattlePlacement_CountValidEntriesFar(0, standbyDjinn);
    UiWindow_SetTilemapEntry(window, 0x5001, 0, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5002, 2, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5003, 0, row + 1, 0);
    UiWindow_SetTilemapEntry(window, 0x5004, 2, row + 1, 0);
    UiWindow_PutGlyph(window, standbyDjinn[0] + '0', 1, row);
    UiWindow_PutGlyph(window, standbyDjinn[1] + '0', 3, row);
    UiWindow_PutGlyph(window, standbyDjinn[2] + '0', 1, row + 1);
    UiWindow_PutGlyph(window, standbyDjinn[3] + '0', 3, row + 1);
  }
  work->menuBusy = 0;
}

// Closes the party status window and frees its heap block.
void UiWork_FinalizeAndReleaseBlock16(void) {
  UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
  Runtime_ReleaseHeapBlock(PARTY_STATUS_BLOCK);
}

// Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the
// `width` x `height` tile rectangle at (x, y) inside `window`, clipped to the
// screen: bit 12 is the lowest bit of the entry's palette number, so this
// switches the rectangle between two palettes. The clip uses 29 for the
// bottom edge test and 20 for the clipped height.
void UiWindow_SetTileAttributeBitRect(const struct UiWindow *window, s32 x,
                                      s32 y, s32 width, s32 height, u32 set) {
  struct UiRenderWork *work = UI_WORK;

  x += window->x + 1;
  y += window->y + 1;
  set &= 1;
  set <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    // The byte offset of the first cell: 32 entries of 2 bytes a row.
    y <<= 6;
    x = y + (x << 1);
    do {
      u16 *cell = (u16 *)(x + (u32)work->tilemap);
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~0x1000;
        entry |= set;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      x += 64;
    } while (height != 0);
    work->dirty = 1;
  }
}

extern const u8 *gRenderResource_PairSourceTable[];

// The three save files. Each is two 4 KB records of backup memory: record
// `file` holds the first half of the 8 KB save buffer and record
// `file + SAVE_FILE_COUNT` the second.
#define SAVE_FILE_COUNT 3
#define SAVE_RECORD_BYTES 0x1000
#define SAVE_NO_FREE_FILE 999

// What the save functions return when they fail, after showing the message.
#define SAVE_ERROR_READ -2      // a record could not be read
#define SAVE_ERROR_WRITE -3     // "Failed to save." (or "Failed to erase.")
#define SAVE_ERROR_ERASE -4     // "Failed to erase."
#define SAVE_ERROR_NO_FILE -5   // no empty file to copy into
#define SAVE_ERROR_NO_MEMORY -9 // "Unable to detect backup memory."

// Writes the play time `frames` (at 60 frames a second) into `out` as hours
// and minutes, "hhh:mm" with the hours right-aligned in three characters, and
// returns `out`. The time stops at 999:59.
void *Text_FormatPlayTime(s32 frames, u8 *out) {
  u8 digits[64];
  u32 totalMinutes;
  u32 hours;
  s32 minutes;
  u8 *s;
  u8 *p;

  totalMinutes = (u32)frames / (60 * 60);
  if (totalMinutes > 999 * 60 + 59)
    totalMinutes = 999 * 60 + 59;

  hours = totalMinutes / 60;
  minutes = totalMinutes % 60;

  s = UiText_FormatNumber(digits, hours, 3);
  *out = *s;
  s++;
  p = out + 1;
  *p = *s;
  p++;
  *p = s[1];
  // Adding 100 gives the minutes a leading zero in their last two digits.
  minutes += 100;
  p++;
  *p = ':';

  s = UiText_FormatNumber(digits, minutes, 2);
  p++;
  *p = s[0];
  p++;
  *p = s[1];
  p[1] = 0;

  return out;
}

// Returns the first save file whose summary shows it empty (level 0), or
// SAVE_NO_FREE_FILE when all three are in use. The summaries must be loaded.
u32 SaveState_FindFreeSummarySlot(void) {
  u32 file;
  struct SaveSummary *summary;

  summary = gSaveWorkspace->summary;
  for (file = 0; file < SAVE_FILE_COUNT; file++, summary++) {
    if (summary->level == 0)
      return file;
  }
  return SAVE_NO_FREE_FILE;
}

// Returns how many save files there are, leaving out those whose summary has
// its send flag set when `excludeSent` is nonzero, or SAVE_ERROR_NO_MEMORY
// when the backup memory cannot be read.
s32 SaveState_CountRecordsExcludingFlagged(s32 excludeSent) {
  s32 file;
  s32 count;
  struct SaveSummary *summary;

  if (SaveState_InitializeWorkspace() != 0) {
    count = SAVE_ERROR_NO_MEMORY;
  } else {
    count = SaveState_LoadSummaryRecords();
    if (excludeSent != 0) {
      summary = gSaveWorkspace->summary;
      for (file = 0; file < SAVE_FILE_COUNT; file++) {
        if ((s8)summary[file].sendFlag != 0)
          count--;
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return count;
}

// The distance in bytes from one field of a save summary to another.
#define SUMMARY_DISTANCE(from, to)                                             \
  ((u32)&((struct SaveSummary *)0)->to - (u32)&((struct SaveSummary *)0)->from)

// Loads the save file summaries for the title menu and returns how many files
// there are: plus 100 when every file has its send flag set, or
// SAVE_ERROR_NO_MEMORY when the backup memory cannot be read. Sets
// gTitleExtraOptionEnabled when a file has any of game flags 48-127 set, and
// gTitleSendOptionEnabled when a file has its send flag set and R and Left are
// held.
s32 SaveState_ScanRecordFlags(void) {
  /* FAKEMATCH: the signed cursor from the party tail keeps the native
   * literal base 0x1070 and the flag offsets 1 and 2. Measured: the
   * fields read as summary[i].sendFlag and .flagCount, or through a
   * walking summary pointer, base the cursor on sendFlag (0x1071)
   * with offsets 0 and 1 (differ 4). */
  s32 error;
  s32 sentCount;
  s32 fileCount;

  error = SaveState_InitializeWorkspace();
  sentCount = 0;
  fileCount = SAVE_ERROR_NO_MEMORY;
  if (error == 0) {
    s32 i;
    s8 *flags;

    fileCount = SaveState_LoadSummaryRecords();
    flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
    gTitleSendOptionEnabled = 0;
    (*(s16 *)&gTitleExtraOptionEnabled) = 0;
    for (i = 0; i < SAVE_FILE_COUNT; i++) {
      if (flags[i * sizeof(struct SaveSummary) +
                SUMMARY_DISTANCE(party[4], sendFlag)] != 0) {
        gTitleSendOptionEnabled = 1;
        sentCount++;
      }
      if (flags[i * sizeof(struct SaveSummary) +
                SUMMARY_DISTANCE(party[4], flagCount)] != 0) {
        (*(s16 *)&gTitleExtraOptionEnabled) = 1;
      }
    }

    if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
      gTitleSendOptionEnabled = 0;
    }
  }
  SaveState_ReleaseWorkspace();
  if (fileCount != 0 && sentCount == fileCount) {
    return fileCount + 100;
  }
  return fileCount;
}

// Prepares the save buffer for writing: stamps the game state with the time
// and options, then fills the file summary shown on the file menus (the
// leader's name, level and class, the play time, area, coins, Djinn of each
// element, the party, the send flag and how many of game flags 48-127 are
// set) and its checksum, the sum of the 968 words from gGameFlagBytes on.
// Returns the checksum.
u32 SaveState_BuildSummaryHeader(void) {
  u16 members[14];
  struct SaveSummary *summary;
  struct BattleUnit *leader;
  u8 *source;
  u8 *destination;
  u32 *word;
  s32 i;
  u32 sum = 0;

  // The summary's 16-byte header comes before the save buffer.
  summary = (struct SaveSummary *)(gSaveBuffer - 16);
  gGameState.buildStamp = Runtime_GetBuildStampTimeFar(0);
  gGameState.playTime = gPlayTimeFrames;
  {
    u32 *copy = (u32 *)gSceneState;
    copy[64] = gPlayTimeFrames;
  }
  gGameState.idleSleepEnabled = gIdleSleepEnabled;

  leader = UnitGetFar(gGameState.selectedActor);
  destination = summary->name;
  source = leader->name;
  for (i = 11; i >= 0; i--)
    *destination++ = *source++;
  summary->level = leader->level;
  summary->playTime = gGameState.playTime;
  summary->area =
      Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
  summary->classId = leader->classIndex;
  summary->coins = gGameState.coins;
  summary->djinn[0] = Party_SumDjinnCountsFar(0);
  summary->djinn[1] = Party_SumDjinnCountsFar(1);
  summary->djinn[2] = Party_SumDjinnCountsFar(2);
  summary->djinn[3] = Party_SumDjinnCountsFar(3);
  // Up to four party members, ended by -1.
  Party_ListActiveOwnersFar(members);
  for (i = 0; i <= 3 && members[i] != 255; i++)
    summary->party[i] = members[i];
  summary->party[i] = -1;
  summary->paletteGlow = gGameState.paletteGlow[0];
  summary->paletteGlowSub = gGameState.paletteGlow[1];
  summary->sendFlag = gGameState.unk20f;
  summary->flagCount = 0;
  for (i = 48; i <= 127; i++) {
    if (GameFlag_TestFar(i))
      summary->flagCount++;
  }
  summary->hasFlag20 = GameFlag_TestFar(32) != 0;
  {
    s32 n;
    u32 *frame = &gGameState.buildStamp;
    u32 frames;

    n = 242;
    /* FAKEMATCH: the one-pass null arm and duplicate frame read end
     * CSE with the bound constant ahead of the frame load. Folding
     * the arm reverses two rows; the conditional and duplicate read
     * disappear, preserving all 174 instructions in every edition. */
    if (!frame) {
      do {
        n = 242;
      } while (0);
    } else {
      n = 242;
      frames = *frame;
    }
    frames = *frame;
    word = (u32 *)gGameFlagBytes;
    i = 0;
    n <<= 2;
    summary->frames = frames;
    // Entered at its test: as a while loop it gains a second test (4 bytes).
    goto test;
    do {
      sum += *word++;
      i++;
    test:;
    } while (i < n);
  }
  summary->checksum = sum;
  return sum;
}

// Saves the game to the current file (gSaveSlot): builds the summary and
// writes both halves of the save buffer. Returns 0, gSaveSlot itself (-1)
// when no file is chosen, or a SAVE_ERROR_* after showing its message.
s16 SaveState_WriteCurrentSlotPair(void) {
  s16 file;
  s16 result;
  s32 error;

  result = 0;
  file = gSaveSlot;
  if (file != -1) {
    error = SaveState_InitializeWorkspace();
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
      result = SAVE_ERROR_NO_MEMORY;
    } else {
      SaveState_BuildSummaryHeader();
      {
        void *half = &gSaveBuffer;
        s32 current;

        error = SaveState_WriteRecord(gSaveSlot, half);
        current = gSaveSlot;
        half = (char *)half + SAVE_RECORD_BYTES;
        error |= SaveState_WriteRecord(current + SAVE_FILE_COUNT, half);
        if (error != 0) {
          UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
          result = SAVE_ERROR_WRITE;
        }
      }
    }
    SaveState_ReleaseWorkspace();
    file = result;
  }
  return file;
}

// Writes both halves of the save buffer, as they are, to save file `file`.
// Returns 0, or a SAVE_ERROR_* after showing its message.
s32 SaveState_WriteSlotPair(s32 file) {
  s32 error;
  s16 result = 0;

  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    void *half = &gSaveBuffer;

    error = SaveState_WriteRecord(file, half);
    half = (char *)half + SAVE_RECORD_BYTES;
    error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT, half);
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_WRITE;
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

// Writes the 64-byte save stamp (gSaveStamp, which lies in the first half of
// the save buffer) into the current file's first record, leaving the rest of
// the record as it was saved. Returns 0, -1 when no file is chosen, or a
// SAVE_ERROR_* after showing its message.
s32 SaveState_ProcessSelectedSlot(void) {
  void *record;
  s32 result;
  s32 error;
  u8 *stamp;

  record = Runtime_BumpAllocateAlternatePool(SAVE_RECORD_BYTES);
  result = 0;
  if (gSaveSlot == -1)
    return -1;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    error = SaveState_ReadRecordPayload(gSaveSlot, record);
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_READ;
    }
    // The stamp's place in the record is its place in the save buffer.
    stamp = (u8 *)record + (s32)gSaveStamp;
    stamp -= (s32)gSaveBuffer;
    Iwram_CopyWords(stamp, gSaveStamp, 16);
    error = SaveState_WriteRecord(gSaveSlot, record);
    if (error != 0) {
      UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
      result = SAVE_ERROR_WRITE;
    }
  }
  SaveState_ReleaseWorkspace();
  MemFree(record);
  return result;
}

// Asks "OK to save over this file?" and, on Yes, writes the save stamp into
// the current file (SaveState_ProcessSelectedSlot) and says "Your game was
// saved." Returns that result.
s32 SaveState_ConfirmAndProcessSelectedSlot(void) {
  s32 result;

  // UB: on No, result is returned without ever being set.
#ifdef UBFIX
  result = 0;
#endif

  UiText_OpenMessageWindow(MsgOverwriteConfirm, 8, 12, 2);
  while (UiWork_IsComplete() == 0) {
    WaitFrames(1);
  }
  if (Menu_RunConfirmSelection(1, 0, 0, 1)) {
    UiWork_FinalizePendingCore();
  } else {
    UiWork_FinalizePendingCore();
    AudioCommand_PlayFar(85);
    result = SaveState_ProcessSelectedSlot();
    if (result >= 0) {
      UiText_ShowPositionedMessageAndWait(MsgGameSaved, 1);
    }
  }
  return result;
}

// The file menu's Copy: lets the player choose a file, reads both of its
// records into the save buffer and writes them to the first empty file.
// Returns 0, -1 when cancelled, or a SAVE_ERROR_* after showing its message
// (the game says "Failed to erase." both when no file is empty and when the
// write fails).
s32 SaveState_CopySlotPair(void) {
  u32 error;
  s32 file;
  s32 result;

  result = 0;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(0, 2);
    if (file == -1) {
      result = file;
    } else {
      void *lower = &gSaveBuffer;
      void *upper;

      error = SaveState_ReadRecordPayload(file, lower);
      upper = (u8 *)lower + SAVE_RECORD_BYTES;
      error |= SaveState_ReadRecordPayload(file + SAVE_FILE_COUNT, upper);
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
        result = SAVE_ERROR_READ;
      } else {
        file = SaveState_FindFreeSummarySlot();
        if (file == SAVE_NO_FREE_FILE) {
          UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
          result = SAVE_ERROR_NO_FILE;
        } else {
          error = SaveState_WriteRecord(file, lower);
          error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT, upper);
          if (error != 0) {
            UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
            result = SAVE_ERROR_WRITE;
          } else {
            UiText_ShowPositionedMessageAndWait(MsgFileCopied, 1);
          }
        }
      }
    }
  }

  SaveState_ReleaseWorkspace();
  return result;
}

// The file menu's Erase: lets the player choose a file, asks "OK to erase
// this?" and on Yes deletes both of its records. Returns 0 (also when the
// player answers No), -1 when cancelled, or a SAVE_ERROR_* after showing its
// message.
s32 SaveState_DeleteSelectedSlot(void) {
  s32 error;
  s32 file;
  s32 result = 0;

  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(0, 3);
    if (file == -1) {
      result = file;
    } else {
      UiText_OpenMessageWindow(MsgEraseConfirm, 8, 1, 2);
      while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
      }
      if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
        UiWork_FinalizePendingCore();
      } else {
        UiWork_FinalizePendingCore();
        error = SaveState_DeleteRecord(file);
        error |= SaveState_DeleteRecord(file + SAVE_FILE_COUNT);
        if (error != 0) {
          UiText_ShowPositionedMessageAndWait(MsgEraseFailed, 1);
          result = SAVE_ERROR_ERASE;
        } else {
          UiText_ShowPositionedMessageAndWait(MsgFileErased, 1);
        }
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)

// A scheduler callback that makes colours 8-11 of BG palette 14 glow: each
// follows a sine wave of the frame counter, eight frames behind the one
// before. With s the sine from -1 to 1, the colour is red 22 + 2s, green
// 16 + s and blue 20 + s (of 31).
void PaletteGlow_UpdateSine(void) {
  s32 *tick;
  volatile u16 *palette;
  s32 colour;
  s32 sine;
  s32 i;

  tick = (s32 *)&gFrameTick;
  palette = &BG_PLTT_COLOR(14, 8);
  i = 0;
  do {
    sine = Trig_Sin((*tick + i * 8) * 0x300);
    colour = sine / 16384;
    {
      u32 red = colour << 1;
      u32 green = colour;

      red += 22;
      green += 16;
      colour += 20;
      /* FAKEMATCH: the one-pass block keeps the blue shift after the three
       * adds, in source order. Measured: without it the scheduler moves the
       * red add below the shifts (differ 6); the colour as one expression
       * differs 25, and r, g, b locals in place of col differ 6, as a do
       * or a for loop. */
      do {
        colour <<= 10;
      } while (0);
      green <<= 5;
      colour |= green;
      colour |= red;
    }
    /* FAKEMATCH: the one-pass block keeps the store ahead of the loop
     * index's increment. Measured: without it, differ 2. */
    do {
      *palette++ = colour;
    } while (0);
  } while (++i <= 3);
}

#endif

#if EDITION_INTERNATIONAL

// Starts the palette glow (PaletteGlow_UpdateSine) in the scheduler.
void PaletteGlow_Start(void) {
  Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

// Stops the palette glow.
void PaletteGlow_Stop(void) {
  Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

// Fills the `width` x `height` tile rectangle at (x, y) inside `window`,
// clipped to the screen as UiWindow_SetTileAttributeBitRect clips, with the
// blank tile entry 0xe006 of the UI tilemap.
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width,
                              s32 height) {
  u16 *tilemap = (u16 *)gWindowWork[0];
  s32 sum;

  sum = x + window->x;
  x = sum + 1;
  sum = y + (window)->y;
  y = sum + 1;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29)
    width = 30 - x;

  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29)
    height = 20 - y;

  if (width > 0 && height > 0) {
    // Byte offsets: 32 entries of 2 bytes a row.
    s32 yOffset = y << 6;
    s32 xOffset = x << 1;
    s32 offset = yOffset + xOffset;
    do {
      s32 remaining = width;
      u16 *cell = (u16 *)(offset + (s32)tilemap);
      if (remaining != 0) {
        do {
          remaining--;
          *cell++ = 0xE006;
        } while (remaining != 0);
      }
      height--;
      offset += 64;
    } while (height != 0);
    ((struct UiRenderWork *)tilemap)->dirty = 1;
  }
}

#endif

// The save file menu shows the chosen file's party as walking sprites in a
// row, and four Djinn sprites, one for each element, over the file's Djinn
// counts. Each sprite stands three tiles (24 pixels) from the one before.
#define SAVE_MENU_SPRITE_SPACING 3
// Scheduler priority of the callbacks that place the sprites every frame.
#define SAVE_MENU_SPRITE_PRIORITY (200 << 4)

// Shows the party of save file `summary` (up to four members) as sprites in
// a row from tile (x, y) inside `window`, and starts placing them every
// frame (SaveMenu_PlacePartySprites).
void SaveMenu_ShowPartySprites(struct UiWindow *window, s32 x, s32 y,
                               struct SaveSummary *summary) {
  struct ShopRuntime *menu = gMenuWork;
  s32 i;
  s32 priority;

  for (i = 0; i < 4 && summary->party[i] != -1; i++) {
    struct AnimationObject *sprite =
        ResourceObject_CreateFar(Party_RemapCharacterIdIfEnabledFar(
            summary->party[i], (s8)summary->hasFlag20));

    if (sprite != NULL) {
      AnimationObjects_SelectAnimationFar(sprite, 1);
      ((struct FieldSprite *)sprite)->flags = 0;
      ((struct FieldSprite *)sprite)->priority = 0;
    }

    menu->partyMemberIcons[i] = sprite;
    menu->partyMemberX[i] =
        (x + window->x + i * SAVE_MENU_SPRITE_SPACING) * 8 + 16;
    menu->partyMemberY[i] = (y + window->y) * 8 + 16;
    menu->partyMemberScale[i] = 0x10000;
  }

  priority = 200;
  priority <<= 4;
  Scheduler_AddOrUpdateCallback(SaveMenu_PlacePartySprites, priority);
}

// Stops placing the party sprites of the save file menu and releases them.
void SaveMenu_HidePartySprites(void) {
  struct ShopRuntime *menu = gMenuWork;
  s32 i;

  Scheduler_RemoveCallback(SaveMenu_PlacePartySprites);
  for (i = 0; i < 4; i++) {
    if (menu->partyMemberIcons[i] != NULL) {
      ResourceObject_ReleaseFar(menu->partyMemberIcons[i]);
      menu->partyMemberIcons[i] = NULL;
    }
  }
}

// A scheduler callback: draws each party sprite of the save file menu at its
// place and scale for this frame.
void SaveMenu_PlacePartySprites(void) {
  struct ShopRuntime *menu = gMenuWork;
  s32 scale[2];
  s32 position[4];
  s32 index = 0;
  s32 sprite;

  while (1) {
    sprite = (s32)menu->partyMemberIcons[index];
    if (sprite != 0) {
      // The position is 16.16 fixed point, at depth 500.
      scale[0] = menu->partyMemberScale[index];
      scale[1] = menu->partyMemberScale[index];
      position[0] = menu->partyMemberX[index] << 16;
      position[1] = 500 << 16;
      position[2] = (menu->partyMemberY[index] << 16) + (500 << 16);
      position[3] = 0;
      Object_ApplyProjectedPlacementFar((struct AnimationObject *)sprite,
                                        position, scale, 0x4000);
    }
    index += 1;
    if (index > 3) {
      break;
    }
  }
}

// Shows the four Djinn sprites, one for each element, in a row from tile
// (x, y) inside `window`, and starts placing them every frame
// (SaveMenu_PlaceDjinnSprites). Does nothing when `window` is NULL.
void SaveMenu_ShowDjinnSprites(struct UiWindow *window, s32 x, s32 y) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++) {
      struct AnimationObject *sprite =
          (struct AnimationObject *)ResourceObject_CreateFar(
              gMenu_PartySpriteResourceIds[i]);

      if (sprite != NULL) {
        AnimationObjects_SelectAnimationFar(sprite, 2);
        ((struct FieldSprite *)sprite)->flags = 0;
        ((struct FieldSprite *)sprite)->priority = 0;
      }

      menu->objects[i] = sprite;
      menu->positionsX[i] =
          (window->x + x + i * SAVE_MENU_SPRITE_SPACING) * 8 + 16;
      menu->positionsY[i] = (window->y + y) * 8 + 16;
    }

    Scheduler_AddOrUpdateCallback(SaveMenu_PlaceDjinnSprites,
                                  SAVE_MENU_SPRITE_PRIORITY);
  }
}

// Stops placing the Djinn sprites of the save file menu and releases them.
void SaveMenu_HideDjinnSprites(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 i;

  Scheduler_RemoveCallback(SaveMenu_PlaceDjinnSprites);
  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
}

// A scheduler callback: draws each Djinn sprite of the save file menu at its
// place, at scale 1.0, for this frame.
void SaveMenu_PlaceDjinnSprites(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 scale[2];
  s32 position[4];
  s32 index;

  index = 0;
  do {
    void *sprite = menu->objects[index];

    if (sprite != NULL) {
      s32 one = 0x10000;
      s32 depth = 500 << 16;

      scale[0] = one;
      scale[1] = one;
      position[0] = menu->positionsX[index] << 16;
      position[1] = depth;
      position[2] = (menu->positionsY[index] << 16) + depth;
      position[3] = 0;
      Object_ApplyProjectedPlacementFar(sprite, position, scale, 0x4000);
    }
    index++;
  } while (index <= 3);
}

// Draws the Djinn counts of save file `summary`, one for each element, two
// digits each, 24 pixels apart on row 16 of `window`, under the Djinn
// sprites. Does nothing when `window` is NULL.
void SaveMenu_DrawDjinnCounts(struct UiWindow *window,
                              struct SaveSummary *summary) {
  s32 i;

  if (window != NULL) {
    for (i = 0; i < 4; i++)
      UiText_DrawNumberAtOffset((s8)summary->djinn[i], 2, window,
                                i * SAVE_MENU_SPRITE_SPACING * 8, 16);
  }
}

// Where each edition draws the "Lv" label and the coin label, in pixels.
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

// Draws the summary of save file `summary` into `window`: the leader's name,
// level and class, the play time and the coins. Does nothing when `window`
// is 0.
void SaveMenu_DrawFileSummary(struct UiWindow *window,
                              struct SaveSummary *summary) {
  u32 playTime[4]; // the play time text
  s32 y;

  if (window != 0) {
    RenderOutput_PrepareForRedraw(window);
    UiWindow_DrawDividerLine(window, 0, 4, 13, 4);
    UiText_DrawStringAtOffset(summary->name, window, 0, 0);
    y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR)
    UiText_DrawStringAtOffset(gStatusMenu_LevelLetterString, window,
                              SUMMARY_LEVEL_X, 0);
#else
    UiText_DrawStringInWindow(gStatusMenu_LevelLetterString, window, 72, 0);
#endif
    UiText_DrawNumberAtOffset(summary->level, 2, window, 80, y);
    UiText_DrawCharacterAtOffset(summary->classId + MsgClassName, window, 0,
                                 16);
#if !EDITION_INTERNATIONAL
    // Coins on the third row, then "Play Time:" and the time.
    y = 32;
    UiText_DrawNumberInWindow(summary->coins, 6, window, 24, y);
    UiText_DrawCharacterAtOffset(MsgCoins, window, 72, 32);
    UiText_DrawCharacterAtOffset(MsgStatusLabel, window, 0, 48);
    UiText_DrawStringInWindow(
        Text_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48, 48);
#else
    // "Play Time:" with the time under it, then the coins.
    UiText_DrawCharacterAtOffset(MsgStatusLabel, window, 0, 32);
    UiText_DrawStringInWindow(
        Text_FormatPlayTime(summary->playTime, (u8 *)playTime), window, 48, 40);
    y = 48;
    UiText_DrawNumberInWindow(summary->coins, 6, window, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
    // French takes the singular "Pièce" for one coin or none.
    if (summary->coins > 1)
      UiText_DrawCharacterAtOffset(MsgCoins, window, SUMMARY_COINS_X, 48);
    else
      UiText_DrawCharacterAtOffset(MsgPiece, window, SUMMARY_COINS_X, 48);
#else
    UiText_DrawCharacterAtOffset(MsgCoins, window, SUMMARY_COINS_X, 48);
#endif
#endif
  }
}

// What the save file menu is choosing a file for, which decides the files
// the cursor may stop on.
enum SaveMenuMode {
  SAVE_MENU_SAVE,     // saving: any file, empty ones too
  SAVE_MENU_CONTINUE, // continuing: files not yet cleared
  SAVE_MENU_COPY,     // copying: any file with data
  SAVE_MENU_ERASE,    // erasing: any file with data
  SAVE_MENU_EXTRA,    // files with any of game flags 48-127 set
  SAVE_MENU_CLEARED   // cleared files only
};

// SaveMenu_SelectSlot's result when no file can be chosen in its mode.
#define SAVE_MENU_NO_FILE -2

// The save file menu: lists the three files with each one's leader and area
// (or why it cannot be chosen) and, for the file under the cursor, its
// summary, its party and its Djinn, with the file's own palette glow. The
// cursor starts on `file` and moves up and down over the files `mode` allows
// (see SaveMenuMode). Returns the file chosen with A, -1 when cancelled with
// B, or SAVE_MENU_NO_FILE when no file is allowed.
#if EDITION_INTERNATIONAL

s32 SaveMenu_SelectSlot(s32 file, s32 mode) {
  struct UiWindow *fileList;
  struct SaveWorkspace *save;
  struct UiWindow *summaryWindow;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modeSprite;
  s32 redraw;
  struct UiRenderWork *uiWork;
  u32 buildStamp;
  void *menuWork;
  s32 result;
  s32 i;
  s32 y;

  menuWork = Runtime_AllocateHeapBlock(55, 0xa70);
  save = *(gWorkSlot + HEAP_SLOT_SAVE);
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  redraw = 1;
  uiWork = *(gWorkSlot + HEAP_SLOT_WINDOW);
  buildStamp = Runtime_GetBuildStampTimeFar();
  // Start on the first file the mode allows.
  if (file < 0)
    file = 0;
  if (mode == SAVE_MENU_CONTINUE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[file].level == 0 ||
                                        (s8)save->summary[file].sendFlag != 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (mode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[file].level == 0 ||
                                        (s8)save->summary[file].flagCount == 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (mode == SAVE_MENU_CLEARED) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[file].level == 0 ||
                                        (s8)save->summary[file].sendFlag == 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (mode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[file].level == 0); i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, menuWork, 0xa70);
  PaletteGlow_Start();
  fileList = UiWindow_Create(1, 2, 28, 7, 2);
  summary = save->summary;
  y = 0;
  for (i = 0; i < SAVE_FILE_COUNT; i++, y += 16, summary++) {
    if (summary->level == 0) {
      UiText_DrawResource(MsgSaveNoData, fileList, 10, y);
    } else if (summary->frames < buildStamp) { // saved by an older build
      UiText_DrawResource(MsgSaveContinue, fileList, 10, y);
    } else if (summary->playTime != *(u32 *)summary->unk38) {
      UiText_DrawResource(MsgSaveCorrupted, fileList, 10, y);
    } else if (mode == SAVE_MENU_CLEARED && (s8)summary->sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, y);
    } else {
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(summary->area + MsgDebugEntryName, fileList, 62, y);
      uiWork->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
  modeSprite = RenderResource_CreatePair(mode, fileList, 72, -24);

  while (1) {
    // Show the file under the cursor after it moves.
    if (redraw != 0) {
      redraw = 0;
      if (save->summary[file].level != 0) {
        PaletteGlow_Update(save->summary[file].paletteGlow,
                           save->summary[file].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = UiWindow_Create(1, 10, 14, 9, 2);
        SaveMenu_DrawFileSummary(summaryWindow, &save->summary[file]);
        WaitFrames(1);
        if (partyWindow == NULL)
          partyWindow = UiWindow_Create(16, 10, 13, 3, 2);
        SaveMenu_HidePartySprites();
        SaveMenu_ShowPartySprites(partyWindow, 0, 0, &save->summary[file]);
        WaitFrames(1);
        if ((s8)save->summary[file].djinn[0] +
                (s8)save->summary[file].djinn[1] +
                (s8)save->summary[file].djinn[2] +
                (s8)save->summary[file].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = UiWindow_Create(16, 14, 13, 5, 2);
          SaveMenu_DrawDjinnCounts(djinnWindow, &save->summary[file]);
          SaveMenu_HideDjinnSprites();
          SaveMenu_ShowDjinnSprites(djinnWindow, 0, 0);
        } else {
          SaveMenu_HideDjinnSprites();
          UiWork_Finalize(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        PaletteGlow_Update(gGameState.paletteGlow[0],
                           gGameState.paletteGlow[1]);
        SaveMenu_HideDjinnSprites();
        SaveMenu_HidePartySprites();
        UiWork_Finalize(djinnWindow, 2);
        UiWork_Finalize(partyWindow, 2);
        UiWork_Finalize(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
      RenderOutput_RedrawSavedRect(fileList);
      UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
      UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
      UiWindow_FillTilemapRect(fileList, 0, file * 2, 26, 1);
    }
    Ui_ApplyTableOffsetToPair(modeSprite);
    WaitFrames(1);
    // Up and down move to the previous or next file the mode allows.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      redraw = 1;
      do {
        file = (file + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT; // previous
        if (mode == SAVE_MENU_SAVE)
          break;
      } while ((
          save->summary[file].level == 0 ||
          (mode == SAVE_MENU_CONTINUE &&
           (s8)save->summary[file].sendFlag != 0) ||
          (mode == SAVE_MENU_EXTRA && (s8)save->summary[file].flagCount == 0) ||
          (mode == SAVE_MENU_CLEARED &&
           (s8)save->summary[file].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      redraw = 1;
      do {
        file = (file + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT; // next
        if (mode == SAVE_MENU_SAVE)
          break;
      } while ((
          save->summary[file].level == 0 ||
          (mode == SAVE_MENU_CONTINUE &&
           (s8)save->summary[file].sendFlag != 0) ||
          (mode == SAVE_MENU_EXTRA && (s8)save->summary[file].flagCount == 0) ||
          (mode == SAVE_MENU_CLEARED &&
           (s8)save->summary[file].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = file;
      break;
    }
  }
  // Close the menu and return to the current game's palette glow.
  SaveMenu_HideDjinnSprites();
  SaveMenu_HidePartySprites();
  UiWork_Finalize(djinnWindow, 2);
  UiWork_Finalize(partyWindow, 2);
  UiWork_Finalize(summaryWindow, 2);
  UiWork_Finalize(fileList, 2);
  PaletteGlow_Stop();
  Runtime_ReleaseHeapBlock(55);
  PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  WaitFrames(1);
done:
  return result;
}
#elif defined(TBS_EDITION_JA)

s32 SaveMenu_SelectSlot(s32 file, s32 mode) {
  struct UiWindow *fileList;
  struct UiWindow *summaryWindow;
  struct SaveWorkspace *save;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modeSprite;
  s32 cursorStyle;
  s32 redraw;
  struct UiRenderWork *uiWork;
  u32 buildStamp;
  void *menuWork;
  struct ShopCursor fileCursor;
  s32 result;
  s32 i;
  s32 x;
  s32 y;
  s32 vramSlot;
  struct RenderOutput *cursorSprite;

  menuWork = Runtime_AllocateHeapBlock(55, 0xa70);
  save = *(gWorkSlot + HEAP_SLOT_SAVE);
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  redraw = 1;
  uiWork = *(gWorkSlot + HEAP_SLOT_WINDOW);
  buildStamp = Runtime_GetBuildStampTimeFar();
  // Start on the first file the mode allows.
  if (file < 0)
    file = 0;
  if (mode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[file].level == 0); i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (mode == SAVE_MENU_CONTINUE) {
    for (i = 0; i < SAVE_FILE_COUNT && ((s8)save->summary[file].sendFlag != 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (mode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT && ((s8)save->summary[file].flagCount == 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (mode == SAVE_MENU_CLEARED) {
    for (i = 0; i < SAVE_FILE_COUNT && ((s8)save->summary[file].sendFlag == 0);
         i++) {
      file++;
      if (file == SAVE_FILE_COUNT)
        file = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, menuWork, 0xa70);
  fileList = UiWindow_Create(1, 2, 28, 7, 2);

  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      UiText_DrawResource(MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) { // an older build
      UiText_DrawResource(MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38) {
      UiText_DrawResource(MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (mode == SAVE_MENU_CLEARED &&
               (s8)save->summary[i].sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, i * 16);
    } else {
      y = i * 16;
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(save->summary[i].area + MsgDebugEntryName, fileList,
                          72, y);
      uiWork->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    x = -12;
    y = file * 16 + 4;
    cursorSprite =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, fileList, x, y);
    cursorSprite->active = 1;
    fileCursor.anchor = cursorSprite;
  }
  cursorStyle = 1;
  modeSprite = RenderResource_CreatePair(mode, fileList, 72, -24);

  while (1) {
    // Show the file under the cursor after it moves.
    if (redraw != 0) {
      redraw = 0;
      if (save->summary[file].level != 0) {
        PaletteGlow_Update(save->summary[file].paletteGlow,
                           save->summary[file].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = UiWindow_Create(1, 10, 14, 9, 2);
        SaveMenu_DrawFileSummary(summaryWindow, &save->summary[file]);
        WaitFrames(1);
        if (partyWindow == NULL)
          partyWindow = UiWindow_Create(16, 10, 13, 3, 2);
        SaveMenu_HidePartySprites();
        SaveMenu_ShowPartySprites(partyWindow, 0, 0, &save->summary[file]);
        WaitFrames(1);
        if ((s8)save->summary[file].djinn[0] +
                (s8)save->summary[file].djinn[1] +
                (s8)save->summary[file].djinn[2] +
                (s8)save->summary[file].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = UiWindow_Create(16, 14, 13, 5, 2);
          SaveMenu_DrawDjinnCounts(djinnWindow, &save->summary[file]);
          SaveMenu_HideDjinnSprites();
          SaveMenu_ShowDjinnSprites(djinnWindow, 0, 0);
        } else {
          SaveMenu_HideDjinnSprites();
          UiWork_Finalize(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        PaletteGlow_Update(gGameState.paletteGlow[0],
                           gGameState.paletteGlow[1]);
        SaveMenu_HideDjinnSprites();
        SaveMenu_HidePartySprites();
        UiWork_Finalize(djinnWindow, 2);
        UiWork_Finalize(partyWindow, 2);
        UiWork_Finalize(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
      x = fileList->x * 8 - 4;
      y = (fileList->y << 3) + file * 16 + 12;
      Shop_SetCursorFar(&fileCursor, x, y, cursorStyle);
      cursorStyle = 3;
    }
    ShopCursor_AdvanceFar(&fileCursor);
    Ui_ApplyTableOffsetToPair(modeSprite);
    WaitFrames(1);
    // Up and down move to the previous or next file the mode allows.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      redraw = 1;
      do {
        file = (file + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT; // previous
        if (mode == SAVE_MENU_SAVE)
          break;
      } while ((
          save->summary[file].level == 0 ||
          (mode == SAVE_MENU_CONTINUE &&
           (s8)save->summary[file].sendFlag != 0) ||
          (mode == SAVE_MENU_EXTRA && (s8)save->summary[file].flagCount == 0) ||
          (mode == SAVE_MENU_CLEARED &&
           (s8)save->summary[file].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      redraw = 1;
      do {
        file = (file + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT; // next
        if (mode == SAVE_MENU_SAVE)
          break;
      } while ((
          save->summary[file].level == 0 ||
          (mode == SAVE_MENU_CONTINUE &&
           (s8)save->summary[file].sendFlag != 0) ||
          (mode == SAVE_MENU_EXTRA && (s8)save->summary[file].flagCount == 0) ||
          (mode == SAVE_MENU_CLEARED &&
           (s8)save->summary[file].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = file;
      break;
    }
  }
  // Close the menu and return to the current game's palette glow.
  SaveMenu_HideDjinnSprites();
  SaveMenu_HidePartySprites();
  UiWork_Finalize(djinnWindow, 2);
  UiWork_Finalize(partyWindow, 2);
  UiWork_Finalize(summaryWindow, 2);
  UiWork_Finalize(fileList, 2);
  Runtime_ReleaseHeapBlock(55);
  PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  WaitFrames(1);
done:
  return result;
}
#endif

// The Save command: lets the player choose a file (the current one first),
// asks "OK to save over this file?" when it has data, then saves the game to
// it, object table included, and makes it the current file. Returns 0 (also
// when the player answers No), -1 when cancelled, or a SAVE_ERROR_* after
// showing its message.
s32 Save_WriteSelectedSlot(void) {
  s32 result;
  s32 file;
  s32 error;
  struct SaveWorkspace *save;

  result = 0;
  error = SaveState_InitializeWorkspace();
  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    SaveState_LoadSummaryRecords();
    save = gSaveWorkspace;
    file = SaveMenu_SelectSlot(gSaveSlot, SAVE_MENU_SAVE);
    if (file == -1) {
      result = file;
    } else {
      if (save->summary[file].level != 0) {
        UiText_ShowPositionedMessageAndWait(MsgOverwriteConfirm, 13);
        while (UiWork_IsComplete() == 0) {
          WaitFrames(1);
        }
        if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
          UiWork_FinalizePendingCore();
          goto done;
        }
        UiWork_FinalizePendingCore();
      }
      gSaveSlot = file;
      AudioCommand_PlayFar(85);
      UiText_ShowPositionedMessageAndWait(MsgSaving, 13);
      while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
      }
      SaveState_BuildSummaryHeader();
      SaveState_CaptureObjectTableFar();
      error = SaveState_WriteRecord(file, gSaveBuffer);
      error |= SaveState_WriteRecord(file + SAVE_FILE_COUNT,
                                     gSaveBuffer + SAVE_RECORD_BYTES);
      UiWork_FinalizePendingCore();
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgSaveFailed, 1);
        result = SAVE_ERROR_WRITE;
      } else {
        UiText_ShowPositionedMessageAndWait(MsgGameSaved, 9);
      }
    }
  }
done:
  SaveState_ReleaseWorkspace();
  return result;
}

// Lets the player choose a file in save menu `mode` (see SaveMenuMode) and
// loads it into the save buffer as the current file, restoring its play time
// and options. Returns 0, -1 when cancelled, or a SAVE_ERROR_* after showing
// its message.
s32 SaveState_LoadRecordIntoWork(s32 mode) {
  s32 result = 0;
  s32 error = SaveState_InitializeWorkspace();

  if (error != 0) {
    UiText_ShowPositionedMessageAndWait(MsgNoBackupMemory, 1);
    result = SAVE_ERROR_NO_MEMORY;
  } else {
    s32 file;

    SaveState_LoadSummaryRecords();
    file = SaveMenu_SelectSlot(gSaveSlot, mode);
    if (file == -1) {
      result = file;
    } else {
      void *half = &gSaveBuffer;

      error = SaveState_ReadRecordPayload(file, half);
      half = (char *)half + SAVE_RECORD_BYTES;
      error |= SaveState_ReadRecordPayload(file + SAVE_FILE_COUNT, half);
      if (error != 0) {
        UiText_ShowPositionedMessageAndWait(MsgLoadFailed, 1);
        result = SAVE_ERROR_READ;
      } else {
        gPlayTimeFrames = gGameState.playTime;
        gIdleSleepEnabled = gGameState.idleSleepEnabled;
        gIdleFrames = 0;
        gSaveSlot = file;
      }
    }
  }
  SaveState_ReleaseWorkspace();
  return result;
}

// Loads the pointing hand cursor's tiles (gResource_FixedBlockBTiles, 128
// bytes) into a free VRAM slot and returns the slot.
s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  VramCache_Load(slot, 0x80, gResource_FixedBlockBTiles);
  return slot;
}

#if EDITION_INTERNATIONAL
// Always 1: the international name entry has no voicing marks to check.
s32 Resource_ReturnTrue(void) { return 1; }
#else

// The kana voicing marks of the Japanese name entry, which join the letter
// before them.
#define NAME_VOICED_MARK 0xde     // dakuten
#define NAME_SEMIVOICED_MARK 0xdf // handakuten

// Returns whether `mark` may follow `letter` in a Japanese name: any
// character but a voicing mark may, and a voicing mark only after a letter
// in its table of ranges (pairs of first and last letter, ended by 0).
s32 NameEntry_AcceptsMark(s32 mark, s32 letter) {
  const u8 *range;

  if (mark < NAME_VOICED_MARK || mark > NAME_SEMIVOICED_MARK)
    return 1;
  if (mark == NAME_VOICED_MARK)
    range = gNameEntry_VoicedMarkRanges;
  else
    range = gNameEntry_SemivoicedMarkRanges;
  for (; *range != 0; range += 2) {
    if (letter >= range[0] && letter <= range[1])
      return 1;
  }
  return 0;
}

// Returns how many letters the Japanese name `name` has, not counting the
// voicing marks.
s32 NameEntry_CountLetters(const u8 *name) {
  s32 count;

  count = 0;
  for (; *name != 0; name++) {
    // Neither NAME_VOICED_MARK nor NAME_SEMIVOICED_MARK.
    if ((u8)(*name - NAME_VOICED_MARK) > 1)
      count++;
  }
  return count;
}
#endif

// Decompresses the type-01 tilemap `source` and copies it over `window`'s
// rectangle, both into the BG screen block at 0x06002000 and into the UI
// tilemap that mirrors it.
void UiWindow_CopyTilemapRegion(const struct UiWindow *window,
                                const void *source) {
  struct UiRenderWork *work = UI_WORK;
  s16 *mirror = (s16 *)work->tilemap;
  s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
  s16 *input = buffer;
  u32 cell;
  s16 *vram;
  s32 row;

  Resource_DecodeType01(source, buffer);
  cell = window->y * 32 + window->x;
  vram = (s16 *)0x06002000 + cell;
  mirror += cell;

  for (row = 0; row < window->height; row++) {
    s32 column;

    for (column = 0; column < window->width; column++) {
      s16 value = *input++;

      *vram++ = value;
      *mirror++ = value;
    }
    vram += 32 - window->width;
    mirror += 32 - window->width;
  }
  MemFree(buffer);
}

// Like UiWindow_SetTileAttributeBitRect, but ORs all of `palette` into the
// palette number of each entry (bits 12-15) after clearing only its lowest
// bit.
void UiWindow_SetTileAttributeRect(const struct UiWindow *window, s32 x, s32 y,
                                   s32 width, s32 height, u32 palette) {
  u8 *tilemap = gWindowWork[0];

  x += window->x + 1;
  y += window->y + 1;
  palette <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    y <<= 6;
    x = y + (x << 1);
    do {
      u16 *cell = (u16 *)(x + (u32)tilemap);
      s32 remaining = width;
      while (remaining != 0) {
        u32 entry = *cell;
        entry &= ~0x1000;
        entry |= palette;
        remaining--;
        *cell = entry;
        cell++;
      }
      height--;
      x += 64;
    } while (height != 0);
    ((struct UiRenderWork *)tilemap)->dirty = 1;
  }
}

// Loads the pointing hand cursor's tiles into VRAM slot `slot`.
void Resource_CopyFixedBlockB(s32 slot) {
  VramCache_Load(slot, 0x80, gResource_FixedBlockBTiles);
}

// Loads the 16x16 sprite gResource_FixedBlockATiles into VRAM slot `slot`.
void Resource_CopyFixedBlockA(s32 slot) {
  VramCache_Load(slot, 0x80, gResource_FixedBlockATiles);
}

// The name entry screen: a keyboard window of letter keys, with a column of
// command keys on its right (Delete and End, and in Japanese the keyboard
// switch), the name above it and the character's portrait.
#define NAME_COMMAND_COLUMN 18 // the keyboard column of the command keys
#define NAME_MAX_LETTERS 5

#if EDITION_INTERNATIONAL
// Puts the string `str` into the UI's text entries and returns its width in
// pixels.
s32 UiText_SetRenderString(const u8 *str) {
  struct UiRenderWork *work;
  u16 *dst;
  s32 count;
  u32 width;
  u32 height;

  work = UI_WORK;
  count = 0;
  if (*str != 0) {
    dst = work->entries;
    do {
      *dst = *str;
      str++;
      dst++;
      count++;
    } while (*str != 0);
  }
  work->entries[count] = 0;
  UiText_MeasureEntryDimensions(0, &width, &height, 0);
  return width;
}

// The text control code that sets the colour of the text after it, from the
// byte that follows ({text_color:N} in the message catalogs).
#define TEXT_COLOR_COMMAND 8

// Draws the name being entered, `name`, at the top left of `window`, with
// the free places up to its seventh character shown as underscores in text
// colour 2.
void UiText_DrawPaddedLabel(struct UiWindow *window, u8 *name) {
  u8 text[20];
  s32 length = 0;

  if (*name != 0) {
    do {
      text[length] = *name;
      name++;
      length++;
    } while (*name != 0);
  }

  text[length++] = TEXT_COLOR_COMMAND;
  text[length++] = 2;

  while (length <= 6) {
    text[length++] = '_';
  }

  text[length++] = TEXT_COLOR_COMMAND;
  text[length++] = 15;
  text[length] = 0;
  UiText_DrawString(text, window, 0, -2);
}

// The width of the window showing the name, in tiles.
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

// Whether the edition draws the Delete and End key labels as text over the
// keyboard, and how wide its End key is, in tiles.
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if defined(TBS_EDITION_DE)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

// The name entry screen for party member `member`: the player types a name
// of up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or
// Start, which jumps there). A puts the key under the cursor into the name,
// B or Delete removes the last letter, and B on an empty name cancels. Ending
// with an empty name keeps the old one. Returns 0, or -1 when cancelled.
s32 NameEntry_EditOwnerName(s32 member) {
  struct UiWindow *label;
  s32 result = 0;
  s32 originalLength = 0;
  s32 length = 0;
  u8 text[16];
  u8 *input = text + 1;
  u8 *name = (u8 *)UnitGetFar(member);
  u8 *uiTilemap = gWindowWork[0];
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct UiWindow *window;
  union MenuCursor cursor;
  union MenuCursor caret;
  s32 column, row, vramSlot;
  struct RenderOutput *output;
  u8 *end;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Ui_LoadWindowGraphics();
  window = (struct UiWindow *)UiWindow_Create(3, 6, 24, 9, 2);
  label = (struct UiWindow *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
  UiWindow_CreateWithSideObject(Localization_LookupEntryId(member), 0, 3, 1);
  UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
  UiText_DrawStringInWindow(gNameEntry_DeleteKeyLabel, window, 144, 32);
  UiText_DrawStringInWindow(gNameEntry_EndKeyLabel, window, 144, 40);
#endif
  UiWindow_DrawDividerLine(window, 18, 0, 18, 7);
  ((struct UiRenderWork *)uiTilemap)->dirty = textDirty;
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < 14; i++) {
      u32 c = name[i];

      input[i] = c;
      if (c != 0) {
        originalLength++;
        length++;
      }
    }
  }
  input[14] = 0;
  UiText_DrawPaddedLabel(label, name);
  vramSlot = Resource_FindFreeEntry();
  column = NAME_COMMAND_COLUMN;
  row = 5;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    output = RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, window, 0, 0);
    cursor.output = output;
    ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 + 140,
                                       window->y * 8 + 52);
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockATiles);
    output = RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, window, 0, 0);
    caret.output = output;
    output->sentinel = 255;
    ((struct FieldSprite *)output->unk10)->priority = 0;
    ShopCursor_SetPositionImmediateFar(&caret.cursor,
                                       UiText_SetRenderString(input) + 70, 22);
  }
  // The key under the cursor flashes between palette 15 and palette 14, a
  // copy of it with colour 4 grey.
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  end = (u8 *)(originalLength + (s32)input);
  while (1) {
    s32 width = 1;

    if (column == NAME_COMMAND_COLUMN) {
      if (row == 4)
        width = 3;
      if (row == 5)
        width = NAME_END_KEY_WIDTH;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
    if (cursorDirty) {
      cursorDirty = 0;
      Shop_SetCursorFar(&cursor.cursor, (window->x + column) * 8 - 7,
                        (window->y + row) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      Shop_SetCursorFar(&caret.cursor, UiText_SetRenderString(input) + 70, 22,
                        3);
    }
    ShopCursor_AdvanceFar(&cursor.cursor);
    ShopCursor_MoveTowardTargetFar(&caret.cursor);
    {
      u32 frame = (gFrameTick >> 1) & 7;

      output = caret.output;
      ((struct FieldSprite *)output->unk10)->x =
          output->x + gNameEntry_CaretWave[frame];
      ((struct FieldSprite *)output->unk10)->y =
          output->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    // The D-pad moves over the keys, skipping the gaps between groups and
    // wrapping around; Start jumps to End.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      row--;
      if (column != NAME_COMMAND_COLUMN) {
        if (row == -1)
          row = 5;
      } else {
        row = (row != 3);
        row = 5 - row;
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      row++;
      if (column != NAME_COMMAND_COLUMN) {
        if (row == 6)
          row = 0;
      } else {
        row = (row != 6);
        row += 4;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column--;
      if (column == -1) {
        column = NAME_COMMAND_COLUMN;
        if (row < 4 || row > 5)
          column = 16;
      } else if (column == 5 || column == 11 || column == 17) {
        column--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column++;
      if (column == 19)
        column = 0;
      else if (column == 5 || column == 11 || column == 17)
        column++;
      if (column == NAME_COMMAND_COLUMN && (row < 4 || row > 5))
        column = 0;
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column = NAME_COMMAND_COLUMN;
      row = 5;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    remove_character:
      if (length != 0) {
        length--;
        *--end = 0;
        RenderOutput_PrepareForRedraw(label);
        UiText_DrawPaddedLabel(label, input);
        textDirty = 1;
        continue;
      }
      result = -1;
      break;
    }
    // A: a command key, or a letter.
    if (!(gKeysRepeat & KEY_A))
      continue;
    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    if (column == NAME_COMMAND_COLUMN) {
      if (row == 5) {
        if (length == 0) {
          caret.output->active = 13;
          RenderOutput_PrepareForRedraw(label);
          UiText_DrawPaddedLabel(label, name);
          WaitFrames(10);
        } else {
          u8 *dst = name;
          u8 *src = input;
          s32 i = 0;
          u8 *dstSave = dst;
          u8 *srcSave = src;

          /* FAKEMATCH: the split restores end the pointer setup's
           * CSE blocks. They disappear, preserving destination/
           * source/index in r2/r1/r0; removing the barrier alone
           * changes them to r1/r0/r2 (14 instructions differ). A for loop
           * copying name[i] = input[i], or walking the pointers,
           * differs 14; the while form with i declared first, 12. */
          if (name)
            src = NULL;
          if (name)
            src = srcSave;
          if (name)
            dst = NULL;
          if (name)
            dst = dstSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (row == 4)
        goto remove_character;
      continue;
    }
    {
      u32 tx = window->x + column + 1;
      u32 cell;
      u32 letter;

      cell = ((window->y + row + 1) << 5) + tx;
      letter = uiTilemap[cell * 2];
      if (length == NAME_MAX_LETTERS)
        continue;
      *end++ = letter;
      *end = 0;
      length++;
      if (length == NAME_MAX_LETTERS) {
        column = NAME_COMMAND_COLUMN;
        row = 5;
        cursorDirty = 1;
      }
      RenderOutput_PrepareForRedraw(label);
      UiText_DrawPaddedLabel(label, input);
      textDirty = 1;
    }
  }
  UiWork_Finalize(window, 2);
  UiWork_Finalize(label, 2);
  UiWork_FinalizeEntityMatchingLocalizedId(member);
  WaitFrames(1);
  return result;
}
#else

// The name entry screen for party member `member`: the player types a name
// of up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or
// Start, which jumps there). A puts the key under the cursor into the name,
// B or Delete removes the last letter, and B on an empty name cancels. Ending
// with an empty name keeps the old one. Returns 0, or -1 when cancelled.
s32 NameEntry_EditOwnerName(s32 member) {
  struct UiWindow *label;
  s32 result = 0;
  s32 keyboard = 0;
  s32 bytes = 0;
  s32 length = 0;
  u8 text[16];
  u8 *input = text + 1;
  u8 *name = (u8 *)UnitGetFar(member);
  u8 *uiTilemap = gWindowWork[0];
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct UiWindow *window;
  union MenuCursor cursor;
  union MenuCursor caret;
  s32 column, row, vramSlot, x;
  struct RenderOutput *output;
  u32 letter;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Ui_LoadWindowGraphics();
  window = (struct UiWindow *)UiWindow_Create(3, 6, 24, 13, 2);
  label = (struct UiWindow *)UiWindow_Create(8, 3, 8, 3, 2);
  UiWindow_CreateWithSideObject(Localization_LookupEntryId(member), 0, 3, 1);
  UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
  UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
  ((struct UiRenderWork *)uiTilemap)->dirty = textDirty;
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < 14; i++) {
      letter = name[i];

      input[i] = letter;
      if (letter != 0) {
        bytes++;
        length++;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1)
          length--;
      }
    }
  }
  input[14] = 0;
  UiText_DrawString(name, label, 0, 0);
  column = NAME_COMMAND_COLUMN;
  vramSlot = Resource_FindFreeEntry();
  row = 10;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    output = RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, window, 0, 0);
    cursor.output = output;
    ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 + 140,
                                       window->y * 8 + 92);
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockATiles);
    output = RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, window, 0, 0);
    caret.output = output;
    output->sentinel = 255;
    ((struct FieldSprite *)output->unk10)->priority = 0;
    x = 72 + NameEntry_CountLetters(input) * 10;
    ShopCursor_SetPositionImmediateFar(&caret.cursor, x, 22);
  }
  // The key under the cursor flashes between palette 15 and palette 14, a
  // copy of it with colour 4 grey.
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    s32 width = 1;

    if (column == NAME_COMMAND_COLUMN) {
      if (row == 0)
        width = 4;
      if (row == 8)
        width = 3;
      if (row == 10)
        width = 3;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
    if (cursorDirty) {
      cursorDirty = 0;
      Shop_SetCursorFar(&cursor.cursor, (window->x + column) * 8 - 7,
                        (window->y + row) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      x = 72 + NameEntry_CountLetters(input) * 10;
      Shop_SetCursorFar(&caret.cursor, x, 22, 3);
    }
    ShopCursor_AdvanceFar(&cursor.cursor);
    ShopCursor_MoveTowardTargetFar(&caret.cursor);
    {
      u32 frame = (gFrameTick >> 1) & 7;

      output = caret.output;
      ((struct FieldSprite *)output->unk10)->x =
          output->x + gNameEntry_CaretWave[frame];
      ((struct FieldSprite *)output->unk10)->y =
          output->y + gNameEntry_CaretWave[(frame + 5) & 7];
    }
    // The D-pad moves over the keys, skipping the gaps between groups and
    // wrapping around; Start jumps to End.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      row--;
      if (column != NAME_COMMAND_COLUMN) {
        if (row == -1)
          row = 10;
        else if (row == 5)
          row = 4;
      } else {
        if (row == -1) {
          row = 10;
        } else {
          row = (row != 7);
          row *= 8;
        }
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      row++;
      if (column != NAME_COMMAND_COLUMN) {
        if (row == 11)
          row = 0;
        if (row == 5)
          row = 6;
      } else {
        if (row == 1)
          row = 8;
        else if (row == 9)
          row = 10;
        else
          row = 0;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column--;
      if (column == -1) {
        if (row == 0 || row == 8 || row == 10)
          column = NAME_COMMAND_COLUMN;
        else
          column = 16;
      } else if (column == 5 || column == 11 || column == 17) {
        column--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column++;
      if (column == 19)
        column = 0;
      else if (column == 5 || column == 11 || column == 17)
        column++;
      if (column == NAME_COMMAND_COLUMN) {
        if (row == 0 || row == 8 || row == 10)
          column = NAME_COMMAND_COLUMN;
        else
          column = 0;
      }
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      column = NAME_COMMAND_COLUMN;
      row = 10;
    }
    if (gKeysPressed & KEY_SELECT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    change_keyboard:
      keyboard ^= 1;
      if (keyboard)
        UiWindow_CopyTilemapRegion(window, gNameEntry_AlternateKeyboardTilemap);
      else
        UiWindow_CopyTilemapRegion(window, gNameEntry_KeyboardTilemap);
      UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
      ((struct UiRenderWork *)uiTilemap)->dirty = 1;
      continue;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    remove_character:
      if (length != 0) {
        length--;
        bytes--;
        letter = input[bytes];
        input[bytes] = 0;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1) {
          bytes--;
          input[bytes] = 0;
        }
      } else {
        result = -1;
        break;
      }
      RenderOutput_PrepareForRedraw(label);
      UiText_DrawString(input, label, 0, 0);
      textDirty = 1;
      continue;
    }
    // A: a command key, or a letter.
    if (!(gKeysRepeat & KEY_A))
      continue;
    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    if (column == NAME_COMMAND_COLUMN) {
      if (row == 10) {
        if (length == 0) {
          caret.output->active = 13;
          RenderOutput_PrepareForRedraw(label);
          UiText_DrawString(name, label, 0, 0);
          WaitFrames(10);
        } else {
          u8 *dst;
          u8 *src;
          s32 i;
          u8 *dstSave;
          u8 *srcSave;

          dst = name;
          src = input;
          i = 0;
          /* FAKEMATCH: the split restores end the pointer setup's
           * CSE blocks. They disappear, preserving destination/
           * source/index in r1/r2/r0; removing the barrier alone
           * changes them to r2/r0/r1 (14 instructions differ). A for loop
           * copying name[i] = input[i] differs 18, one walking the
           * pointers 14; the while form with i declared first, 12. */
          dstSave = dst;
          srcSave = src;
          if (window->height)
            dst = NULL;
          if (window->height)
            dst = dstSave;
          if (window->height)
            src = NULL;
          if (window->height)
            src = srcSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (row == 8)
        goto remove_character;
      if (row == 0)
        goto change_keyboard;
      continue;
    }
    {
      u32 tx = window->x + column + 1;
      u32 cell;
      u8 *dst;

      cell = ((window->y + row + 1) << 5) + tx;
      letter = uiTilemap[cell * 2];
      if (letter == 0xfe) // the keyboard's dakuten key
        letter = NAME_VOICED_MARK;
      if (letter == 0xff) // the keyboard's handakuten key
        letter = NAME_SEMIVOICED_MARK;
      if (length == NAME_MAX_LETTERS && (u8)(letter - NAME_VOICED_MARK) > 1)
        continue;
      dst = input + bytes;
      if (!NameEntry_AcceptsMark(letter, input[bytes - 1])) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        continue;
      }
      *dst = letter;
      bytes++;
      input[bytes] = 0;
      length++;
      if ((u8)(letter - NAME_VOICED_MARK) <= 1)
        length--;
      if (length == NAME_MAX_LETTERS) {
        column = NAME_COMMAND_COLUMN;
        row = 10;
        cursorDirty = 1;
      }
      RenderOutput_PrepareForRedraw(label);
      UiText_DrawString(input, label, 0, 0);
      textDirty = 1;
    }
  }
  UiWork_Finalize(window, 2);
  UiWork_Finalize(label, 2);
  UiWork_FinalizeEntityMatchingLocalizedId(member);
  WaitFrames(1);
  return result;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif defined(TBS_EDITION_ES)
#define TALK_WINDOW_X 9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X 8
#define TALK_WINDOW_WIDTH 16
#endif

// The Colosso choice of what to do: "Descriptions", "Cheer" or "Nothing",
// chosen with a pointing hand. Returns the choice (0-2), or -1 when
// cancelled with B.
s32 PartyTalkMenu_Choose(s32 unused) {
  /* FAKEMATCH: the shared cursor union preserves pointer-store ordering; an
   * equal-size struct changes stack-address construction in all six editions.
   */
  struct UiWindow *window;
  union MenuCursor cursor;
  s32 slot;
  s32 row;
  s32 moved;
  u16 *palette;

  moved = 1;
  window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
  UiText_DrawCharacterAtOffset(MsgTalkChoice, window, 8, 0);
  UiText_DrawCharacterAtOffset(MsgTalkChoice + 1, window, 8, 16);
  UiText_DrawCharacterAtOffset(MsgTalkChoice + 2, window, 8, 32);
  slot = Resource_FindFreeEntry();
  row = 0;
  if (slot < 96) {
    VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
    cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
    ShopCursor_SetPositionImmediateFar(&cursor.cursor, window->x * 8 - 3,
                                       window->y * 8 + 9);
  }
  // The choice under the cursor flashes between palette 15 and palette 14,
  // a copy of it with colour 4 grey.
  palette = (u16 *)0x050001c0;
  Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                                  14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                                  15);
    if (moved) {
      moved = 0;
      Shop_SetCursorFar(&cursor.cursor, window->x * 8 - 3,
                        (window->y + row * 2) * 8 + 9, 3);
    }
    ShopCursor_AdvanceFar(&cursor.cursor);
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(111);
      row--;
      moved = 1;
      if (row == -1)
        row = 2;
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(111);
      row++;
      moved = 1;
      if (row == 3)
        row = 0;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(113);
      row = -1;
      break;
    }
    if (gKeysRepeat & KEY_A) {
      AudioCommand_PlayFar(112);
      break;
    }
  }
  UiWork_Finalize(window, 2);
  WaitFrames(1);
  Resource_ResetEntry(slot);
  return row;
}

// Shows "The <element> Djinni <name> joined <member>." with the element's
// Djinni portrait while the join fanfare (sound 81) plays, until it ends or A,
// B, L or R is pressed. `djinn` is the Djinni's number in its element.
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn) {
  struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
  s32 name;
  struct UiWindow *window;
  s32 sprite[3];
  s32 *entry;
  s32 palette, handle;
  s32 id;
  s32 text;
  u32 zero;
  s32 *p;

  window = NULL;
  zero = 0;
  name = gDjinn_ElementSprites[element & 3];
  entry = sprite;
  window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = Localization_LookupEntryId(name);
    UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
    *p = palette | 0xe000;

    work->result[0] = zero;
    work->result[1] = zero;

    // The Djinni's name is ability 300 + 20 * element + djinn.
    UiWork_PushValueSlot(member, 1);
    UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

    text = UiText_BuildRenderEntriesMode1(element + MsgVenusDjinnJoined);

    UiText_QueueRenderEntries(window, text, 36, 2, zero);

    AudioCommand_PlayFar(81);

    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}

// Returns the portrait character of party member `member` (0-8): from
// gCharacter_ChildSpriteTable while game flag 32 is set, from
// gCharacter_SpriteTable otherwise; 0 for any other member.
s32 Party_LookupCharacterValueByFlag32(u32 index) {
  if (index > 8) {
    return 0;
  }
  if (GameFlag_TestFar(32) == 0) {
    return gCharacter_SpriteTable[index];
  }
  return gCharacter_ChildSpriteTable[index];
}

// Shows "<member> joined your party." with the member's portrait while the
// join fanfare (sound 81) plays, until it ends or A, B, L or R is pressed.
void Party_ShowJoinedMessage(s32 member) {
  struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
  struct UiWindow *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 palette, handle;
  s32 *p;

  window = UiWindow_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    UiGlyph_LoadEntryWithPalette(
        Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)),
        0, &handle, &palette, 14, 0);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;
    work->result[0] = 0;
    work->result[1] = 0;
    UiWork_PushValueSlot(member, 1);
    UiText_QueueRenderEntries(
        window, UiText_BuildRenderEntriesMode1(MsgJoinedParty), 36, 2, 0);
    AudioCommand_PlayFar(81);
    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}

// Shows "<first> and <second> joined your party." with both portraits while
// the join fanfare (sound 81) plays, until it ends or A, B, L or R is pressed.
void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  u8 *work = gWindowWork[0];
  struct UiWindow *window;
  s32 sprite2[3];
  s32 sprite1[3];
  s32 *entry1;
  s32 *entry2;
  s32 handle1, palette, handle2;
  u32 zero;
  s32 *p;

  // The window starts as NULL, though it is assigned before it is read; the
  // same zero is the style argument below.
  window = NULL;
  entry1 = sprite1;
  window = UiWindow_Create(1, 1, 28, 5, 0);
  zero = 0;

  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

    ((struct UiRenderWork *)work)->dirty = 1;

    UiGlyph_LoadEntryWithPalette(
        Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(first)),
        0, &handle1, &palette, 14, zero);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry1;
    *p++ = zero;
    *p++ = 0x800c000c;
    *p = palette | 0xe000;

    entry2 = sprite2;
    UiGlyph_LoadEntryWithPalette(
        Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(second)),
        0, &handle2, &palette, 15, zero);
    p = entry2;
    *p++ = zero;
    *p++ = 0x802c000c;
    *p = palette | 0xf000;

    ((struct UiRenderWork *)work)->result[0] = zero;
    ((struct UiRenderWork *)work)->result[1] = zero;

    UiWork_PushValueSlot(first, 1);
    UiWork_PushValueSlot(second, 1);

    UiText_QueueRenderEntries(
        window, UiText_BuildRenderEntriesMode1(MsgPairJoinedParty), 68, 2,
        zero);

    AudioCommand_PlayFar(81);

    do {
      Runtime_PushSlotEntry(entry1, 250);
      Runtime_PushSlotEntry(entry2, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle1);
    Resource_ResetEntry(handle2);
  }
}

// Decompresses picture `picture` of gRenderResource_PairSourceTable (two
// 32x32 sprites side by side, 512 bytes) into VRAM slot `slot`. The heap
// block is not freed when `slot` is not a valid VRAM slot.
void RenderResource_LoadPair(s32 picture, s32 slot) {
  void *stagingBuffer = Runtime_AllocateBlock(14, 0x400);
  const u8 *resource = gRenderResource_PairSourceTable[picture];

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Resource_DecodeByteLz(resource, stagingBuffer);
    VramCache_Load(slot, 0x200, stagingBuffer);
    Runtime_ReleaseHeapBlock(14);
  }
}

// Shows picture `picture` of gRenderResource_PairSourceTable as two 32x32
// sprites side by side at (x, y) in `window`, on draw layer 253, and
// returns the left one (the right one follows it in the window's list).
// Returns NULL when no VRAM slot is free.
struct RenderOutput *
RenderResource_CreatePair(s32 picture, struct UiWindow *window, s32 x, s32 y) {
  s32 slot;
  struct RenderOutput *first;
  struct RenderOutput *second;

  slot = Resource_FindFreeEntry();
  if (slot >= VRAM_CACHE_ENTRY_COUNT)
    return NULL;

  // 0x4000 is the wide shape bit of attribute 0, with size 2: 64x32.
  RenderResource_LoadPair(picture, slot);
  first = RenderOutput_Create(slot, 0x80004000, window, x, y);
  first->sentinel = 0xFD;
  second = RenderOutput_Create(slot, 0x80004000, window, x + 32, y);
  second->sentinel = 0xFD;
  second->oam.index += 8; // the right half's tiles
  return first;
}

// Bobs the two sprites of a pair (see RenderResource_CreatePair) up and down
// by gUi_PairBobOffsets, one step every four frames.
void Ui_ApplyTableOffsetToPair(struct RenderOutput *obj) {
  s32 value;

  value = (u8)obj->y + gUi_PairBobOffsets[(gFrameTick >> 2) & 7];
  obj->oam.y = value;
  obj = obj->next;
  /* FAKEMATCH: the volatile read loads gFrameTick again for the second sprite,
   * as the bytes do; read plainly, CSE reuses the first index and the function
   * differs 33, 15 instructions for 22. */
  value =
      (u8)obj->y + gUi_PairBobOffsets[(*(volatile u32 *)&gFrameTick >> 2) & 7];
  obj->oam.y = value;
}

// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
// into VRAM slot `slot`, remapping its first 768 bytes' colours by table
// when `remap` is nonzero.
void RenderResource_LoadFrame(s32 icon, s32 slot, s32 remap) {
  s32 size = 1024;
  void *buffer = Runtime_AllocateBlock(14, size);
  u16 *icons = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Resource_DecodeByteLz((void *)((u32)icons + icons[icon]), buffer);
    if (remap != 0)
      Runtime_RemapBytesByTableFar(buffer, 768);
    VramCache_Load(slot, size, buffer);
    Runtime_ReleaseHeapBlock(14);
  }
}

// Shows command icon `icon` (see RenderResource_LoadFrame) as a 32x32
// 256-colour sprite at (x, y) in `window`, on draw layer 251. Returns NULL
// when no VRAM slot is free.
struct RenderOutput *RenderResource_CreateFrame(s32 icon, s32 remap,
                                                struct UiWindow *window, s32 x,
                                                s32 y) {
  s32 slot;
  struct RenderOutput *output;

  slot = Resource_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    RenderResource_LoadFrame(icon, slot, remap);
    output = RenderOutput_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
    output->oam.palette256 = 1;
    output->sentinel = 0xfb;
  }
  return output;
}

// Makes the sprite `obj` pulse: scales it about its centre by
// gUi_ObjectPulseScales (in 1/65536, used in 1/256), one step every two
// frames, and puts it on draw layer 252. Does nothing when `obj` is NULL.
void Ui_ApplyTableScaleToObject(struct RenderOutput *obj) {
  s32 v = gUi_ObjectPulseScales[(gFrameTick >> 1) & 7];
  struct AffineTransform effect;

  if (v < 0)
    v += 255;
  v >>= 8;

  if (obj != 0) {
    effect.scaleX = v;
    effect.scaleY = v;
    effect.angle = 0;
    obj->oam.matrix = AffineMatrix_BuildForEffect(&effect);
    obj->oam.affineMode = 3;
    obj->oam.x = obj->x + 0xfff0;
    obj->oam.y = (u8)obj->y + 0xf0;
    obj->sentinel = 0xfc;
  }
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void) {
#else
s32 Menu_BuildLocalizedPatternTiles(void) {
#endif
  u32 *vram = (u32 *)0x06006280;
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = vram + set * 0x60 + n * 0x10;
      s32 x;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (x = 1; x <= 7; x++) {
        s32 mi = n;

        if (set == 1 && x <= 1) {
          continue;
        }
        if (set == 0 && n > x - 2) {
          mi = x - 2;
          if (mi < 0) {
            mi = 0;
          }
        }
        tile[x] = XorWord(tile[x], gMenu_BarPatternRows[mi].word0);
        tile[x + 8] = XorWord(tile[x + 8], gMenu_BarPatternRows[mi].word1);
      }
    }
  }
}

// Void changes pop {r1}; bx r1 to r0 and reorders a preview's argument setup.
#ifdef UBFIX
void UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 arg1, s32 arg2,
                                  s32 arg3, s32 unused) {
#else
int UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 arg1, s32 arg2,
                                 s32 arg3, s32 unused) {
#endif
  s32 tileOffset = arg3 * 2;
  s32 tile = tileOffset + 0xF315;

  UiWindow_SetTilemapEntry(window, 0x400 | tile, arg1, arg2, 0);
  UiWindow_SetTilemapEntry(window, tileOffset + 0xF314, arg1 + 1, arg2, 0);
  UiWindow_SetTilemapEntry(window, tile, arg1 + 2, arg2, 0);
}

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift) {
  s32 neg;
  s32 rshift;
  s32 lshift;
  s32 i;

  neg = -shift;
  i = 0;
  rshift = neg * 4;
  lshift = shift * 4;

  do {
    u32 result;
    u32 shifted;
    u32 fallback;
    s32 j;

    shifted = *second++;
    result = 0;
    fallback = *first++;
    if (shift < 0)
      shifted >>= rshift;
    else
      shifted <<= lshift;

    j = 7;
    do {
      result <<= 4;
      if (shifted > 0x0FFFFFFF)
        result += shifted >> 28;
      else
        result += fallback >> 28;
      j--;
      shifted <<= 4;
      fallback <<= 4;
    } while (j >= 0);

    *output++ = result;
    i++;
  } while (i <= 7);
}

void Link_DrawShiftedTilePair(s32 offset) {
  s32 phase = (gFrameCount >> 2) & 3;

  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase++;
  Graphics_MergeShiftedTileRows((u32 *)0x06000220, (u32 *)gRomShiftedTilePair,
                                (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows((u32 *)0x06000240,
                                (u32 *)(gRomShiftedTilePair + 32),
                                (u32 *)(offset + 32), phase);
}

void Graphics_ExpandVramTilesByColorTable(u16 *dst) {
  s32 bank = 0;
  s32 palOffset = 0;
  s32 dstBank = 0;

  do {
    s32 row = 0;
    s32 dstOffset = (dstBank + bank) << 6;

    do {
      u16 *out = (u16 *)((u8 *)dst + dstOffset);
      u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
      s32 col = 0;

      do {
        u32 packed = *src++;
        u32 dec = 0;
        s32 nibble = 0;

        do {
          u32 color = gGraphics_ExpandNibbleTable[(packed & 15) + palOffset]
                      << (nibble * 4);
          nibble++;
          packed >>= 4;
          dec |= color;
        } while (nibble <= 3);

        col++;
        *out++ = dec;
      } while (col <= 15);

      row++;
      dstOffset += 32;
    } while (row <= 9);

    palOffset += 16;
    dstBank += 4;
    bank++;
  } while (bank <= 1);
}

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  Ui_PrepareTransferFromTableEntry(resource);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  ItemIcon_Compose(resource, 0x1a);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index) {
  s32 out;
  s32 cur;
  s32 ret;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  cur = index;
  Ability_LoadGlyph(resource, 0, &cur, &out, 1);
  ret = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return ret;
}

s32 Ui_LoadEntryForKind(u32 kind, s32 value) {
  s32 out;
  s32 cur;
  u32 no;

  no = kind;
  cur = value;
  if (no > 7U) {
    no = 0;
  }
  if (GameFlag_TestFar(0x20) != 0) {
    switch (no) {
    case 0:
      no = 0x38;
      break;
    case 1:
      no = 0x39;
      break;
    }
  }
  UiGlyph_LoadEntryWithPalette(no, 0, &cur, &out, 0xE, 1);
  return out;
}

//--------------------------------------------------------------------------
// get_table_word_zero.c
// Window: return the start of the fixed tile table.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Return gUi_FixedTileBlocks[0]; treat any nonzero no as 0 as well.
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no) {
  if (no != 0)
    no = 0;
  return gUi_FixedTileBlocks[no];
}

void Ui_RunCopiedRoutine(GlyphTransfer *work) {

  RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
  ((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
      (s32)work->encoded, (u8 *)work);
  Runtime_ReleaseHeapBlock(49);
}

//--------------------------------------------------------------------------
// create_countdown_label_window.c
// Link: open the remaining-time heading window.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Open a 6x4 window at the upper left, write its heading and return it.
//--------------------------------------------------------------------------
struct UiWindow *Link_CreateCountdownLabelWindow(void) {
  struct UiWindow *win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) ||                      \
    defined(TLA_EDITION_IT)
  UiText_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
  UiText_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
  return win;
}

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1) {
  GlyphTransfer *buffer =
      Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **slot = &buffer->encoded;
  void *target = (void *)((u32)base + base[arg1]);
  s32 ret;

  *slot = target;
  Resource_DecodeByteLz(target, buffer);
  ret = VramCache_Reload(arg0, (s32)buffer);
  Runtime_ReleaseHeapBlock(0x11);
  return ret;
}

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph,
                              s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *src;
  u8 *dst;
  s32 i;
  u32 index;

  decoded = Runtime_AllocateHeapBlock(17, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Resource_DecodeByteLz(decoded->encoded, decoded);
  remapped = MemAlloc(0x400);
  src = decoded->input;
  dst = remapped;
  for (i = 0; i < 0x400; i++) {
    index = *src++;
    if (palette->slot[index] == 255) {
      palette->slot[index] = palette->count;
      if (palette->count < 64) {
        ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
        palette->count++;
      }
    }
    *dst++ = palette->slot[index];
  }
  Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, REG_DMA3);
  MemFree(remapped);
  Runtime_ReleaseHeapBlock(17);
}

struct SelectionEntry {
  struct MenuSprite sprite;
  u16 resource, unused;
  s32 posX, posY;
  s32 kind;
};

struct SelectionWork {
  struct GlyphPalette palette;
  struct SelectionEntry entries[6];
  struct UiWindow *window;
  s32 count;
  struct AffineTransform affine;
  s32 matrix, selection, previous;
  s32 kinds[6];
};

void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind) {
  struct SelectionEntry *entry = &work->entries[index];

  UiText_LoadRemappedGlyph(&work->palette, kind, index * 16);
  work->entries[index].kind = kind;
  MenuSprite_Init(entry->sprite, 0x80002000);
  entry->sprite.oam.f.tile =
      Resource_LoadIndexedIntoBuffer(work->entries[index].resource, kind);
}

void Graphics_SetBg1Priority3(void) {
  u32 value = REG_BG1CNT;

  value &= ~3;
  value |= 3;
  REG_BG1CNT = value;
}

void Graphics_ClearBg1ControlBit2(void) {
  u32 value = REG_BG1CNT;

  value &= ~3;
  REG_BG1CNT = value;
}

void Graphics_InstallBg1VCountHandler(void) {
  u32 address = 0x04000012;
  s32 zero = 0;

  *(volatile u16 *)address = zero;
  /* FAKEMATCH: instruction order only: the do-while keeps the handler
     load ahead of the index. Plain forms measured (2026-10-13):
     REG_BG0VOFS = 0 or Io_Write16 with a direct call load the halfword
     zero from the pool (differ 6); through an s32 zero the store
     matches but the call sets r0 before loading the handler (differ 2). */
  do {
    address = (u32)Graphics_ClearBg1ControlBit2;
  } while (0);
  Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y) {
  struct UiWindow *result;

  result = UiText_OpenMessageWindow(message, x, y, 1);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  return result;
}

#if EDITION_INTERNATIONAL
#define WINDOW_X 22 /* Window X position */
#else
#define WINDOW_X 23
#endif

s32 Ui_RunSelectionScreen(s32 mode) {
  u8 party[4];
  struct UiRenderWork *render;
  struct SelectionWork *work;
  struct MenuCell *runtime;
  s32 rebuild, tradeCount, scale, result;
  s32 cnt;
  s32 keys, repeat;
  s32 slot;
  s32 i, num;

  render = UI_WORK;
  slot = VramCache_AllocEntry(0x400);
  rebuild = 1;
  tradeCount = 0;
  work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
  for (i = 255; i >= 0; i--)
    work->palette.slot[i] = 255;
  work->palette.count = 1;
  if (mode == 0)
    work->window = UiWindow_Create(20, 17, 10, 3, 6);
  else
    work->window = UiWindow_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
  work->previous = -1;
  if (mode == 0) {
    num = 0;
    work->kinds[num++] = 14;
    work->kinds[num++] = 4;
    work->kinds[num++] = 7;
    work->kinds[num] = -1;
    work->selection = gLinkCountdownWork->commandSelection[0];
  } else {
    work->selection = gLinkCountdownWork->commandSelection[1];
    num = 0;
    work->kinds[num++] = 0;
    work->kinds[num++] = 1;
    if (*((s32 *)Djinn_GetRecoveryTableFar(0)) != 0) {
      BattlePlacement_CountValidEntriesFar(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      work->kinds[num++] = 15;
      if (tradeCount != 0)
        work->kinds[num++] = 16;
    }
    work->kinds[num++] = 2;
    work->kinds[num++] = 3;
    work->kinds[num] = -1;
  }
  for (cnt = 0; cnt <= 5; cnt++) {
    work->entries[cnt].resource = slot;
    if (work->kinds[cnt] == -1)
      break;
    UiText_DrawCharacter(work, cnt, work->kinds[cnt]);
  }
  work->count = cnt;
  work->affine.scaleX = 320;
  work->affine.scaleY = 304;
  work->affine.angle = 0;
  cnt = 0;
  if (cnt < work->count) {
    do {
      s32 x = (cnt - work->count) * 24;
      work->entries[cnt].posX = x + 155;
      if (mode != 0)
        work->entries[cnt].posX = x + WINDOW_X * 8 - 5;
      work->entries[cnt].posY = 136;
      cnt++;
    } while (cnt < work->count);
  }
  Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
  Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

  for (;;) {
    scale = gUi_BounceScales[(gFrameCount * 2) & 31];
    scale = (scale - 256) / 4 + 304;
    work->affine.scaleY = work->affine.scaleX = scale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    if (rebuild != 0) {
      rebuild = 0;
      render->menuBusy = 1;
      if (mode) {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - work->count; panel++) {
          for (cnt = 0; cnt <= 2; cnt++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(render->tilemap + (cnt + 17) * 32 + panel * 3 + WINDOW_X - 18 +
                (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < work->count; panel++) {
          count = work->count;
          for (cnt = 0; cnt <= 2; cnt++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH: vram, offset and mask restore two forms the bytes
               * prove. Set ahead of the index, they are three more moves out of
               * the column loop in loop.c's first pass, which drops its
               * threshold so the tile pattern's row part stays in the column
               * loop until the second pass: the row loop reduces the index giv
               * first and copies it first (inits index r6, pattern r5; per row
               * mov ip, r6 and str r5, [sp, #4]). volatile keeps the VRAM base
               * add apart from the store it addresses. Without the three 24,
               * without offset 564, without mask 470, without volatile 112. */
              volatile u16 *vram =
                  (volatile u16 *)0x0600f800 + 21 * 32 + WINDOW_X;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH: needed to match: tmp, set twice (the tile, then the
               * column's index), is no giv, so the tile is recomputed in the
               * column loop as the bytes show (adds r3, r7, r0; adds r3, r3,
               * r7); 267 differ without it, 846 in both modes. */
              vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
                  panel * 16 + cnt * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp -
                count * 3 + WINDOW_X) = 0;
            }
          }
        }
      } else {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - work->count; panel++) {
          for (cnt = 0; cnt <= 2; cnt++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(render->tilemap + (cnt + 17) * 32 + panel * 3 + 2 + (col & 3)) =
                  0xf07f;
          }
        }
        for (panel = 0; panel < work->count; panel++) {
          /* FAKEMATCH: needed to match: the empty loop restores the CSE block
           * boundary at the panel body's head, so the exit test compares count
           * instead of reloading work->count (223 differ without it). */
          do {
          } while (0);
          count = work->count;
          for (cnt = 0; cnt <= 2; cnt++) {
            s32 col;
            for (col = 0; col <= 2; col++) {
              s32 tmp;
              /* FAKEMATCH: as in mode 1: vram, offset and mask keep the tile
               * pattern in the column loop through loop.c's first pass,
               * volatile keeps the VRAM add apart from its store. */
              volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
              s32 offset = 0x100;
              s32 mask = 3;
              /* FAKEMATCH: needed to match: tmp, as in mode 1 (271 differ
               * without it, 846 in both modes). */
              vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
                  panel * 16 + cnt * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp -
                count * 3 + 20) = 0;
            }
          }
        }
      }
    }
    if (work->previous != work->selection) {
      RenderOutput_PrepareForRedraw(work->window);
      UiText_DrawCharacterAtOffset(work->entries[work->selection].kind +
                                       MsgCommandName,
                                   work->window, 0, 0);
      work->previous = work->selection;
      UiText_DrawCharacter(work, work->selection, work->kinds[work->selection]);
    }
    work->matrix = AffineMatrix_BuildForEffect(&work->affine);
    for (cnt = 0; cnt < work->count; cnt++) {
      struct SelectionEntry *entry = &work->entries[cnt];
      if (cnt == work->selection) {
        entry->sprite.oam.f.affineIndex = work->matrix;
        entry->sprite.oam.f.affine = 3;
        entry->sprite.oam.f.x = entry->posX + scale * 7 / 512 - 14;
        entry->sprite.oam.f.y = entry->posY + scale * 3 / 256 - 20;
        Runtime_PushSlotEntry(entry, 0xf1);
      } else {
        entry->sprite.oam.f.x = entry->posX;
        entry->sprite.oam.f.y = entry->posY;
        entry->sprite.oam.f.affineIndex = 0;
        entry->sprite.oam.f.affine = 0;
      }
    }
    keys = gKeysPressed;
    repeat = gKeysRepeat;
    runtime = gLinkCountdownWork;
    if (runtime->autoEnabled != 0) {
      keys = repeat = 0;
      if (runtime->autoDelay == 0) {
        if (runtime->autoMode == 1) {
          if (work->kinds[work->selection] == 3)
            keys = repeat = 1;
          else
            keys = repeat = 32;
          runtime->autoDelay = 30;
        } else if (runtime->autoMode == 0) {
          s32 kind = work->kinds[work->selection];
          if (kind == 16 || (tradeCount == 0 && kind == 15)) {
            if (runtime->autoEnabled == 1) {
              struct UiWindow *message;
              if (kind == 15)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialUnleashCommand, 15, 8);
              else if (kind == 16)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialSummon, 15, 8);
              BattlePresentation_WaitForPromptAtFar(102, 155);
              UiWork_Finalize(message, 1);
              runtime->autoEnabled++;
              runtime->autoDelay = 45;
            } else {
              keys = repeat = 1;
              runtime->autoDelay = 200;
            }
          } else {
            keys = repeat = 16;
            runtime->autoDelay = 40;
          }
        } else {
          runtime->autoDelay = 60;
          keys = repeat = 1;
        }
      } else {
        runtime->autoDelay--;
      }
    }
    if ((keys & 0x300) && mode != 0) {
      result = -2;
      break;
    }
    if (keys & 1) {
      result = work->kinds[work->selection];
      break;
    }
    if (mode != 0 && (keys & 2)) {
      AudioCommand_PlayFar(0x71);
      result = -1;
      break;
    }
    if (repeat & 0x90) {
      AudioCommand_PlayFar(0x6f);
      work->selection = (work->selection + 1) % work->count;
    } else if (repeat & 0x60) {
      AudioCommand_PlayFar(0x6f);
      work->selection = (work->selection + work->count - 1) % work->count;
    } else if (gLinkCountdownWork->active == 0) {
      result = work->kinds[0];
      break;
    }
    QueueIoWriteDelay2(0x04000000, 0x1741);
    render->menuBusy = 0;
    WaitFrames(1);
  }
  if (mode != 0)
    gLinkCountdownWork->commandSelection[1] = work->selection;
  else
    gLinkCountdownWork->commandSelection[0] = work->selection;
  for (cnt = 0; cnt < work->count; cnt++)
    Resource_ResetEntry(work->entries[cnt].resource);
  render->menuBusy = 1;
  UiWork_Finalize(work->window, 1);
  {
    s32 panel;
    for (panel = 0; panel <= 6; panel++) {
      for (cnt = 0; cnt <= 2; cnt++) {
        s32 col;
        /* FAKEMATCH: the add of 0x44a stays apart from the store it addresses
         * (adds r3, r7; strh r6, [r3]); as a plain store combine folds it into
         * strh r6, [r3, r7], 0 -> 33. An ordinary row pointer with three
         * post-increment stores shortens Ui_RunSelectionScreen by 24 bytes
         * in tbs-en instead of matching its address formation. */
        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(render->tilemap + (cnt + 17) * 32 + panel * 3 + 5 +
                            (col & 3)) = 0xf07f;
      }
    }
  }
  render->dirty = 1;
  WaitFrames(1);
  Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
  Runtime_SetIrqHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
  MemFree(work);
  render->menuBusy = 0;
  WaitFrames(1);
  return result;
}

void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt) {
  struct UiRenderWork *render = UI_WORK;

  alt &= 1;
  alt <<= 12;

  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }

  if (width > 0 && height > 0) {
    do {
      u16 *p = &((u16 (*)[32])render->tilemap)[y][x];
      s32 col = width;

      if (col != 0) {
        do {
          u32 tile = *p;
          tile = (tile & ~0x1000) | alt;
          col--;
          *p = tile;
          p++;
        } while (col != 0);
      }

      UiRender_MarkRow(render, y);
      height--;
      y++;
    } while (height != 0);
  }
}

s32 BattleLayout_HighlightPartyPanels(u16 *ids) {
  struct BattleWork *battle = gBattleWork;
  s32 count;
  s32 i;
  s32 j;

  count = BattleParty_ListActorIdsFar(1, 0);
  Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
  i = 0;
  if (ids[0] != 0xff) {
  next_id:
    for (j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++) {
      if (battle->partyUnits[j] == 0xff) {
        j = 4;
        break;
      }
    }
    if (j != 4)
      Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
    if (++i < 4 && ids[i] != 0xff)
      goto next_id;
  }
  return 0;
}

// Void changes pop {r1}; bx r1 to r0 and reorders three preview argument pairs.
#ifdef UBFIX
void UiText_FormatNumberToHalfwords(s16 *out, s32 value) {
#else
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value) {
#endif
  s16 *dst;
  s32 n;
  u8 *src;

  dst = out;
  NumberText_FormatDecimal(value);
  src = (u8 *)((u32)&gNumberTextBuffer);
  n = 0xD;
  do {
    n -= 1;
    *dst = *src;
    src += 1;
    dst += 1;
  } while (n >= 0);
}

s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid,
                               const struct OwnerActionSlot *newGrid, u16 *out,
                               s32 *gained, s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 added;
  s32 removed;

  added = 0;
  removed = 0;
  count = 0;
  for (i = 0; i < 32 && newGrid[i].encodedAction != 0; i++) {
    if (BattleActGet(newGrid[i].encodedAction)->flags & 0x80) {
      out[count] = newGrid[i].encodedAction & ACTION_ID_MASK;
      count++;
      for (j = 0; j < 32; j++) {
        if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) &
             ACTION_ID_MASK) == 0)
          break;
      }
      if (j == 32) {
        added++;
        out[count - 1] |= 0x8000;
      }
    }
  }
  for (i = 0; i < 32 && oldGrid[i].encodedAction != 0; i++) {
    if (BattleActGet(oldGrid[i].encodedAction)->flags & 0x80) {
      for (j = 0; j < 32; j++) {
        if (((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) &
             ACTION_ID_MASK) == 0)
          break;
      }
      if (j == 32) {
        removed++;
        out[count] = (oldGrid[i].encodedAction & ACTION_ID_MASK) | 0x4000;
        count++;
      }
    }
  }
  *gained = added;
  *lost = removed;
  return count;
}

// Loads an ability icon into a free VRAM slot and shows it as a 16x16 sprite
// at pixel (x, y) inside input. Does nothing when no slot is free.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void Ui_CreateOutputFromResourceSlot(struct UiWindow *input, s32 x, s32 y,
                                     s32 ability) {
#else
s32 Ui_CreateOutputFromResourceSlot(struct UiWindow *input, s32 x, s32 y,
                                    s32 ability) {
#endif
  s32 slot;

  slot = Resource_FindFreeEntry();
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(ability, slot);
    RenderOutput_Create(slot, 0x40000000, input, x, y); // 16x16 sprite
  }
}

struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *win, s32 owner,
                                             s32 code, s32 page,
                                             s32 *pageCount) {
  struct BattleUnit *unit;
  struct BattleUnit *snap;
  struct BattleAct *info;
  s16 *buf;
  s32 oldHp;
  s32 oldPp;
  s32 oldAtk;
  s32 oldDef;
  s32 oldAgi;
  u16 *list;
  s32 total;
  s32 n;
  s32 marks;
  s32 rows;
  s32 col;
  s32 row;
  s32 pages;
  s32 gained;
  s32 lost;
  s32 tile;
  s32 val;
  s32 i;

  void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising) {
    struct RenderOutput *output = RenderOutput_AcquireFree();
    struct PreviewSprite *entry;
    u32 zero;
    if (output) {
      output->active = 1;
      output->kind = 1;
      /* FAKEMATCH: instruction order only: a char store (alias set 0) keeps
         the index store ahead of the attribute word in sched2. Plain forms
         measured (2026-10-13) for this and the two typed stores below:
         output->index, entry->attributes.word or entry->tile.value alone
         differ 2, 2 and 4; all three plain differ 2 (the entry copy moves
         after the x and y stores), with entry set first or after the
         sentinel still 2, after x or y 38. */
      *(s8 *)&output->index = VramCache_AllocEntry(128);
      entry = (struct PreviewSprite *)((u8 *)output + 16);
      output->sentinel = 240;
      output->x = 120;
      output->y = 120;
      /* FAKEMATCH: typed like the window pointer, the word store waits for the
       * window load but not for the x and y stores, so sched2 places the entry
       * copy before them */
      *(struct UiWindow **)&entry->attributes = (struct UiWindow *)0x40000400;
      zero = 0;
      /* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores
       * (sched2 order) */
      *(void **)&entry->tile = (void *)zero;
      entry->attributes.bits.x = win->x * 8 + x;
      entry->attributes.bits.y = win->y * 8 + y;
      /* FAKEMATCH: advancing and restoring the captured pointer keeps
       * the window chain in r9. Without the pair, r8/r9 allocation
       * changes and two extra instructions remain (80 -> 82, 22 rows). */
      win++;
      win--;
      entry->tile.bits.index = VramCache_Reload(
          (u8)output->index,
          (s32)(rising ? gDjinnMenu_ArrowUpTiles : gDjinnMenu_ArrowDownTiles));
      RenderOutput_AppendToList((struct RenderOutputList *)win, output);
    }
  }

  unit = UnitGetFar(owner);
  if (win != 0)
    UiWork_Finalize(win, 1);

  if (page == 0)
    win = UiWindow_Create(0, 8, 21, 11, 6);
  else
    win = UiWindow_Create(0, 5, 21, 14, 6);
  if (win == 0)
    return 0;

  buf = MemAlloc(128);
  snap = MemAlloc(332);
  list = Runtime_BumpAllocateAlternatePool(96);
  Iwram_CopyWords(snap, unit, 332);

  col = (code >> 8) & 15;
  row = code & 255;
  if (Djinn_IsActiveFar(owner, col, row) != 0)
    Djinn_DeactivateFar(owner, col, row);
  else
    Djinn_ActivateFar(owner, col, row);
  UnitCalcFar(owner);

  total = DjinnMenu_ListChangedDjinn(snap->actionSlots, unit->actionSlots, list,
                                     &gained, &lost);
  pages = ((total - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= total)
    page = pages;

  if (page == 0) {
    s32 label = MsgPreviewStatLabel;

    UiText_DrawCharacterAtOffset(label, win, 0, 8);
    UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
    UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
    UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
    UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
    UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

    oldHp = snap->hp;
    UiText_FormatNumberToHalfwords(buf, oldHp);
#if !EDITION_INTERNATIONAL
    UiText_RenderWideStringInWindow(buf + 7, win, 4, 1);
#else
    UiText_RenderWideStringInWindow(buf + 7, win, 5, 1);
#endif
    oldPp = snap->pp;
    UiText_FormatNumberToHalfwords(buf, oldPp);
#if !EDITION_INTERNATIONAL
    UiText_RenderWideStringInWindow(buf + 7, win, 4, 2);
#else
    UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
#endif
    oldAtk = snap->atk;
    UiText_FormatNumberToHalfwords(buf, oldAtk);
#if !EDITION_INTERNATIONAL
    UiText_RenderWideStringInWindow(buf + 7, win, 4, 3);
#else
    UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
#endif
    oldDef = snap->def;
    UiText_FormatNumberToHalfwords(buf, oldDef);
#if !EDITION_INTERNATIONAL
    UiText_RenderWideStringInWindow(buf + 7, win, 4, 4);
#else
    UiText_RenderWideStringInWindow(buf + 8, win, 6, 4);
#endif
    oldAgi = snap->agi;
    UiText_FormatNumberToHalfwords(buf, oldAgi);
#if !EDITION_INTERNATIONAL
    UiText_RenderWideStringInWindow(buf + 7, win, 4, 5);
#else
    UiText_RenderWideStringInWindow(buf + 8, win, 6, 5);
#endif
    UiText_FormatNumberToHalfwords(buf, snap->luck);
    UiText_RenderWideStringInWindow(buf + 8, win, 5, 6);

    UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
    if (gained != 0 || lost != 0)
      UiWork_SetParamNibble(2);
#if !EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffset(MsgBattleAdditionalPsynergyHelp, win, 40, 64);
#else
    UiText_DrawCharacterAtOffset(MsgBattleAdditionalPsynergyHelp, win, 24, 64);
#endif
    UiWork_SetParamNibble(15);
  }

  if (page > 0) {
    marks = 0;
    rows = total;
    if (total > 4)
      rows = 5;
    i = page * 5 - 5;
    for (n = 0; n < rows && i < total; n++, i++) {
      info = BattleActGet(list[i]);
      if (info->element != 4)
        UiWindow_SetTilemapEntry(win, 0x5001 + info->element, 15, n * 2, 0);

      tile = info->range;
      if (tile == 255)
        tile = 11;
      else
        tile--;
      UiWindow_DrawThreeTileColumn(win, 16, n * 2, tile, 0);

      Ui_CreateOutputFromResourceSlot(win, 0, n * 16 - 4, list[i] & 0x3fff);
      if ((list[i] & 0x8000) != 0)
        UiWork_SetParamNibble(4);
      else if ((list[i] & 0x4000) != 0)
        UiWork_SetParamNibble(2);
      else
        UiWork_SetParamNibble(15);

      UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + MsgAbilityName, win, 16,
                                   n * 16);
      UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
      UiWindow_SetTilemapEntry(win, 0xf005, 12, n * 2, 0);
#else
      UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
#endif
      UiText_DrawNumberInWindow(BattleActGet(list[i])->ppCost, 2, win, 104,
                                n * 16);
    }

    if (gained != 0) {
      UiWork_SetParamNibble(4);
      UiText_DrawCharacterAtOffset(MsgPsynergyGained, win, 32, 80);
      marks = 1;
    }
    if (lost != 0) {
      UiWork_SetParamNibble(2);
      UiText_DrawCharacterAtOffset(MsgPsynergyLost, win, 32, marks * 8 + 80);
      marks++;
    }
    if (marks == 0)
      UiText_DrawCharacterAtOffset(MsgPsynergyUnchanged, win, 32, 80);
    UiWork_SetParamNibble(15);
    UiWork_SetParamNibble(15);
    UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
  }

  if (page == 0) {
    UiText_DrawCharacterAtOffset(MsgClassName + snap->classIndex, win, 0, 0);
    UiText_DrawCharacterAtOffset(MsgClassName + unit->classIndex, win, 80, 0);
    if (snap->classIndex != unit->classIndex)
      UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
    else
      UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

    UiText_FormatNumberToHalfwords(buf, unit->hp);
    UiText_RenderWideStringInWindow(buf + 7, win, 11, 1);
    if (unit->hp != oldHp) {
      val = 0;
      if (unit->hp > oldHp)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 14, val);
    }

    UiText_FormatNumberToHalfwords(buf, unit->pp);
    UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
    if (unit->pp != oldPp) {
      val = 0;
      if (unit->pp > oldPp)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 22, val);
    }

    UiText_FormatNumberToHalfwords(buf, unit->atk);
    UiText_RenderWideStringInWindow(buf + 7, win, 11, 3);
    if (unit->atk != oldAtk) {
      val = 0;
      if (unit->atk > oldAtk)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 30, val);
    }

    UiText_FormatNumberToHalfwords(buf, unit->def);
    UiText_RenderWideStringInWindow(buf + 7, win, 11, 4);
    if (unit->def != oldDef) {
      val = 0;
      if (unit->def > oldDef)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 38, val);
    }

    UiText_FormatNumberToHalfwords(buf, unit->agi);
    UiText_RenderWideStringInWindow(buf + 7, win, 11, 5);
    if (unit->agi != oldAgi) {
      val = 0;
      if (unit->agi > oldAgi)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 46, val);
    }

    UiText_FormatNumberToHalfwords(buf, unit->luck);
    UiText_RenderWideStringInWindow(buf + 8, win, 12, 6);
    if (unit->luck != snap->luck) {
      val = 0;
      if (unit->luck > snap->luck)
        val = 1;
      DjinnMenu_DrawStatArrow(80, 54, val);
    }
  }

  Iwram_CopyWords(unit, snap, 332);
  MemFree(list);
  MemFree(snap);
  MemFree(buf);
  return win;
}

#if EDITION_INTERNATIONAL
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusExp - 10 + (row))
#endif

struct StatusCursor {
  s8 pos[5];
  s32 entry;
  s32 lastEntry;
  s32 mode;
  s32 limit;
  s32 col;
  s32 row;
};

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner) {
  struct MenuSprite objFace;
  struct MenuSprite objCursor;
  s32 table[11];
  s8 selection[11];
  struct MenuSprite objIcon[11];
  struct MenuSprite objName;
  struct StatusCursor cur;
  struct UiRenderWork *base;
  struct UiWindow *winMain;
  struct UiWindow *winDesc;
  s32 idx;
  s32 handleCursor;
  struct MenuPoint pix;
  s32 handleFrame;
  s32 cnt;
  s32 extended;
  s32 keys;
  u16 *text;
  s32 iconIdx;
  s32 *q;
  s32 defDelta;
  s32 i;
  s32 j;
  s32 atkDelta;
  s32 redraw;
  struct BattleUnit *object;
  struct BattleUnit *saved;
  const struct OwnerStatusStatCursor *stat;
  struct MenuSprite *p;
  s32 agiDelta;
  s32 code;
  s32 n;
  struct MenuCell *screen;
  s32 x;

  base = UI_WORK;
  redraw = 1;
  idx = -1;
  handleFrame = VramCache_AllocEntry(0x200);
  cnt = 0;
  extended = Party_SumDjinnCountsFar(-1);

  screen = ((struct MenuCell *)gWindowWork[42]);
  screen->busy = 1;
  if (screen->work != NULL) {
    UiWork_Finalize(screen->work, 1);
    screen->work = NULL;
  }
  AudioCommand_PlayFar(0x70);

  for (i = 4; i >= 0; i--) {
    cur.pos[i] = 0;
  }
  cur.entry = 0;
  cur.lastEntry = 0;
  cur.mode = 0;

  handleCursor = VramCache_AllocEntry(0x80);
  for (i = 0; i <= 10; i++) {
    table[i] = VramCache_AllocEntry(0x80);
    selection[i] = -1;
  }

  if (listCount != 0) {
    for (i = 0; i <= 5; i++) {
      s32 kind = list[i];

      if (kind == 0xFF) {
        break;
      }
      if (kind != 0xFE) {
        if (kind == owner) {
          idx = i;
          break;
        }
      }
    }
  }

  pix.x = 0;
  pix.y = 0;
  winMain = UiWindow_Create(0, 0, 30, 20, 6);
  winDesc = UiWindow_Create(0, 14, 30, 6, 10);
  UiWindow_MarkVisibleTileAttributes();

  for (;;) {
    keys = gKeysRepeat;

    objName.oam.word.attr01 = 0x80000400;
    objName.oam.word.attr23 = 0;
    objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
    objName.oam.f.x = 8;
    objName.oam.f.y = 24;
    objName.oam.f.palette = 14;
    Runtime_PushSlotEntry((s32 *)&objName, 240);

    switch (cur.entry) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
      cur.mode = 0;
      if (extended != 0) {
        cur.limit = 9;
      } else {
        cur.limit = 7;
      }
      break;
    case 10:
    case 11:
    case 12:
    case 13:
      cur.mode = 1;
      cur.limit = 4;
      break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      cur.mode = 2;
      cur.limit = cnt;
      break;
    }

    Ui_SetRectHighlight(winMain->x, winMain->y, winMain->width, winMain->height,
                        15);

    if (cnt != 0) {
      if ((u32)cur.mode <= 1) {
        n = cur.pos[cur.mode];
        if ((keys & 0x80) != 0) {
          keys = 0;
          AudioCommand_PlayFar(0x6F);
          n++;
          if (n >= cur.limit) {
            n = 0;
            if (cur.mode == 1) {
              cur.mode = 2;
              n = cur.pos[cur.mode];
            }
          }
        } else if ((keys & 0x40) != 0) {
          keys = 0;
          AudioCommand_PlayFar(0x6F);
          n--;
          if (n < 0) {
            n = cur.limit - 1;
            if (cur.mode == 1) {
              cur.mode = 2;
              n = cur.pos[cur.mode];
            }
          }
        } else if ((keys & 0x31) != 0) {
          keys = 0;
          AudioCommand_PlayFar(0x6F);
          cur.mode ^= 2;
          if ((gKeysRepeat & KEY_A) != 0) {
            n = cur.pos[cur.mode];
          } else if ((gKeysRepeat & KEY_LEFT) != 0) {
            n = cnt - 1;
          } else {
            n = 0;
          }
        }
        cur.pos[cur.mode] = (s8)n;
      } else if (cur.mode == 2) {
        n = cur.pos[cur.mode];
        if (n >= cur.limit) {
          n = cur.limit - 1;
        }
        if (n < 0) {
          cur.mode = 0;
          n = cur.pos[0];
        } else if ((keys & 0x10) != 0) {
          keys = 0;
          n++;
          if (n >= cur.limit) {
            cur.mode = 0;
            n = cur.pos[0];
          }
          AudioCommand_PlayFar(0x6F);
        } else if ((keys & 0x20) != 0) {
          keys = 0;
          n--;
          if (n < 0) {
            cur.mode = 0;
            n = cur.pos[0];
          }
          AudioCommand_PlayFar(0x6F);
        } else if ((keys & 0xC1) != 0) {
          keys = 0;
          cur.mode = 0;
          n = cur.pos[0];
          AudioCommand_PlayFar(0x6F);
        }
        cur.pos[cur.mode] = (s8)n;
      }
    }

    if (cur.mode == 0) {
      s32 k = cur.pos[cur.mode];

      if (extended == 0) {
        k += 9;
      }
      stat = &gOwnerStatus_StatCursors[k];
      cur.entry = stat->entry;
      cur.col = stat->column;
      cur.row = stat->row;
      Ui_SetRectHighlight(winMain->x + stat->highlightColumn + 1,
                          winMain->y + stat->highlightRow + 1,
                          stat->highlightWidth, 1, 14);
    } else if (cur.mode == 1) {
      cur.entry = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
      cur.col = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
      cur.row = gOwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
    } else if (cur.mode == 2) {
      cur.entry = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
      cur.col = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
      cur.row = gOwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
    }

    if (cur.lastEntry != cur.entry) {
      cur.lastEntry = cur.entry;
      redraw = 2;
    }
    pix.x = cur.col * 8;
    pix.y = cur.row * 8;

    {
      struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

      objFace.oam.word.attr01 = 0xC0002400;
      objFace.oam.word.attr23 = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
      /* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store
       * before the move into sl and the 0xFC00 load before the face tile load,
       * which may alias that store; native moves into sl between the two stores
       * and loads the tile first (4 instructions) */
      asm("");
#endif
      objFace.oam.f.tile = face->oam.f.tile;
      objFace.oam.f.x = 0xAC;
      objFace.oam.f.y = 56;
      Runtime_PushSlotEntry((s32 *)&objFace, 240);
    }

    objCursor.oam.word.attr01 = 0x40000400;
    objCursor.oam.word.attr23 = 0;
    /* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount
     * literal falls past the face x literal's 64-byte ldrh range and opens the
     * next pool, as native; the pool and its branch move two instructions
     * earlier, 85 -> 63. */
    /* FAKEMATCH: removing both barriers gives EN 1392/1392, common 1390, differ
     * 4: gFrameCount joins the earlier pool and the branch follows two extra
     * loads. ES 1458/1458, common 1455, differ 6: attr23/mov sl order and the
     * 0x3ff load also move. 2026-10-11: arm_reorg runs before shorten_branches
     * and counts the empty asm at its 4-byte default length while it emits
     * nothing; the halfword constant fix needs that much more estimated length
     * after it, and no C statement or single pattern-length change (strh, str,
     * orr, and tested over all 413 English objects) supplies it. In ES and IT
     * both asms are scheduling barriers only (differ 8 without them). */
    asm("");
    objCursor.oam.f.tile =
        VramCache_Reload(handleCursor, (s32)gResource_FixedBlockBTiles);
    objCursor.oam.f.x =
        (pix.x + winMain->x * 8 - ((gFrameCount & 4) >> 2)) + 16;
    objCursor.oam.f.y =
        (u8)((pix.y + winMain->y * 8 - ((gFrameCount & 4) >> 2)) + 16);
    objCursor.oam.f.affineIndex = 8;
    Runtime_PushSlotEntry((s32 *)&objCursor, 241);

    if (redraw != 0) {
      object = UnitGetFar(owner);
#if EDITION_INTERNATIONAL
      UiWindow_MarkVisibleTileAttributes();
#endif
      Ui_FillVramBlockPattern();

      if ((redraw & 1) != 0) {
        RenderOutput_RedrawSavedRect(winMain);
        UiWindow_MapTextCanvasTiles(winDesc->x, winDesc->y, winDesc->width,
                                    winDesc->height, 0);
        UiWindow_DrawDividerLine(winMain, 0, 14, 29, 14);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        switch (System_GetLanguage()) {
        case 2:
          UiText_DrawStringAtOffset((u8 *)object->name, winMain, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NivString, winMain, 51,
                                    0);
          UiText_DrawNumberInWindow(object->level, 2, winMain, 72, 0);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)object->name, winMain, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NvString, winMain, 56,
                                    0);
          UiText_DrawNumberInWindow(object->level, 2, winMain, 72, 0);
          break;
        default:
          UiText_DrawStringAtOffset((u8 *)object->name, winMain, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString, winMain, 56,
                                    0);
          UiText_DrawNumberInWindow(object->level, 2, winMain, 72, 0);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)object->name, winMain, 0, 0);
#if defined(TBS_EDITION_FR)
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString, winMain, 51,
                                  0);
#else
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString, winMain, 56,
                                  0);
#endif
        UiText_DrawNumberInWindow(object->level, 2, winMain, 72, 0);
#endif
        UiText_DrawCharacterAtOffset(MsgOwnerStatusExp, winMain, 0, 8);
        UiText_DrawNumberInWindow(object->experience, 8, winMain, 40, 8);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        switch (System_GetLanguage()) {
        case 1:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString, winMain, 40,
                                    24);
          break;
        case 2:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, winMain, 40,
                                    24);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, winMain, 40,
                                    24);
          break;
        case 4:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, winMain, 40,
                                    24);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, winMain, 40, 24);
#endif
        UiText_DrawNumberInWindow(object->hp, 4, winMain, 56, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, winMain, 88,
                                  24);
        UiText_DrawNumberInWindow(object->maxHp, 4, winMain, 96, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString, winMain, 40, 32);
        UiText_DrawNumberInWindow(object->pp, 4, winMain, 56, 32);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, winMain, 88,
                                  32);
        UiText_DrawNumberInWindow(object->maxPp, 4, winMain, 96, 32);

        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(0), winMain, 136, 16);
        UiText_DrawNumberInWindow(object->atk, 3, winMain, OWNER_STATUS_VALUE_X,
                                  16);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(1), winMain, 136, 24);
        UiText_DrawNumberInWindow(object->def, 3, winMain, OWNER_STATUS_VALUE_X,
                                  24);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(2), winMain, 136, 32);
        UiText_DrawNumberInWindow(object->agi, 3, winMain, OWNER_STATUS_VALUE_X,
                                  32);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(3), winMain, 136, 40);
        UiText_DrawNumberInWindow(object->luck, 3, winMain,
                                  OWNER_STATUS_VALUE_X, 40);

        UiText_DrawCharacterAtOffset(MsgClassName + object->classIndex, winMain,
                                     0, 48);
        if (extended != 0) {
          UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(9), winMain, 0, 72);
        }
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(5), winMain, 0, 80);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(6), winMain, 0, 88);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(7), winMain, 0, 96);

        for (j = 0; j <= 3; j++) {
          s32 pal;

          pal = 1;
          if (extended != 0) {
            pal = 0;
          }
          UiWindow_SetTilemapEntry(winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
          if (extended != 0) {
            UiText_DrawNumberInWindow(object->djinnActiveCounts[j], 1, winMain,
                                      40 + j * 32, 72);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, winMain,
                                      48 + j * 32, 72);
            UiText_DrawNumberInWindow(object->djinnOwnedCounts[j], 1, winMain,
                                      48 + j * 32 + 8, 72);
          }
          UiText_DrawNumberInWindow(Unit_GetElementLevelFar(owner, j), 2,
                                    winMain, 48 + j * 32, 80);
          UiText_DrawNumberInWindow(object->element[j].power, 3, winMain,
                                    40 + j * 32, 88);
          UiText_DrawNumberInWindow(object->element[j].resistance, 3, winMain,
                                    40 + j * 32, 96);
        }

        i = 0;
        if (object->hp == 0) {
          selection[0] = 16;
          i = 1;
        }
        do {
          if (object->restraint != 0) {
            selection[i++] = 15;
          }
          if (i > 7) {
            break;
          }
          if (object->grimReaper != 0) {
            selection[i++] = 8;
          }
          if (i > 7) {
            break;
          }
          if (object->poison == 1) {
            selection[i++] = 1;
          }
          if (i > 7) {
            break;
          }
          if (object->poison == 2) {
            selection[i++] = 2;
          }
          if (i > 7) {
            break;
          }
          if (object->psySeal != 0) {
            selection[i++] = 4;
          }
          if (i > 7) {
            break;
          }
          if (object->stun != 0) {
            selection[i++] = 3;
          }
          if (i > 7) {
            break;
          }
          if (object->sleep != 0) {
            selection[i++] = 5;
          }
          if (i > 7) {
            break;
          }
          if (object->haunt != 0) {
            selection[i++] = 7;
          }
          if (i > 7) {
            break;
          }
          if (object->delusion != 0) {
            selection[i++] = 6;
          }
          if (i > 7) {
            break;
          }
          if (object->attackTurns != 0) {
            if (object->attackStage > 0) {
              selection[i++] = 9;
            }
            if (object->attackStage < 0) {
              selection[i++] = 10;
            }
          }
          if (i > 7) {
            break;
          }
          if (object->defenseTurns != 0) {
            if (object->defenseStage > 0) {
              selection[i++] = 11;
            }
            if (object->defenseStage < 0) {
              selection[i++] = 12;
            }
          }
          if (i > 7) {
            break;
          }
          if (object->resistanceTurns != 0) {
            if (object->resistanceStage > 0) {
              selection[i++] = 13;
            }
            if (object->resistanceStage < 0) {
              selection[i++] = 14;
            }
          }
          if (i > 7) {
            break;
          }
          if (object->agilityStage > 0) {
            selection[i++] = 17;
          }
          if (object->agilityStage < 0) {
            selection[i++] = 18;
          }
        } while (0);

        for (j = 0; j < i; j++) {
          Resource_LoadTableEntryToBuffer(selection[j], table[j]);
        }
        if (i == 0) {
          selection[0] = 0;
          i = 1;
        }
        if (i <= 10) {
          for (j = i; j < 11; j++) {
            selection[j] = -1;
          }
        }
        cnt = i;
        if (selection[0] == 0) {
          UiText_DrawCharacterAtOffset(object->hp != 0 ? MsgOwnerStatusNormal
                                                       : MsgOwnerStatusDowned,
                                       winMain, 112, 0);
        }
      }

      text = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*text));
      if ((u32)cur.entry > 13) {
        iconIdx = selection[cur.entry - 14];
        if (iconIdx == 0 && object->hp == 0) {
          iconIdx = 16;
        }

        saved = (struct BattleUnit *)MemAlloc(sizeof(*object));
        code = 0;
        Iwram_CopyWords(saved, object, sizeof(*object));

        atkDelta = object->atk;
        defDelta = object->def;
        agiDelta = object->agi;
        object->attackStage = 0;
        object->defenseStage = 0;
        object->agilityStage = 0;
        UnitCalcFar(owner);
        atkDelta -= object->atk;
        defDelta -= object->def;
        agiDelta -= object->agi;
        Iwram_CopyWords(object, saved, sizeof(*object));
        MemFree(saved);

        switch (iconIdx) {
        case 8:
          code = object->grimReaper;
          break;
        case 9:
          code = atkDelta;
          break;
        case 10:
          code = -atkDelta;
          break;
        case 11:
          code = defDelta;
          break;
        case 12:
          code = -defDelta;
          break;
        case 17:
          code = agiDelta;
          break;
        case 18:
          code = -agiDelta;
          break;
        case 13:
        case 14:
          code = object->resistanceStage * 20;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
          if (code < 0) {
            code = -code;
          }
#endif
          break;
        default:
          break;
        }
        UiWork_PushValueSlot(code, 5);
        UiText_CopyMessageString(iconIdx + MsgOwnerStatusConditionHelp, text,
                                 OWNER_STATUS_TEXT_MAX);
      } else if (cur.entry == 2 && object->level <= 98) {
        UiWork_PushValueSlot(
            Unit_GetExperienceForLevelFar(owner, object->level + 1) -
                object->experience,
            5);
        UiText_CopyMessageString(MsgOwnerStatusNextLevel, text,
                                 OWNER_STATUS_TEXT_MAX);
      } else {
        UiText_CopyMessageString(cur.entry + MsgOwnerStatusEntryHelp, text,
                                 OWNER_STATUS_TEXT_MAX);
      }

      UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
      MemFree(text);
      redraw = 0;
      base->dirty = 1;
    }

    p = objIcon;
    q = table;
    {
      struct VramBlockCacheEntry *vc;

      for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++) {
        p->oam.word.attr01 = 0x40000400;
        p->oam.word.attr23 = 0;
        p->oam.f.tile = vc[*q++].offset >> 5;
        p->oam.f.x = x;
        p->oam.f.y = (u8)(winMain->y * 8 + 8);
        if (selection[i] > 0) {
          Runtime_PushSlotEntry((s32 *)p, 240);
        }
        x += 15;
        p++;
      }
    }

    if (gLinkCountdownWork->active == 0) {
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      break;
    }

    if (listCount != 0) {
      if ((keys & 0x100) != 0) {
        idx++;
        if (idx >= listCount) {
          idx = 0;
        }
        owner = list[idx];
        redraw = 1;
        AudioCommand_PlayFar(0x6F);
      } else if ((keys & 0x200) != 0) {
        idx--;
        if (idx < 0) {
          idx = listCount - 1;
        }
        owner = list[idx];
        redraw = 1;
        AudioCommand_PlayFar(0x6F);
      }
    }
    WaitFrames(1);
  }

  {
    s32 *h = table;

    for (i = 10; i >= 0; i--) {
      Resource_ResetEntry(*h++);
    }
  }
  Resource_ResetEntry(handleFrame);
  Resource_ResetEntry(handleCursor);
  WaitFrames(1);
#if EDITION_INTERNATIONAL
  UiWindow_MarkVisibleTileAttributes();
#endif
  UiWork_Finalize(winMain, 1);
  UiWork_Finalize(winDesc, 1);
  PartyStatusDraw(gBattleWork->partyStatus);
  // UB: the indexed scalar pointer steps outside gBattleWork to slot 57.
  // The named slot changes native instructions at the same 0xcf8-byte EN
  // extent; using gWorkSlot instead adds eight bytes.
#ifdef UBFIX
  gLinkCountdownWork->busy = 0;
#else
  ((struct MenuCell **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]
      ->busy = 0;
#endif
  WaitFrames(1);
  return 0;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#define TEXT_LOAD UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TEXT_COUNT 32
#define TEXT_LOAD UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

// Entire gLinkCountdownWork block; MenuCell in menu_list.h reads
// the region from 0x28 through 0xe4.
struct LinkRoundState {
  struct MenuSprite entries[3];
  u8 entryActive[3];
  u8 unk27;
  s32 targetOffset;
  s32 currentOffset;
  s32 unk30;
  s32 unk34;
  s32 unk38;
  s32 unk3c;
  s32 unk40;
  // A pointer field swaps two native initialization stores; keep its encoded
  // handle and convert at the window API boundaries.
  s32 displayHandle;
  s32 secondaryHandle;
  s32 timer;
  s32 enabled;
  u8 cost[8][4];
  u8 pick[34];
  u16 list[33];
  s32 hintCount;
  s32 hintTimer;
  s32 actorId;
  u32 selected[8];
};

#define COUNTDOWN ((struct LinkRoundState *)gLinkCountdownWork)
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

extern const u8 gDjinnMenu_BlinkTile[32];
extern const u8 gDjinnMenu_ElementXOffsets[4];
extern const u8 gDjinnMenu_ElementYOffsets[4];

s32 Battle_SelectAbility(s32 owner, s32 unused) {
  struct CursorSprite slot;
  u16 buf[64];
  struct UiRenderWork *ctx;
  struct UiWindow *detailWin;
  s32 prevRow;
  s32 res;
  u32 *table;
  s32 cnt;
  s32 prevPage;
  struct UiWindow *msgWin;
  s32 mask;
  s32 savedRow;
  s32 subSelection;
  s32 redraw;
  s32 isUsed;
  s32 subCnt;
  struct MenuCell *menu;
  struct BattleUnit *unit;
  struct DjinnRecoveryList *list;
  struct UiWindow *listWin;
  struct UiWindow *iconWin;
  struct PartyStatusWindow *cursor;
  u32 *pos;
  u32 cur;
  u32 blink;
  s32 page;
  s32 row;
  u32 code;
  s32 pages;
  s32 pal;
  struct MenuPoint cursorPos;
  s32 trig;
  s32 rep;
  s32 opts;
  s32 x;
  s32 y;
  s32 i;
  s32 j;
  struct CursorSprite *curSpr;

  ctx = UI_WORK;
  detailWin = 0;
  prevRow = -1;
  res = VramCache_AllocEntry(0x80);
  table = Runtime_BumpAllocateAlternatePool(0xa8 * 2);
  cnt = 0;
  prevPage = -1;
  code = 0;
  mask = 0;
  subSelection = 0;
  subCnt = 0;
  redraw = 0;
  isUsed = 0;
  msgWin = UiWindow_Create(0, 4, 30, 4, 42);
  UiWork_SetAltFlagAndClearTable(1);
  listWin = UiWindow_Create(21, 9, 9, 11, 6);
  MenuCell_GetCursor(page, row, savedRow);
  unit = UnitGetFar(owner);

  for (x = 0; x <= 3; x++) {
    for (y = 0, pos = (u32 *)((cnt << 2) + (u32)table); y <= 19; y++) {
      if (unit->djinnActive[x] & (u32)(1 << y)) {
        *pos = (u32)((x << 8) | y);
        pos++;
        cnt++;
      } else if (unit->djinnAvailable[x] & (u32)(1 << y)) {
        u32 v;
        s32 k;
        list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                     (u32)owner > 7 ? 1 : 0))
                    ->list;
        for (k = 0; k < list->count; k++) {
          if (list->entries[k].unitId == owner &&
              list->entries[k].element == x && list->entries[k].index == y) {
            break;
          }
        }
        v = (u32)((x << 8) | y | 0x10000);
        *pos = v;
        if (list->entries[k].turns > 0) {
          v |= (u32)(list->entries[k].turns << 17);
          *pos = v;
        }
        cnt++;
        pos++;
      }
    }
  }
  table[cnt] = 0x80000000;
  ctx->dirty = 1;

  /* FAKEMATCH: forced store: this dead store of curSpr before the loop keeps
   * the native allocation; without it the function differs 394 (1193
   * instructions for 1201) in tbs-en and tbs-ja (measured 2026-10-14). */
  curSpr = &slot;
  for (;;) {
    if (page != prevPage || row != prevRow || redraw != 0) {
      cur = table[page + row];
      mask = 0;
      ctx->menuBusy = 1;
      Ui_SetRectHighlight(listWin->x + 1, listWin->y + prevRow * 2 + 1,
                          listWin->width - 2, 1, 15);
      if (subSelection != 0) {
        UiWork_Finalize(msgWin, 1);
        msgWin = UiWindow_Create(0, 4, 30, 4, 42);
        Ui_FillVramBlockPattern();
      }
      redraw = 0;
      if (cnt != 0) {
        s32 k;
        u32 selected;
        UiWork_ClearValueNameTables();
        isUsed = 0;
        /* FAKEMATCH: this one-pass loop retains the native redraw
         * reload after the selected-entry load. A plain assignment
         * folds k to zero (1201 -> 1200 instructions, 27 rows differ);
         * reading selected in the comparison moves the reload early
         * (2 rows differ). No loop instructions survive. */
        selected = COUNTDOWN->selected[redraw];
        for (k = redraw; k == redraw;) {
          k = redraw;
          break;
        }
        if (selected == cur) {
          isUsed = 1;
        } else {
          for (;;) {
            /* FAKEMATCH: needed to match: isUsed is zero on every search-loop
             * continuation. Its step dependency prevents loop.c from
             * strength-reducing k into a pointer; CSE later removes the flag
             * term. Plain k differs 35 instructions in EN; this form restores
             * the native reload and indexing (differ 0). */
            k += 1 - isUsed;
            if (k > 7)
              break;
            if ((u32)COUNTDOWN->selected[k] == cur) {
              isUsed = 1;
              break;
            }
          }
        }
        if (isUsed != 0) {
          TEXT_LOAD(MsgBattleDjinnTwice, buf, TEXT_COUNT);
          if (detailWin != 0) {
            UiWork_Finalize(detailWin, 1);
            detailWin = 0;
            subSelection = 0;
          }
        } else if (cur & 0x10000) {
          if (cur & 0x3e0000) {
            UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
            TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) +
                          MsgDjinnEffect,
                      buf, TEXT_COUNT);
            if (detailWin != 0) {
              UiWork_Finalize(detailWin, 1);
              detailWin = 0;
              subSelection = 0;
            }
          } else {
            detailWin = (struct UiWindow *)DjinnMenu_ShowChangePreview(
                detailWin, owner, (s32)cur, subSelection, &subCnt);
            UiText_CopyMessageString(MsgBattleSummonStandby, buf, TEXT_COUNT);
            mask = 1 << ((cur & 0xf00) >> 8);
          }
        } else {
          detailWin = (struct UiWindow *)DjinnMenu_ShowChangePreview(
              detailWin, owner, (s32)cur, subSelection, &subCnt);
          TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) +
                        MsgDjinnEffect,
                    buf, TEXT_COUNT);
        }
      } else {
        TEXT_LOAD(MsgBattleNoDjinn, buf, TEXT_COUNT);
      }
      ctx->menuBusy = 0;
      if (subSelection == 0) {
        ctx->menuBusy = 1;
        UiWork_Finalize(msgWin, 1);
        msgWin = UiWindow_Create(0, 4, 30, 4, 42);
        ctx->menuBusy = (u8)subSelection;
      }
      UiText_RenderWideStringAtOffset(buf, msgWin, 0, 4);
      prevRow = row;
      if (page != prevPage) {
        RenderOutput_RedrawSavedRect(listWin);
        j = 0;
        while ((code = table[page + j]) != 0x80000000) {
          UiWindow_SetTilemapEntry(listWin, (s32)((code & 0xf00) >> 8) + 0x5001,
                                   0, j * 2, 0);
          if (code & 0x3e0000) {
            UiWork_SetParamNibble(4);
          } else if (code & 0x10000) {
            UiWork_SetParamNibble(2);
          }
          UiText_DrawCharacterAtOffset(
              (s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + MsgDjinnName,
              listWin, 8, j * 16);
          if (code & 0x3e0000) {
            UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17),
                                      TURNS_DIGITS, listWin, TURNS_X, j * 16);
          }
          UiWork_SetParamNibble(15);
          j++;
          if (j > 4) {
            break;
          }
        }
        prevPage = page;
      }
      if (cnt > 5) {
        for (j = 0; j < (pages = ((cnt + 4) / 5)); j++) {
          s32 tile = j + 0xf301;
          if (j == (page / 5)) {
            tile = j + 0xf30b;
          }
          UiWindow_SetTilemapEntry(listWin, tile,
                                   listWin->width - pages + j - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(listWin->x + 1, listWin->y + row * 2 + 1,
                          listWin->width - 2, 1, 14);
      ctx->dirty = 1;
      ctx->menuBusy = 0;
    }

    if (cnt > 5) {
      for (j = 0; j < (cnt + 4) / 5; j++) {
        s32 tile = j + 0xf301;
        if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11) {
          if (j == (page / 5)) {
            tile = j + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            listWin, tile, listWin->width - ((cnt + 4) / 5) + j - 2, -1, 0);
      }
      if ((gKeysHeld & 0x100) == 0) {
        UiWindow_SetTilemapEntry(listWin, 0xf334,
                                 listWin->width - (cnt + 4) / 5 - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWin, 0xf335, listWin->width - 2, -1, 0);
      } else {
        UiWindow_SetTilemapEntry(listWin, 0xf011,
                                 listWin->width - (cnt + 4) / 5 - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWin, 0xf012, listWin->width - 2, -1, 0);
      }
      UiRender_MarkRow(ctx, listWin->y - 1);
    }

    trig = gKeysPressed;
    rep = gKeysRepeat;
    opts = gKeysHeld;
    menu = gLinkCountdownWork;
    if (menu->autoEnabled != 0) {
      rep = 0;
      trig = 0;
      opts = 0;
      if (menu->autoDelay == 0) {
        menu->autoDelay = 60;
        rep = 1;
        trig = 1;
      } else {
        menu->autoDelay--;
      }
    }
    menu = gLinkCountdownWork;
    if (menu->active == 0 || (trig & 2)) {
      AudioCommand_PlayFar(0x71);
      code = -1;
      break;
    }
    if (trig & 1) {
      u32 selection;
      u32 turns;
      if (cnt != 0) {
        selection = table[page + row];
        turns = selection & 0x3e0000;
        if (turns == 0) {
          if (isUsed == 0) {
            menu->page = page;
            menu->row = row;
            menu->preferredRow = savedRow;
            code = selection;
            break;
          }
        } else if (isUsed == 0) {
          Ui_FillVramBlockPattern();
          UiWork_ClearValueNameTables();
          UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
          UiText_CopyMessageString(MsgBattleDjinnRecovering, buf, TEXT_COUNT);
          UiText_RenderWideStringAtOffset(buf, msgWin, 0, 4);
        }
        AudioCommand_PlayFar(0x72);
      } else {
        code = -1;
        break;
      }
    }

    if (cnt != 0) {
      if (rep & 0x80) {
        AudioCommand_PlayFar(0x6f);
        row++;
        if (row == 5 || page + row == cnt) {
          row = 0;
        }
        savedRow = row;
      } else if (rep & 0x40) {
        AudioCommand_PlayFar(0x6f);
        row--;
        if (row < 0) {
          if (page == ((cnt - 1) / 5) * 5) {
            row = cnt - page - 1;
          } else {
            row = 4;
          }
        }
        savedRow = row;
      } else if (opts & 0x100) {
        if (detailWin != 0) {
          for (i = 0; i < subCnt; i++) {
            s32 tile = i + 0xf301;
            if ((gFrameCount & 15) <= 11 && i == subSelection - 1) {
              tile = subSelection - 1 + 0xf30b;
            }
            UiWindow_SetTilemapEntry(detailWin, tile,
                                     detailWin->width - subCnt + i - 2, -1, 0);
          }
          UiWindow_SetTilemapEntry(detailWin, 0xf334,
                                   detailWin->width - subCnt - 3, -1, 0);
          UiWindow_SetTilemapEntry(detailWin, 0xf335, detailWin->width - 2, -1,
                                   0);
          UiRender_MarkRow(ctx, detailWin->y);
        }
        if (subSelection == 0 && subCnt != 0) {
          if (detailWin != 0) {
            RenderOutput_ClearList(detailWin);
          }
          subSelection = 1;
          redraw = 1;
        } else {
          if (subSelection > subCnt) {
            subSelection = subCnt;
          }
          if (subSelection != 0) {
            if (rep & 16) {
              AudioCommand_PlayFar(0x6f);
              subSelection++;
              if (subSelection > subCnt) {
                subSelection = 1;
              }
              redraw = 1;
            } else if (rep & 32) {
              AudioCommand_PlayFar(0x6f);
              subSelection--;
              if (subSelection <= 0) {
                subSelection = subCnt;
              }
              redraw = 1;
            }
          }
        }
      } else if (subSelection != 0) {
        if (detailWin != 0) {
          RenderOutput_ClearList(detailWin);
        }
        subSelection = 0;
        redraw = 1;
      } else if (rep & 16) {
        AudioCommand_PlayFar(0x6f);
        Input_ResetKeyRepeat();
        if (page + 5 >= cnt) {
          if (page != 0) {
            page = 0;
            row = savedRow;
          }
        } else {
          page += 5;
          row = savedRow;
          if (page == ((cnt - 1) / 5) * 5) {
            row = cnt - page - 1;
            if (row > savedRow) {
              row = savedRow;
            }
          }
        }
      } else if (rep & 32) {
        AudioCommand_PlayFar(0x6f);
        Input_ResetKeyRepeat();
        if (page != 0) {
          page -= 5;
          row = savedRow;
        } else {
          page = ((cnt - 1) / 5) * 5;
          row = savedRow;
          if (page != 0) {
            row = cnt - page - 1;
            if (row > savedRow) {
              row = savedRow;
            }
          }
        }
      }
    }

    {
      s32 cx;

      /* FAKEMATCH: the one-pass calculation keeps row's reload after
       * the x shift. Without the block, mov r2, r10 fills the ldrh
       * delay slot before lsls (2 differing instruction rows). */
      do {
        cx = listWin->x * 8;
      } while (0);
      /* FAKEMATCH: instruction order: row read in a low register here is
       * reloaded before the subtract, and row * 2 inherits that reload, so mov
       * r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority,
       * and the reload otherwise has the later luid) */
      asm("" : : "l"(row));
      cursorPos.x = cx - 2;
    }
    cursorPos.y = (row * 2 + listWin->y) * 8 + 20;
    /* FAKEMATCH: forced re-point: curSpr already holds &slot; storing it again
     * here keeps the native reload; without it the function differs 36 in
     * tbs-en and tbs-ja (measured 2026-10-14). */
    curSpr = &slot;
    ((u32 *)curSpr)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)curSpr)[2] = 0;
    curSpr->tile = VramCache_Reload(res, (s32)gResource_FixedBlockBTiles);
    curSpr->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    curSpr->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (cnt != 0) {
      Runtime_PushSlotEntry(curSpr, 242);
    }
    cursor = CURSOR_OWNER;
    blink = gFrameCount & 4;
    iconWin = cursor->window;
    if (cursor->flags & 2) {
      for (i = 0; i <= 3; i++) {
        pal = 15 - (blink != 0);
        if (((1 << i) & mask) == 0) {
          pal = 15;
        }
        Ui_SetRectHighlight(iconWin->x + gDjinnMenu_ElementXOffsets[i] + 1,
                            iconWin->y + gDjinnMenu_ElementYOffsets[i] + 1, 2,
                            2, pal);
      }
    }
    if (gFrameCount & 4) {
      Iwram_CopyWords((void *)0x06006500, gDjinnMenu_BlinkTile, 32);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    } else {
      Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
      Iwram_CopyWords((void *)0x06006520, gDjinnMenu_BlinkTile, 32);
    }
    WaitFrames(1);
  }

  if (CURSOR_OWNER->flags & 2) {
    Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1,
                        CURSOR_OWNER->window->y + 1, 4, 4, 15);
  }
  Resource_ResetEntry(res);
  UiWork_Finalize(msgWin, 1);
  UiWork_Finalize(detailWin, 1);
  UiWork_Finalize(listWin, 1);
  UiWindow_MarkVisibleTileAttributes();
  UiWork_SetAltFlagAndClearTable(0);
  MemFree(table);
  WaitFrames(1);
  return (s32)code;
}

//--------------------------------------------------------------------------
// classify_use_ability.c
// Summon selection menu and checks for item usability in battle
//--------------------------------------------------------------------------

#define BATTLE_OK 0x80 // flags: usable in battle

#if !GAME_TLA

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13 // Column for the required Djinni count
#else
#define SUMMON_COST_X 10
#endif

enum {
  SUMMON_ELEMENT_TILE = 0x5001,       // Element icon
  LIST_TILE_PAGE_NUMBER = 0xf301,     // Page number
  LIST_TILE_PAGE_NUMBER_LIT = 0xf30b, // Current page number
  LIST_TILE_PAGES_LEFT = 0xf334,      // Left arrow
  LIST_TILE_PAGES_RIGHT = 0xf335      // Right arrow
};

#define SUMMON_END 32 // End of summon list

//--------------------------------------------------------------------------
// Summon selection
// Show learned summons four per page; return the selected summon number.
// Cancel returns -1.
// standby: number of standby Djinn for each element
//--------------------------------------------------------------------------
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby) {
  struct CursorSprite cursor;
  struct MenuSprite sprites[4];
  u8 available[33]; // Learned summons
  u8 ordered[33];   // Order with usable summons first
  u16 text[64];
  s32 handles[4];
  u8 visible[4];
  s32 glyph;
  s32 count;
  struct UiWindow *window;
  struct UiRenderWork *render;
  s32 drawnRow, drawnPage, row, page;
  s32 cursorHandle;
  struct UiWindow *description, *counts;
  s32 mask, preferred;
  struct MenuPoint cursorPosition;
  const struct SummonDefinition *summon;
  struct MenuCell *nav;
  struct MenuSprite *sprite;
  struct CursorSprite *cur;
  s32 used;
  s32 id, tile, affordable;
  s32 pressed, repeated, result;
  s32 index;

  render = UI_WORK;
  drawnRow = -1;
  drawnPage = -1;
  cursorHandle = (s16)VramCache_AllocEntry(128); // One 16x16 sprite
  description = UiWindow_Create(0, 4, 30, 4, 42);
  counts = UiWindow_Create(20, 8, 10, 3, 6);
  mask = 0;
  MenuCell_GetCursor(page, row, preferred);
#if EDITION_INTERNATIONAL
  window = UiWindow_Create(13, 11, 17, 9, 6);
#else
  window = UiWindow_Create(16, 11, 14, 9, 6);
#endif

  // Icon sprites for each row.
  for (index = 0; index < 4; index++) {
    s32 y = index * 2;

    sprite = &sprites[index];
    MenuSprite_Init(sprites[index], MENU_SPRITE_SIZE_16);
    sprite->oam.f.x = window->x * 8 + 8;
    sprite->oam.f.y = (y + window->y) * 8 + 4;
  }
  for (index = 0; index < 4; index++) {
    s32 slot = VramCache_AllocEntry(128);
    handles[index] = slot;
    sprites[index].oam.f.tile = VramCache_Reload(slot, -1);
  }

  // Move usable summons to the front in reverse traversal; keep others in
  // order.
  count = Summon_ListLearnedFar(available);
  /* FAKEMATCH: instruction order only (block placement and schedule):
     with the first scan's exit inside this one-pass block, loop.c's
     find_and_verify_loops finds no barrier at depth 0 and leaves the
     main loop's select block in place, as the native has it (without
     the block it moves after the scan: differ 224, 6 branches fewer);
     inside it, count and used are stored before count - 1 is built
     (differ 2 with used = 0 before the block). */
  do {
    s32 i;

    used = 0;
    for (i = count - 1; i >= 0; i--) {
      const struct SummonDefinition *s;
      s32 n, k;

      k = available[i];
      s = SummonDefinition_Get(k);
      for (n = 0; n <= 3; n++) {
        if (s->djinnRequired[n] > standby[n])
          break;
      }
      if (n == 4) {
        ordered[used++] = k;
        available[i] = SUMMON_END;
      }
    }
  } while (0);
  {
    s32 i, k;

    for (i = 0; i < count; i++) {
      k = available[i];
      if (k != SUMMON_END)
        ordered[used++] = k;
    }
  }
  ordered[used] = SUMMON_END;

  for (;;) {
    cur = &cursor;

    //------------------------------------------------------------------
    // Redraw when the page or row changes.
    //------------------------------------------------------------------
    if (page != drawnPage || row != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();

      // Description and required Djinni elements.
      {
        const struct SummonDefinition *s;
        s32 element;

        s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
        UiText_CopyMessageString(s->nameMessageId + MsgAbilityDescription, text,
                                 52);
#else
        UiText_DecodeMessage(s->nameMessageId + MsgAbilityDescription, text,
                             32);
#endif
        UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0,
                                        4);
        drawnRow = row;
        mask = 0;
        for (element = 0; element <= 3; element++) {
          if (s->djinnRequired[element] != 0)
            mask |= 1 << element;
        }
      }

      // Standby counts and the four summons on this page.
      if (page != drawnPage) {
        RenderOutput_RedrawSavedRect(window);
        {
          s32 element;

          for (element = 0; element <= 3; element++) {
            UiWindow_SetTilemapEntry(counts, element + SUMMON_ELEMENT_TILE,
                                     element * 2, 0, 0);
            UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1,
                              0, 0);
          }
        }
        {
          s32 element, col, y;

          index = 0;
          if ((id = ordered[page + index]) != SUMMON_END) {
          next:
            summon = SummonDefinition_Get(id);
            col = 0;
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] > standby[element])
                break;
            }
            affordable = element == 4;
            Ability_LoadGlyph(summon->nameMessageId & 0x3fff, 0,
                              &handles[index], &glyph, 1);
            y = index * 2;
            sprites[index].oam.f.tile = glyph;
            if (!affordable)
              UiWork_SetParamNibble(2); // Color 2 for unavailable summons
            UiText_DrawCharacterAtOffset(
                SummonDefinition_Get(id)->nameMessageId + MsgAbilityName,
                window, 16, index * 16);
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] != 0) {
                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
                                         col * 2 + SUMMON_COST_X, y, 0);
                UiWindow_PutGlyph(window, summon->djinnRequired[element] + '0',
                                  col * 2 + SUMMON_COST_X + 1, y, 0);
                col++;
              }
            }
            UiWork_SetParamNibble(15);
            visible[index++] = 1;
            if (index <= 3 && (id = ordered[page + index]) != SUMMON_END)
              goto next;
          }
          while (index <= 3)
            visible[index++] = 0;
        }
        drawnPage = page;
      }

      // Page numbers
      if (count > 4) {
        for (index = 0; index < (count + 3) / 4; index++) {
          tile = index + LIST_TILE_PAGE_NUMBER;
          if (index == page / 4)
            tile = index + LIST_TILE_PAGE_NUMBER_LIT;
          UiWindow_SetTilemapEntry(
              window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    //------------------------------------------------------------------
    // Per-frame display
    //------------------------------------------------------------------
    for (index = 0; index <= 3; index++) {
      if (visible[index] != 0)
        Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
    }

    // The cursor wobbles every four frames.
    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (row * 2 + window->y) * 8 + 20;
    ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cur)[2] = 0;
    cur->tile =
        VramCache_Reload((u16)cursorHandle, (s32)gResource_FixedBlockBTiles);
    cur->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    cur->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    /* FAKEMATCH: register choice and order: the cursor is pushed through
       sprite, re-pointed at it here as a struct MenuSprite *, which keeps
       the frame loop's registers as the native allocates them. Measured
       (2026-10-13): pushing &cursor or cur directly gives 947
       instructions for 946, differ 87 in all six editions. */
    sprite = (struct MenuSprite *)&cursor;
    Runtime_PushSlotEntry(sprite, 242);

    // Blink required elements' standby counts every eight frames.
    {
      s32 element, tile, blink;

      blink = gFrameCount & 8;
      for (element = 0; element <= 3; element++) {
        tile = 15 - (blink != 0);
        if ((1 << element & mask) == 0)
          tile = 15;
        Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1,
                            tile);
      }
    }

    // Page numbers (current page blinking) and left/right arrows.
    if (count > 4) {
      for (index = 0; index < (count + 3) / 4; index++) {
        tile = index + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11 && index == page / 4)
          tile = index + LIST_TILE_PAGE_NUMBER_LIT;
        UiWindow_SetTilemapEntry(
            window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
      }
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
                               window->width - (count + 3) / 4 - 3, -1, 0);
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                               -1, 0);
      UiRender_MarkRow(render, window->y - 1);
    }

    //------------------------------------------------------------------
    // Input
    //------------------------------------------------------------------
    MenuCell_SetCursor(page, row, preferred);
    nav = gLinkCountdownWork;
    pressed = gKeysPressed;
    repeated = gKeysRepeat;

    // Automatic input confirms every 120 frames.
    if (nav->autoEnabled != 0) {
      repeated = 0;
      pressed = 0;
      if (nav->autoDelay == 0) {
        nav->autoDelay = 120;
        repeated = KEY_A;
        pressed = KEY_A;
      } else {
        nav->autoDelay--;
      }
    }
    if (pressed & KEY_A) {
      result = ordered[page + row];
      break;
    }
    if (gLinkCountdownWork->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeated & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      row++;
      if (row == 4 || page + row == count)
        row = 0;
      preferred = row;
    } else if (repeated & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      row--;
      if (row < 0) {
        if (page == ((count - 1) / 4) * 4)
          row = count - page - 1; // Last page
        else
          row = 3;
      }
      preferred = row;
    } else if (repeated & KEY_RIGHT) {
      // Next page, wrapping from last to first.
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (page + 4 >= count) {
        if (page != 0) {
          page = 0;
          row = preferred;
        }
      } else {
        page += 4;
        row = preferred;
        if (page == ((count - 1) / 4) * 4) {
          row = count - page - 1;
          if (row > preferred)
            row = preferred;
        }
      }
    } else if (repeated & KEY_LEFT) {
      // Previous page, wrapping from first to last.
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (page != 0) {
        page -= 4;
        row = preferred;
      } else {
        page = ((count - 1) / 4) * 4;
        row = preferred;
        if (page != 0) {
          row = count - page - 1;
          if (row > preferred)
            row = preferred;
        }
      }
    }
    WaitFrames(1);
  }

  // Cleanup
  WaitFrames(1);
  for (index = 0; index < 4; index++)
    Resource_ResetEntry(handles[index]);
  Resource_ResetEntry((u16)cursorHandle);
  UiWork_Finalize(counts, 1);
  UiWork_Finalize(description, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  return result;
}
#endif

//--------------------------------------------------------------------------
// Check whether an item can be used in battle.
// Return: 0 usable, 1 unusable, 2 unusable in battle.
// unit: character index
// item: inventory item value
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item) {
  s32 ret;
  struct ItemDefinition *def;

  if (item == 0) {
    return 1;
  }
  def = Item_GetDirectFar(item);
  ret = 1;
  if (def->useType == 3) // Equipment that grants Psynergy
  {
    return ret;
  }
  if (def->actionId == 0) {
    return ret;
  }
  if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0)) {
    return ret;
  }
  if ((BATTLE_OK & BattleActGet(def->actionId)->flags) == 0) {
    return 2;
  }
  return 0;
}

void Vram_CopyTile(u32 source_tile, u32 destination_tile) {
  Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
          (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32, 0x84000008,
          REG_DMA3);
}

s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count) {
  struct UiRenderWork *render;
  s32 drawnPage;
  s32 drawnRow;
  s32 cursorHandle;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint pos;
  struct MenuCell *cell;
  struct UiWindow *window;
  s32 page;
  s32 row;
  s32 entry;
  s32 tile;
  s32 edge;
  s32 reason;
  s32 result;
  s32 i;
  struct CursorSprite cursor;
  struct MenuSprite *spr;
  struct CursorSprite *cur;
  u16 buffer[64];
  s32 handles[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];

  render = UI_WORK;
  drawnPage = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = 0;
  MenuCell_GetCursor(page, row, preferredRow);
  window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y = i * 2;

    spr = &sprites[i];
    MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
    spr->oam.f.x = window->x * 8 + 8;
    spr->oam.f.y = (y + window->y) * 8 + 4;
  }

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    handles[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(handles[i], -1);
  }

  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  cur = &cursor;
  for (;;) {
    if (page != drawnPage || row != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
        if (Item_ClassifyUseAbility(owner, entries[page + row]) == 2) {
          TEXT_LOAD(MsgBattleItemUnavailable, buffer, TEXT_COUNT);
        } else {
#if EDITION_INTERNATIONAL
          UiText_CopyMessageString((entries[page + row] & ITEM_ID_MASK) +
                                       MsgItemPlainName,
                                   buffer, 52);
#else
          UiText_DecodeMessage((entries[page + row] & ITEM_ID_MASK) +
                                   MsgItemPlainName,
                               buffer, 32);
#endif
        }
      } else {
        TEXT_LOAD(MsgBattleNoItems, buffer, TEXT_COUNT);
      }
      UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
      drawnRow = row;

      if (page != drawnPage) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          entry = entries[page + i];
          if (entry == 0) {
            break;
          }
          Item_GetDirectFar(entry);
          UiWork_SetParamNibble(15);
          if (Item_ClassifyUseAbility(owner, entry) != 0) {
            UiWork_SetParamNibble(4);
          } else if ((entry & INVENTORY_BROKEN) != 0) {
            UiWork_SetParamNibble(2);
          }
          UiText_DrawCharacterAtOffset((entry & ITEM_ID_MASK) + MsgItemName,
                                       window, 16, i * 16);
          UiWork_SetParamNibble(15);
          sprites[i].oam.f.tile =
              Resource_LoadKind26EntryToBuffer(entry, handles[i]);
        }
        iconCount = i;
        drawnPage = page;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          tile = i + LIST_TILE_PAGE_NUMBER;
          if (i == page / BATTLE_LIST_ROWS) {
            tile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
          UiWindow_SetTilemapEntry(
              window, tile,
              window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
        }
      }

      Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
        tile = i + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11) {
          if (i == page / BATTLE_LIST_ROWS) {
            tile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
        }
        UiWindow_SetTilemapEntry(
            window, tile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      edge = -1;
      UiWindow_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                               edge, 0);
      UiRender_MarkRow(render, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      spr = &sprites[i];
      Runtime_PushSlotEntry(spr, 240);
    }

    pos.x = window->x * 8 - 2;
    pos.y = (row * 2 + window->y) * 8 + 20;
    /* FAKEMATCH: register choice (spill slots) only: re-pointing cur each
       frame orders the spill slots of the cursor address and row*2 as the
       native frame does. Measured (2026-10-13, after MenuCell_SetCursor):
       without it differ 56; with it but without the assignment before
       the loop, differ 14; the cursor written directly, with no cur,
       781 instructions for 770 (differ 209). Battle_SelectAbility sets
       curSpr the same two ways, untagged (394 and 36 without them). */
    cur = &cursor;
    ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cur)[2] = 0;
    cur->tile = VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    cur->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
    cur->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(cur, 242);
    }

    MenuCell_SetCursor(page, row, preferredRow);
    cell = gLinkCountdownWork;

    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = page + row;
        Item_GetDirectFar(entries[result]);
        reason = 0;
        if ((entries[result] & INVENTORY_BROKEN) == 0) {
          reason = Item_ClassifyUseAbility(owner, entries[result]);
          if (reason == 0) {
            break;
          }
        }
        AudioCommand_PlayFar(114);
        if (reason == 2) {
          TEXT_LOAD(MsgBattleItemUnavailable, buffer, TEXT_COUNT);
        } else if ((entries[result] & INVENTORY_BROKEN) != 0) {
          TEXT_LOAD(MsgBattleItemBroken, buffer, TEXT_COUNT);
        } else {
          TEXT_LOAD(MsgBattleItemNoEffect, buffer, TEXT_COUNT);
        }
        Ui_FillVramBlockPattern();
        UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
      } else {
        result = -1;
        break;
      }
    } else if (cell->active == 0 || (gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(113);
      result = -1;
      break;
    }

    if (count != 0) {
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        AudioCommand_PlayFar(111);
        row++;
        if (row == BATTLE_LIST_ROWS || page + row == count) {
          row = 0;
        }
        preferredRow = row;
      } else if ((gKeysRepeat & KEY_UP) != 0) {
        AudioCommand_PlayFar(111);
        row--;
        if (row < 0) {
          if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            row = count - page - 1;
          } else {
            row = BATTLE_LIST_ROWS - 1;
          }
        }
        preferredRow = row;
      } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(111);
        Input_ResetKeyRepeat();
        if (page + BATTLE_LIST_ROWS >= count) {
          if (page != 0) {
            page = 0;
            row = preferredRow;
          }
        } else {
          page += BATTLE_LIST_ROWS;
          row = preferredRow;
          if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            row = count - page - 1;
            if (row > preferredRow) {
              row = preferredRow;
            }
          }
        }
      } else if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(111);
        Input_ResetKeyRepeat();
        if (page != 0) {
          page -= BATTLE_LIST_ROWS;
          row = preferredRow;
        } else {
          page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
          row = preferredRow;
          if (page != 0) {
            row = count - page - 1;
            if (row > preferredRow) {
              row = preferredRow;
            }
          }
        }
      }
    }
    WaitFrames(1);
  }

  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  Resource_ResetEntry(cursorHandle);
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    Resource_ResetEntry(handles[i]);
  }
  WaitFrames(1);
  return result;
}

s32 BattleMenu_RunActionSelection(s32 unitId, u16 *actions, s32 count) {
  struct UiRenderWork *render;
  s32 drawnPage;
  s32 page;
  s32 drawnRow;
  s32 cursorHandle;
  struct BattleUnit *unit;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint pos;
  struct MenuCell *nav;
  struct UiWindow *window;
  s32 row;
  s32 entry;
  s32 tile;
  s32 edge;
  s32 range;
  s32 result;
  s32 i;
  struct MenuSprite cursor;
  struct MenuSprite *spr;
  u16 buffer[64];
  s32 handles[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];
  s32 glyph;

  render = UI_WORK;
  drawnRow = -1;
  drawnPage = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = UnitGetFar(unitId);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = BATTLE_LIST_ROWS;
  MenuCell_GetCursor(page, row, preferredRow);
  window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y = i * 2;

    spr = &sprites[i];
    MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
    spr->oam.f.x = window->x * 8 + 8;
    spr->oam.f.y = (y + window->y) * 8 + 4;
  }

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    handles[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(handles[i], -1);
  }

  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  spr = &cursor;
  for (;;) {
    if (page != drawnPage || row != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
#if EDITION_INTERNATIONAL
        UiText_CopyMessageString((actions[page + row] & ACTION_ID_MASK) +
                                     MsgAbilityDescription,
                                 buffer, 52);
#else
        UiText_DecodeMessage((actions[page + row] & ACTION_ID_MASK) +
                                 MsgAbilityDescription,
                             buffer, 32);
#endif
      } else {
        TEXT_LOAD(MSGNO(MsgBattleNoPsynergy), buffer, TEXT_COUNT);
      }
      UiText_RenderWideStringAtOffset(buffer, messageWindow, 0, 4);
      drawnRow = row;

      if (page != drawnPage) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          struct BattleAct *action;

          entry = actions[page + i];
          if (entry == 0) {
            break;
          }
          action = BattleActGet(entry);
          UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
          UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
          UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
          Ability_LoadGlyph(entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
          sprites[i].oam.f.tile = glyph;

          if ((action->flags & ACTION_USABLE_IN_BATTLE) == 0) {
            UiWork_SetParamNibble(4);
          } else if (action->ppCost > unit->pp) {
            UiWork_SetParamNibble(2);
          } else if (unit->psySeal != 0) {
            UiWork_SetParamNibble(9);
          }

          render->level = 5;
          UiText_DrawCharacterAtOffset(entry + MsgAbilityName, window, 16,
                                       i * 16);
          UiText_DrawNumberAtOffset(action->ppCost, 2, window, 104, i * 16);
          UiWork_SetParamNibble(15);
          render->level = 15;

          if (action->element != 4) {
            UiWindow_SetTilemapEntry(window, action->element + 0x5001, 15,
                                     i * 2, 0);
          }

          range = action->range;
          if (range == 255) {
            range = 11;
          } else {
            range--;
          }
          UiWindow_DrawThreeTileColumn(window, 16, i * 2, range, 0);
        }
        iconCount = i;
        drawnPage = page;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          tile = i + 0xf301;
          if (i == page / BATTLE_LIST_ROWS) {
            tile = i + 0xf30b;
          }
          UiWindow_SetTilemapEntry(
              window, tile,
              window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
        }
      }

      Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
        tile = i + 0xf301;
        if ((gFrameCount & 15) <= 11) {
          if (i == page / BATTLE_LIST_ROWS) {
            tile = i + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            window, tile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      edge = -1;
      UiWindow_SetTilemapEntry(
          window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3,
          edge, 0);
      UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, edge, 0);
      UiRender_MarkRow(render, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      Runtime_PushSlotEntry(&sprites[i], 240);
    }

    pos.x = window->x * 8 - 4;
    pos.y = (row * 2 + window->y) * 8 + 20;
    ((u32 *)spr)[1] = 0x40000000;
    ((u32 *)spr)[2] = 0;
    spr->oam.f.tile =
        VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
    spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(spr, 242);
    }

    MenuCell_SetCursor(page, row, preferredRow);
    nav = gLinkCountdownWork;

    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = page + row;
        if ((BattleActGet(actions[result])->flags & ACTION_USABLE_IN_BATTLE) !=
            0) {
          break;
        }
        /* FAKEMATCH: a dead store here keeps the test above from being
           threaded past the break. Measured (2026-10-13, after
           MenuCell_SetCursor): without it the unusable branch skips the
           native jump through the next block (793 instructions for 794,
           differ 11 in every edition). */
        result = -1;
      } else {
        result = -1;
        break;
      }
    } else if (nav->active == 0 || (gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(113);
      result = -1;
      break;
    }

    if (count != 0) {
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        AudioCommand_PlayFar(111);
        row++;
        if (row == BATTLE_LIST_ROWS || page + row == count) {
          row = 0;
        }
        preferredRow = row;
      } else if ((gKeysRepeat & KEY_UP) != 0) {
        AudioCommand_PlayFar(111);
        row--;
        if (row < 0) {
          if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            row = count - page - 1;
          } else {
            row = BATTLE_LIST_ROWS - 1;
          }
        }
        preferredRow = row;
      } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(111);
        Input_ResetKeyRepeat();
        if (page + BATTLE_LIST_ROWS >= count) {
          if (page != 0) {
            page = 0;
            row = preferredRow;
          }
        } else {
          page += BATTLE_LIST_ROWS;
          row = preferredRow;
          if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            row = count - page - 1;
            if (row > preferredRow) {
              row = preferredRow;
            }
          }
        }
      } else if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(111);
        Input_ResetKeyRepeat();
        if (page != 0) {
          row = preferredRow;
          page -= BATTLE_LIST_ROWS;
        } else {
          page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
          row = preferredRow;
          if (page != 0) {
            row = count - page - 1;
            if (row > preferredRow) {
              row = preferredRow;
            }
          }
        }
      }
    }
    WaitFrames(1);
  }

  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(window, 1);
  WaitFrames(1);
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    Resource_ResetEntry(handles[i]);
  }
  Resource_ResetEntry(cursorHandle);
  WaitFrames(1);
  return result;
}

struct CursorSlot {
  u8 x;
  u8 y;
  u8 flags;
  s8 index;
};

s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  u16 ids[8];
  struct SelectionSprite entries[6];
  struct CursorSlot table[6];
  struct EffectPosition markPos;
  u16 selectionIds[14];
  u8 selectionSlot[8];
  struct EffectPosition pos;
  u16 name[15];
  struct EffectPosition namePos;
  struct EffectPosition targetPos;
  struct AffineTransform effect;
  struct BattleUnit *unit;
  struct BattleWork *runtime;
  struct UiWindow *window;
  /* FAKEMATCH: the earlier combined pin/barrier removal gave EN 1585/1585,
   * common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320.
   * ES 1684/1684, common 1569, differ 230: infoWin/selection swap r10/r9 and
   * spill slots permute. Both extents stayed unchanged. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
  struct UiWindow *infoWin;
#else
  /* FAKEMATCH: register choice only: as an ordinary local the info window is
   * spilled and the frame shrinks to 320 bytes (differ 492); the native keeps
   * it in r9. 2026-10-11: its live length is doubled by update_equiv_regs (the
   * first set carries a constant REG_EQUAL), giving priority 2104 against &pos
   * 2171 and the hoisted bit-field zero 2142; one duplicated no-effect block
   * reaches 2143 against 2149 (differ 202), two push &pos below the zero
   * (differ 449), and an initializer without the note sends it to r10 as in
   * ES/IT (differ 96). */
  register struct UiWindow *infoWin asm("r9");
#endif
  s32 cursor;
  s32 cnt;
  s32 total;
  s32 redraw;
  s32 slotId;
  s32 pending;
  s32 matrix;
  s32 found;
  s32 column;
  s32 rows;
  s32 count;
  s32 i;
  s32 j;
  s32 n;
  s32 nx;
  s32 ny;
  s32 pressed;
  s32 y;
  s32 width;
  s32 repeat;
  s32 result;
  struct CursorSlot *slot;
  struct SelectionSprite *entry;
  struct SelectionSprite *head;
  struct BattleUnit *who;

  runtime = gBattleWork;
  cnt = 0;
  redraw = 0xFFFF;
  slotId = VramCache_AllocEntry(256);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
  /* FAKEMATCH: register choice only: this value-preserving null
   * initializer is needed to match infoWin in r10; a direct zero swaps
   * high registers and spill slots (2026-10-13: infoWin = 0 differs 94
   * in ES and IT). */
  infoWin = (struct UiWindow *)((u32)(u16)selection >> 16);
#else
  infoWin = 0;
#endif
  if (spread == 0)
    spread = 1;
  // UB: slot 57 is outside the scalar gBattleWork pointer. Named accesses
  // add 12 bytes here in EN; using gWorkSlot adds 16 bytes.
#ifdef UBFIX
  if (mode == 2 || mode == 4)
    gLinkCountdownWork->slideOffset = -2;
  else
    gLinkCountdownWork->slideOffset = 16;
#else
  if (mode == 2 || mode == 4)
    ((struct MenuCell **)&gBattleWork)[48]->slideOffset = -2;
  else
    ((struct MenuCell **)&gBattleWork)[48]->slideOffset = 16;
#endif
  for (i = 0; i < 6; i++)
    table[i].flags = 0;
  cursor = -1;
  if (mode == 2) {
    for (i = 0; i <= 5 && runtime->partyUnits[i] != 0xFF; i++) {
      ids[cnt] = runtime->partyUnits[i];
      cnt++;
    }
  } else if (mode == 4) {
    ids[0] = (u16)selection;
    cnt = 1;
  } else {
    for (i = 0; i <= 5 && runtime->enemyIds[i] != 0xFF; i++) {
      ids[cnt] = runtime->enemyIds[i];
      cnt++;
    }
  }
  ids[cnt] = 0xFF;
  total = cnt;
  if (mode == 2) {
    if (spread != 0xFF && kind != 0) {
      found = 0;
      for (j = 0; j < cnt; j++) {
        struct BattleUnit *st;

        if (ids[j] == 0xFE)
          continue;
        st = UnitGetFar(ids[j]);
        switch (kind) {
        case 5:
          if (st->hp == 0) {
            selection = ids[j];
            found = 1;
          }
          break;
        case 3:
          if (st->poison != 0)
            found = 1;
          break;
        case 4:
          if (st->delusion != 0 || st->stun != 0 || st->sleep != 0 ||
              st->psySeal != 0 || st->grimReaper != 0)
            found = 1;
          break;
        case 6:
          if (st->delusion != 0 || st->stun != 0 || st->sleep != 0 ||
              st->psySeal != 0 || st->grimReaper != 0 || st->poison != 0 ||
              st->haunt != 0)
            found = 1;
          break;
        }
        if (found != 0) {
          selection = ids[j];
          break;
        }
      }
    }
    for (j = 0; j < cnt; j++) {
      if (ids[j] == selection)
        break;
    }
    if (j != cnt)
      cursor = j;
  }
  if (cursor < 0)
    cursor = (cnt - 1) / 2;
  while (ids[cursor] == 0xFE || (GameFlag_TestFar(0x16C) && mode == 1 &&
                                 UnitGetFar(ids[cursor])->hp == 0)) {
    cursor = cursor + cnt - 1;
    cursor %= cnt;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(selection, (s32 *)&markPos);
    table[0].flags = 8;
    table[0].x = (u8)markPos.x;
    table[0].y = 0x80;
  }
  window = UiWindow_Create(0, 12, 30, 4, 74);
  while (1) {
    head = entries;
    pending = 0;
    {
      s32 t = cursor * 2;

      /* FAKEMATCH: the separate switch arms end CSE before the
       * projection call, keeping the scaled offset reload ahead of
       * the ids load. Sharing their block reverses two rows; no
       * switch branch remains in any edition. Plain forms measured
       * (2026-10-13, EN and ES alike): ids[cursor] or a block-local
       * t = cursor * 2 reload ids before cursor * 2 (differ 2);
       * cursor[ids], *(cursor + ids) and (u8 *)ids + cursor * 2
       * differ 24; the call ahead of head and pending 12 to 24. */
      switch (t) {
      case 0:
        t = cursor * 2;
        break;
      default:
        t = cursor * 2;
        break;
      }
      BattleMotion_ProjectConditionalPositionFar(*(u16 *)((u8 *)ids + t),
                                                 (s32 *)&pos);
    }
    ((s32 *)head)[1] = 0x40002000;
    ((s32 *)head)[2] = 0;
    head->tile = VramCache_Reload(slotId, (((gFrameCount >> 2) & 31) << 8) +
                                              (s32)gMenu_AnimatedCursorTiles);
    pos.y += Trig_Sin(gFrameCount << 12) / 32768;
    y = pos.y;
    if (table[0].flags & 1) {
      nx = (pos.x + table[0].x) / 2;
      ny = (y + table[0].y) / 2;
      if ((pos.x - nx < 0 ? -(pos.x - nx) : pos.x - nx) <= 7)
        pending = 1;
      pos.x = nx;
      table[0].x = (u8)nx;
      table[0].y = (u8)ny;
      pos.y = ny;
    } else if ((u8)table[0].flags <= 3) {
      table[0].x = (u8)pos.x;
      pos.y = y;
      table[0].y = (u8)y;
      table[0].flags = 1;
    } else {
      pos.x = table[0].x;
      pos.y = table[0].y;
      table[0].flags = (u8)(table[0].flags - 4);
      if (table[0].flags <= 3)
        table[0].flags = 1;
    }
    head->x = pos.x - 8;
    head->y = (u8)(pos.y - 16);
    Runtime_PushSlotEntry((s32 *)head, 240);
    if (spread == 0xFF) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_IT)
    /* FAKEMATCH: seventeen dead stores restore the native frame's spill slots
     * (table, entries, ids and cursor * 2 at sp+36 to sp+24, above &effect and
     * selectionSlot); without them differ 102 in tbs-en, every row a spill
     * offset. ES and IT match without them. */
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
    effect.angle = 0;
#endif
    matrix = AffineMatrix_BuildForEffect(&effect);
    if (redraw & 1) {
      cnt = 0;
      for (i = 0; i < 6; i++)
        table[i].flags &= (u8)~2;
      for (i = 0; (u32)i < spread; i++) {
        {
          n = cursor + i;
          if (n < total && ids[n] != 0xFE) {
            selectionIds[cnt] = ids[n];
            table[i].flags |= 2;
            if (table[i].index != i) {
              table[i].flags &= (u8)~1;
              table[i].index = (s8)i;
            }
            selectionSlot[cnt] = (u8)i;
            cnt++;
          }
          if (i != 0 && (n = cursor - i) >= 0 && ids[n] != 0xFE) {
            selectionIds[cnt] = ids[n];
            table[6 - i].flags |= 2;
            if (table[6 - i].index != -i) {
              table[6 - i].flags &= (u8)~1;
              table[6 - i].index = (s8)-i;
            }
            selectionSlot[cnt] = (u8)(6 - i);
            cnt++;
          }
        }
      }
      for (i = 0; i < 6; i++) {
        if ((table[i].flags & 2) == 0)
          table[i].index = 6;
      }
      selectionIds[cnt] = 0xFF;
      BattlePres_SetActorModesFar(selectionIds, 1);
      if (ids[cursor] <= 7) {
        if (spread != 0xFF && kind != 0) {
          unit = UnitGetFar(ids[cursor]);
          BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
          if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);
          switch (kind) {
          case 7: {
            s32 anchor;

            anchor = pos.x / 8;
            column = anchor - 4;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            if (System_GetLanguage() == 4) {
              if (anchor + 6 > 29)
                column = 20;
              infoWin = UiWindow_Create(column, 8, 10, 3, 6);
            } else if (System_GetLanguage() == 3) {
              if (anchor + 7 > 29)
                column = 19;
              infoWin = UiWindow_Create(column, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                column = 21;
              infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            }
#elif defined(TBS_EDITION_FR)
            if (anchor + 5 > 29)
              column = 21;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#elif defined(TBS_EDITION_JA)
            if (anchor + 4 > 29)
              column = 22;
            infoWin = UiWindow_Create(column, 8, 8, 3, 6);
#else
            if (anchor + 4 > 29)
              column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#endif
          }
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetSetDjinni, infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            break;
          case 1: {
            s32 anchor;

            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 6 > 29)
              column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
          }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            switch (System_GetLanguage()) {
            case 1:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString, infoWin, 0,
                                        0);
              break;
            case 2:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, infoWin, 0,
                                        0);
              break;
            case 3:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, infoWin, 0,
                                        0);
              break;
            case 4:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, infoWin, 0,
                                        0);
              break;
            }
#else
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, infoWin, 0,
                                      0);
#endif
            UiText_DrawNumberInWindow(unit->hp, 4, infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, infoWin,
                                      48, 0);
            UiText_DrawNumberInWindow(unit->maxHp, 4, infoWin, 56, 0);
            break;
          case 2: {
            s32 anchor;

            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 6 > 29)
              column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
          }
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString, infoWin, 0,
                                      0);
            UiText_DrawNumberInWindow(unit->pp, 4, infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, infoWin,
                                      48, 0);
            UiText_DrawNumberInWindow(unit->maxPp, 4, infoWin, 56, 0);
            break;
          case 5: {
            s32 anchor;

            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 5 > 29)
              column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
          }
            if (unit->hp != 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetDowned, infoWin, 0, 0);
            break;
          case 3: {
            s32 anchor;

            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 5 > 29)
              column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
          }
            if (unit->poison == 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetCurePoison, infoWin, 0, 0);
            break;
          case 4:
            count = 0;
            if (unit->delusion != 0)
              count = 1;
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (count == 0)
              count = 1;
            rows = 9 - count;
            if (rows <= 3)
              rows = 4;
            {
              s32 anchor;

              anchor = pos.x / 8;
              column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  column = 13;
                infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
              }
#elif defined(TBS_EDITION_JA)
              if (anchor + 7 > 29)
                column = 16;
              infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                column = 14;
              infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion, infoWin, 0,
                                           0);
              count = 1;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, infoWin, 0,
                                           count * 8);
              ++count;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (count == 0)
              goto no_condition;
            break;
          default:
            break;
          case 6:
            count = 0;
            if (unit->poison != 0)
              count = 1;
            if (unit->delusion != 0)
              count++;
            if (unit->stun != 0)
              count++;
            if (unit->sleep != 0)
              count++;
            if (unit->psySeal != 0)
              count++;
            if (unit->grimReaper != 0)
              count++;
            if (unit->haunt != 0)
              count++;
            if (count == 0)
              count = 1;
            rows = 9 - count;
            if (rows <= 3)
              rows = 4;
            {
              s32 anchor;

              anchor = pos.x / 8;
              column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  column = 13;
                infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
              }
#elif defined(TBS_EDITION_JA)
              if (anchor + 7 > 29)
                column = 16;
              infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                column = 14;
              infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->poison != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCurePoison, infoWin, 0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, infoWin, 0,
                                           count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureHaunt, infoWin, 0,
                                           8 * count);
              count++;
            }
            if (count != 0)
              break;
          no_condition:
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetNoEffect, infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            break;
          }
        }
      } else if (spread != 0xFF) {
        who = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor],
                                                   (s32 *)&namePos);
        namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (who->classId == 125 || who->classId == 122) {
          width = MsgMysteryPersonName;
          if (who->classId == 125)
            width++;
#if defined(TBS_EDITION_JA)
          UiText_DecodeMessage(width, name, 10);
#else
          UiText_CopyMessageString(width, name, 14);
#endif
        } else {
          for (i = 0; i <= 13; i++) {
            name[i] = n = who->name[i];
            if (!n)
              break;
          }
          name[i] = 0;
        }
        width = UiText_GetWideStringWidth(name);
        namePos.x -= width / 2;
        namePos.x -= 8;
        if (namePos.x + width > 224)
          namePos.x = 224 - width;
        if (namePos.x < 0)
          namePos.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(name, window, namePos.x, 4);
      }
      redraw &= ~1;
    }
    if (pending != 0) {
      for (i = 1; i < cnt; i++) {
        entry = &head[i];
        slot = &table[selectionSlot[i]];
        BattleMotion_ProjectConditionalPositionFar(selectionIds[i],
                                                   (s32 *)&targetPos);
        targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
        *entry = *head;
        if (slot->flags & 1) {
          s32 x = (targetPos.x + slot->x) / 2;
          s32 y = (targetPos.y + slot->y) / 2;

          targetPos.x = x;
          slot->x = x;
          targetPos.y = y;
          slot->y = y;
        } else {
          s32 x = entry->x;
          s32 y = entry->y + 8;

          slot->flags = 1;
          targetPos.x = x;
          slot->x = x;
          targetPos.y = y;
          slot->y = y;
        }
        entry->mode = 1;
        entry->x = targetPos.x - 8;
        entry->y = targetPos.y - 12;
        if (spread == 0xFF)
          entry->affine = 0;
        else
          entry->affine = 1;
        entry->param = matrix;
        Runtime_PushSlotEntry((s32 *)entry, 240);
      }
    }
    pressed = gKeysPressed;
    repeat = gKeysRepeat;
    if (gLinkCountdownWork->autoEnabled) {
      repeat = 0;
      pressed = 0;
      if (gLinkCountdownWork->autoDelay == 0) {
        gLinkCountdownWork->autoDelay = 60;
        repeat = 1;
        pressed = 1;
      } else {
        gLinkCountdownWork->autoDelay--;
      }
    }
    if (pressed & 1) {
      s32 id;
      s32 k;

      id = ids[cursor];
      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && runtime->partyUnits[k] != 0xFF; k++) {
        if (runtime->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && runtime->enemyIds[k] != 0xFF; k++) {
          if (runtime->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      cursor = result;
    } else if (spread != 0xFF) {
      if (repeat & 0x90) {
        AudioCommand_PlayFar(111);
        do {
          cursor++;
          cursor %= total;
        } while (ids[cursor] == 0xFE);
        redraw |= 1;
      }
      if (repeat & 0x60) {
        AudioCommand_PlayFar(111);
        do {
          cursor = cursor + total - 1;
          cursor %= total;
        } while (ids[cursor] == 0xFE);
        redraw |= 1;
      }
    }
    if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
      AudioCommand_PlayFar(113);
      cursor = -1;
      break;
    }
    WaitFrames(1);
    if (redraw == 0)
      break;
  }
  WaitFrames(1);
  Resource_ResetEntry(slotId);
  if (infoWin != 0)
    UiWork_Finalize(infoWin, 1);
  UiWork_Finalize(window, 1);
  BattlePres_SetActorModesFar(ids, 0);
  gLinkCountdownWork->slideOffset = 0;
  WaitFrames(1);
  return cursor;
}

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

void UpdateLinkSessionCountdown(void) {
  struct LinkRoundState *state = COUNTDOWN;
  struct BattleWork *runtime;
  struct MenuSprite *entry;
  u8 *active;
  struct LinkSignature *signature;
  s32 target;
  s32 current;
  s32 difference;
  s32 step;
  s32 newOffset;
  s32 timer;
  s32 nextTimer;
  s32 seconds;
  s32 i;

  if (state != 0) {
    target = state->targetOffset;
    current = state->currentOffset;
    if (target != current) {
      difference = target - current;
      step = difference / 3;
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Runtime_PushSlotEntry(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    if (state->enabled == 0)
      return;

    runtime = gBattleWork;
    if (runtime->linkPaused == 0)
      goto load_timer;
    state->timer = 0;
    return;

  signature_tail:
    if (signature->v != 'V')
      goto signature_done;
    if (signature->s != 'S')
      goto signature_done;
    state->timer = COUNTDOWN_START_FRAMES;
    timer = state->timer;
    goto signature_done;

  load_timer:
    timer = state->timer;
    if (timer >= 0)
      goto timer_ready;
    signature =
        (struct LinkSignature *)gLinkPeerSignatures[runtime->linkSide ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      return;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = (s32)Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      return;

    seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

    if (state->displayHandle != 0)
      UiText_DrawNumberInWindow(seconds, 2,
                                (struct UiWindow *)state->displayHandle, 16, 8);
  }
}

s32 UiText_ShowLocalizedMessageAndWait(void) {
  s16 buffer[64];
  struct UiWindow *work;
  s32 result;
  struct BattleWork *battle;
  s32 mode;

  battle = gBattleWork;
  mode = gGameState.battleStart;
  if (gGameState.battleStart == 2 || mode <= 2 || (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if ((s8)battle->challengeFlag != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    work = UiWindow_Create(0, 7, 30, 4, 42);
    Ui_FillVramBlockPattern();
    TEXT_LOAD(MsgNoTimeToRun, buffer, TEXT_COUNT);
    UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
    do {
      WaitFrames(1);
    } while ((gKeysPressed & (KEY_A | KEY_B)) == 0 && COUNTDOWN->timer != 0);
    UiWork_Finalize(work, 1);
  }
  return result;
}

//--------------------------------------------------------------------------
// classify_entry_kind.c
// Battle: ability target selection category
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Return the target selection category for an ability's effect.
// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure status ailments,
// 5 revive, 6 cure all status ailments.
// act: ability data
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->flags & 15;
  if (type == 1) // Restore HP
    kind = 1;
  if (type == 11) // Restore PP
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // Cure poison
    kind = 3;
  if (effect == 4) // Cure status ailments
    kind = 4;
  if (effect == 64) // Cure all status ailments
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive
    kind = 5;
  return kind;
}

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in,
                                s32 count) {
  struct LinkRoundState *state;
  struct BattleWork *link;
  struct BattleWork *session;
  u16 *peer;
  u16 *sync;
  struct BattleActionRecord *ent;
  struct BattleUnit *actor;
  struct UiWindow *win;
  struct MenuSprite *slot;
  u16 hdr[2];
  s32 aux;
  s32 actorId;
  s32 mode;
  s32 kind;
  s32 param;
  s32 res1024;
  s32 res512;
  s32 miss;
  s32 i;
  s32 k;
  s32 order;

  void DrawPartyPanels(void) {
    u16 data[2];
    data[0] = 0xff;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
  }

  aux = 256;
  kind = 0;
  param = 0;
  res1024 = VramCache_AllocEntry(1024);
  res512 = VramCache_AllocEntry(512);
  state = Runtime_AllocateBlock(57, 260);
  Resource_LoadIndexedIntoBuffer(res1024, 0);
  Graphics_ExpandVramTilesByColorTable(0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  link = gBattleWork;
  for (k = 0; k < 8; k++) {
    state->selected[k] = 0x80000000;
  }
  for (k = 0; k < 3; k++) {
    state->entryActive[k] = 0;
  }
  state->targetOffset = 0;
  state->currentOffset = 0;
  state->timer = -1;
  state->unk3c = 0;
  state->unk40 = 0;
  state->enabled = 0;
  state->secondaryHandle = 0;
  state->displayHandle = 0;
  if (link->twoSided != 0) {
    state->enabled = 1;
    sync = (u16 *)gSerialTransfer.payload;
    sync[4] = 'V';
    sync[5] = 'S';
    sync[6] = 'S';
    sync[7] = 'T';
    miss = 0;
    for (;;) {
      peer = (u16 *)&gLinkPeerSignatures[link->linkSide ^ 1];
      if (link->linkPaused != 0) {
        count = -1;
        state->enabled = 0;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        if (++miss > 24) {
          count = -1;
          state->enabled = 0;
          break;
        }
      } else {
        miss = 0;
        if (peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' &&
            peer[7] == 'T') {
          break;
        }
        if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' &&
            peer[7] == 'S') {
          break;
        }
        if (!(peer[0] == 'E' && peer[1] == 'X' && peer[2] == 'E' &&
              peer[3] == 'C')) {
          count = -1;
          state->enabled = 0;
          break;
        }
      }
      WaitFrames(1);
    }
  }
  Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

  while (1) {
    DrawPartyPanels();
    state->entryActive[2] = 0;
    state->actorId = -1;
    state->hintCount = 0;
    if (GameFlag_TestFar(366) != 0) {
      state->hintCount = 1;
      state->hintTimer = 60;
    }
    if (count > 0) {
      mode = Ui_RunSelectionScreen(0);
    } else {
      mode = 14;
    }
    if (mode == 7) {
      void *block;
      s32 selection;
      s32 handle;

      block = MemAlloc(12);
      if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0) {
        selection = 2;
      } else {
        selection = 1;
      }
      handle = BattleParty_ListActorIdsFar(selection, block);
      WaitFrames(1);
      Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
      MemFree(block);
      continue;
    }
    if (mode == 4) {
      if (UiText_ShowLocalizedMessageAndWait() != 0) {
        continue;
      }
      count = 1;
      ent = out;
      ent->unitId = in[0];
      ent->priority = 0x7ffe;
      ent->command = 99;
      ent->parameter = 0;
      ent->target = 256;
      break;
    }
    if (mode != 14) {
      break;
    }
    AudioCommand_PlayFar(154);
    i = 0;
    if (i >= count) {
      break;
    }

    for (;;) {
      if (i == 0) {
        BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++) {
          state->cost[i][k] = state->cost[i - 1][k];
        }
      }

      ent = &out[i];
      actorId = in[i];
      // UB: UnitGetFar returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      actor = UnitGetFar(actorId);
#else
      /* FAKEMATCH: instruction order only: called through a cast as
         returning a doubleword, the call also sets r1, so the r1
         reload for classId no longer depends on the in reload and
         sched2 loads i * 2 before in, as native (by its prototype,
         differ 2). */
      actor = (struct BattleUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
#endif
      COUNTDOWN->actorId = actorId;
      COUNTDOWN->unk40 = 0;
      slot = &COUNTDOWN->entries[2];
      slot->oam.word.attr01 = 0x80000400;
      slot->oam.word.attr23 = 0;
      slot->oam.f.tile = Ui_LoadEntryForKind(actor->classId, res512);
#if EDITION_INTERNATIONAL
      slot->oam.f.x = 0;
#else
      slot->oam.f.x = 4;
#endif
      slot->oam.f.y = 0x80;
      slot->oam.f.palette = 14;
      state->entryActive[2] = 1;

    mark_visible:
      UiWindow_MarkVisibleTileAttributes();
      COUNTDOWN->entryActive[0] = 0;
      COUNTDOWN->selected[i] = 0x80000000;
      /* FAKEMATCH: twelve dead stores restore the native frame's spill slots (i
       * << 1, i << 2 and i << 4 at sp+32 to sp+24, above &hdr at sp+20);
       * without them differ 50 in every edition, every row a spill offset. */
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = 0;
      hdr[0] = (u16)actorId;
      hdr[1] = 255;
      BattlePres_SetActorModesFar(hdr, 1);
      BattleLayout_HighlightPartyPanels(hdr);
      WaitFrames(1);
      mode = Ui_RunSelectionScreen(1);
      WaitFrames(1);
      if (mode == -2) {
        void *block;
        s32 handle;

        block = MemAlloc(12);
        handle = BattleParty_ListActorIdsFar(1, block);
        state->entryActive[2] = 0;
        WaitFrames(1);
        Ui_RunOwnerStatusScreen(block, handle, actorId);
        state->entryActive[2] = 1;
        MemFree(block);
        continue;
      }
      BattlePres_SetActorModesFar(hdr, 0);
      if (mode == -1) {
        if (i == 0) {
          break;
        }
        i--;
        continue;
      }

      if (COUNTDOWN->timer == 0) {
        mode = 3;
      }
      slot = &COUNTDOWN->entries[0];
      slot->oam.word.attr23 = 0;
      slot->oam.word.attr01 = 0x80002400;
      slot->oam.f.tile = Resource_LoadIndexedIntoBuffer(res1024, mode);
      if (mode == 15) {
        slot->oam.f.x = 128;
      } else {
        slot->oam.f.x = 96;
      }
      slot->oam.f.y = 136;
      state->entryActive[0] = 1;
      switch (mode) {
      case 0:
        win = UiWindow_Create(11, 17, 11, 3, 6);
#if defined(TBS_EDITION_FR)
        UiText_DrawCharacterAtOffset(MsgCommandName, win, 14, 0);
#else
        UiText_DrawCharacterAtOffset(MsgCommandName, win, 16, 0);
#endif
        state->entries[0].oam.f.x = 64;
        AudioCommand_PlayFar(112);
        mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
        UiWork_Finalize(win, 1);
        if (mode == -1) {
          goto mark_visible;
        }
        kind = 0;
        aux = mode;
        ent->range = 1;
        break;

      case 1: {
        struct MenuSprite *slot;
        struct BattleAct *ability;
        struct UiRenderWork *ui;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 cur;

        AudioCommand_PlayFar(112);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      psynergy_menu:
        list = state->list;
        pick = state->pick;
        state->entries[0].oam.f.x = 48;
        cnt = 0;
        k = 0;
        cur = actor->actionSlots[k].encodedAction & ACTION_ID_MASK;
        if (cur != 0) {
          do {
            if ((BattleActGet(cur)->flags & 0x80) != 0) {
              pick[cnt] = k;
              list[cnt] = cur;
              cnt++;
            }
            k++;
            if (k == 32) {
              break;
            }
            cur = actor->actionSlots[k].encodedAction & ACTION_ID_MASK;
          } while (cur != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = BattleMenu_RunActionSelection(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        param = actor->actionSlots[pick[mode]].encodedAction & ACTION_ID_MASK;
        ability = BattleActGet(param);
        cost = ability->range;
        slot = &state->entries[1];
        handle = VramCache_AllocEntry(128);
        ui = UI_WORK;
        win = UiWindow_Create(8, 17, 18, 3, 6);
        state->entries[0].oam.f.x = 40;
        slot->oam.word.attr01 = 0x40000400;
        slot->oam.word.attr23 = 0;
        slot->oam.f.x = win->x * 8 + 8;
        slot->oam.f.y = win->y * 8 + 4;
        /* FAKEMATCH: instruction order only (pool placement): arm_reorg counts
         * this empty asm as a 4-byte insn, so the pool goes in after mov r1,
         * r10 as native, whose constant 40 acts as a range-60
         * (zero_extendhisi2) pool load (differ 14). 2026-10-11: arm_reorg runs
         * before shorten_branches and counts the empty asm at its 4-byte
         * default length while it emits nothing; the halfword constant fix
         * needs that much more estimated length after it, and no C statement or
         * single pattern-length change (strh, str, orr, and tested over all 413
         * English objects) supplies it. */
        __asm__ volatile("");
        slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(param, handle);
        state->entryActive[1] = 1;
        ui->level = 5;
        if ((s32)ability->ppCost > (s32)actor->pp) {
          UiWork_SetParamNibble(2);
        } else if (actor->psySeal != 0) {
          UiWork_SetParamNibble(9);
        }
        UiText_DrawCharacterAtOffset(param + MsgAbilityName, win, 16, 0);
        UiText_DrawNumberAtOffset(ability->ppCost, 2, win, 104, 0);
        ui->level = 15;
        UiWork_SetParamNibble(15);
        UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
        UiWindow_SetTilemapEntry(win, 61445, 12, 0, 0);
#else
        UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
#endif
        if (ability->element != 4) {
          UiWindow_SetTilemapEntry(win, ability->element + 20481, 15, 0, 0);
        }
        ent->range = cost;
        AudioCommand_PlayFar(112);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(win, 1);
        if (mode == -1) {
          goto psynergy_menu;
        }
        kind = 1;
        aux = mode;
        break;
      }

      case 16: {
        const struct SummonDefinition *djinn;
        struct BattleAct *ability;
        struct MenuSprite *slot;
        s32 handle;
        s32 ok;
        s32 j;
        s32 n;

        AudioCommand_PlayFar(112);
        MenuCell_SetCursor(0, 0, 0);
      djinn_menu:
#if EDITION_INTERNATIONAL
        state->entries[0].oam.f.x = 80;
#else
        state->entries[0].oam.f.x = 104;
#endif
        BattleLayout_HighlightPartyPanels(hdr);
        mode = SummonMenu_SelectSummon(0, 0, state->cost[i]);
        BattleLayout_HighlightPartyPanels(hdr);
        if (mode == -1) {
          goto mark_visible;
        }
        kind = 6;
        param = mode;
        WaitFrames(1);
        slot = &state->entries[1];
        djinn = SummonDefinition_Get(mode);
        ability = BattleActGet(djinn->nameMessageId);
        handle = VramCache_AllocEntry(128);
#if EDITION_INTERNATIONAL
        win = UiWindow_Create(10, 17, 17, 3, 6);
#else
        win = UiWindow_Create(10, 17, 14, 3, 6);
#endif
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (djinn->djinnRequired[j] > state->cost[i][j]) {
            break;
          }
        }
        ok = (j == 4);
        state->entries[0].oam.f.x = 56;
        slot->oam.word.attr01 = 0x40000400;
        slot->oam.word.attr23 = 0;
        /* FAKEMATCH: instruction order only (pool placement, schedule): counted
         * as 4 bytes, the empty asm puts the pool after mov r2, r9 as native
         * (constant 56) and keeps ok = 1 - (j != 4) among the slot stores
         * (differ 12). 2026-10-11: arm_reorg runs before shorten_branches and
         * counts the empty asm at its 4-byte default length while it emits
         * nothing; the halfword constant fix needs that much more estimated
         * length after it, and no C statement or single pattern-length change
         * (strh, str, orr, and tested over all 413 English objects) supplies
         * it. */
        __asm__ volatile("");
        slot->oam.f.x = win->x * 8 + 8;
        slot->oam.f.y = win->y * 8 + 4;
        slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            djinn->nameMessageId & 0x3fff, handle);
        state->entryActive[1] = 1;
        if (ok == 0) {
          UiWork_SetParamNibble(2);
        }
        UiText_DrawCharacterAtOffset(SummonDefinition_Get(mode)->nameMessageId +
                                         MsgAbilityName,
                                     win, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (djinn->djinnRequired[j] != 0) {
#if EDITION_INTERNATIONAL
            UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 13, 0, 0);
            UiWindow_PutGlyph(win, djinn->djinnRequired[j] + 48, n * 2 + 14, 0,
                              0);
#else
            UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 10, 0, 0);
            UiWindow_PutGlyph(win, djinn->djinnRequired[j] + 48, n * 2 + 11, 0,
                              0);
#endif
            n++;
          }
        }
        if (ok != 0) {
          AudioCommand_PlayFar(112);
        } else {
          AudioCommand_PlayFar(114);
        }
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, ability->range,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        ent->range = ability->range;
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(win, 1);
        if (mode == -1) {
          goto djinn_menu;
        }
        for (j = 0; j <= 3; j++) {
          if (djinn->djinnRequired[j] > state->cost[i][j]) {
            state->cost[i][j] = 0;
            break;
          }
          state->cost[i][j] -= djinn->djinnRequired[j];
        }
        aux = mode;
        break;
      }

      case 15: {
        struct BattleAct *ability;
        struct UiWindow *hint;
        s32 cost;
        s32 element;
        s32 index;

        AudioCommand_PlayFar(112);
        MenuCell_SetCursor(0, 0, 0);
      summon_menu:
        state->entries[0].oam.f.x = 144;
        COUNTDOWN->selected[i] = 0x80000000;
        mode = Battle_SelectAbility(actorId, 1);
        ent->range = 0;
        if (mode == -1) {
          goto mark_visible;
        }
        kind = 5;
        param = mode;
        COUNTDOWN->selected[i] = mode;
        element = (param >> 8) & 15;
        index = param & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability = BattleActGet(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          win = UiWindow_Create(11, 17, 10, 3, 6);
          state->entries[0].oam.f.x = 64;
          UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
          UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName, win,
                                       16, 0);
          ent->range = cost;
          WaitFrames(1);
          AudioCommand_PlayFar(112);
          mode = BattleTarget_RunSelection(
              actorId, ability->targetMode, cost,
              Battle_ClassifyEntryKind((const struct BattleAct *)ability));
          if (state->hintCount != 0) {
            hint =
                UiText_ShowMessageAndWaitComplete(MsgDjinnUnleashHint, 15, 8);
            while (UiWork_IsComplete() == 0) {
              WaitFrames(1);
            }
            UiWork_Finalize(hint, 1);
            state->hintCount++;
            state->hintTimer = 45;
          }
          UiWork_Finalize(win, 1);
          if (mode == -1) {
            goto summon_menu;
          }
          aux = mode;
          break;
        }
        win = UiWindow_Create(11, 17, 10, 3, 6);
        state->entries[0].oam.f.x = 64;
        UiWork_SetParamNibble(2);
        UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
        UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName, win,
                                     16, 0);
        UiWork_SetParamNibble(15);
        ent->range = 1;
        WaitFrames(1);
        AudioCommand_PlayFar(112);
        mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
        UiWork_Finalize(win, 1);
        if (mode == -1) {
          goto summon_menu;
        }
        aux = mode;
        break;
      }

      case 2: {
        struct MenuSprite *slot;
        struct BattleAct *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 cur;
        s32 selection;

        AudioCommand_PlayFar(112);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      item_menu:
        state->entries[0].oam.f.x = 96;
        pick = state->pick;
        list = state->list;
        cnt = 0;
        k = 0;
        cur = actor->inventory[k];
        if (cur != 0) {
          do {
            Item_GetDirectFar(cur);
            if (Item_ClassifyUseAbility(actorId, cur) == 0) {
              list[cnt] = cur;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            cur = actor->inventory[k];
          } while (cur != 0);
        }
        k = 0;
        cur = actor->inventory[k];
        if (cur != 0) {
          do {
            Item_GetDirectFar(cur);
            if (Item_ClassifyUseAbility(actorId, cur) != 0) {
              list[cnt] = cur;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            cur = actor->inventory[k];
          } while (cur != 0);
        }
        list[cnt] = 0;
        mode = ItemList_SelectEntry(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        selection = pick[mode];
        param = selection;
        ability = BattleActGet(
            Item_GetDirectFar(actor->inventory[selection])->actionId);
        cost = ability->range;
        slot = &state->entries[1];
        handle = VramCache_AllocEntry(128);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        win = UiWindow_Create(9, 17, 16, 3, 6);
#else
        win = UiWindow_Create(9, 17, 15, 3, 6);
#endif
        state->entries[0].oam.f.x = 48;
        slot->oam.word.attr01 = 0x40000400;
        slot->oam.word.attr23 = 0;
        /* FAKEMATCH: instruction order only (pool placement): counted as 4
         * bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native
         * (constant 48) (differ 10). 2026-10-11: arm_reorg runs before
         * shorten_branches and counts the empty asm at its 4-byte default
         * length while it emits nothing; the halfword constant fix needs that
         * much more estimated length after it, and no C statement or single
         * pattern-length change (strh, str, orr, and tested over all 413
         * English objects) supplies it. */
        __asm__ volatile("");
        slot->oam.f.x = win->x * 8 + 8;
        slot->oam.f.y = win->y * 8 + 4;
        slot->oam.f.tile = Resource_LoadKind26EntryToBuffer(
            actor->inventory[selection], handle);
        state->entryActive[1] = 1;
        UiText_DrawCharacterAtOffset(
            (actor->inventory[selection] & 0x1ff) + MsgItemName, win, 24, 0);
        ent->range = cost;
        AudioCommand_PlayFar(112);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(win, 1);
        if (mode == -1) {
          goto item_menu;
        }
        kind = 2;
        aux = mode;
        break;
      }

      case 3:
        kind = 3;
        break;
      }

      AudioCommand_PlayFar(110);
      ent->unitId = (u16)actorId;
      order = actor->agi;
      if (order != 0) {
        order += (u32)(actor->agi * Random16()) >> 20;
      }
      ent->priority = order;
      if (i != 0 && in[i] == in[i - 1]) {
        ent->priority = (s16)order / 2;
      }
      if ((s16)ent->priority < 0) {
        ent->priority = 2000;
      }
      ent->command = kind;
      ent->parameter = param;
      ent->target = aux;
      i++;
      if (i < count) {
        continue;
      }
      break;
    }

    if (i < count) {
      continue;
    }
    break;
  }

  if (state->enabled != 0) {
    sync = (u16 *)gSerialTransfer.payload;
    sync[4] = 'E';
    sync[5] = 'D';
    sync[6] = 'V';
    sync[7] = 'S';
  }
  if (state->displayHandle != 0) {
    UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
  }
  Resource_ResetEntry(res512);
  Resource_ResetEntry(res1024);
  Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
  if (state->enabled != 0) {
    s32 DrawLocalizedResource(void) {
      u16 data[64];
      TEXT_LOAD(MsgWaitingForOpponent, data, TEXT_COUNT);
      UiText_RenderWideStringAtOffset(
          data, (struct UiWindow *)state->displayHandle, 0, 4);
    }

    session = gBattleWork;
    mode = 0;
    if (state->displayHandle == 0 && session->linkPaused == 0) {
      state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
      Ui_FillVramBlockPattern();
      DrawLocalizedResource();
    } else {
      state->displayHandle = 0;
    }
    for (;;) {
      peer = (u16 *)&gLinkPeerSignatures[session->linkSide ^ 1];
      if (session->linkPaused != 0) {
        count = -1;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        mode++;
        if (mode > 24) {
          count = -1;
          break;
        }
      } else {
        mode = 0;
        if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' &&
            peer[7] == 'S') {
          break;
        }
        if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' &&
              peer[7] == 'T')) {
          mode = 1;
        }
      }
      WaitFrames(1);
    }
    if (state->displayHandle != 0) {
      UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
    }
  }
  Camera_ConfigureSceneFar(0);
  Runtime_ReleaseHeapBlock(57);
  return count;
}

struct WindowLine {
  u16 left;
  u16 span;
};

struct WindowBuffer {
  u8 unk000[4];
  struct WindowLine lines[160];
};

void UiWork_CloseAndRelease(void);

void AffineEffect_UpdateFrame(void) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *sprite;
  struct AffineTransform effect;
  s32 scale;
  s32 matrix;
  s32 edge;

  s32 y;
  u32 end;
  u32 i;
  u16 *line;
  struct DisplayTransitionState *buffers;

  work = gMenuSelectWork;
  sprite = work->entries;
  scale = gUi_BounceScales[(gFrameCount * 2) & 31];
  scale = (scale - 256) / 4 + 304;
  effect.scaleX = scale;
  effect.scaleY = scale;
  effect.angle = 0;
  matrix = AffineMatrix_BuildForEffect(&effect);
  for (i = 0; i < work->count; i++, sprite++) {
    u32 *attr;
    s32 slot;
    s32 px;

    px = sprite->x;
    if (px == 0)
      continue;
    if (i == work->selection) {
      s32 left = px + scale * 7 / 512 - 20;

      if (sprite->y != 0)
        y = sprite->y + scale * 3 / 256 - 20;
      else
        y = (scale * 15 / 256 - 30) & 255;
      attr = &sprite->link;
      *attr++ = 0;
      *attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
      *attr = gVramBlockCache[sprite->slot].offset >> 5;
      slot = 246;
    } else {
      u32 base;
      u32 value;

      attr = &sprite->link;
      /* FAKEMATCH: needed to match: reading the y halfword through attr
       * keeps the attribute-pointer copy before its load; sprite->y
       * moves the copy two instructions later. */
      y = ((s16 *)attr)[7];
      *attr++ = 0;
      value = y | (px << 16) | 0x80002000;
      base = gVramBlockCache[sprite->slot].offset;
      *attr++ = value;
      *attr = base >> 5;
      slot = 245;
    }
    Runtime_PushSlotEntry(sprite, slot);
  }
  if (work->row == 0) {
    struct WindowBuffer *window;

    s32 span;

    buffers = (struct DisplayTransitionState *)gDisplayTransition;
    if (buffers == 0 || work->count == 0)
      return;
    window = (struct WindowBuffer *)buffers->pages[buffers->page];
    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    end = work->entries[work->selection].y + 24;
    end += (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[24].span;
    i = 24;
    if (i < end) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i < end);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[0].span;
    for (i = 0; i <= 23; i++) {
      *line = (*line & 0xff) | edge;
      line += 2;
    }
  } else {
    struct WindowBuffer *window;
    s32 start;

    s32 span;

    buffers = (struct DisplayTransitionState *)gDisplayTransition;
    if (buffers == 0 || work->count == 0)
      return;
    window = (struct WindowBuffer *)buffers->pages[buffers->page];
    edge = work->entries[work->selection].x;
    span = (scale * 12 - 2817) / 256;
    edge = ((edge - span) << 8) + (edge + span) + 23;
    start = work->entries[work->selection].y;
    start -= (scale * 32 - 7937) / 512 + 1;
    line = &window->lines[start].span;
    i = start;
    if (i <= 135) {
      do {
        *line = (*line & 0xff) | edge;
        i++;
        line += 2;
      } while (i <= 135);
    }
    edge = work->entries[0].x;
    if (work->selection == 0)
      edge -= (scale * 12 - 2817) / 256;
    edge <<= 8;
    line = &window->lines[136].span;
    for (i = 136; i <= 159; i++) {
      *line = (*line & 0xff) | edge;
      line += 2;
    }
  }
}

void *AffineEffect_InitializeWork(void) {
  void *work;
  work = Runtime_AllocateBlock(58, 152);
  Dma_Fill32(0, work, 152);
  Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
  return work;
}

void Menu_EndResourceSelection(void) {
  struct UiWindow *child;
  s32 i;
  struct ResourceMenuWork *work;

  work = gMenuSelectWork;
  Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
  child = work->window;
  if (child != 0) {
    UiWork_Finalize(child, 2);
  }
  i = 0;
  while (i < work->count) {
    Resource_ResetEntry(work->entries[i].slot);
    i += 1;
  }
  Runtime_ReleaseHeapBlock(0x3A);
  WaitFrames(1U);
}

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = gMenuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  RenderOutput_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + MsgCommandName;
  UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(112);
      return work->selection;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(113);
      return -1;
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(113);
      return -1;
    }
    if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
      AudioCommand_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
      AudioCommand_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 cur;
  s32 diff;
  s32 resourceId;
  s32 dist;
  s32 pos;
  s32 step;
  s32 delay;
  const u8 *table;
  struct ResourceMenuWork *state;

  state = gMenuSelectWork;
  step = 1;
  delay = 12;
  state->selection = start;
  if (goal < start)
    step = -1;
  pos = start;

  for (;;) {
    RenderOutput_PrepareForRedraw(state->window);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + MsgCommandName;
    }
    UiText_DrawCharacterAtOffset(resourceId, state->window, 0, 0);

    cur = state->selection;
    table = gMenu_SelectionStepDelays;
    diff = cur - goal;
    dist = AbsoluteDifference(diff, cur, goal);
    WaitFrames(table[dist] + delay);

    if (pos == goal)
      break;

    state->selection += step;
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    pos += step;
  }

  WaitFrames(48);
  AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
  return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = MemAlloc(size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
  VramCache_Load(slot, size, buffer);
  MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 flags;

  work = gMenuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, no);
    entry->x = index * 24 + 32;
    flags = 136;
    entry->y = flags;
    entry->slot = slot;
    work->resourceIds[index] = no;
  }
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *menu = gMenuSelectWork;
  s32 x;
  s32 i;
  s32 count;

  menu->width = width + 2;
  menu->resourceBase = resourceBase;
  menu->row = row;
  count = menu->count;
  x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
  for (i = 0; i < menu->count; i++) {
    struct ResourceMenuEntry *entry = &menu->entries[i];

    entry->x = x * 8;
    entry->y = row * 8;
    x += 3;
  }
  menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resourceBase) {
  struct ResourceMenuWork *work;
  s32 i;
  struct ResourceMenuEntry *entry;

  work = gMenuSelectWork;
  work->width = width + 2;
  work->resourceBase = resourceBase;
  work->row = y;
  for (i = 0; i < work->count; i++) {
    entry = &work->entries[i];
    entry->y = y << 3;
    entry->x = x << 3;
    x += 3;
  }
  work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

static __inline__ s32 TableGet(s8 *table, s32 index) { return table[index]; }

s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 triple;
  s32 ret;
  s8 *table;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  triple = group * 3;
  table = gMenu_TopEntryPositionByCommand;
  offset = triple << 1;
  selection = TableGet(table, selection + offset) - 1;
  if (selection < 0) {
    selection = 0;
  }

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  if (group == 0) {
    Menu_AppendResourceEntry(15);
  }
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (ret >= 0) {
    ret = gMenu_TopEntryCommandByPosition[ret + offset + 1];
  }

  return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1) {
  UiWindow_OpenMode1AndWaitFrame();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
  arg1 = Menu_SelectResource(arg0, arg1 - 1);
  Menu_EndResourceSelection();
  UiWork_CloseAndRelease();
  return arg1;
}

s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = SaveState_ScanRecordFlags();
  if (type < 0) {
    return -1;
  }
  if (type == 0) {
    return 0;
  }
  if (type == 3) {
    group = 1;
  } else if (type == 0x67) {
    group = 2;
  } else if (type > 0x64) {
    group = 3;
  } else {
    initial = 1;
  }
  AffineEffect_InitializeWork();
  if ((group == 0) || (group == 3)) {
    Menu_AppendResourceEntry(0x15);
  }
  if (group <= 1U) {
    Menu_AppendResourceEntry(0x16);
  }
  if ((group == 0) || (group == 3)) {
    Menu_AppendResourceEntry(0x17);
  }
  Menu_AppendResourceEntry(0x18);
  if ((*(s16 *)gTitleExtraOptionEnabled) != 0) {
    Menu_AppendResourceEntry(0x1D);
  }
  if (gTitleSendOptionEnabled != 0) {
    Menu_AppendResourceEntry(0x1E);
  }
  Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    ret = gMenu_SaveSlotActionByPosition[ret + (group * 6)];
  }
  return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;

  if (state->previousMode != state->selection) {
    state->previousMode = state->selection;
    UiWindow_ClearInteriorTiles(state->lowerWindow, 8, 40, MODE_LABEL_RIGHT,
                                80);

    switch (state->selection) {
    case 0: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X,
                                   48);
      UiText_DrawCharacterAtOffset(text + 2, state->lowerWindow, MODE_LABEL_X,
                                   56);
      UiText_DrawCharacterAtOffset(text + 3, state->lowerWindow, MODE_LABEL_X,
                                   64);
      text += 4;
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 72);
      break;
    }
    case 1: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 40);
      UiText_DrawCharacterAtOffset(text + 1, state->lowerWindow, MODE_LABEL_X,
                                   48);
      text += 2;
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 56);
      break;
    }
    case 2:
    default: {
      s32 text = MsgPasswordLevel;

      UiText_DrawCharacterAtOffset(text++, state->lowerWindow, MODE_LABEL_X,
                                   40);
      UiText_DrawCharacterAtOffset(text, state->lowerWindow, MODE_LABEL_X, 48);
      break;
    }
    }
  }
}

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;

  if (state->previousMode != state->selection) {
    if (state->selection == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource(MsgPasswordTransferHelp, state->lowerWindow, 16, 4);
      UiText_DrawResource(MsgPasswordTransferHelp + 1, state->lowerWindow, 16,
                          16);
    } else if (state->previousMode == 0) {
      RenderOutput_ClearList(state->lowerWindow);
      UiText_DrawResource(MsgCableTransferHelp, state->lowerWindow, 0, 4);
      UiText_DrawResource(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
      UiText_DrawResource(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
    }
    state->previousMode = state->selection;
  }
}
#else
void Menu_DrawModeIndicator(void) {
  struct ResourceMenuWork *state = gMenuSelectWork;
  s16 *shown = &state->previousMode;
  s16 *current = &state->selection;

  if (*shown != *current) {
    *shown = *current;
    RenderOutput_PrepareForRedraw(state->lowerWindow);
    if (*current == 0) {
#if defined(TBS_EDITION_DE)
      UiText_DrawCharacterAtOffset(MsgPasswordTransferHelp, state->lowerWindow,
                                   32, 8);
      UiText_DrawCharacterAtOffset(MsgPasswordTransferHelp + 1,
                                   state->lowerWindow, 32, 24);
#else
      UiText_DrawResource(MsgPasswordTransferHelp, state->lowerWindow,
                          PASSWORD_HELP_X, 4);
      UiText_DrawResource(MsgPasswordTransferHelp + 1, state->lowerWindow,
                          PASSWORD_HELP_X, 16);
#endif
    } else {
#if defined(TBS_EDITION_DE)
      UiText_DrawCharacterAtOffset(MsgCableTransferHelp, state->lowerWindow, 0,
                                   0);
      UiText_DrawCharacterAtOffset(MsgCableTransferHelp + 1, state->lowerWindow,
                                   0, 16);
      UiText_DrawCharacterAtOffset(MsgCableTransferHelp + 2, state->lowerWindow,
                                   0, 32);
#else
      UiText_DrawResource(MsgCableTransferHelp, state->lowerWindow, 0, 4);
      UiText_DrawResource(MsgCableTransferHelp + 1, state->lowerWindow, 0, 16);
      UiText_DrawResource(MsgCableTransferHelp + 2, state->lowerWindow, 0, 28);
#endif
    }
  }
}
#endif

s32 Menu_SelectResourceLayout(s32 mode) {
  struct ResourceMenuWork *work;
  struct UiWindow *window;
  s32 msg;
  struct UiWindow **upper;
  s32 result;

  AffineEffect_InitializeWork();
  work = gMenuSelectWork;
  if (mode == 0) {
    Menu_AppendResourceEntry(44);
    Menu_AppendResourceEntry(45);
  } else {
    Menu_AppendResourceEntry(46);
    Menu_AppendResourceEntry(47);
    Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
    Menu_CenterResourceEntries(17, 5, 0);
#endif
  }
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_IT)
  Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
#elif defined(TBS_EDITION_FR)
  Menu_CenterResourceEntries(17, 9, 0);
#else
  Menu_CenterResourceEntries(17, 5, 0);
#endif
  if (mode != 0) {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
    work->previousMode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_IT)
    window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
    window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
    window = UiWindow_Create(5, 0, 22, 4, 2);
#else
    window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
    msg = MsgPasswordSelection;
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(msg, window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
    window = UiWindow_Create(1, 4, 28, 12, 2);
#else
    window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
    work->lowerWindow = window;
    UiText_DrawResource(msg + 1, window, 8, 0);
    UiText_DrawResource(msg + 2, work->lowerWindow, 8, 11);
    msg += 3;
    UiText_DrawResource(msg, work->lowerWindow, 8, 22);
#else
    window = UiWindow_Create(5, 4, 21, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(msg + 1, window, 0, 4);
    msg += 2;
    UiText_DrawResource(msg, work->lowerWindow, 0, 16);
#endif
  } else {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
    work->previousMode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
    window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
    window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    window = UiWindow_Create(5, 0, 20, 4, 2);
#else
    window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
    upper = &work->upperWindow;
    *upper = window;
#if EDITION_INTERNATIONAL
    UiText_DrawResource(MsgTransferMethod, window, 2, 4);
#else
    UiText_DrawResource(MsgTransferMethod, window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    work->lowerWindow = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
    work->lowerWindow = UiWindow_Create(0, 5, 30, 7, 2);
#else
    work->lowerWindow = UiWindow_Create(5, 5, 21, 7, 2);
#endif
  }
  result = Menu_RunResourceSelectionLoop(0);
  if (mode != 0)
    Scheduler_RemoveCallback(Menu_DrawModeLabel);
  else
    Scheduler_RemoveCallback(Menu_DrawModeIndicator);
  RenderOutput_PrepareForRedraw(*upper);
  RenderOutput_PrepareForRedraw(work->lowerWindow);
  UiWork_Finalize(*upper, 2);
  UiWork_Finalize(work->lowerWindow, 2);
  WaitFrames(1);
  Menu_EndResourceSelection();
  return result;
}

//--------------------------------------------------------------------------
// confirm_selection.c
// Menu: arrange command icons for selection, such as Yes/No
//--------------------------------------------------------------------------
// Icon numbers refer to ResourceId_CommandIcons.
// Show the selected command's name in a window.
//--------------------------------------------------------------------------
// Select icons 0x11-0x14 centered on row 17.
// selection: initial position; return the selected position.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
  Menu_CenterResourceEntries(17, 8, 0); // Name window width
#elif EDITION_INTERNATIONAL
  Menu_CenterResourceEntries(17, 7, 0);
#else
  Menu_CenterResourceEntries(17, 6, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

//--------------------------------------------------------------------------
// Select icons 0x19-0x1c centered on row 17.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
  Menu_CenterResourceEntries(17, 10, 0);
#else
  Menu_CenterResourceEntries(17, 8, 0);
#endif
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

//--------------------------------------------------------------------------
// Select Yes/No (icons 5/6); cancel returns 1 (No).
// low: 0 places the menu on row 0; otherwise row 17.
// message: first name message, or 0 for command names.
// w: name window width, or 0 for width 3.
// selection: initial position
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 message, s32 w, s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (w == 0) {
    w = 3;
  }
  if (low != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, w, message);

  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

//--------------------------------------------------------------------------
// Select Yes/No at (x, y); cancel returns 1 (No).
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 selection) {
  s32 message = MsgCommandYes;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_LayoutResourceEntries(x, y, 3, message);
  selection = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  if (selection == -1) {
    selection = 1;
  }
  return selection;
}

//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// Menu: select between command icons 0x20 and 0x21
//--------------------------------------------------------------------------
// Name window width for each edition
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) ||                    \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// Select icons 0x20 and 0x21 centered on row 17.
// selection: initial position; return the selected position.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 selection) {
  s32 ret;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return ret;
}

void Scheduler_ScheduleCallbackCAfterFrames(void) {
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Menu_RunSelection, 0xC80);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
#endif
}

void Menu_DrawSelectionRow(struct UiWindow *work, s32 first,
                           const s16 *second) {
  s16 selected = first;
  s32 label = Scene_GetAreaNameIndexFar(selected, *second) + MsgDebugEntryName;
  RenderOutput_PrepareForRedraw(work);
  UiText_DrawNumber(selected, 3, work, 0, 14);
  UiText_DrawNumber(*second, 3, work, MENU_LABEL_X, 14);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 0);
  UiText_DrawResource(selected + MsgDebugCategoryName, work, 0, 0);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 14);
  UiText_DrawResource(label, work, MENU_LABEL_X, 0);
}

// The scheduler and the debug menu's far call ignore this selection result.
// Plain void removes two result moves (252 to 248 bytes in tbs-en) and
// reorders Event_SetDestinationFar's argument setup.
#ifdef UBFIX
void Menu_RunSelection(void) {
#else
s16 Menu_RunSelection(void) {
#endif
  s32 resource;
  s16 mode;
  s16 secondary;
  struct MenuSprite object;
  s16 primary;
  struct UiWindow *work;
  s16 result;

  work = 0;
  mode = 0;
  primary = gGameState.defaultScene;
  secondary = gGameState.defaultEntrance;
  work = UiWindow_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(work, primary, &secondary);
  UiTextResource_Initialize(&object, &resource);

  while (gKeysHeld != 0)
    WaitFrames(1);

  for (;;) {
    result = Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
    if (result == -1) {
      UiTextResource_Release(resource);
      UiWork_Finalize(work, 2);
      Event_SetDestinationFar(primary, secondary);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }
    if (result == -2) {
      UiTextResource_Release(resource);
      UiWork_Finalize(work, 2);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }

    UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
    primary = result;
    WaitFrames(1);
  }
}

s32 Menu_HandleSelectionRowInput(struct UiWindow *arg, s16 value, s16 *sub,
                                 s16 *mode) {
  if (gKeysRepeat & KEY_A)
    return -1;
  if (gKeysRepeat & KEY_B)
    return -2;
  if (gKeysRepeat & KEY_DOWN) {
    *mode ^= 1;
  } else if (gKeysRepeat & KEY_UP) {
    *mode ^= 1;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (*mode == 0) {
      value++;
    } else {
      (*sub)++;
      if (*sub > 99)
        *sub = 0;
    }
    if (value > 200)
      value = 0;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gKeysRepeat & KEY_LEFT) {
    if (*mode == 0) {
      value--;
    } else {
      (*sub)--;
      if (*sub < 0)
        *sub = 99;
    }
    if (value < 0)
      value = 200;
    Menu_DrawSelectionRow(arg, value, sub);
  } else if (gKeysRepeat & KEY_R) {
    if (*mode == 0) {
      *sub = 0;
      value += 10;
    } else {
      *sub += 10;
      if (*sub > 99)
        *sub -= 99;
    }
    if (value > 200)
      value = 0;
    Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
  } else if (gKeysRepeat & KEY_L) {
    if (*mode == 0) {
      *sub = 0;
      value -= 10;
    } else {
      *sub -= 10;
      if (*sub < 0)
        *sub += 99;
    }
    if (value < 0)
      value = 200;
    Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
  }
  return value;
}

//--------------------------------------------------------------------------
// run_selection_with_cursor_object.c
// Menu: flag grid, toggled with A at the cursor
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Show a 16x16 flag grid per page and process input until closed.
// Start on page 8; always return 0.
//--------------------------------------------------------------------------
s32 Menu_RunSelectionWithCursorObject(void) {
  struct MenuSprite obj; // Cursor object
  s32 cur[2];            // Cursor column and row
  s32 res;
  s32 page;
  struct UiWindow *win;
  s32 ret;

  page = 8;
  win = 0;
  cur[0] = 0;
  cur[1] = 0;
  win = UiWindow_Create(1, 0, 28, 20, 2);
  Menu_DrawFlagBitTable(win, page);
  UiTextResource_Initialize(&obj, &res);
  for (;;) {
    WaitFrames(1);
    ret = Menu_HandleFlagGridInput(win, &page, cur);
    if (ret == -1)
      break; // Close
    if (ret == 1)
      Menu_DrawFlagBitTable(win, page); // Flag or page changed
    UiTextResource_SetPosition(&obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
  }
  UiTextResource_Release(res);
  UiWork_Finalize(win, 2);
  return 0;
}

//--------------------------------------------------------------------------
// format_hex.c
// Text: convert a number to a hexadecimal string
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Write the lowest count digits of value, up to five, most significant first
// as uppercase hexadecimal in out; do not append a null terminator.
//--------------------------------------------------------------------------
void Text_FormatHex(u32 value, u32 count, u8 *out) {
  u8 buf[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 d = value & 15;
    if (d <= 9)
      buf[i] = d + '0';
    else
      buf[i] = d + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = buf[i];
}

void Menu_DrawFlagBitTable(struct UiWindow *window, s32 startFlag) {
  s32 row;
  s32 y;
  s32 flag;
  char label[5];
  char bits[17];

  RenderOutput_PrepareForRedraw(window);
  UiText_DrawStringInWindow((u8 *)gMenu_HexDigitsString, window, 48, 0);

  flag = startFlag << 8;
  for (row = 0; row != 16; row++) {
    s32 i;

    y = row * 8 + 16;

    for (i = 0; i != 5; i++) {
      label[i] = 0;
    }
    Text_FormatHex(flag, 3, label);
    UiText_DrawStringInWindow((u8 *)label, window, 0, y);
    UiText_DrawStringInWindow((u8 *)gMenu_ColonString, window, 32, y);

    for (i = 0; i < 16; i++) {
      s32 val = GameFlag_TestFar(flag);
      bits[i] = (val != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
  }
}

s32 Menu_HandleFlagGridInput(struct UiWindow *window, s32 *page, s32 *cursor) {
  s32 *row = &cursor[1];

  if (gKeysRepeat & KEY_A) {
    s32 flag = ((*page << 4) + *row << 4) + cursor[0];

    if (GameFlag_TestFar(flag))
      GameFlag_ClearBitFar(flag);
    else
      GameFlag_SetBitFar(flag);
    return 1;
  }
  if ((gKeysPressed & KEY_B) || (gKeysRepeat & KEY_SELECT))
    return -1;
  if (gKeysRepeat & KEY_UP) {
    if (--*row < 0)
      *row = 15;
  } else if (gKeysRepeat & KEY_DOWN) {
    if (++*row > 15)
      *row = 0;
  } else if (gKeysRepeat & KEY_LEFT) {
    if (--cursor[0] < 0)
      cursor[0] = 15;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (++cursor[0] > 15)
      cursor[0] = 0;
  } else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START)) {
    *page -= 10;
    if (*page < 0)
      *page = 15;
    return 1;
  } else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START)) {
    *page += 10;
    if (*page > 15)
      *page = 0;
    return 1;
  } else if (gKeysRepeat & KEY_L) {
    if (--*page < 0)
      *page = 15;
    return 1;
  } else if (gKeysRepeat & KEY_R) {
    if (++*page > 15)
      *page = 0;
    return 1;
  }
  return 0;
}

void Menu_ReservedNoOp(void) {}

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index) {
  GameFlag_SetBitFar((element * 0x14) + index + 0x30);
  Djinn_AddToOwnerFar(owner, element, index);
}

s32 Menu_RunWorkspaceResultLoop(void) {
  s32 result;

  result = Menu_RunWorkspaceSelectionLoop();
  if (result == -1) {
    return -1;
  }
  if (result == 0) {
    if (Save_WriteSelectedSlot() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  } else if (result == 1) {
    UiText_ShowPositionedMessageAndWait(MsgSleepMode, 1);
    gSleepRequested = result;
  } else if (result == 2) {
    if (Menu_RunWorkspaceOptions() == -1) {
      return Menu_RunWorkspaceResultLoop();
    }
  }
  return 0;
}

s32 DebugMenu_BrowseIcons(void) {
  s32 redraw;
  s32 tile;
  s32 page;
  s32 mode;
  struct UiWindow *window;
  s32 slot;
  s32 base;
  s32 i;
  s32 row;
  s32 x;
  s32 y;

  window = NULL;
  page = 0;
  mode = 0;
  redraw = 1;
  gMenuCtrlWork->suspended = redraw;
  WaitFrames(1);

  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      page--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      page++;
    }
    if (gKeysRepeat & KEY_L) {
      mode--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      mode++;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & KEY_B)
      break;
    if (redraw != 0) {
      redraw = 0;
      page = (page + 8) % 8;
      mode = (mode + 3) % 3;
      UiWork_Finalize(window, 2);
      window = UiWindow_Create(10, 0, 18, 12, 2);
      if (mode == 0)
        UiText_DrawStringInWindow(gWorkspace_ItemTitle, window, 0, 0);
      else if (mode == 1)
        UiText_DrawStringInWindow(gWorkspace_EnergyTitle, window, 0, 0);
      else
        UiText_DrawStringInWindow(gWorkspace_StatusTitle, window, 0, 0);
      UiText_DrawStringInWindow(gWorkspace_PageLabel, window, 0, 8);
      UiText_DrawNumberInWindow(page, 0, window, 40, 8);
      base = page << 5;
      UiText_DrawNumberInWindow(base, 3, window, 64, 8);
      UiText_DrawStringInWindow(gWorkspace_RangeDash, window, 88, 8);
      UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
      for (i = 0; i <= 31; i++) {
        slot = -1;
        row = i / 8;
        x = (i - row * 8) * 16;
        y = row * 16 + 16;
        if (mode == 0) {
          UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        } else if (mode == 1) {
          UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Ui_BuildPatternToSlot(i, 0, slot);
          RenderOutput_Create(slot, 0x40000000, window, x, y);
        }
      }
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  gMenuCtrlWork->suspended = 0;
  return 0;
}

#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

s32 DebugMenu_BrowseEntryGlyphs(void) {
  struct UiRenderWork *work;
  struct UiWindow *portrait;
  s32 redraw;
  struct UiWindow *window;
  s32 count;
  s32 total;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancel;
  u32 slotOffset;

  work = UI_WORK;
  redraw = 1;
  portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
  window = UiWindow_Create(10, 10, 14, 3, 2);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  count = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  total = count + i;

  /* FAKEMATCH: the count-derived mask is needed to match the native mask
   * in r9 and work in fp; a direct constant swaps them (four instructions).
   * 2026-10-13: cancel = KEY_B differs 12 of 179, KEY_B tested directly
   * 20, cancel = KEY_B set first in the function 48. */
  cancel = ((u32)(u16)count >> 16) | KEY_B;
  /* FAKEMATCH: defining the offset before the loop keeps its load in r1;
   * defining it inside redraw uses r3 (4 instructions differ in all six
   * editions). */
  slotOffset = RENDER_SLOT_VALUE_OFFSET + 2;
  while (1) {
    if (gKeysRepeat & KEY_LEFT) {
      redraw = 1;
      index--;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      redraw = 1;
      index++;
    }
    if (gKeysRepeat & KEY_L) {
      redraw = 1;
      index -= 10;
    }
    if (gKeysRepeat & KEY_R) {
      redraw = 1;
      index += 10;
    }
    if (gKeysRepeat & KEY_A)
      break;
    if (gKeysRepeat & cancel)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + total) % total;
      RenderOutput_PrepareForRedraw(window);
      if (index < count)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - count) * 2 + 1] + 128;
      {
        /* FAKEMATCH: the word temporary is needed to match palette's
         * r2 through the slot store; direct assignment chooses r3.
         * 2026-10-13: slot assigned directly with 15 passed as a
         * literal differs 4 of 179 (the 15 takes r3 after the slot
         * store); so do the loaded temporary alone, the load
         * written as an index and the assignment inside the
         * argument list; pal alone, without loaded, differs 69. */
        s32 pal;
        s32 loaded = *(u16 *)((u8 *)work + slotOffset);
        pal = 15;
        slot = loaded;
        UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
      }
      UiText_DrawNumberInWindow(index, 2, window, 0, 0);
      UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  UiWork_Finalize(portrait, 2);
  WaitFrames(1);
  return 0;
}
