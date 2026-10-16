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
#include "flags.h"
#include "static_assert.h"
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

// Edition-specific text and window layout values.
#if !TBS_PATCH_1
#define DJINN_PREVIEW_VITALS_X 4
#else
#define DJINN_PREVIEW_VITALS_X 5
#endif

#if !TBS_PATCH_1
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 7
#define DJINN_PREVIEW_STAT_X 4
#else
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 8
#define DJINN_PREVIEW_STAT_X 6
#endif

#if !TBS_PATCH_1
#define DJINN_PREVIEW_HELP_X 40
#else
#define DJINN_PREVIEW_HELP_X 24
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define PSYNERGY_COST_SECOND_TILE 0xf005
#else
#define PSYNERGY_COST_SECOND_TILE 0xf01e
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)
#define OWNER_STATUS_LEVEL_X 51
#else
#define OWNER_STATUS_LEVEL_X 56
#endif

#if TBS_PATCH_1
#define SUMMON_MENU_WINDOW_X 13
#define SUMMON_MENU_WINDOW_WIDTH 17
#else
#define SUMMON_MENU_WINDOW_X 16
#define SUMMON_MENU_WINDOW_WIDTH 14
#endif

#if TBS_PATCH_1
#define BATTLE_CLASS_ICON_X 0
#else
#define BATTLE_CLASS_ICON_X 4
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)
#define BATTLE_COMMAND_NAME_X 14
#else
#define BATTLE_COMMAND_NAME_X 16
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_ICON_X 80
#else
#define BATTLE_SUMMON_ICON_X 104
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_WINDOW_WIDTH 17
#else
#define BATTLE_SUMMON_WINDOW_WIDTH 14
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_ELEMENT_COLUMN 13
#define BATTLE_SUMMON_COST_COLUMN 14
#else
#define BATTLE_SUMMON_ELEMENT_COLUMN 10
#define BATTLE_SUMMON_COST_COLUMN 11
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define BATTLE_ITEM_WINDOW_WIDTH 16
#else
#define BATTLE_ITEM_WINDOW_WIDTH 15
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define RESOURCE_MODE0_WINDOW_X 7
#define RESOURCE_MODE0_WINDOW_WIDTH 17
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define RESOURCE_MODE0_WINDOW_X 5
#define RESOURCE_MODE0_WINDOW_WIDTH 21
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define RESOURCE_MODE0_WINDOW_X 5
#define RESOURCE_MODE0_WINDOW_WIDTH 22
#else
#define RESOURCE_MODE0_WINDOW_X 9
#define RESOURCE_MODE0_WINDOW_WIDTH 13
#endif

#if (LANGUAGE == LANGUAGE_GERMAN)
#define RESOURCE_MODE1_WINDOW_X 1
#define RESOURCE_MODE1_WINDOW_WIDTH 28
#else
#define RESOURCE_MODE1_WINDOW_X 3
#define RESOURCE_MODE1_WINDOW_WIDTH 25
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN)
#define TRANSFER_METHOD_WINDOW_X 6
#define TRANSFER_METHOD_WINDOW_WIDTH 18
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define TRANSFER_METHOD_WINDOW_X 4
#define TRANSFER_METHOD_WINDOW_WIDTH 21
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TRANSFER_METHOD_WINDOW_X 5
#define TRANSFER_METHOD_WINDOW_WIDTH 20
#else
#define TRANSFER_METHOD_WINDOW_X 9
#define TRANSFER_METHOD_WINDOW_WIDTH 13
#endif

#if TBS_PATCH_1
#define TRANSFER_METHOD_LABEL_X 2
#else
#define TRANSFER_METHOD_LABEL_X 0
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_SPANISH) ||        \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TRANSFER_HELP_WINDOW_X 1
#define TRANSFER_HELP_WINDOW_WIDTH 28
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define TRANSFER_HELP_WINDOW_X 0
#define TRANSFER_HELP_WINDOW_WIDTH 30
#else
#define TRANSFER_HELP_WINDOW_X 5
#define TRANSFER_HELP_WINDOW_WIDTH 21
#endif

#if (LANGUAGE == LANGUAGE_GERMAN)
#define ENTRY11_COMMAND_LABEL_WIDTH 8
#elif TBS_PATCH_1
#define ENTRY11_COMMAND_LABEL_WIDTH 7
#else
#define ENTRY11_COMMAND_LABEL_WIDTH 6
#endif

#if TBS_PATCH_1
#define ENTRY19_COMMAND_LABEL_WIDTH 10
#else
#define ENTRY19_COMMAND_LABEL_WIDTH 8
#endif

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
// is tried first). Each slot has its own palette, 14 or 15. In the prologue,
// characters 0 and 1 show portraits 18 and 19 instead. `unused` is
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

  if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
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

  if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
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
  // UB: slot[-1] steps before the single party-status work slot. Reading
  // the actual owners grows the EN function from 0x68 to 0x6c bytes.
  // Keep the original address formation only in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = WORK_SLOT(PARTY_STATUS_BLOCK);
  struct UiRenderWork *work = UI_WORK;
#else
  void **slot = (void **)&gPartyStatusWork;
  struct PartyStatusWindow *layout = *slot;
  struct UiRenderWork *work = slot[-1];
#endif
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
#ifdef UBFIX
      // At zero (including a multiple-of-eight remainder), the native
      // right shifts by 32 yield zero. Keep the empty patterns without
      // evaluating those undefined C shifts.
    } else if (length > 0) {
#else
    } else if (length >= 0) {
#endif
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
  // UB: the negative indices leave the single party-status work slot.
  // Named owners grow the EN function from 0x3d4 to 0x3dc bytes. Keep the
  // original address formation and load order in the matching build.
#ifdef UBFIX
  struct PartyStatusWindow *layout = WORK_SLOT(PARTY_STATUS_BLOCK);
  struct BattleWork *battle = gBattleWork;
  struct UiWindow *window = layout->window;
  struct UiRenderWork *work = UI_WORK;
#else
  void **slot = (void **)gPartyStatusWork;
  struct PartyStatusWindow *layout = slot[0];
  struct BattleWork *battle = slot[-7];
  struct UiWindow *window = layout->window;
  struct UiRenderWork *work = slot[-1];
#endif
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
  u8 *digitsCursor;
  u8 *textCursor;

  totalMinutes = (u32)frames / (60 * 60);
  if (totalMinutes > 999 * 60 + 59)
    totalMinutes = 999 * 60 + 59;

  hours = totalMinutes / 60;
  minutes = totalMinutes % 60;

  digitsCursor = UiText_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  textCursor = out + 1;
  *textCursor = *digitsCursor;
  textCursor++;
  *textCursor = digitsCursor[1];
  // Adding 100 gives the minutes a leading zero in their last two digits.
  minutes += 100;
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, minutes, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

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
  summary->hasFlag20 = GameFlag_TestFar(FLAG_CHILDHOOD) != 0;
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

#if (LANGUAGE != LANGUAGE_JAPANESE) ||                                         \
    (GAME_TLA && (LANGUAGE == LANGUAGE_JAPANESE))

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
#ifdef UBFIX
    // Wrap the frame phase before Trig_Sin takes its low sixteen bits.
    sine = Trig_Sin((((u32)*tick + i * 8U) * 0x300U) & 0xffffU);
#else
    sine = Trig_Sin((*tick + i * 8) * 0x300);
#endif
    colour = sine / 16384;
    {
#ifdef UBFIX
      // A negative sine must be shifted as unsigned bits.
      u32 red = (u32)colour << 1;
#else
      u32 red = colour << 1;
#endif
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

#if TBS_PATCH_1

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
#if (LANGUAGE == LANGUAGE_SPANISH)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL UiText_DrawStringAtOffset
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#define SUMMARY_DRAW_LEVEL UiText_DrawStringAtOffset
#else
#define SUMMARY_LEVEL_X 72
#define SUMMARY_COINS_X 48
#define SUMMARY_DRAW_LEVEL UiText_DrawStringInWindow
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
    SUMMARY_DRAW_LEVEL(gStatusMenu_LevelLetterString, window, SUMMARY_LEVEL_X,
                       0);
    UiText_DrawNumberAtOffset(summary->level, 2, window, 80, y);
    UiText_DrawCharacterAtOffset(summary->classId + MsgClassName, window, 0,
                                 16);
#if !TBS_PATCH_1
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
#if (LANGUAGE == LANGUAGE_FRENCH)
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
#if TBS_PATCH_1 || LANGUAGE == LANGUAGE_JAPANESE

s32 SaveMenu_SelectSlot(s32 selectedFile, s32 menuMode) {
  struct UiWindow *fileList;
#if TBS_PATCH_1
  struct SaveWorkspace *save;
  struct UiWindow *summaryWindow;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modePicture;
  s32 summaryDirty;
  struct UiRenderWork *render;
  u32 buildStamp;
  struct ShopRuntime *spriteWork;
  s32 result;
  s32 i;
  s32 y;
#else
  struct UiWindow *summaryWindow;
  struct SaveWorkspace *save;
  struct UiWindow *partyWindow;
  struct UiWindow *djinnWindow;
  struct SaveSummary *summary;
  struct RenderOutput *modePicture;
  s32 cursorStyle;
  s32 summaryDirty;
  struct UiRenderWork *render;
  u32 buildStamp;
  struct ShopRuntime *spriteWork;
  struct ShopCursor fileCursor;
  s32 result;
  s32 i;
  s32 x;
  s32 y;
  s32 vramSlot;
  struct RenderOutput *cursorSprite;
#endif

  spriteWork = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, sizeof(*spriteWork));
  save = *(gWorkSlot + HEAP_SLOT_SAVE);
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  render = *(gWorkSlot + HEAP_SLOT_WINDOW);
  buildStamp = Runtime_GetBuildStampTimeFar();
  // Start on the first file the mode allows.
  if (selectedFile < 0)
    selectedFile = 0;
#if TBS_PATCH_1
  if (menuMode == SAVE_MENU_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                (save->summary[selectedFile].level == 0 ||
                 (s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == SAVE_MENU_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 (s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
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
    } else if (menuMode == SAVE_MENU_CLEARED && (s8)summary->sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, y);
    } else {
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(summary->area + MsgDebugEntryName, fileList, 62, y);
      render->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
#else
  if (menuMode != SAVE_MENU_SAVE) {
    for (i = 0; i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_CONTINUE) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_EXTRA) {
    for (i = 0; i < SAVE_FILE_COUNT &&
                ((s8)save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }
  if (menuMode == SAVE_MENU_CLEARED) {
    for (i = 0;
         i < SAVE_FILE_COUNT && ((s8)save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  // The file list, and the mode's sprite above it.
  Dma_Fill32(0, spriteWork, sizeof(*spriteWork));
  fileList = UiWindow_Create(1, 2, 28, 7, 2);

  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      UiText_DrawResource(MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) { // an older build
      UiText_DrawResource(MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38) {
      UiText_DrawResource(MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (menuMode == SAVE_MENU_CLEARED &&
               (s8)save->summary[i].sendFlag == 0) {
      UiText_DrawResource(MsgSaveNotCleared, fileList, 10, i * 16);
    } else {
      y = i * 16;
      UiText_DrawString(summary->name, fileList, SAVE_SLOT_NAME_X, y);
      UiText_DrawResource(save->summary[i].area + MsgDebugEntryName, fileList,
                          72, y);
      render->dirty = 1;
    }
  }
  UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
  UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    x = -12;
    y = selectedFile * 16 + 4;
    cursorSprite =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, fileList, x, y);
    cursorSprite->active = 1;
    fileCursor.anchor = cursorSprite;
  }
  cursorStyle = 1;
#endif
  modePicture = RenderResource_CreatePair(menuMode, fileList, 72, -24);

  while (1) {
    // Show the file under the cursor after it moves.
    if (summaryDirty != 0) {
      summaryDirty = 0;
      if (save->summary[selectedFile].level != 0) {
        PaletteGlow_Update(save->summary[selectedFile].paletteGlow,
                           save->summary[selectedFile].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = UiWindow_Create(1, 10, 14, 9, 2);
        SaveMenu_DrawFileSummary(summaryWindow, &save->summary[selectedFile]);
        WaitFrames(1);
        if (partyWindow == NULL)
          partyWindow = UiWindow_Create(16, 10, 13, 3, 2);
        SaveMenu_HidePartySprites();
        SaveMenu_ShowPartySprites(partyWindow, 0, 0,
                                  &save->summary[selectedFile]);
        WaitFrames(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = UiWindow_Create(16, 14, 13, 5, 2);
          SaveMenu_DrawDjinnCounts(djinnWindow, &save->summary[selectedFile]);
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
#if TBS_PATCH_1
      RenderOutput_RedrawSavedRect(fileList);
      UiWindow_DrawDividerLine(fileList, 0, 2, 27, 2);
      UiWindow_DrawDividerLine(fileList, 0, 4, 27, 4);
      UiWindow_FillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
    }
#else
      x = fileList->x * 8 - 4;
      y = (fileList->y << 3) + selectedFile * 16 + 12;
      Shop_SetCursorFar(&fileCursor, x, y, cursorStyle);
      cursorStyle = 3;
    }
    ShopCursor_AdvanceFar(&fileCursor);
#endif
    Ui_ApplyTableOffsetToPair(modePicture);
    WaitFrames(1);
    // Up and down move to the previous or next file the mode allows.
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile =
            (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT; // previous
        if (menuMode == SAVE_MENU_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == SAVE_MENU_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == SAVE_MENU_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == SAVE_MENU_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      summaryDirty = 1;
      do {
        selectedFile =
            (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT; // next
        if (menuMode == SAVE_MENU_SAVE)
          break;
      } while ((save->summary[selectedFile].level == 0 ||
                (menuMode == SAVE_MENU_CONTINUE &&
                 (s8)save->summary[selectedFile].sendFlag != 0) ||
                (menuMode == SAVE_MENU_EXTRA &&
                 (s8)save->summary[selectedFile].flagCount == 0) ||
                (menuMode == SAVE_MENU_CLEARED &&
                 (s8)save->summary[selectedFile].sendFlag == 0)));
    } else if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    } else if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = selectedFile;
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
#if TBS_PATCH_1
  PaletteGlow_Stop();
#else
#endif
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

#if (LANGUAGE != LANGUAGE_JAPANESE)
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
#define NAME_MAX_BYTES 14 // stored name bytes, excluding its terminator

#if TBS_PATCH_1
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
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

// Whether the edition draws the Delete and End key labels as text over the
// keyboard, and how wide its End key is, in tiles.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if (LANGUAGE == LANGUAGE_GERMAN)
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
  struct UiWindow *nameWindow;
  s32 result = 0;
  s32 originalLength = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = UnitGetFar(member)->name;
  struct UiRenderWork *render = UI_WORK;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct UiWindow *keyboardWindow;
  union MenuCursor keyCursor;
  union MenuCursor nameCaret;
  s32 keyColumn, keyRow, vramSlot;
  struct RenderOutput *output;
  u8 *end;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Ui_LoadWindowGraphics();
  keyboardWindow = (struct UiWindow *)UiWindow_Create(3, 6, 24, 9, 2);
  nameWindow = (struct UiWindow *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
  UiWindow_CreateWithSideObject(Localization_LookupEntryId(member), 0, 3, 1);
  UiWindow_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
  UiText_DrawStringInWindow(gNameEntry_DeleteKeyLabel, keyboardWindow, 144, 32);
  UiText_DrawStringInWindow(gNameEntry_EndKeyLabel, keyboardWindow, 144, 40);
#endif
  UiWindow_DrawDividerLine(keyboardWindow, 18, 0, 18, 7);
  render->dirty = textDirty;
  // Reserve a zero byte before editedName for the empty-name backspace test.
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < NAME_MAX_BYTES; i++) {
      u32 c = originalName[i];

      editedName[i] = c;
      if (c != 0) {
        originalLength++;
        letterCount++;
      }
    }
  }
  editedName[NAME_MAX_BYTES] = 0;
  UiText_DrawPaddedLabel(nameWindow, originalName);
  vramSlot = Resource_FindFreeEntry();
  keyColumn = NAME_COMMAND_COLUMN;
  keyRow = 5;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    output =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    keyCursor.output = output;
    ShopCursor_SetPositionImmediateFar(&keyCursor.cursor,
                                       keyboardWindow->x * 8 + 140,
                                       keyboardWindow->y * 8 + 52);
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockATiles);
    output =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    nameCaret.output = output;
    output->sentinel = 255;
    ((struct FieldSprite *)output->unk10)->priority = 0;
    ShopCursor_SetPositionImmediateFar(
        &nameCaret.cursor, UiText_SetRenderString(editedName) + 70, 22);
  }
  // The key under the cursor flashes between palette 15 and palette 14, a
  // copy of it with colour 4 grey.
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  end = (u8 *)(originalLength + (s32)editedName);
  while (1) {
    s32 width = 1;

    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 4)
        width = 3;
      if (keyRow == 5)
        width = NAME_END_KEY_WIDTH;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    UiWindow_SetTileAttributeRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                  14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                  15);
    if (cursorDirty) {
      cursorDirty = 0;
      Shop_SetCursorFar(&keyCursor.cursor,
                        (keyboardWindow->x + keyColumn) * 8 - 7,
                        (keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      Shop_SetCursorFar(&nameCaret.cursor,
                        UiText_SetRenderString(editedName) + 70, 22, 3);
    }
    ShopCursor_AdvanceFar(&keyCursor.cursor);
    ShopCursor_MoveTowardTargetFar(&nameCaret.cursor);
    {
      u32 frame = (gFrameTick >> 1) & 7;

      output = nameCaret.output;
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
      keyRow--;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = 5;
      } else {
        keyRow = (keyRow != 3);
        keyRow = 5 - keyRow;
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow++;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == 6)
          keyRow = 0;
      } else {
        keyRow = (keyRow != 6);
        keyRow += 4;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        keyColumn = NAME_COMMAND_COLUMN;
        if (keyRow < 4 || keyRow > 5)
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN && (keyRow < 4 || keyRow > 5))
        keyColumn = 0;
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 5;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        *--end = 0;
        RenderOutput_PrepareForRedraw(nameWindow);
        UiText_DrawPaddedLabel(nameWindow, editedName);
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
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 5) {
        if (letterCount == 0) {
          nameCaret.output->active = 13;
          RenderOutput_PrepareForRedraw(nameWindow);
          UiText_DrawPaddedLabel(nameWindow, originalName);
          WaitFrames(10);
        } else {
          u8 *dst = originalName;
          u8 *src = editedName;
          s32 i = 0;
          u8 *dstSave = dst;
          u8 *srcSave = src;

          /* FAKEMATCH: the split restores end the pointer setup's
           * CSE blocks. They disappear, preserving destination/
           * source/index in r2/r1/r0; removing the barrier alone
           * changes them to r1/r0/r2 (14 instructions differ). A for loop
           * copying name[i] = input[i], or walking the pointers,
           * differs 14; the while form with i declared first, 12. */
          if (originalName)
            src = NULL;
          if (originalName)
            src = srcSave;
          if (originalName)
            dst = NULL;
          if (originalName)
            dst = dstSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (keyRow == 4)
        goto remove_character;
      continue;
    }
    {
      u32 tx = keyboardWindow->x + keyColumn + 1;
      u32 cell;
      u32 letter;

      cell = ((keyboardWindow->y + keyRow + 1) << 5) + tx;
      // Key glyphs use the low byte of the mirrored tilemap entries.
      letter = ((u8 *)&render->tilemap)[cell * 2];
      if (letterCount == NAME_MAX_LETTERS)
        continue;
      *end++ = letter;
      *end = 0;
      letterCount++;
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = 5;
        cursorDirty = 1;
      }
      RenderOutput_PrepareForRedraw(nameWindow);
      UiText_DrawPaddedLabel(nameWindow, editedName);
      textDirty = 1;
    }
  }
  UiWork_Finalize(keyboardWindow, 2);
  UiWork_Finalize(nameWindow, 2);
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
  struct UiWindow *nameWindow;
  s32 result = 0;
  s32 keyboard = 0;
  s32 bytes = 0;
  s32 letterCount = 0;
  u8 text[NAME_MAX_BYTES + 2];
  u8 *editedName = text + 1;
  u8 *originalName = UnitGetFar(member)->name;
  struct UiRenderWork *render = UI_WORK;
  s32 textDirty = 1;
  s32 cursorDirty = 1;
  struct UiWindow *keyboardWindow;
  union MenuCursor keyCursor;
  union MenuCursor nameCaret;
  s32 keyColumn, keyRow, vramSlot, x;
  struct RenderOutput *output;
  u32 letter;
  u16 *palette;

  // The keyboard, the name and the portrait.
  Ui_LoadWindowGraphics();
  keyboardWindow = (struct UiWindow *)UiWindow_Create(3, 6, 24, 13, 2);
  nameWindow = (struct UiWindow *)UiWindow_Create(8, 3, 8, 3, 2);
  UiWindow_CreateWithSideObject(Localization_LookupEntryId(member), 0, 3, 1);
  UiWindow_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
  UiWindow_DrawDividerLine(keyboardWindow, 18, 0, 18, 12);
  render->dirty = textDirty;
  // Reserve a zero byte before editedName for the empty-name backspace test.
  text[0] = result;
  {
    s32 i;

    for (i = 0; i < NAME_MAX_BYTES; i++) {
      letter = originalName[i];

      editedName[i] = letter;
      if (letter != 0) {
        bytes++;
        letterCount++;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1)
          letterCount--;
      }
    }
  }
  editedName[NAME_MAX_BYTES] = 0;
  UiText_DrawString(originalName, nameWindow, 0, 0);
  keyColumn = NAME_COMMAND_COLUMN;
  vramSlot = Resource_FindFreeEntry();
  keyRow = 10;
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockBTiles);
    output =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    keyCursor.output = output;
    ShopCursor_SetPositionImmediateFar(&keyCursor.cursor,
                                       keyboardWindow->x * 8 + 140,
                                       keyboardWindow->y * 8 + 92);
  }
  vramSlot = Resource_FindFreeEntry();
  if (vramSlot <= 95) {
    VramCache_Load(vramSlot, 128, gResource_FixedBlockATiles);
    output =
        RenderOutput_Create(vramSlot, OUTPUT_SIZE_16X16, keyboardWindow, 0, 0);
    nameCaret.output = output;
    output->sentinel = 255;
    ((struct FieldSprite *)output->unk10)->priority = 0;
    x = 72 + NameEntry_CountLetters(editedName) * 10;
    ShopCursor_SetPositionImmediateFar(&nameCaret.cursor, x, 22);
  }
  // The key under the cursor flashes between palette 15 and palette 14, a
  // copy of it with colour 4 grey.
  palette = (u16 *)0x050001c0;
  Dma_Set((void *)0x050001e0, palette, 0x84000008, REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    s32 width = 1;

    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 0)
        width = 4;
      if (keyRow == 8)
        width = 3;
      if (keyRow == 10)
        width = 3;
    }
    // Flash the key under the cursor, then move the cursor and the caret.
    UiWindow_SetTileAttributeRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                  14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(keyboardWindow, keyColumn, keyRow, width, 1,
                                  15);
    if (cursorDirty) {
      cursorDirty = 0;
      Shop_SetCursorFar(&keyCursor.cursor,
                        (keyboardWindow->x + keyColumn) * 8 - 7,
                        (keyboardWindow->y + keyRow) * 8 + 15, 3);
    }
    if (textDirty) {
      textDirty = 0;
      x = 72 + NameEntry_CountLetters(editedName) * 10;
      Shop_SetCursorFar(&nameCaret.cursor, x, 22, 3);
    }
    ShopCursor_AdvanceFar(&keyCursor.cursor);
    ShopCursor_MoveTowardTargetFar(&nameCaret.cursor);
    {
      u32 frame = (gFrameTick >> 1) & 7;

      output = nameCaret.output;
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
      keyRow--;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == -1)
          keyRow = 10;
        else if (keyRow == 5)
          keyRow = 4;
      } else {
        if (keyRow == -1) {
          keyRow = 10;
        } else {
          keyRow = (keyRow != 7);
          keyRow *= 8;
        }
      }
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyRow++;
      if (keyColumn != NAME_COMMAND_COLUMN) {
        if (keyRow == 11)
          keyRow = 0;
        if (keyRow == 5)
          keyRow = 6;
      } else {
        if (keyRow == 1)
          keyRow = 8;
        else if (keyRow == 9)
          keyRow = 10;
        else
          keyRow = 0;
      }
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn--;
      if (keyColumn == -1) {
        if (keyRow == 0 || keyRow == 8 || keyRow == 10)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 16;
      } else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17) {
        keyColumn--;
      }
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn++;
      if (keyColumn == 19)
        keyColumn = 0;
      else if (keyColumn == 5 || keyColumn == 11 || keyColumn == 17)
        keyColumn++;
      if (keyColumn == NAME_COMMAND_COLUMN) {
        if (keyRow == 0 || keyRow == 8 || keyRow == 10)
          keyColumn = NAME_COMMAND_COLUMN;
        else
          keyColumn = 0;
      }
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      cursorDirty = 1;
      keyColumn = NAME_COMMAND_COLUMN;
      keyRow = 10;
    }
    if (gKeysPressed & KEY_SELECT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    change_keyboard:
      keyboard ^= 1;
      if (keyboard)
        UiWindow_CopyTilemapRegion(keyboardWindow,
                                   gNameEntry_AlternateKeyboardTilemap);
      else
        UiWindow_CopyTilemapRegion(keyboardWindow, gNameEntry_KeyboardTilemap);
      UiWindow_DrawDividerLine(keyboardWindow, 18, 0, 18, 12);
      render->dirty = 1;
      continue;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    remove_character:
      if (letterCount != 0) {
        letterCount--;
        bytes--;
        letter = editedName[bytes];
        editedName[bytes] = 0;
        if ((u8)(letter - NAME_VOICED_MARK) <= 1) {
          bytes--;
          editedName[bytes] = 0;
        }
      } else {
        result = -1;
        break;
      }
      RenderOutput_PrepareForRedraw(nameWindow);
      UiText_DrawString(editedName, nameWindow, 0, 0);
      textDirty = 1;
      continue;
    }
    // A: a command key, or a letter.
    if (!(gKeysRepeat & KEY_A))
      continue;
    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
    if (keyColumn == NAME_COMMAND_COLUMN) {
      if (keyRow == 10) {
        if (letterCount == 0) {
          nameCaret.output->active = 13;
          RenderOutput_PrepareForRedraw(nameWindow);
          UiText_DrawString(originalName, nameWindow, 0, 0);
          WaitFrames(10);
        } else {
          u8 *dst;
          u8 *src;
          s32 i;
          u8 *dstSave;
          u8 *srcSave;

          dst = originalName;
          src = editedName;
          i = 0;
          /* FAKEMATCH: the split restores end the pointer setup's
           * CSE blocks. They disappear, preserving destination/
           * source/index in r1/r2/r0; removing the barrier alone
           * changes them to r2/r0/r1 (14 instructions differ). A for loop
           * copying name[i] = input[i] differs 18, one walking the
           * pointers 14; the while form with i declared first, 12. */
          dstSave = dst;
          srcSave = src;
          if (keyboardWindow->height)
            dst = NULL;
          if (keyboardWindow->height)
            dst = dstSave;
          if (keyboardWindow->height)
            src = NULL;
          if (keyboardWindow->height)
            src = srcSave;
          while (i < 15) {
            *dst++ = *src++;
            i++;
          }
        }
        break;
      }
      if (keyRow == 8)
        goto remove_character;
      if (keyRow == 0)
        goto change_keyboard;
      continue;
    }
    {
      u32 tx = keyboardWindow->x + keyColumn + 1;
      u32 cell;
      u8 *dst;

      cell = ((keyboardWindow->y + keyRow + 1) << 5) + tx;
      // Key glyphs use the low byte of the mirrored tilemap entries.
      letter = ((u8 *)&render->tilemap)[cell * 2];
      if (letter == 0xfe) // the keyboard's dakuten key
        letter = NAME_VOICED_MARK;
      if (letter == 0xff) // the keyboard's handakuten key
        letter = NAME_SEMIVOICED_MARK;
      if (letterCount == NAME_MAX_LETTERS &&
          (u8)(letter - NAME_VOICED_MARK) > 1)
        continue;
      dst = editedName + bytes;
      if (!NameEntry_AcceptsMark(letter, editedName[bytes - 1])) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        continue;
      }
      *dst = letter;
      bytes++;
      editedName[bytes] = 0;
      letterCount++;
      if ((u8)(letter - NAME_VOICED_MARK) <= 1)
        letterCount--;
      if (letterCount == NAME_MAX_LETTERS) {
        keyColumn = NAME_COMMAND_COLUMN;
        keyRow = 10;
        cursorDirty = 1;
      }
      RenderOutput_PrepareForRedraw(nameWindow);
      UiText_DrawString(editedName, nameWindow, 0, 0);
      textDirty = 1;
    }
  }
  UiWork_Finalize(keyboardWindow, 2);
  UiWork_Finalize(nameWindow, 2);
  UiWork_FinalizeEntityMatchingLocalizedId(member);
  WaitFrames(1);
  return result;
}
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif (LANGUAGE == LANGUAGE_SPANISH)
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
  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
    cursor.output = RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, 0, 0);
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
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      row--;
      moved = 1;
      if (row == -1)
        row = 2;
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      row++;
      moved = 1;
      if (row == 3)
        row = 0;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      row = -1;
      break;
    }
    if (gKeysRepeat & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
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

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

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
// gCharacter_ChildSpriteTable during the prologue, from
// gCharacter_SpriteTable otherwise; 0 for any other member.
s32 Party_LookupCharacterValueByFlag32(u32 index) {
  if (index > 8) {
    return 0;
  }
  if (GameFlag_TestFar(FLAG_CHILDHOOD) == 0) {
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
    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;
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

    ((struct UiRenderWork *)work)->speakerVoiceSound = zero;
    ((struct UiRenderWork *)work)->speakerVoiceCountdown = zero;

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
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Resource_DecodeByteLz((void *)((u32)offsetTable + offsetTable[icon]),
                          buffer);
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
  s32 scale = gUi_ObjectPulseScales[(gFrameTick >> 1) & 7];
  struct AffineTransform effect;

  if (scale < 0)
    scale += 255;
  scale >>= 8;

  if (obj != 0) {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    obj->oam.matrix = AffineMatrix_BuildForEffect(&effect);
    obj->oam.affineMode = 3;
    obj->oam.x = obj->x + 0xfff0;
    obj->oam.y = (u8)obj->y + 0xf0;
    obj->sentinel = 0xfc;
  }
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT 6
#define RANGE_PATTERN_PAIR_WORDS 16

// Builds two sets of six paired bar-pattern tiles. Each pair starts in
// color 4, then XORs row masks from gMenu_BarPatternRows.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void) {
#else
s32 Menu_BuildLocalizedPatternTiles(void) {
#endif
  u32 *vram = (u32 *)0x06006280;
  s32 patternSet;
  s32 patternIndex;

  for (patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
    for (patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT; patternIndex++) {
      u32 *tile =
          vram + patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
          patternIndex * RANGE_PATTERN_PAIR_WORDS;
      s32 row;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (row = 1; row <= 7; row++) {
        s32 sourcePattern = patternIndex;

        if (patternSet == 1 && row <= 1) {
          continue;
        }
        if (patternSet == 0 && patternIndex > row - 2) {
          sourcePattern = row - 2;
          if (sourcePattern < 0) {
            sourcePattern = 0;
          }
        }
        tile[row] =
            XorWord(tile[row], gMenu_BarPatternRows[sourcePattern].word0);
        tile[row + 8] =
            XorWord(tile[row + 8], gMenu_BarPatternRows[sourcePattern].word1);
      }
    }
  }
}

// Draws a range indicator across three adjacent tiles at (x, y), using the
// selected pattern and a mirrored left end.
// Void changes pop {r1}; bx r1 to r0 and reorders a preview's argument setup.
#ifdef UBFIX
void UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 x, s32 y,
                                  s32 pattern, s32 unused) {
#else
int UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 x, s32 y,
                                 s32 pattern, s32 unused) {
#endif
  s32 patternOffset = pattern * 2;
  s32 rightTile = patternOffset + 0xF315;

  UiWindow_SetTilemapEntry(window, 0x400 | rightTile, x, y, 0);
  UiWindow_SetTilemapEntry(window, patternOffset + 0xF314, x + 1, y, 0);
  UiWindow_SetTilemapEntry(window, rightTile, x + 2, y, 0);
}

// Merge two 8x8 4bpp tiles row by row. Shift the second tile horizontally by
// shift pixels, using the first tile wherever the shifted pixel is transparent.
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift) {
  s32 oppositeShift;
  s32 rightShift;
  s32 leftShift;
  s32 row;

  oppositeShift = -shift;
  row = 0;
  rightShift = oppositeShift * 4;
  leftShift = shift * 4;

  do {
    u32 result;
    u32 shifted;
    u32 fallback;
    s32 pixel;

    shifted = *second++;
    result = 0;
    fallback = *first++;
    if (shift < 0)
      shifted >>= rightShift;
    else
      shifted <<= leftShift;

    pixel = 7;
    do {
      result <<= 4;
      if (shifted > 0x0FFFFFFF)
        result += shifted >> 28;
      else
        result += fallback >> 28;
      pixel--;
      shifted <<= 4;
      fallback <<= 4;
    } while (pixel >= 0);

    *output++ = result;
    row++;
  } while (row <= 7);
}

// Animate the two link indicator tiles with opposing two/three-pixel shifts.
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

// Expand two palette variants of the ten source tile rows at 0x06000600,
// mapping each packed nibble through gGraphics_ExpandNibbleTable.
void Graphics_ExpandVramTilesByColorTable(u16 *dst) {
  s32 bank = 0;
  s32 paletteOffset = 0;
  s32 destinationBank = 0;

  do {
    s32 row = 0;
    s32 destinationOffset = (destinationBank + bank) << 6;

    do {
      u16 *output = (u16 *)((u8 *)dst + destinationOffset);
      u16 *source = (u16 *)((u8 *)0x06000600 + (row << 5));
      s32 column = 0;

      do {
        u32 packed = *source++;
        u32 expanded = 0;
        s32 nibble = 0;

        do {
          u32 color = gGraphics_ExpandNibbleTable[(packed & 15) + paletteOffset]
                      << (nibble * 4);
          nibble++;
          packed >>= 4;
          expanded |= color;
        } while (nibble <= 3);

        column++;
        *output++ = expanded;
      } while (column <= 15);

      row++;
      destinationOffset += 32;
    } while (row <= 9);

    paletteOffset += 16;
    destinationBank += 4;
    bank++;
  } while (bank <= 1);
}

// Compose a table-entry glyph in temporary transfer work, reload its cache
// slot, then release the temporary work and return the tile index.
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  Ui_PrepareTransferFromTableEntry(resource);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

// Compose the item icon with style 0x1a in temporary transfer work and reload
// the requested cache slot. Return its tile index.
s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  ItemIcon_Compose(resource, 0x1a);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

// Build the ability glyph in temporary transfer work, reload the requested
// cache slot and return its tile index.
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index) {
  s32 tile;
  s32 slot;
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  slot = index;
  Ability_LoadGlyph(resource, 0, &slot, &tile, 1);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

// Load a party portrait into the supplied cache slot and return its tile.
// In the prologue, Isaac and Garet use their childhood portrait entries.
s32 Ui_LoadEntryForKind(u32 kind, s32 value) {
  s32 tile;
  s32 slot;
  u32 portrait;

  portrait = kind;
  slot = value;
  if (portrait > 7U) {
    portrait = 0;
  }
  if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
    switch (portrait) {
    case 0:
      portrait = 0x38;
      break;
    case 1:
      portrait = 0x39;
      break;
    }
  }
  UiGlyph_LoadEntryWithPalette(portrait, 0, &slot, &tile, 0xE, 1);
  return tile;
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

// Copies the 4bpp decoder into RAM, calls it with the transfer's encoded
// source and byte buffer, then releases the copied code. Its ARM entry reads
// the stream through r0 and writes bytes through r1, as UiIcon_Decode does.
void Ui_RunCopiedRoutine(GlyphTransfer *work) {
  RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
      work->encoded, work->input);
  Runtime_ReleaseHeapBlock(49);
}

//--------------------------------------------------------------------------
// create_countdown_label_window.c
// Link: open the remaining-time heading window.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Open a 6x4 window at the upper left, write its heading and return it.
//--------------------------------------------------------------------------
#if (LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH) ||           \
    (GAME_TLA && (LANGUAGE == LANGUAGE_ITALIAN))
#define LINK_COUNTDOWN_DRAW_HEADING UiText_DrawStringAtOffset
#else
#define LINK_COUNTDOWN_DRAW_HEADING UiText_DrawStringInWindow
#endif

// Create the link timer window and draw its fixed time heading.
struct UiWindow *Link_CreateCountdownLabelWindow(void) {
  struct UiWindow *win = UiWindow_Create(0, 0, 6, 4, 6);

  LINK_COUNTDOWN_DRAW_HEADING(gLink_TimeLabelString, win, 0, 0);
  return win;
}

// Decode a command-icon table entry in temporary transfer work and reload
// the requested cache slot. Return its tile index.
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon) {
  GlyphTransfer *buffer =
      Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **encoded = &buffer->encoded;
  void *target = (void *)((u32)offsetTable + offsetTable[icon]);
  s32 result;

  *encoded = target;
  Resource_DecodeByteLz(target, buffer);
  result = VramCache_Reload(slotIndex, (s32)buffer);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

// Load a command icon into 8bpp OBJ VRAM, sharing a compact palette across
// the selection's icons. Copy each newly encountered color from OBJ to BG
// palette storage before uploading the remapped pixels.
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph,
                              s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *source;
  u8 *destination;
  s32 i;
  u32 index;

  decoded = Runtime_AllocateHeapBlock(17, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Resource_DecodeByteLz(decoded->encoded, decoded);
  remapped = MemAlloc(0x400);
  source = decoded->input;
  destination = remapped;
  for (i = 0; i < 0x400; i++) {
    index = *source++;
    if (palette->slot[index] == 255) {
      palette->slot[index] = palette->count;
      if (palette->count < 64) {
        ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
        palette->count++;
      }
    }
    *destination++ = palette->slot[index];
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
  s32 iconCount;
  struct AffineTransform affine;
  s32 affineIndex, selectedIndex, drawnIndex;
  s32 commandKinds[6];
};

// Load a selection icon, initialize its sprite and bind the decoded tiles.
// The icon's kind also chooses its command-name label.
void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 icon) {
  struct SelectionEntry *entry = &work->entries[index];

  UiText_LoadRemappedGlyph(&work->palette, icon, index * 16);
  work->entries[index].kind = icon;
  MenuSprite_Init(entry->sprite, 0x80002000);
  entry->sprite.oam.f.tile =
      Resource_LoadIndexedIntoBuffer(work->entries[index].resource, icon);
}

// Put BG1 behind the other priority levels while the command strip is active.
void Graphics_SetBg1Priority3(void) {
  u32 value = REG_BG1CNT;

  value &= ~3;
  value |= 3;
  REG_BG1CNT = value;
}

// Restore BG1 priority 0; the VCount handler applies this below the strip.
void Graphics_ClearBg1ControlBit2(void) {
  u32 value = REG_BG1CNT;

  value &= ~3;
  REG_BG1CNT = value;
}

// Clear BG0 vertical scroll and install the BG1 priority change at line 136.
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

// Open a message at (x, y), wait for its text to finish, and return its window
// so the caller can hold it open for a separate prompt.
struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y) {
  struct UiWindow *result;

  result = UiText_OpenMessageWindow(message, x, y, 1);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  return result;
}

#if TBS_PATCH_1
#define WINDOW_X 22 /* Window X position */
#else
#define WINDOW_X 23
#endif

// Select a command icon, returning its kind, -1 for cancel or -2 for a
// shoulder-button switch. Mode 0 is the top menu; the other mode selects
// a battle command and remembers its own cursor. Tutorial input is replayed
// through the same selection path.
s32 Ui_RunSelectionScreen(s32 mode) {
  u8 party[4];
  struct UiRenderWork *render;
  struct SelectionWork *selectionUi;
  struct MenuCell *navigation;
  s32 stripDirty, tradeCount, pulseScale, command;
  s32 entryIndex;
  s32 keys, repeat;
  s32 iconSlot;
  s32 i, kindCount;

  render = UI_WORK;
  iconSlot = VramCache_AllocEntry(0x400);
  stripDirty = 1;
  tradeCount = 0;
  selectionUi = Runtime_BumpAllocateAlternatePool(sizeof(*selectionUi));
  for (i = 255; i >= 0; i--)
    selectionUi->palette.slot[i] = 255;
  selectionUi->palette.count = 1;
  if (mode == 0)
    selectionUi->window = UiWindow_Create(20, 17, 10, 3, 6);
  else
    selectionUi->window = UiWindow_Create(WINDOW_X, 17, 30 - WINDOW_X, 3, 6);
  selectionUi->drawnIndex = -1;
  if (mode == 0) {
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 14;
    selectionUi->commandKinds[kindCount++] = 4;
    selectionUi->commandKinds[kindCount++] = 7;
    selectionUi->commandKinds[kindCount] = -1;
    selectionUi->selectedIndex = gLinkCountdownWork->commandSelection[0];
  } else {
    selectionUi->selectedIndex = gLinkCountdownWork->commandSelection[1];
    kindCount = 0;
    selectionUi->commandKinds[kindCount++] = 0;
    selectionUi->commandKinds[kindCount++] = 1;
    if (*((s32 *)Djinn_GetRecoveryTableFar(0)) != 0) {
      BattlePlacement_CountValidEntriesFar(0, party);
      for (i = 0; i <= 3; i++)
        tradeCount += party[i];
      selectionUi->commandKinds[kindCount++] = 15;
      if (tradeCount != 0)
        selectionUi->commandKinds[kindCount++] = 16;
    }
    selectionUi->commandKinds[kindCount++] = 2;
    selectionUi->commandKinds[kindCount++] = 3;
    selectionUi->commandKinds[kindCount] = -1;
  }
  // Decode each visible command through one shared remapped palette.
  for (entryIndex = 0; entryIndex <= 5; entryIndex++) {
    selectionUi->entries[entryIndex].resource = iconSlot;
    if (selectionUi->commandKinds[entryIndex] == -1)
      break;
    UiText_DrawCharacter(selectionUi, entryIndex,
                         selectionUi->commandKinds[entryIndex]);
  }
  selectionUi->iconCount = entryIndex;
  selectionUi->affine.scaleX = 320;
  selectionUi->affine.scaleY = 304;
  selectionUi->affine.angle = 0;
  entryIndex = 0;
  if (entryIndex < selectionUi->iconCount) {
    do {
      s32 x = (entryIndex - selectionUi->iconCount) * 24;
      selectionUi->entries[entryIndex].posX = x + 155;
      if (mode != 0)
        selectionUi->entries[entryIndex].posX = x + WINDOW_X * 8 - 5;
      selectionUi->entries[entryIndex].posY = 136;
      entryIndex++;
    } while (entryIndex < selectionUi->iconCount);
  }
  Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
  Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

  for (;;) {
    pulseScale = gUi_BounceScales[(gFrameCount * 2) & 31];
    pulseScale = (pulseScale - 256) / 4 + 304;
    selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    if (stripDirty != 0) {
      stripDirty = 0;
      render->menuBusy = 1;
      if (mode) {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 +
                WINDOW_X - 18 + (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
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
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
                count * 3 + WINDOW_X) = 0;
            }
          }
        }
      } else {
        s32 panel;
        s32 count;
        for (panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
            s32 col;
            for (col = 0; col <= 2; col++)
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + 2 +
                (col & 3)) = 0xf07f;
          }
        }
        for (panel = 0; panel < selectionUi->iconCount; panel++) {
          /* FAKEMATCH: needed to match: the empty loop restores the CSE block
           * boundary at the panel body's head, so the exit test compares count
           * instead of reloading work->count (223 differ without it). */
          do {
          } while (0);
          count = selectionUi->iconCount;
          for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
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
              vram[panel * 3 + entryIndex * 32 + (col & mask) - count * 3] =
                  tmp = panel * 16 + entryIndex * 4 + col + offset;
              tmp = col & mask;
              *(render->tilemap + (entryIndex + 17) * 32 + panel * 3 + tmp -
                count * 3 + 20) = 0;
            }
          }
        }
      }
    }
    if (selectionUi->drawnIndex != selectionUi->selectedIndex) {
      RenderOutput_PrepareForRedraw(selectionUi->window);
      UiText_DrawCharacterAtOffset(
          selectionUi->entries[selectionUi->selectedIndex].kind +
              MsgCommandName,
          selectionUi->window, 0, 0);
      selectionUi->drawnIndex = selectionUi->selectedIndex;
      UiText_DrawCharacter(
          selectionUi, selectionUi->selectedIndex,
          selectionUi->commandKinds[selectionUi->selectedIndex]);
    }
    selectionUi->affineIndex =
        AffineMatrix_BuildForEffect(&selectionUi->affine);
    for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
      struct SelectionEntry *entry = &selectionUi->entries[entryIndex];
      if (entryIndex == selectionUi->selectedIndex) {
        entry->sprite.oam.f.affineIndex = selectionUi->affineIndex;
        entry->sprite.oam.f.affine = 3;
        entry->sprite.oam.f.x = entry->posX + pulseScale * 7 / 512 - 14;
        entry->sprite.oam.f.y = entry->posY + pulseScale * 3 / 256 - 20;
        Runtime_PushSlotEntry(entry, 0xf1);
      } else {
        entry->sprite.oam.f.x = entry->posX;
        entry->sprite.oam.f.y = entry->posY;
        entry->sprite.oam.f.affineIndex = 0;
        entry->sprite.oam.f.affine = 0;
      }
    }
    // The tutorial drives the same command selection using synthetic keys.
    keys = gKeysPressed;
    repeat = gKeysRepeat;
    navigation = gLinkCountdownWork;
    if (navigation->autoEnabled != 0) {
      keys = repeat = 0;
      if (navigation->autoDelay == 0) {
        if (navigation->autoMode == 1) {
          if (selectionUi->commandKinds[selectionUi->selectedIndex] == 3)
            keys = repeat = 1;
          else
            keys = repeat = 32;
          navigation->autoDelay = 30;
        } else if (navigation->autoMode == 0) {
          s32 kind = selectionUi->commandKinds[selectionUi->selectedIndex];
          if (kind == 16 || (tradeCount == 0 && kind == 15)) {
            if (navigation->autoEnabled == 1) {
              struct UiWindow *message;
              if (kind == 15)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialUnleashCommand, 15, 8);
              else if (kind == 16)
                message = UiText_ShowMessageAndWaitComplete(
                    MsgDjinnTutorialSummon, 15, 8);
              BattlePresentation_WaitForPromptAtFar(102, 155);
              UiWork_Finalize(message, 1);
              navigation->autoEnabled++;
              navigation->autoDelay = 45;
            } else {
              keys = repeat = 1;
              navigation->autoDelay = 200;
            }
          } else {
            keys = repeat = 16;
            navigation->autoDelay = 40;
          }
        } else {
          navigation->autoDelay = 60;
          keys = repeat = 1;
        }
      } else {
        navigation->autoDelay--;
      }
    }
    if ((keys & KEYS_SHOULDERS) && mode != 0) {
      command = -2;
      break;
    }
    if (keys & KEY_A) {
      command = selectionUi->commandKinds[selectionUi->selectedIndex];
      break;
    }
    if (mode != 0 && (keys & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      command = -1;
      break;
    }
    if (repeat & (KEY_DOWN | KEY_RIGHT)) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + 1) % selectionUi->iconCount;
    } else if (repeat & (KEY_UP | KEY_LEFT)) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selectionUi->selectedIndex =
          (selectionUi->selectedIndex + selectionUi->iconCount - 1) %
          selectionUi->iconCount;
    } else if (gLinkCountdownWork->active == 0) {
      command = selectionUi->commandKinds[0];
      break;
    }
    QueueIoWriteDelay2(0x04000000, 0x1741);
    render->menuBusy = 0;
    WaitFrames(1);
  }
  if (mode != 0)
    gLinkCountdownWork->commandSelection[1] = selectionUi->selectedIndex;
  else
    gLinkCountdownWork->commandSelection[0] = selectionUi->selectedIndex;
  for (entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
    Resource_ResetEntry(selectionUi->entries[entryIndex].resource);
  render->menuBusy = 1;
  UiWork_Finalize(selectionUi->window, 1);
  {
    s32 panel;
    for (panel = 0; panel <= 6; panel++) {
      for (entryIndex = 0; entryIndex <= 2; entryIndex++) {
        s32 col;
        /* FAKEMATCH: the add of 0x44a stays apart from the store it addresses
         * (adds r3, r7; strh r6, [r3]); as a plain store combine folds it into
         * strh r6, [r3, r7], 0 -> 33. An ordinary row pointer with three
         * post-increment stores shortens Ui_RunSelectionScreen by 24 bytes
         * in tbs-en instead of matching its address formation. */
        for (col = 0; col <= 2; col++)
          *(volatile u16 *)(render->tilemap + (entryIndex + 17) * 32 +
                            panel * 3 + 5 + (col & 3)) = 0xf07f;
      }
    }
  }
  // Restore the normal BG1 handler after clearing the command strip.
  render->dirty = 1;
  WaitFrames(1);
  Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
  Runtime_SetIrqHandler(2, 0, 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
  MemFree(selectionUi);
  render->menuBusy = 0;
  WaitFrames(1);
  return command;
}

// Set bit 12 of each tile in a screen-space rectangle, preserving the other
// tile attributes. Keep the original bottom-edge test (29) and height (20).
void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 paletteBit) {
  struct UiRenderWork *render = UI_WORK;

  paletteBit &= 1;
  paletteBit <<= 12;

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
      u16 *cell = &((u16 (*)[32])render->tilemap)[y][x];
      s32 remaining = width;

      if (remaining != 0) {
        do {
          u32 tile = *cell;
          tile = (tile & ~0x1000) | paletteBit;
          remaining--;
          *cell = tile;
          cell++;
        } while (remaining != 0);
      }

      UiRender_MarkRow(render, y);
      height--;
      y++;
    } while (height != 0);
  }
}

// Dim the party strip, then highlight the actor IDs up to the 0xff terminator.
// Missing IDs are ignored; the search stops at the party's own terminator.
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

// The decimal formatter writes eleven characters plus a terminator. Keep
// the original two trailing bytes in the widened copy as well.
#define NUMBER_TEXT_WIDE_LENGTH 14

// Formats a decimal number, then widens the first 14 shared-buffer bytes
// into halfwords. The caller supplies space for 14 output halfwords.
// Void changes pop {r1}; bx r1 to r0 and reorders three preview argument pairs.
#ifdef UBFIX
void UiText_FormatNumberToHalfwords(s16 *out, s32 value) {
#else
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value) {
#endif
  s16 *destination;
  s32 remaining;
  u8 *source;

  destination = out;
  NumberText_FormatDecimal(value);
  source = gNumberTextBuffer;
  for (remaining = 0; remaining < NUMBER_TEXT_WIDE_LENGTH; remaining++) {
    *destination++ = *source++;
  }
}

// Preview marks are separate from the action ID in the low fourteen bits.
#define DJINN_PREVIEW_ABILITY_GAINED 0x8000
#define DJINN_PREVIEW_ABILITY_LOST 0x4000
#define DJINN_PREVIEW_ABILITY_CAPACITY 48

// List Psynergy in the new grid, marking newly gained entries with bit 15,
// then append lost entries with bit 14. Return the number of entries and
// report the gained/lost counts separately.
s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid,
                               const struct OwnerActionSlot *newGrid, u16 *out,
                               s32 *gained, s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 gainedCount;
  s32 lostCount;

  gainedCount = 0;
  lostCount = 0;
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
        gainedCount++;
        out[count - 1] |= DJINN_PREVIEW_ABILITY_GAINED;
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
        lostCount++;
        out[count] = (oldGrid[i].encodedAction & ACTION_ID_MASK) |
                     DJINN_PREVIEW_ABILITY_LOST;
        count++;
      }
    }
  }
  *gained = gainedCount;
  *lost = lostCount;
  return count;
}

// Loads an ability icon into the first free VRAM cache slot and requests a
// 16x16 sprite at pixel (x, y) inside the window. Skips a full VRAM cache.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void Ui_CreateOutputFromResourceSlot(struct UiWindow *window, s32 x, s32 y,
                                     s32 abilityId) {
#else
s32 Ui_CreateOutputFromResourceSlot(struct UiWindow *window, s32 x, s32 y,
                                    s32 abilityId) {
#endif
  s32 iconSlot;

  iconSlot = Resource_FindFreeEntry();
  if (iconSlot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(abilityId, iconSlot);
    // If the output pool is full, the creator releases iconSlot itself.
    RenderOutput_Create(iconSlot, OUTPUT_SIZE_16X16, window, x, y);
  }
}

// Temporarily toggle a Djinni and recalculate its owner's stats to show the
// change. Page 0 compares stats; later pages show gained and lost Psynergy.
// Restore the complete unit snapshot before returning the preview window.
struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *win, s32 owner,
                                             s32 encodedDjinn, s32 page,
                                             s32 *pageCount) {
  struct BattleUnit *unit;
  struct BattleUnit *savedUnit;
  struct BattleAct *ability;
  s16 *numberText;
  s32 oldHp;
  s32 oldPp;
  s32 oldAtk;
  s32 oldDef;
  s32 oldAgi;
  u16 *changedAbilities;
  s32 abilityCount;
  s32 visibleRow;
  s32 summaryRows;
  s32 visibleRows;
  s32 element;
  s32 djinnIndex;
  s32 pages;
  s32 gained;
  s32 lost;
  s32 rangePattern;
  s32 rising;
  s32 i;

  // Attach the rising/falling stat arrow to the preview window captured here.
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

  // Save the unit before the temporary Djinn toggle changes its class/stats.
  // Fourteen widened characters fit well inside the 64-halfword text buffer.
  // With a valid refreshed old list, class rebuilding can remove at most
  // sixteen actions, including any equipment actions displaced by the class.
  // Its new 32-slot list and these lost entries fit the 48-entry allocation.
  numberText = MemAlloc(64 * sizeof(*numberText));
  savedUnit = MemAlloc(sizeof(*savedUnit));
  changedAbilities = Runtime_BumpAllocateAlternatePool(
      DJINN_PREVIEW_ABILITY_CAPACITY * sizeof(*changedAbilities));
  Iwram_CopyWords(savedUnit, unit, sizeof(*savedUnit));

  element = (encodedDjinn >> 8) & 15;
  djinnIndex = encodedDjinn & 255;
  if (Djinn_IsActiveFar(owner, element, djinnIndex) != 0)
    Djinn_DeactivateFar(owner, element, djinnIndex);
  else
    Djinn_ActivateFar(owner, element, djinnIndex);
  UnitCalcFar(owner);

  abilityCount =
      DjinnMenu_ListChangedDjinn(savedUnit->actionSlots, unit->actionSlots,
                                 changedAbilities, &gained, &lost);
  pages = ((abilityCount - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= abilityCount)
    page = pages;

  // The first page compares stats; the remaining pages compare Psynergy.
  if (page == 0) {
    s32 label = MsgPreviewStatLabel;

    UiText_DrawCharacterAtOffset(label, win, 0, 8);
    UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
    UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
    UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
    UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
    UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

    oldHp = savedUnit->hp;
    UiText_FormatNumberToHalfwords(numberText, oldHp);
    UiText_RenderWideStringInWindow(numberText + 7, win, DJINN_PREVIEW_VITALS_X,
                                    1);
    oldPp = savedUnit->pp;
    UiText_FormatNumberToHalfwords(numberText, oldPp);
    UiText_RenderWideStringInWindow(numberText + 7, win, DJINN_PREVIEW_VITALS_X,
                                    2);
    oldAtk = savedUnit->atk;
    UiText_FormatNumberToHalfwords(numberText, oldAtk);
    UiText_RenderWideStringInWindow(numberText +
                                        DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                    win, DJINN_PREVIEW_STAT_X, 3);
    oldDef = savedUnit->def;
    UiText_FormatNumberToHalfwords(numberText, oldDef);
    UiText_RenderWideStringInWindow(numberText +
                                        DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                    win, DJINN_PREVIEW_STAT_X, 4);
    oldAgi = savedUnit->agi;
    UiText_FormatNumberToHalfwords(numberText, oldAgi);
    UiText_RenderWideStringInWindow(numberText +
                                        DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                    win, DJINN_PREVIEW_STAT_X, 5);
    UiText_FormatNumberToHalfwords(numberText, savedUnit->luck);
    UiText_RenderWideStringInWindow(numberText + 8, win, 5, 6);

    UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
    if (gained != 0 || lost != 0)
      UiWork_SetParamNibble(2);
    UiText_DrawCharacterAtOffset(MsgBattleAdditionalPsynergyHelp, win,
                                 DJINN_PREVIEW_HELP_X, 64);
    UiWork_SetParamNibble(15);
  }

  if (page > 0) {
    summaryRows = 0;
    visibleRows = abilityCount;
    if (abilityCount > 4)
      visibleRows = 5;
    i = page * 5 - 5;
    for (visibleRow = 0; visibleRow < visibleRows && i < abilityCount;
         visibleRow++, i++) {
      ability = BattleActGet(changedAbilities[i]);
      if (ability->element != ELEMENT_NONE)
        UiWindow_SetTilemapEntry(win, 0x5001 + ability->element, 15,
                                 visibleRow * 2, 0);

      rangePattern = ability->range;
      if (rangePattern == ABILITY_RANGE_ALL)
        rangePattern = 11;
      else
        rangePattern--;
      UiWindow_DrawThreeTileColumn(win, 16, visibleRow * 2, rangePattern, 0);

      Ui_CreateOutputFromResourceSlot(win, 0, visibleRow * 16 - 4,
                                      changedAbilities[i] & ACTION_ID_MASK);
      if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED) != 0)
        UiWork_SetParamNibble(4);
      else if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST) != 0)
        UiWork_SetParamNibble(2);
      else
        UiWork_SetParamNibble(15);

      UiText_DrawCharacterAtOffset((changedAbilities[i] & ACTION_ID_MASK) +
                                       MsgAbilityName,
                                   win, 16, visibleRow * 16);
      UiWindow_SetTilemapEntry(win, 0xf01f, 11, visibleRow * 2, 0);
      UiWindow_SetTilemapEntry(win, PSYNERGY_COST_SECOND_TILE, 12,
                               visibleRow * 2, 0);
      UiText_DrawNumberInWindow(BattleActGet(changedAbilities[i])->ppCost, 2,
                                win, 104, visibleRow * 16);
    }

    if (gained != 0) {
      UiWork_SetParamNibble(4);
      UiText_DrawCharacterAtOffset(MsgPsynergyGained, win, 32, 80);
      summaryRows = 1;
    }
    if (lost != 0) {
      UiWork_SetParamNibble(2);
      UiText_DrawCharacterAtOffset(MsgPsynergyLost, win, 32,
                                   summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      UiText_DrawCharacterAtOffset(MsgPsynergyUnchanged, win, 32, 80);
    UiWork_SetParamNibble(15);
    UiWork_SetParamNibble(15);
    UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
  }

  if (page == 0) {
    UiText_DrawCharacterAtOffset(MsgClassName + savedUnit->classIndex, win, 0,
                                 0);
    UiText_DrawCharacterAtOffset(MsgClassName + unit->classIndex, win, 80, 0);
    if (savedUnit->classIndex != unit->classIndex)
      UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
    else
      UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

    UiText_FormatNumberToHalfwords(numberText, unit->hp);
    UiText_RenderWideStringInWindow(numberText + 7, win, 11, 1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 14, rising);
    }

    UiText_FormatNumberToHalfwords(numberText, unit->pp);
    UiText_RenderWideStringInWindow(numberText + 7, win, 11, 2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 22, rising);
    }

    UiText_FormatNumberToHalfwords(numberText, unit->atk);
    UiText_RenderWideStringInWindow(numberText + 7, win, 11, 3);
    if (unit->atk != oldAtk) {
      rising = 0;
      if (unit->atk > oldAtk)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 30, rising);
    }

    UiText_FormatNumberToHalfwords(numberText, unit->def);
    UiText_RenderWideStringInWindow(numberText + 7, win, 11, 4);
    if (unit->def != oldDef) {
      rising = 0;
      if (unit->def > oldDef)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 38, rising);
    }

    UiText_FormatNumberToHalfwords(numberText, unit->agi);
    UiText_RenderWideStringInWindow(numberText + 7, win, 11, 5);
    if (unit->agi != oldAgi) {
      rising = 0;
      if (unit->agi > oldAgi)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 46, rising);
    }

    UiText_FormatNumberToHalfwords(numberText, unit->luck);
    UiText_RenderWideStringInWindow(numberText + 8, win, 12, 6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 54, rising);
    }
  }

  // Previewing must not change the owner's Djinn, class or battle stats.
  Iwram_CopyWords(unit, savedUnit, sizeof(*savedUnit));
  MemFree(changedAbilities);
  MemFree(savedUnit);
  MemFree(numberText);
  return win;
}

#if TBS_PATCH_1
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) (MsgOwnerStatusExp - 10 + (row))
#endif

#define STATUS_CURSOR_GENERAL 0
#define STATUS_CURSOR_COMBAT_STATS 1
#define STATUS_CURSOR_CONDITIONS 2

struct StatusCursor {
  // XOR2 navigation can transiently use index3; keep all five positions.
  s8 groupPositions[5];
  s32 helpEntry;
  s32 drawnHelpEntry;
  s32 group;
  s32 groupSize;
  s32 column;
  s32 row;
};

// Show a party member's stats and condition help until B or the link session
// ends. The supplied list enables shoulder-button member switching. Measure
// stage deltas temporarily, then restore the unit from its snapshot.
s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner) {
  struct MenuSprite portraitSprite;
  struct MenuSprite cursorSprite;
  s32 statusSlots[11];
  s8 statusKinds[11];
  struct MenuSprite statusSprites[11];
  struct MenuSprite nameSprite;
  struct StatusCursor cursor;
  struct UiRenderWork *render;
  struct UiWindow *statusWindow;
  struct UiWindow *helpWindow;
  s32 ownerIndex;
  s32 cursorSlot;
  struct MenuPoint cursorPosition;
  s32 portraitSlot;
  s32 statusCount;
  s32 hasDjinn;
  s32 keys;
  u16 *helpText;
  s32 statusKind;
  s32 *statusSlot;
  s32 defenseDelta;
  s32 i;
  s32 j;
  s32 attackDelta;
  s32 redraw;
  struct BattleUnit *unit;
  struct BattleUnit *savedUnit;
  const struct OwnerStatusStatCursor *cursorStop;
  struct MenuSprite *statusSprite;
  s32 agilityDelta;
  s32 helpValue;
  s32 groupPosition;
  struct MenuCell *navigation;
  s32 iconX;

  render = UI_WORK;
  redraw = 1;
  ownerIndex = -1;
  portraitSlot = VramCache_AllocEntry(0x200);
  statusCount = 0;
  hasDjinn = Party_SumDjinnCountsFar(-1);

  navigation = ((struct MenuCell *)gWindowWork[42]);
  navigation->busy = 1;
  if (navigation->work != NULL) {
    UiWork_Finalize(navigation->work, 1);
    navigation->work = NULL;
  }
  AudioCommand_PlayFar(SOUND_MENU_CONFIRM);

  for (i = 4; i >= 0; i--) {
    cursor.groupPositions[i] = 0;
  }
  cursor.helpEntry = 0;
  cursor.drawnHelpEntry = 0;
  cursor.group = STATUS_CURSOR_GENERAL;

  cursorSlot = VramCache_AllocEntry(0x80);
  for (i = 0; i <= 10; i++) {
    statusSlots[i] = VramCache_AllocEntry(0x80);
    statusKinds[i] = -1;
  }

  if (listCount != 0) {
    for (i = 0; i <= 5; i++) {
      s32 kind = list[i];

      if (kind == 0xFF) {
        break;
      }
      if (kind != 0xFE) {
        if (kind == owner) {
          ownerIndex = i;
          break;
        }
      }
    }
  }

  cursorPosition.x = 0;
  cursorPosition.y = 0;
  // The stat grid and its explanation have separate lifetimes and redraws.
  statusWindow = UiWindow_Create(0, 0, 30, 20, 6);
  helpWindow = UiWindow_Create(0, 14, 30, 6, 10);
  UiWindow_MarkVisibleTileAttributes();

  for (;;) {
    keys = gKeysRepeat;

    nameSprite.oam.word.attr01 = 0x80000400;
    nameSprite.oam.word.attr23 = 0;
    nameSprite.oam.f.tile = Ui_LoadEntryForKind(owner, portraitSlot);
    nameSprite.oam.f.x = 8;
    nameSprite.oam.f.y = 24;
    nameSprite.oam.f.palette = 14;
    Runtime_PushSlotEntry((s32 *)&nameSprite, 240);

    switch (cursor.helpEntry) {
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
      cursor.group = STATUS_CURSOR_GENERAL;
      if (hasDjinn != 0) {
        cursor.groupSize = 9;
      } else {
        cursor.groupSize = 7;
      }
      break;
    case 10:
    case 11:
    case 12:
    case 13:
      cursor.group = STATUS_CURSOR_COMBAT_STATS;
      cursor.groupSize = 4;
      break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      cursor.group = STATUS_CURSOR_CONDITIONS;
      cursor.groupSize = statusCount;
      break;
    }

    Ui_SetRectHighlight(statusWindow->x, statusWindow->y, statusWindow->width,
                        statusWindow->height, 15);

    if (statusCount != 0) {
      if ((u32)cursor.group <= STATUS_CURSOR_COMBAT_STATS) {
        groupPosition = cursor.groupPositions[cursor.group];
        if ((keys & KEY_DOWN) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            groupPosition = 0;
            if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
              cursor.group = STATUS_CURSOR_CONDITIONS;
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if ((keys & KEY_UP) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          groupPosition--;
          if (groupPosition < 0) {
            groupPosition = cursor.groupSize - 1;
            if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
              cursor.group = STATUS_CURSOR_CONDITIONS;
              groupPosition = cursor.groupPositions[cursor.group];
            }
          }
        } else if ((keys & 0x31) != 0) {
          keys = 0;
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          // Toggle toward conditions; group3 is transient until reclassified.
          cursor.group ^= STATUS_CURSOR_CONDITIONS;
          if ((gKeysRepeat & KEY_A) != 0) {
            groupPosition = cursor.groupPositions[cursor.group];
          } else if ((gKeysRepeat & KEY_LEFT) != 0) {
            groupPosition = statusCount - 1;
          } else {
            groupPosition = 0;
          }
        }
        cursor.groupPositions[cursor.group] = (s8)groupPosition;
      } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
        groupPosition = cursor.groupPositions[cursor.group];
        if (groupPosition >= cursor.groupSize) {
          groupPosition = cursor.groupSize - 1;
        }
        if (groupPosition < 0) {
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
        } else if ((keys & KEY_RIGHT) != 0) {
          keys = 0;
          groupPosition++;
          if (groupPosition >= cursor.groupSize) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        } else if ((keys & KEY_LEFT) != 0) {
          keys = 0;
          groupPosition--;
          if (groupPosition < 0) {
            cursor.group = STATUS_CURSOR_GENERAL;
            groupPosition = cursor.groupPositions[0];
          }
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        } else if ((keys & 0xC1) != 0) {
          keys = 0;
          cursor.group = STATUS_CURSOR_GENERAL;
          groupPosition = cursor.groupPositions[0];
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        }
        cursor.groupPositions[cursor.group] = (s8)groupPosition;
      }
    }

    if (cursor.group == STATUS_CURSOR_GENERAL) {
      s32 k = cursor.groupPositions[cursor.group];

      if (hasDjinn == 0) {
        k += 9;
      }
      cursorStop = &gOwnerStatus_StatCursors[k];
      cursor.helpEntry = cursorStop->entry;
      cursor.column = cursorStop->column;
      cursor.row = cursorStop->row;
      Ui_SetRectHighlight(statusWindow->x + cursorStop->highlightColumn + 1,
                          statusWindow->y + cursorStop->highlightRow + 1,
                          cursorStop->highlightWidth, 1, 14);
    } else if (cursor.group == STATUS_CURSOR_COMBAT_STATS) {
      cursor.helpEntry =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_EquipmentCursors[cursor.groupPositions[cursor.group]]
              .row;
    } else if (cursor.group == STATUS_CURSOR_CONDITIONS) {
      cursor.helpEntry =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .entry;
      cursor.column =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .column;
      cursor.row =
          gOwnerStatus_ConditionCursors[cursor.groupPositions[cursor.group]]
              .row;
    }

    if (cursor.drawnHelpEntry != cursor.helpEntry) {
      cursor.drawnHelpEntry = cursor.helpEntry;
      redraw = 2;
    }
    cursorPosition.x = cursor.column * 8;
    cursorPosition.y = cursor.row * 8;

    {
      struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

      portraitSprite.oam.word.attr01 = 0xC0002400;
      portraitSprite.oam.word.attr23 = 0;
#if TBS_PATCH_4
      /* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store
       * before the move into sl and the 0xFC00 load before the face tile load,
       * which may alias that store; native moves into sl between the two stores
       * and loads the tile first (4 instructions) */
      asm("");
#endif
      portraitSprite.oam.f.tile = face->oam.f.tile;
      portraitSprite.oam.f.x = 0xAC;
      portraitSprite.oam.f.y = 56;
      Runtime_PushSlotEntry((s32 *)&portraitSprite, 240);
    }

    cursorSprite.oam.word.attr01 = 0x40000400;
    cursorSprite.oam.word.attr23 = 0;
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
    cursorSprite.oam.f.tile =
        VramCache_Reload(cursorSlot, (s32)gResource_FixedBlockBTiles);
    cursorSprite.oam.f.x =
        (cursorPosition.x + statusWindow->x * 8 - ((gFrameCount & 4) >> 2)) +
        16;
    cursorSprite.oam.f.y = (u8)((cursorPosition.y + statusWindow->y * 8 -
                                 ((gFrameCount & 4) >> 2)) +
                                16);
    cursorSprite.oam.f.affineIndex = 8;
    Runtime_PushSlotEntry((s32 *)&cursorSprite, 241);

    if (redraw != 0) {
      unit = UnitGetFar(owner);
#if TBS_PATCH_1
      UiWindow_MarkVisibleTileAttributes();
#endif
      Ui_FillVramBlockPattern();

      if ((redraw & 1) != 0) {
        RenderOutput_RedrawSavedRect(statusWindow);
        UiWindow_MapTextCanvasTiles(helpWindow->x, helpWindow->y,
                                    helpWindow->width, helpWindow->height, 0);
        UiWindow_DrawDividerLine(statusWindow, 0, 14, 29, 14);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case 2:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NivString, statusWindow,
                                    51, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_NvString, statusWindow,
                                    56, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        default:
          UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString,
                                    statusWindow, 56, 0);
          UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)unit->name, statusWindow, 0, 0);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_LevelString, statusWindow,
                                  OWNER_STATUS_LEVEL_X, 0);
        UiText_DrawNumberInWindow(unit->level, 2, statusWindow, 72, 0);
#endif
        UiText_DrawCharacterAtOffset(MsgOwnerStatusExp, statusWindow, 0, 8);
        UiText_DrawNumberInWindow(unit->experience, 8, statusWindow, 40, 8);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
        switch (System_GetLanguage()) {
        case 1:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString, statusWindow,
                                    40, 24);
          break;
        case 2:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, statusWindow,
                                    40, 24);
          break;
        case 3:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString, statusWindow,
                                    40, 24);
          break;
        case 4:
          UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, statusWindow,
                                    40, 24);
          break;
        }
#else
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString, statusWindow, 40,
                                  24);
#endif
        UiText_DrawNumberInWindow(unit->hp, 4, statusWindow, 56, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, statusWindow,
                                  88, 24);
        UiText_DrawNumberInWindow(unit->maxHp, 4, statusWindow, 96, 24);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString, statusWindow, 40,
                                  32);
        UiText_DrawNumberInWindow(unit->pp, 4, statusWindow, 56, 32);
        UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString, statusWindow,
                                  88, 32);
        UiText_DrawNumberInWindow(unit->maxPp, 4, statusWindow, 96, 32);

        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(0), statusWindow, 136,
                                     16);
        UiText_DrawNumberInWindow(unit->atk, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 16);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(1), statusWindow, 136,
                                     24);
        UiText_DrawNumberInWindow(unit->def, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 24);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(2), statusWindow, 136,
                                     32);
        UiText_DrawNumberInWindow(unit->agi, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 32);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(3), statusWindow, 136,
                                     40);
        UiText_DrawNumberInWindow(unit->luck, 3, statusWindow,
                                  OWNER_STATUS_VALUE_X, 40);

        UiText_DrawCharacterAtOffset(MsgClassName + unit->classIndex,
                                     statusWindow, 0, 48);
        if (hasDjinn != 0) {
          UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(9), statusWindow, 0,
                                       72);
        }
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(5), statusWindow, 0,
                                     80);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(6), statusWindow, 0,
                                     88);
        UiText_DrawCharacterAtOffset(OWNER_STATUS_LABEL(7), statusWindow, 0,
                                     96);

        for (j = 0; j <= 3; j++) {
          s32 pal;

          pal = 1;
          if (hasDjinn != 0) {
            pal = 0;
          }
          UiWindow_SetTilemapEntry(statusWindow, 0x5001 + j, 7 + j * 4, pal + 8,
                                   0);
          if (hasDjinn != 0) {
            UiText_DrawNumberInWindow(unit->djinnActiveCounts[j], 1,
                                      statusWindow, 40 + j * 32, 72);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      statusWindow, 48 + j * 32, 72);
            UiText_DrawNumberInWindow(unit->djinnOwnedCounts[j], 1,
                                      statusWindow, 48 + j * 32 + 8, 72);
          }
          UiText_DrawNumberInWindow(Unit_GetElementLevelFar(owner, j), 2,
                                    statusWindow, 48 + j * 32, 80);
          UiText_DrawNumberInWindow(unit->element[j].power, 3, statusWindow,
                                    40 + j * 32, 88);
          UiText_DrawNumberInWindow(unit->element[j].resistance, 3,
                                    statusWindow, 40 + j * 32, 96);
        }

        i = 0;
        if (unit->hp == 0) {
          statusKinds[0] = 16;
          i = 1;
        }
        do {
          if (unit->restraint != 0) {
            statusKinds[i++] = 15;
          }
          if (i > 7) {
            break;
          }
          if (unit->grimReaper != 0) {
            statusKinds[i++] = 8;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 1) {
            statusKinds[i++] = 1;
          }
          if (i > 7) {
            break;
          }
          if (unit->poison == 2) {
            statusKinds[i++] = 2;
          }
          if (i > 7) {
            break;
          }
          if (unit->psySeal != 0) {
            statusKinds[i++] = 4;
          }
          if (i > 7) {
            break;
          }
          if (unit->stun != 0) {
            statusKinds[i++] = 3;
          }
          if (i > 7) {
            break;
          }
          if (unit->sleep != 0) {
            statusKinds[i++] = 5;
          }
          if (i > 7) {
            break;
          }
          if (unit->haunt != 0) {
            statusKinds[i++] = 7;
          }
          if (i > 7) {
            break;
          }
          if (unit->delusion != 0) {
            statusKinds[i++] = 6;
          }
          if (i > 7) {
            break;
          }
          if (unit->attackTurns != 0) {
            if (unit->attackStage > 0) {
              statusKinds[i++] = 9;
            }
            if (unit->attackStage < 0) {
              statusKinds[i++] = 10;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->defenseTurns != 0) {
            if (unit->defenseStage > 0) {
              statusKinds[i++] = 11;
            }
            if (unit->defenseStage < 0) {
              statusKinds[i++] = 12;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->resistanceTurns != 0) {
            if (unit->resistanceStage > 0) {
              statusKinds[i++] = 13;
            }
            if (unit->resistanceStage < 0) {
              statusKinds[i++] = 14;
            }
          }
          if (i > 7) {
            break;
          }
          if (unit->agilityStage > 0) {
            statusKinds[i++] = 17;
          }
          if (unit->agilityStage < 0) {
            statusKinds[i++] = 18;
          }
        } while (0);

        for (j = 0; j < i; j++) {
          Resource_LoadTableEntryToBuffer(statusKinds[j], statusSlots[j]);
        }
        if (i == 0) {
          statusKinds[0] = 0;
          i = 1;
        }
        if (i <= 10) {
          for (j = i; j < 11; j++) {
            statusKinds[j] = -1;
          }
        }
        statusCount = i;
        if (statusKinds[0] == 0) {
          UiText_DrawCharacterAtOffset(unit->hp != 0 ? MsgOwnerStatusNormal
                                                     : MsgOwnerStatusDowned,
                                       statusWindow, 112, 0);
        }
      }

      helpText = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*helpText));
      if ((u32)cursor.helpEntry > 13) {
        statusKind = statusKinds[cursor.helpEntry - 14];
        if (statusKind == 0 && unit->hp == 0) {
          statusKind = 16;
        }

        // Remove stages on a snapshot to measure their displayed contribution.
        savedUnit = (struct BattleUnit *)MemAlloc(sizeof(*unit));
        helpValue = 0;
        Iwram_CopyWords(savedUnit, unit, sizeof(*unit));

        attackDelta = unit->atk;
        defenseDelta = unit->def;
        agilityDelta = unit->agi;
        unit->attackStage = 0;
        unit->defenseStage = 0;
        unit->agilityStage = 0;
        UnitCalcFar(owner);
        attackDelta -= unit->atk;
        defenseDelta -= unit->def;
        agilityDelta -= unit->agi;
        Iwram_CopyWords(unit, savedUnit, sizeof(*unit));
        MemFree(savedUnit);

        switch (statusKind) {
        case 8:
          helpValue = unit->grimReaper;
          break;
        case 9:
          helpValue = attackDelta;
          break;
        case 10:
          helpValue = -attackDelta;
          break;
        case 11:
          helpValue = defenseDelta;
          break;
        case 12:
          helpValue = -defenseDelta;
          break;
        case 17:
          helpValue = agilityDelta;
          break;
        case 18:
          helpValue = -agilityDelta;
          break;
        case 13:
        case 14:
          helpValue = unit->resistanceStage * 20;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
          if (helpValue < 0) {
            helpValue = -helpValue;
          }
#endif
          break;
        default:
          break;
        }
        UiWork_PushValueSlot(helpValue, 5);
        UiText_CopyMessageString(statusKind + MsgOwnerStatusConditionHelp,
                                 helpText, OWNER_STATUS_TEXT_MAX);
      } else if (cursor.helpEntry == 2 && unit->level <= 98) {
        UiWork_PushValueSlot(
            Unit_GetExperienceForLevelFar(owner, unit->level + 1) -
                unit->experience,
            5);
        UiText_CopyMessageString(MsgOwnerStatusNextLevel, helpText,
                                 OWNER_STATUS_TEXT_MAX);
      } else {
        UiText_CopyMessageString(cursor.helpEntry + MsgOwnerStatusEntryHelp,
                                 helpText, OWNER_STATUS_TEXT_MAX);
      }

      UiText_RenderWideStringAtOffset(helpText, (struct UiWindow *)helpWindow,
                                      0, 4);
      MemFree(helpText);
      redraw = 0;
      render->dirty = 1;
    }

    statusSprite = statusSprites;
    statusSlot = statusSlots;
    {
      struct VramBlockCacheEntry *cache;

      for (i = 0, cache = gVramBlockCache, iconX = 112; i <= 10; i++) {
        statusSprite->oam.word.attr01 = 0x40000400;
        statusSprite->oam.word.attr23 = 0;
        statusSprite->oam.f.tile = cache[*statusSlot++].offset >> 5;
        statusSprite->oam.f.x = iconX;
        statusSprite->oam.f.y = (u8)(statusWindow->y * 8 + 8);
        if (statusKinds[i] > 0) {
          Runtime_PushSlotEntry((s32 *)statusSprite, 240);
        }
        iconX += 15;
        statusSprite++;
      }
    }

    if (gLinkCountdownWork->active == 0) {
      break;
    }
    if ((gKeysPressed & KEY_B) != 0) {
      break;
    }

    if (listCount != 0) {
      if ((keys & KEY_R) != 0) {
        ownerIndex++;
        if (ownerIndex >= listCount) {
          ownerIndex = 0;
        }
        owner = list[ownerIndex];
        redraw = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      } else if ((keys & KEY_L) != 0) {
        ownerIndex--;
        if (ownerIndex < 0) {
          ownerIndex = listCount - 1;
        }
        owner = list[ownerIndex];
        redraw = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
    }
    WaitFrames(1);
  }

  {
    s32 *statusSlotCursor = statusSlots;

    for (i = 10; i >= 0; i--) {
      Resource_ResetEntry(*statusSlotCursor++);
    }
  }
  Resource_ResetEntry(portraitSlot);
  Resource_ResetEntry(cursorSlot);
  WaitFrames(1);
#if TBS_PATCH_1
  UiWindow_MarkVisibleTileAttributes();
#endif
  UiWork_Finalize(statusWindow, 1);
  UiWork_Finalize(helpWindow, 1);
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

#if TBS_PATCH_1
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define TARGET_MYSTERY_NAME_COPY UiText_DecodeMessage
#define TARGET_MYSTERY_NAME_CAPACITY 10
#else
#define TARGET_MYSTERY_NAME_COPY UiText_CopyMessageString
#define TARGET_MYSTERY_NAME_CAPACITY 14
#endif

#if LANGUAGE == LANGUAGE_GERMAN
#define TRANSFER_MODE_DRAW_HELP UiText_DrawCharacterAtOffset
#define TRANSFER_PASSWORD_HELP_X 32
#define TRANSFER_PASSWORD_HELP_FIRST_Y 8
#define TRANSFER_PASSWORD_HELP_LAST_Y 24
#define TRANSFER_CABLE_HELP_FIRST_Y 0
#define TRANSFER_CABLE_HELP_LAST_Y 32
#else
#define TRANSFER_MODE_DRAW_HELP UiText_DrawResource
#define TRANSFER_PASSWORD_HELP_X PASSWORD_HELP_X
#define TRANSFER_PASSWORD_HELP_FIRST_Y 4
#define TRANSFER_PASSWORD_HELP_LAST_Y 16
#define TRANSFER_CABLE_HELP_FIRST_Y 4
#define TRANSFER_CABLE_HELP_LAST_Y 28
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

// A selected Djinni packs its element/index, standby bit and recovery turns.
#define DJINN_ENTRY_STANDBY 0x10000
#define DJINN_ENTRY_RECOVERY_MASK 0x3e0000
#define DJINN_ENTRY_RECOVERY_SHIFT 17
#define DJINN_ENTRY_END 0x80000000

// Select a Djinni for the current battle command. Entries encode its element,
// index, standby state and recovery turns; -1 means cancel. R opens the
// stat/Psynergy preview, and tutorial input follows the normal input path.
s32 Battle_SelectAbility(s32 owner, s32 unused) {
  struct CursorSprite cursor;
  u16 messageText[64];
  struct UiRenderWork *render;
  struct UiWindow *previewWindow;
  s32 drawnRow;
  s32 cursorSlot;
  u32 *djinnEntries;
  s32 djinnCount;
  s32 drawnFirstEntry;
  struct UiWindow *messageWindow;
  s32 standbyElements;
  s32 preferredRow;
  s32 previewPage;
  s32 redraw;
  s32 alreadySelected;
  s32 previewPages;
  struct MenuCell *navigation;
  struct BattleUnit *unit;
  struct DjinnRecoveryList *recovery;
  struct UiWindow *listWindow;
  struct UiWindow *iconWindow;
  struct PartyStatusWindow *partyStatus;
  u32 *entryCursor;
  u32 selectedDjinn;
  u32 blinkPhase;
  s32 firstEntry;
  s32 selectedRow;
  u32 encodedDjinn;
  s32 listPages;
  s32 palette;
  struct MenuPoint cursorPos;
  s32 pressed;
  s32 repeated;
  s32 held;
  s32 element;
  s32 djinnIndex;
  s32 i;
  s32 j;
  struct CursorSprite *cursorSprite;

  render = UI_WORK;
  previewWindow = 0;
  drawnRow = -1;
  cursorSlot = VramCache_AllocEntry(0x80);
  djinnEntries = Runtime_BumpAllocateAlternatePool(0xa8 * 2);
  djinnCount = 0;
  drawnFirstEntry = -1;
  encodedDjinn = 0;
  standbyElements = 0;
  previewPage = 0;
  previewPages = 0;
  redraw = 0;
  alreadySelected = 0;
  messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
  UiWork_SetAltFlagAndClearTable(1);
  listWindow = UiWindow_Create(21, 9, 9, 11, 6);
  MenuCell_GetCursor(firstEntry, selectedRow, preferredRow);
  unit = UnitGetFar(owner);

  // Pack Djinn state and recovery turns before paging the selection list.
  for (element = 0; element <= 3; element++) {
    for (djinnIndex = 0,
        entryCursor = (u32 *)((djinnCount << 2) + (u32)djinnEntries);
         djinnIndex <= 19; djinnIndex++) {
      if (unit->djinnActive[element] & (u32)(1 << djinnIndex)) {
        *entryCursor = (u32)((element << 8) | djinnIndex);
        entryCursor++;
        djinnCount++;
      } else if (unit->djinnAvailable[element] & (u32)(1 << djinnIndex)) {
        u32 v;
        s32 k;
        recovery = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                         (u32)owner > 7 ? 1 : 0))
                        ->list;
        for (k = 0; k < recovery->count; k++) {
          if (recovery->entries[k].unitId == owner &&
              recovery->entries[k].element == element &&
              recovery->entries[k].index == djinnIndex) {
            break;
          }
        }
        v = (u32)((element << 8) | djinnIndex | DJINN_ENTRY_STANDBY);
        *entryCursor = v;
        if (recovery->entries[k].turns > 0) {
          v |= (u32)(recovery->entries[k].turns << DJINN_ENTRY_RECOVERY_SHIFT);
          *entryCursor = v;
        }
        djinnCount++;
        entryCursor++;
      }
    }
  }
  djinnEntries[djinnCount] = DJINN_ENTRY_END;
  render->dirty = 1;

  /* FAKEMATCH: forced store: this dead store of curSpr before the loop keeps
   * the native allocation; without it the function differs 394 (1193
   * instructions for 1201) in tbs-en and tbs-ja (measured 2026-10-14). */
  cursorSprite = &cursor;
  for (;;) {
    // Rebuild descriptions only when the selection or preview page changes.
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
        redraw != 0) {
      selectedDjinn = djinnEntries[firstEntry + selectedRow];
      standbyElements = 0;
      render->menuBusy = 1;
      Ui_SetRectHighlight(listWindow->x + 1, listWindow->y + drawnRow * 2 + 1,
                          listWindow->width - 2, 1, 15);
      if (previewPage != 0) {
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        Ui_FillVramBlockPattern();
      }
      redraw = 0;
      if (djinnCount != 0) {
        s32 k;
        u32 selected;
        UiWork_ClearValueNameTables();
        alreadySelected = 0;
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
        if (selected == selectedDjinn) {
          alreadySelected = 1;
        } else {
          for (;;) {
            /* FAKEMATCH: needed to match: isUsed is zero on every search-loop
             * continuation. Its step dependency prevents loop.c from
             * strength-reducing k into a pointer; CSE later removes the flag
             * term. Plain k differs 35 instructions in EN; this form restores
             * the native reload and indexing (differ 0). */
            k += 1 - alreadySelected;
            if (k > 7)
              break;
            if ((u32)COUNTDOWN->selected[k] == selectedDjinn) {
              alreadySelected = 1;
              break;
            }
          }
        }
        if (alreadySelected != 0) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleDjinnTwice, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
          if (previewWindow != 0) {
            UiWork_Finalize(previewWindow, 1);
            previewWindow = 0;
            previewPage = 0;
          }
        } else if (selectedDjinn & DJINN_ENTRY_STANDBY) {
          if (selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiWork_PushValueSlot(
                (s32)((selectedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                5);
            BATTLE_MENU_MESSAGE_COPY((s32)(((selectedDjinn & 0xf00) >> 8) * 20 +
                                           (selectedDjinn & 0xff)) +
                                         MsgDjinnEffect,
                                     messageText, BATTLE_MENU_MESSAGE_CAPACITY);
            if (previewWindow != 0) {
              UiWork_Finalize(previewWindow, 1);
              previewWindow = 0;
              previewPage = 0;
            }
          } else {
            previewWindow = (struct UiWindow *)DjinnMenu_ShowChangePreview(
                previewWindow, owner, (s32)selectedDjinn, previewPage,
                &previewPages);
            UiText_CopyMessageString(MsgBattleSummonStandby, messageText,
                                     BATTLE_MENU_MESSAGE_CAPACITY);
            standbyElements = 1 << ((selectedDjinn & 0xf00) >> 8);
          }
        } else {
          previewWindow = (struct UiWindow *)DjinnMenu_ShowChangePreview(
              previewWindow, owner, (s32)selectedDjinn, previewPage,
              &previewPages);
          BATTLE_MENU_MESSAGE_COPY((s32)(((selectedDjinn & 0xf00) >> 8) * 20 +
                                         (selectedDjinn & 0xff)) +
                                       MsgDjinnEffect,
                                   messageText, BATTLE_MENU_MESSAGE_CAPACITY);
        }
      } else {
        BATTLE_MENU_MESSAGE_COPY(MsgBattleNoDjinn, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      render->menuBusy = 0;
      if (previewPage == 0) {
        render->menuBusy = 1;
        UiWork_Finalize(messageWindow, 1);
        messageWindow = UiWindow_Create(0, 4, 30, 4, 42);
        render->menuBusy = (u8)previewPage;
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(listWindow);
        j = 0;
        while ((encodedDjinn = djinnEntries[firstEntry + j]) !=
               DJINN_ENTRY_END) {
          UiWindow_SetTilemapEntry(listWindow,
                                   (s32)((encodedDjinn & 0xf00) >> 8) + 0x5001,
                                   0, j * 2, 0);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiWork_SetParamNibble(4);
          } else if (encodedDjinn & DJINN_ENTRY_STANDBY) {
            UiWork_SetParamNibble(2);
          }
          UiText_DrawCharacterAtOffset(
              (s32)(((encodedDjinn & 0xf00) >> 8) * 20 +
                    (encodedDjinn & 0xff)) +
                  MsgDjinnName,
              listWindow, 8, j * 16);
          if (encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) {
            UiText_DrawNumberAtOffset(
                (s32)((encodedDjinn & DJINN_ENTRY_RECOVERY_MASK) >>
                      DJINN_ENTRY_RECOVERY_SHIFT),
                TURNS_DIGITS, listWindow, TURNS_X, j * 16);
          }
          UiWork_SetParamNibble(15);
          j++;
          if (j >= BATTLE_LIST_ROWS) {
            break;
          }
        }
        drawnFirstEntry = firstEntry;
      }
      if (djinnCount > BATTLE_LIST_ROWS) {
        for (j = 0; j < (listPages = ((djinnCount + 4) / BATTLE_LIST_ROWS));
             j++) {
          s32 tile = j + 0xf301;
          if (j == (firstEntry / BATTLE_LIST_ROWS)) {
            tile = j + 0xf30b;
          }
          UiWindow_SetTilemapEntry(
              listWindow, tile, listWindow->width - listPages + j - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(listWindow->x + 1,
                          listWindow->y + selectedRow * 2 + 1,
                          listWindow->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (djinnCount > BATTLE_LIST_ROWS) {
      for (j = 0; j < (djinnCount + 4) / BATTLE_LIST_ROWS; j++) {
        s32 tile = j + 0xf301;
        if ((gKeysHeld & KEY_R) || (gFrameCount & 15) <= 11) {
          if (j == (firstEntry / BATTLE_LIST_ROWS)) {
            tile = j + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            listWindow, tile,
            listWindow->width - ((djinnCount + 4) / BATTLE_LIST_ROWS) + j - 2,
            -1, 0);
      }
      if ((gKeysHeld & KEY_R) == 0) {
        UiWindow_SetTilemapEntry(
            listWindow, 0xf334,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWindow, 0xf335, listWindow->width - 2, -1,
                                 0);
      } else {
        UiWindow_SetTilemapEntry(
            listWindow, 0xf011,
            listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3, -1, 0);
        UiWindow_SetTilemapEntry(listWindow, 0xf012, listWindow->width - 2, -1,
                                 0);
      }
      UiRender_MarkRow(render, listWindow->y - 1);
    }

    // Tutorial input replaces live keys but uses the same navigation below.
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    held = gKeysHeld;
    navigation = gLinkCountdownWork;
    if (navigation->autoEnabled != 0) {
      repeated = 0;
      pressed = 0;
      held = 0;
      if (navigation->autoDelay == 0) {
        navigation->autoDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        navigation->autoDelay--;
      }
    }
    navigation = gLinkCountdownWork;
    if (navigation->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      encodedDjinn = -1;
      break;
    }
    if (pressed & KEY_A) {
      u32 selection;
      u32 turns;
      if (djinnCount != 0) {
        selection = djinnEntries[firstEntry + selectedRow];
        turns = selection & DJINN_ENTRY_RECOVERY_MASK;
        if (turns == 0) {
          if (alreadySelected == 0) {
            navigation->page = firstEntry;
            navigation->row = selectedRow;
            navigation->preferredRow = preferredRow;
            encodedDjinn = selection;
            break;
          }
        } else if (alreadySelected == 0) {
          Ui_FillVramBlockPattern();
          UiWork_ClearValueNameTables();
          UiWork_PushValueSlot((s32)((turns & DJINN_ENTRY_RECOVERY_MASK) >>
                                     DJINN_ENTRY_RECOVERY_SHIFT),
                               5);
          UiText_CopyMessageString(MsgBattleDjinnRecovering, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
          UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
        }
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      } else {
        encodedDjinn = -1;
        break;
      }
    }

    if (djinnCount != 0) {
      if (repeated & KEY_DOWN) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == djinnCount) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if (repeated & KEY_UP) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry ==
              ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS) {
            selectedRow = djinnCount - firstEntry - 1;
          } else {
            selectedRow = 4;
          }
        }
        preferredRow = selectedRow;
      } else if (held & KEY_R) {
        if (previewWindow != 0) {
          for (i = 0; i < previewPages; i++) {
            s32 tile = i + 0xf301;
            if ((gFrameCount & 15) <= 11 && i == previewPage - 1) {
              tile = previewPage - 1 + 0xf30b;
            }
            UiWindow_SetTilemapEntry(
                previewWindow, tile,
                previewWindow->width - previewPages + i - 2, -1, 0);
          }
          UiWindow_SetTilemapEntry(previewWindow, 0xf334,
                                   previewWindow->width - previewPages - 3, -1,
                                   0);
          UiWindow_SetTilemapEntry(previewWindow, 0xf335,
                                   previewWindow->width - 2, -1, 0);
          UiRender_MarkRow(render, previewWindow->y);
        }
        if (previewPage == 0 && previewPages != 0) {
          if (previewWindow != 0) {
            RenderOutput_ClearList(previewWindow);
          }
          previewPage = 1;
          redraw = 1;
        } else {
          if (previewPage > previewPages) {
            previewPage = previewPages;
          }
          if (previewPage != 0) {
            if (repeated & KEY_RIGHT) {
              AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
              previewPage++;
              if (previewPage > previewPages) {
                previewPage = 1;
              }
              redraw = 1;
            } else if (repeated & KEY_LEFT) {
              AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
              previewPage--;
              if (previewPage <= 0) {
                previewPage = previewPages;
              }
              redraw = 1;
            }
          }
        }
      } else if (previewPage != 0) {
        if (previewWindow != 0) {
          RenderOutput_ClearList(previewWindow);
        }
        previewPage = 0;
        redraw = 1;
      } else if (repeated & KEY_RIGHT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + BATTLE_LIST_ROWS >= djinnCount) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry ==
              ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      } else if (repeated & KEY_LEFT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = ((djinnCount - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = djinnCount - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
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
        cx = listWindow->x * 8;
      } while (0);
      /* FAKEMATCH: instruction order: row read in a low register here is
       * reloaded before the subtract, and row * 2 inherits that reload, so mov
       * r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority,
       * and the reload otherwise has the later luid) */
      asm("" : : "l"(selectedRow));
      cursorPos.x = cx - 2;
    }
    cursorPos.y = (selectedRow * 2 + listWindow->y) * 8 + 20;
    /* FAKEMATCH: forced re-point: curSpr already holds &slot; storing it again
     * here keeps the native reload; without it the function differs 36 in
     * tbs-en and tbs-ja (measured 2026-10-14). */
    cursorSprite = &cursor;
    ((u32 *)cursorSprite)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cursorSprite)[2] = 0;
    cursorSprite->tile =
        VramCache_Reload(cursorSlot, (s32)gResource_FixedBlockBTiles);
    cursorSprite->x = cursorPos.x + ((gFrameCount & 4) >> 1) - 6;
    cursorSprite->y = cursorPos.y - ((gFrameCount & 4) >> 2) - 8;
    if (djinnCount != 0) {
      Runtime_PushSlotEntry(cursorSprite, 242);
    }
    partyStatus = CURSOR_OWNER;
    blinkPhase = gFrameCount & 4;
    iconWindow = partyStatus->window;
    if (partyStatus->flags & 2) {
      for (i = 0; i <= 3; i++) {
        palette = 15 - (blinkPhase != 0);
        if (((1 << i) & standbyElements) == 0) {
          palette = 15;
        }
        Ui_SetRectHighlight(iconWindow->x + gDjinnMenu_ElementXOffsets[i] + 1,
                            iconWindow->y + gDjinnMenu_ElementYOffsets[i] + 1,
                            2, 2, palette);
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
  // Restore the party strip before releasing the Djinn menu's resources.
  Resource_ResetEntry(cursorSlot);
  UiWork_Finalize(messageWindow, 1);
  UiWork_Finalize(previewWindow, 1);
  UiWork_Finalize(listWindow, 1);
  UiWindow_MarkVisibleTileAttributes();
  UiWork_SetAltFlagAndClearTable(0);
  MemFree(djinnEntries);
  WaitFrames(1);
  return (s32)encodedDjinn;
}

//--------------------------------------------------------------------------
// classify_use_ability.c
// Summon selection menu and checks for item usability in battle
//--------------------------------------------------------------------------

#define BATTLE_OK 0x80 // flags: usable in battle

#if !GAME_TLA

#if TBS_PATCH_1
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
  u8 learnedSummons[33]; // Learned summons
  u8 orderedSummons[33]; // Order with usable summons first
  u16 text[64];
  s32 iconSlots[4];
  u8 visibleSummons[4];
  s32 glyph;
  s32 count;
  struct UiWindow *window;
  struct UiRenderWork *render;
  s32 drawnRow, drawnFirstEntry, selectedRow, firstEntry;
  s32 cursorHandle;
  struct UiWindow *description, *counts;
  s32 mask, preferredRow;
  struct MenuPoint cursorPosition;
  const struct SummonDefinition *summon;
  struct MenuCell *navigation;
  struct MenuSprite *sprite;
  struct CursorSprite *cur;
  s32 orderedCount;
  s32 id, tile, affordable;
  s32 pressed, repeated, result;
  s32 index;

  render = UI_WORK;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = (s16)VramCache_AllocEntry(128); // One 16x16 sprite
  description = UiWindow_Create(0, 4, 30, 4, 42);
  counts = UiWindow_Create(20, 8, 10, 3, 6);
  mask = 0;
  MenuCell_GetCursor(firstEntry, selectedRow, preferredRow);
  window =
      UiWindow_Create(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH, 9, 6);

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
    iconSlots[index] = slot;
    sprites[index].oam.f.tile = VramCache_Reload(slot, -1);
  }

  // Move usable summons to the front in reverse traversal; keep others in
  // order.
  count = Summon_ListLearnedFar(learnedSummons);
  /* FAKEMATCH: instruction order only (block placement and schedule):
     with the first scan's exit inside this one-pass block, loop.c's
     find_and_verify_loops finds no barrier at depth 0 and leaves the
     main loop's select block in place, as the native has it (without
     the block it moves after the scan: differ 224, 6 branches fewer);
     inside it, count and used are stored before count - 1 is built
     (differ 2 with used = 0 before the block). */
  do {
    s32 i;

    orderedCount = 0;
    for (i = count - 1; i >= 0; i--) {
      const struct SummonDefinition *definition;
      s32 n, k;

      k = learnedSummons[i];
      definition = SummonDefinition_Get(k);
      for (n = 0; n <= 3; n++) {
        if (definition->djinnRequired[n] > standby[n])
          break;
      }
      if (n == 4) {
        orderedSummons[orderedCount++] = k;
        learnedSummons[i] = SUMMON_END;
      }
    }
  } while (0);
  {
    s32 i, k;

    for (i = 0; i < count; i++) {
      k = learnedSummons[i];
      if (k != SUMMON_END)
        orderedSummons[orderedCount++] = k;
    }
  }
  orderedSummons[orderedCount] = SUMMON_END;

  for (;;) {
    cur = &cursor;

    //------------------------------------------------------------------
    // Redraw when the page or row changes.
    //------------------------------------------------------------------
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();

      // Description and required Djinni elements.
      {
        const struct SummonDefinition *definition;
        s32 element;

        definition =
            SummonDefinition_Get(orderedSummons[firstEntry + selectedRow]);
        BATTLE_MENU_MESSAGE_COPY(definition->nameMessageId +
                                     MsgAbilityDescription,
                                 text, BATTLE_MENU_MESSAGE_CAPACITY);
        UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0,
                                        4);
        drawnRow = selectedRow;
        mask = 0;
        for (element = 0; element <= 3; element++) {
          if (definition->djinnRequired[element] != 0)
            mask |= 1 << element;
        }
      }

      // Standby counts and the four summons on this page.
      if (firstEntry != drawnFirstEntry) {
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
          if ((id = orderedSummons[firstEntry + index]) != SUMMON_END) {
          next:
            summon = SummonDefinition_Get(id);
            col = 0;
            for (element = 0; element <= 3; element++) {
              if (summon->djinnRequired[element] > standby[element])
                break;
            }
            affordable = element == 4;
            Ability_LoadGlyph(summon->nameMessageId & 0x3fff, 0,
                              &iconSlots[index], &glyph, 1);
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
            visibleSummons[index++] = 1;
            if (index <= 3 &&
                (id = orderedSummons[firstEntry + index]) != SUMMON_END)
              goto next;
          }
          while (index <= 3)
            visibleSummons[index++] = 0;
        }
        drawnFirstEntry = firstEntry;
      }

      // Page numbers
      if (count > 4) {
        for (index = 0; index < (count + 3) / 4; index++) {
          tile = index + LIST_TILE_PAGE_NUMBER;
          if (index == firstEntry / 4)
            tile = index + LIST_TILE_PAGE_NUMBER_LIT;
          UiWindow_SetTilemapEntry(
              window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
        }
      }
      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    //------------------------------------------------------------------
    // Per-frame display
    //------------------------------------------------------------------
    for (index = 0; index <= 3; index++) {
      if (visibleSummons[index] != 0)
        Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
    }

    // The cursor wobbles every four frames.
    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
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
        if ((gFrameCount & 15) <= 11 && index == firstEntry / 4)
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
    MenuCell_SetCursor(firstEntry, selectedRow, preferredRow);
    navigation = gLinkCountdownWork;
    pressed = gKeysPressed;
    repeated = gKeysRepeat;

    // Automatic input confirms every 120 frames.
    if (navigation->autoEnabled != 0) {
      repeated = 0;
      pressed = 0;
      if (navigation->autoDelay == 0) {
        navigation->autoDelay = 120;
        repeated = KEY_A;
        pressed = KEY_A;
      } else {
        navigation->autoDelay--;
      }
    }
    if (pressed & KEY_A) {
      result = orderedSummons[firstEntry + selectedRow];
      break;
    }
    if (gLinkCountdownWork->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeated & KEY_DOWN) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selectedRow++;
      if (selectedRow == 4 || firstEntry + selectedRow == count)
        selectedRow = 0;
      preferredRow = selectedRow;
    } else if (repeated & KEY_UP) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selectedRow--;
      if (selectedRow < 0) {
        if (firstEntry == ((count - 1) / 4) * 4)
          selectedRow = count - firstEntry - 1; // Last page
        else
          selectedRow = 3;
      }
      preferredRow = selectedRow;
    } else if (repeated & KEY_RIGHT) {
      // Next page, wrapping from last to first.
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry + 4 >= count) {
        if (firstEntry != 0) {
          firstEntry = 0;
          selectedRow = preferredRow;
        }
      } else {
        firstEntry += 4;
        selectedRow = preferredRow;
        if (firstEntry == ((count - 1) / 4) * 4) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    } else if (repeated & KEY_LEFT) {
      // Previous page, wrapping from first to last.
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      if (firstEntry != 0) {
        firstEntry -= 4;
        selectedRow = preferredRow;
      } else {
        firstEntry = ((count - 1) / 4) * 4;
        selectedRow = preferredRow;
        if (firstEntry != 0) {
          selectedRow = count - firstEntry - 1;
          if (selectedRow > preferredRow)
            selectedRow = preferredRow;
        }
      }
    }
    WaitFrames(1);
  }

  // Cleanup
  WaitFrames(1);
  for (index = 0; index < 4; index++)
    Resource_ResetEntry(iconSlots[index]);
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
  s32 itemUseStatus;
  struct ItemDefinition *definition;

  if (item == 0) {
    return 1;
  }
  definition = Item_GetDirectFar(item);
  itemUseStatus = 1;
  if (definition->useType == 3) // Equipment that grants Psynergy
  {
    return itemUseStatus;
  }
  if (definition->actionId == 0) {
    return itemUseStatus;
  }
  if ((definition->type != 0) && (Item_CanOwnerEquip(unit, item) == 0)) {
    return itemUseStatus;
  }
  if ((BATTLE_OK & BattleActGet(definition->actionId)->flags) == 0) {
    return 2;
  }
  return 0;
}

// Copy one 4bpp VRAM tile, ignoring the palette and flip bits of each entry.
void Vram_CopyTile(u32 sourceTile, u32 destinationTile) {
  Dma_Set((u8 *)0x06000000 + (sourceTile & 0x3ff) * 32,
          (u8 *)0x06000000 + (destinationTile & 0x3ff) * 32, 0x84000008,
          REG_DMA3);
}

// Choose an inventory-list position for battle use, or -1 on cancel. Keep
// unavailable items visible with their help text, but reject confirmation.
s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count) {
  struct UiRenderWork *render;
  s32 drawnFirstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint cursorPosition;
  struct MenuCell *navigation;
  struct UiWindow *window;
  s32 firstEntry;
  s32 selectedRow;
  s32 item;
  s32 iconTile;
  s32 pageTile;
  s32 unusableReason;
  s32 result;
  s32 i;
  struct CursorSprite cursor;
  struct MenuSprite *iconSprite;
  struct CursorSprite *cursorSprite;
  u16 messageText[64];
  s32 iconSlots[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];

  render = UI_WORK;
  drawnFirstEntry = -1;
  drawnRow = -1;
  cursorHandle = VramCache_AllocEntry(128);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = 0;
  MenuCell_GetCursor(firstEntry, selectedRow, preferredRow);
  window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y = i * 2;

    iconSprite = &sprites[i];
    MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
    iconSprite->oam.f.x = window->x * 8 + 8;
    iconSprite->oam.f.y = (y + window->y) * 8 + 4;
  }

  // Reserve one glyph slot per visible row; paging reloads these same slots.
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], -1);
  }

  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  cursorSprite = &cursor;
  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
        if (Item_ClassifyUseAbility(owner, entries[firstEntry + selectedRow]) ==
            2) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else {
          BATTLE_MENU_MESSAGE_COPY(
              (entries[firstEntry + selectedRow] & ITEM_ID_MASK) +
                  MsgItemPlainName,
              messageText, BATTLE_MENU_MESSAGE_CAPACITY);
        }
      } else {
        BATTLE_MENU_MESSAGE_COPY(MsgBattleNoItems, messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;

      // Cursor moves update help; only a page change reloads the row icons.
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          item = entries[firstEntry + i];
          if (item == 0) {
            break;
          }
          Item_GetDirectFar(item);
          UiWork_SetParamNibble(15);
          if (Item_ClassifyUseAbility(owner, item) != 0) {
            UiWork_SetParamNibble(4);
          } else if ((item & INVENTORY_BROKEN) != 0) {
            UiWork_SetParamNibble(2);
          }
          UiText_DrawCharacterAtOffset((item & ITEM_ID_MASK) + MsgItemName,
                                       window, 16, i * 16);
          UiWork_SetParamNibble(15);
          sprites[i].oam.f.tile =
              Resource_LoadKind26EntryToBuffer(item, iconSlots[i]);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          iconTile = i + LIST_TILE_PAGE_NUMBER;
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
          UiWindow_SetTilemapEntry(
              window, iconTile,
              window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
        }
      }

      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
        iconTile = i + LIST_TILE_PAGE_NUMBER;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + LIST_TILE_PAGE_NUMBER_LIT;
          }
        }
        UiWindow_SetTilemapEntry(
            window, iconTile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      pageTile = -1;
      UiWindow_SetTilemapEntry(
          window, LIST_TILE_PAGES_LEFT,
          window->width - (count + 4) / BATTLE_LIST_ROWS - 3, pageTile, 0);
      UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2,
                               pageTile, 0);
      UiRender_MarkRow(render, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      iconSprite = &sprites[i];
      Runtime_PushSlotEntry(iconSprite, 240);
    }

    cursorPosition.x = window->x * 8 - 2;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    /* FAKEMATCH: register choice (spill slots) only: re-pointing cur each
       frame orders the spill slots of the cursor address and row*2 as the
       native frame does. Measured (2026-10-13, after MenuCell_SetCursor):
       without it differ 56; with it but without the assignment before
       the loop, differ 14; the cursor written directly, with no cur,
       781 instructions for 770 (differ 209). Battle_SelectAbility sets
       curSpr the same two ways, untagged (394 and 36 without them). */
    cursorSprite = &cursor;
    ((u32 *)cursorSprite)[1] = MENU_SPRITE_SIZE_16;
    ((u32 *)cursorSprite)[2] = 0;
    cursorSprite->tile =
        VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    cursorSprite->x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    cursorSprite->y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(cursorSprite, 242);
    }

    MenuCell_SetCursor(firstEntry, selectedRow, preferredRow);
    navigation = gLinkCountdownWork;

    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = firstEntry + selectedRow;
        Item_GetDirectFar(entries[result]);
        unusableReason = 0;
        if ((entries[result] & INVENTORY_BROKEN) == 0) {
          unusableReason = Item_ClassifyUseAbility(owner, entries[result]);
          if (unusableReason == 0) {
            break;
          }
        }
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        if (unusableReason == 2) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else if ((entries[result] & INVENTORY_BROKEN) != 0) {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemBroken, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        } else {
          BATTLE_MENU_MESSAGE_COPY(MsgBattleItemNoEffect, messageText,
                                   BATTLE_MENU_MESSAGE_CAPACITY);
        }
        Ui_FillVramBlockPattern();
        UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      } else {
        result = -1;
        break;
      }
    } else if (navigation->active == 0 || (gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == count) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_UP) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
          } else {
            selectedRow = BATTLE_LIST_ROWS - 1;
          }
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + BATTLE_LIST_ROWS >= count) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      } else if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          firstEntry -= BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
        } else {
          firstEntry = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
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
    Resource_ResetEntry(iconSlots[i]);
  }
  WaitFrames(1);
  return result;
}

// Select a Psynergy-list position, or -1 on cancel. The list shows PP cost
// and range, and confirmation rejects abilities the unit cannot use.
s32 BattleMenu_RunActionSelection(s32 unitId, u16 *actions, s32 count) {
  struct UiRenderWork *render;
  s32 drawnFirstEntry;
  s32 firstEntry;
  s32 drawnRow;
  s32 cursorHandle;
  struct BattleUnit *unit;
  struct UiWindow *messageWindow;
  s32 iconCount;
  s32 preferredRow;
  struct MenuPoint cursorPosition;
  struct MenuCell *navigation;
  struct UiWindow *window;
  s32 selectedRow;
  s32 encodedAction;
  s32 iconTile;
  s32 pageTile;
  s32 rangePattern;
  s32 result;
  s32 i;
  struct MenuSprite cursor;
  struct MenuSprite *iconSprite;
  u16 messageText[64];
  s32 iconSlots[BATTLE_LIST_ROWS];
  struct MenuSprite sprites[BATTLE_LIST_ROWS];
  s32 glyph;

  render = UI_WORK;
  drawnRow = -1;
  drawnFirstEntry = -1;
  cursorHandle = VramCache_AllocEntry(128);
  unit = UnitGetFar(unitId);
  messageWindow = UiWindow_Create(0, 5, 30, 4, 42);
  iconCount = BATTLE_LIST_ROWS;
  MenuCell_GetCursor(firstEntry, selectedRow, preferredRow);
  window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    s32 y = i * 2;

    iconSprite = &sprites[i];
    MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
    iconSprite->oam.f.x = window->x * 8 + 8;
    iconSprite->oam.f.y = (y + window->y) * 8 + 4;
  }

  // Reserve one glyph slot per visible row; paging reloads these same slots.
  for (i = 0; i < BATTLE_LIST_ROWS; i++) {
    iconSlots[i] = VramCache_AllocEntry(128);
    sprites[i].oam.f.tile = VramCache_Reload(iconSlots[i], -1);
  }

  Vram_CopyTile(0xf018, 0x200);
  Vram_CopyTile(0xf018, 0x201);
  Vram_CopyTile(0xf019, 0x210);
  Vram_CopyTile(0xf019, 0x211);

  iconSprite = &cursor;
  for (;;) {
    if (firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
      render->menuBusy = 1;
      Ui_SetRectHighlight(window->x + 1, window->y + drawnRow * 2 + 1,
                          window->width - 2, 1, 15);
      Ui_FillVramBlockPattern();
      if (count != 0) {
        BATTLE_MENU_MESSAGE_COPY(
            (actions[firstEntry + selectedRow] & ACTION_ID_MASK) +
                MsgAbilityDescription,
            messageText, BATTLE_MENU_MESSAGE_CAPACITY);
      } else {
        BATTLE_MENU_MESSAGE_COPY(MSGNO(MsgBattleNoPsynergy), messageText,
                                 BATTLE_MENU_MESSAGE_CAPACITY);
      }
      UiText_RenderWideStringAtOffset(messageText, messageWindow, 0, 4);
      drawnRow = selectedRow;

      // Cursor moves update help; only a page change reloads the row icons.
      if (firstEntry != drawnFirstEntry) {
        RenderOutput_RedrawSavedRect(window);
        for (i = 0; i < BATTLE_LIST_ROWS; i++) {
          struct BattleAct *action;

          encodedAction = actions[firstEntry + i];
          if (encodedAction == 0) {
            break;
          }
          action = BattleActGet(encodedAction);
          UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
          UiWindow_SetTilemapEntry(window, PSYNERGY_COST_SECOND_TILE, 12, i * 2,
                                   0);
          Ability_LoadGlyph(encodedAction & ACTION_ID_MASK, 0, &iconSlots[i],
                            &glyph, 1);
          sprites[i].oam.f.tile = glyph;

          if ((action->flags & ACTION_USABLE_IN_BATTLE) == 0) {
            UiWork_SetParamNibble(4);
          } else if (action->ppCost > unit->pp) {
            UiWork_SetParamNibble(2);
          } else if (unit->psySeal != 0) {
            UiWork_SetParamNibble(9);
          }

          render->level = 5;
          UiText_DrawCharacterAtOffset(encodedAction + MsgAbilityName, window,
                                       16, i * 16);
          UiText_DrawNumberAtOffset(action->ppCost, 2, window, 104, i * 16);
          UiWork_SetParamNibble(15);
          render->level = 15;

          if (action->element != 4) {
            UiWindow_SetTilemapEntry(window, action->element + 0x5001, 15,
                                     i * 2, 0);
          }

          rangePattern = action->range;
          if (rangePattern == ABILITY_RANGE_ALL) {
            rangePattern = 11;
          } else {
            rangePattern--;
          }
          UiWindow_DrawThreeTileColumn(window, 16, i * 2, rangePattern, 0);
        }
        iconCount = i;
        drawnFirstEntry = firstEntry;
      }

      if (count > BATTLE_LIST_ROWS) {
        for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
          iconTile = i + 0xf301;
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + 0xf30b;
          }
          UiWindow_SetTilemapEntry(
              window, iconTile,
              window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
        }
      }

      Ui_SetRectHighlight(window->x + 1, window->y + selectedRow * 2 + 1,
                          window->width - 2, 1, 14);
      render->dirty = 1;
      render->menuBusy = 0;
    }

    if (count > BATTLE_LIST_ROWS) {
      for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
        iconTile = i + 0xf301;
        if ((gFrameCount & 15) <= 11) {
          if (i == firstEntry / BATTLE_LIST_ROWS) {
            iconTile = i + 0xf30b;
          }
        }
        UiWindow_SetTilemapEntry(
            window, iconTile,
            window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
      }
      pageTile = -1;
      UiWindow_SetTilemapEntry(
          window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3,
          pageTile, 0);
      UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, pageTile, 0);
      UiRender_MarkRow(render, window->y - 1);
    }

    for (i = 0; i < iconCount; i++) {
      Runtime_PushSlotEntry(&sprites[i], 240);
    }

    cursorPosition.x = window->x * 8 - 4;
    cursorPosition.y = (selectedRow * 2 + window->y) * 8 + 20;
    ((u32 *)iconSprite)[1] = 0x40000000;
    ((u32 *)iconSprite)[2] = 0;
    iconSprite->oam.f.tile =
        VramCache_Reload(cursorHandle, (s32)gResource_FixedBlockBTiles);
    iconSprite->oam.f.x = cursorPosition.x + ((gFrameCount & 4) >> 1) - 4;
    iconSprite->oam.f.y = cursorPosition.y - ((gFrameCount & 4) >> 2) - 8;
    if (count != 0) {
      Runtime_PushSlotEntry(iconSprite, 242);
    }

    MenuCell_SetCursor(firstEntry, selectedRow, preferredRow);
    navigation = gLinkCountdownWork;

    if ((gKeysPressed & KEY_A) != 0) {
      if (count != 0) {
        result = firstEntry + selectedRow;
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
    } else if (navigation->active == 0 || (gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }

    if (count != 0) {
      if ((gKeysRepeat & KEY_DOWN) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow++;
        if (selectedRow == BATTLE_LIST_ROWS ||
            firstEntry + selectedRow == count) {
          selectedRow = 0;
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_UP) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        selectedRow--;
        if (selectedRow < 0) {
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
          } else {
            selectedRow = BATTLE_LIST_ROWS - 1;
          }
        }
        preferredRow = selectedRow;
      } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry + BATTLE_LIST_ROWS >= count) {
          if (firstEntry != 0) {
            firstEntry = 0;
            selectedRow = preferredRow;
          }
        } else {
          firstEntry += BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
            }
          }
        }
      } else if ((gKeysRepeat & KEY_LEFT) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        if (firstEntry != 0) {
          selectedRow = preferredRow;
          firstEntry -= BATTLE_LIST_ROWS;
        } else {
          firstEntry = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
          selectedRow = preferredRow;
          if (firstEntry != 0) {
            selectedRow = count - firstEntry - 1;
            if (selectedRow > preferredRow) {
              selectedRow = preferredRow;
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
    Resource_ResetEntry(iconSlots[i]);
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

// Choose a party/enemy target and animate its selection markers. Return the
// encoded side and party position, or -1 on cancel. Healing and status-cure
// categories choose an appropriate initial ally and supply condition help.
s32 BattleTarget_RunSelection(s32 selection, s32 mode, u32 spread, u32 kind) {
  u16 candidateIds[8];
  struct SelectionSprite cursorSprites[6];
  struct CursorSlot cursorTracks[6];
  struct EffectPosition actorPosition;
  u16 spreadActorIds[14];
  u8 spreadTrackIndices[8];
  struct EffectPosition selectedPosition;
  u16 targetName[15];
  struct EffectPosition namePosition;
  struct EffectPosition markerPosition;
  struct AffineTransform effect;
  struct BattleUnit *unit;
  struct BattleWork *battle;
  struct UiWindow *window;
  /* FAKEMATCH: the earlier combined pin/barrier removal gave EN 1585/1585,
   * common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320.
   * ES 1684/1684, common 1569, differ 230: infoWin/selection swap r10/r9 and
   * spill slots permute. Both extents stayed unchanged. */
#if TBS_PATCH_4
  struct UiWindow *conditionWindow;
#else
  /* FAKEMATCH: register choice only: as an ordinary local the info window is
   * spilled and the frame shrinks to 320 bytes (differ 492); the native keeps
   * it in r9. 2026-10-11: its live length is doubled by update_equiv_regs (the
   * first set carries a constant REG_EQUAL), giving priority 2104 against &pos
   * 2171 and the hoisted bit-field zero 2142; one duplicated no-effect block
   * reaches 2143 against 2149 (differ 202), two push &pos below the zero
   * (differ 449), and an initializer without the note sends it to r10 as in
   * ES/IT (differ 96). */
  register struct UiWindow *conditionWindow asm("r9");
#endif
  s32 targetIndex;
  s32 targetCount;
  s32 candidateCount;
  s32 redraw;
  s32 cursorSlot;
  s32 drawCursors;
  s32 matrix;
  s32 found;
  s32 helpColumn;
  s32 helpRow;
  s32 count;
  s32 i;
  s32 j;
  s32 n;
  s32 cursorX;
  s32 cursorY;
  s32 pressed;
  s32 y;
  s32 width;
  s32 repeated;
  s32 result;
  struct CursorSlot *track;
  struct SelectionSprite *marker;
  struct SelectionSprite *firstSprite;
  struct BattleUnit *target;

  battle = gBattleWork;
  targetCount = 0;
  redraw = 0xFFFF;
  cursorSlot = VramCache_AllocEntry(256);
#if TBS_PATCH_4
  /* FAKEMATCH: register choice only: this value-preserving null
   * initializer is needed to match infoWin in r10; a direct zero swaps
   * high registers and spill slots (2026-10-13: infoWin = 0 differs 94
   * in ES and IT). */
  conditionWindow = (struct UiWindow *)((u32)(u16)selection >> 16);
#else
  conditionWindow = 0;
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
    cursorTracks[i].flags = 0;
  targetIndex = -1;
  if (mode == 2) {
    for (i = 0; i <= 5 && battle->partyUnits[i] != 0xFF; i++) {
      candidateIds[targetCount] = battle->partyUnits[i];
      targetCount++;
    }
  } else if (mode == 4) {
    candidateIds[0] = (u16)selection;
    targetCount = 1;
  } else {
    for (i = 0; i <= 5 && battle->enemyIds[i] != 0xFF; i++) {
      candidateIds[targetCount] = battle->enemyIds[i];
      targetCount++;
    }
  }
  // Keep the candidate list separate from the markers drawn for a spread.
  candidateIds[targetCount] = 0xFF;
  candidateCount = targetCount;
  // Restorative actions prefer an ally who benefits from their effect.
  if (mode == 2) {
    if (spread != ABILITY_RANGE_ALL && kind != 0) {
      found = 0;
      for (j = 0; j < targetCount; j++) {
        struct BattleUnit *candidate;

        if (candidateIds[j] == 0xFE)
          continue;
        candidate = UnitGetFar(candidateIds[j]);
        switch (kind) {
        case 5:
          if (candidate->hp == 0) {
            selection = candidateIds[j];
            found = 1;
          }
          break;
        case 3:
          if (candidate->poison != 0)
            found = 1;
          break;
        case 4:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0)
            found = 1;
          break;
        case 6:
          if (candidate->delusion != 0 || candidate->stun != 0 ||
              candidate->sleep != 0 || candidate->psySeal != 0 ||
              candidate->grimReaper != 0 || candidate->poison != 0 ||
              candidate->haunt != 0)
            found = 1;
          break;
        }
        if (found != 0) {
          selection = candidateIds[j];
          break;
        }
      }
    }
    for (j = 0; j < targetCount; j++) {
      if (candidateIds[j] == selection)
        break;
    }
    if (j != targetCount)
      targetIndex = j;
  }
  if (targetIndex < 0)
    targetIndex = (targetCount - 1) / 2;
  while (candidateIds[targetIndex] == 0xFE ||
         (GameFlag_TestFar(0x16C) && mode == 1 &&
          UnitGetFar(candidateIds[targetIndex])->hp == 0)) {
    targetIndex = targetIndex + targetCount - 1;
    targetIndex %= targetCount;
  }
  if (mode != 2) {
    BattleMotion_ProjectConditionalPositionFar(selection,
                                               (s32 *)&actorPosition);
    cursorTracks[0].flags = 8;
    cursorTracks[0].x = (u8)actorPosition.x;
    cursorTracks[0].y = 0x80;
  }
  window = UiWindow_Create(0, 12, 30, 4, 74);
  while (1) {
    firstSprite = cursorSprites;
    drawCursors = 0;
    {
      s32 t = targetIndex * 2;

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
        t = targetIndex * 2;
        break;
      default:
        t = targetIndex * 2;
        break;
      }
      BattleMotion_ProjectConditionalPositionFar(
          *(u16 *)((u8 *)candidateIds + t), (s32 *)&selectedPosition);
    }
    ((s32 *)firstSprite)[1] = 0x40002000;
    ((s32 *)firstSprite)[2] = 0;
    firstSprite->tile =
        VramCache_Reload(cursorSlot, (((gFrameCount >> 2) & 31) << 8) +
                                         (s32)gMenu_AnimatedCursorTiles);
    selectedPosition.y += Trig_Sin(gFrameCount << 12) / 32768;
    y = selectedPosition.y;
    if (cursorTracks[0].flags & 1) {
      cursorX = (selectedPosition.x + cursorTracks[0].x) / 2;
      cursorY = (y + cursorTracks[0].y) / 2;
      if ((selectedPosition.x - cursorX < 0
               ? -(selectedPosition.x - cursorX)
               : selectedPosition.x - cursorX) <= 7)
        drawCursors = 1;
      selectedPosition.x = cursorX;
      cursorTracks[0].x = (u8)cursorX;
      cursorTracks[0].y = (u8)cursorY;
      selectedPosition.y = cursorY;
    } else if ((u8)cursorTracks[0].flags <= 3) {
      cursorTracks[0].x = (u8)selectedPosition.x;
      selectedPosition.y = y;
      cursorTracks[0].y = (u8)y;
      cursorTracks[0].flags = 1;
    } else {
      selectedPosition.x = cursorTracks[0].x;
      selectedPosition.y = cursorTracks[0].y;
      cursorTracks[0].flags = (u8)(cursorTracks[0].flags - 4);
      if (cursorTracks[0].flags <= 3)
        cursorTracks[0].flags = 1;
    }
    firstSprite->x = selectedPosition.x - 8;
    firstSprite->y = (u8)(selectedPosition.y - 16);
    Runtime_PushSlotEntry((s32 *)firstSprite, 240);
    if (spread == ABILITY_RANGE_ALL) {
      effect.scaleX = 256;
      effect.scaleY = 256;
    } else {
      effect.scaleX = 176;
      effect.scaleY = 176;
    }
    effect.angle = 0;
#if !(LANGUAGE == LANGUAGE_SPANISH) && !(LANGUAGE == LANGUAGE_ITALIAN)
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
      targetCount = 0;
      for (i = 0; i < 6; i++)
        cursorTracks[i].flags &= (u8)~2;
      for (i = 0; (u32)i < spread; i++) {
        {
          n = targetIndex + i;
          if (n < candidateCount && candidateIds[n] != 0xFE) {
            spreadActorIds[targetCount] = candidateIds[n];
            cursorTracks[i].flags |= 2;
            if (cursorTracks[i].index != i) {
              cursorTracks[i].flags &= (u8)~1;
              cursorTracks[i].index = (s8)i;
            }
            spreadTrackIndices[targetCount] = (u8)i;
            targetCount++;
          }
          if (i != 0 && (n = targetIndex - i) >= 0 && candidateIds[n] != 0xFE) {
            spreadActorIds[targetCount] = candidateIds[n];
            cursorTracks[6 - i].flags |= 2;
            if (cursorTracks[6 - i].index != -i) {
              cursorTracks[6 - i].flags &= (u8)~1;
              cursorTracks[6 - i].index = (s8)-i;
            }
            spreadTrackIndices[targetCount] = (u8)(6 - i);
            targetCount++;
          }
        }
      }
      for (i = 0; i < 6; i++) {
        if ((cursorTracks[i].flags & 2) == 0)
          cursorTracks[i].index = 6;
      }
      spreadActorIds[targetCount] = 0xFF;
      BattlePres_SetActorModesFar(spreadActorIds, 1);
      if (candidateIds[targetIndex] <= 7) {
        if (spread != ABILITY_RANGE_ALL && kind != 0) {
          unit = UnitGetFar(candidateIds[targetIndex]);
          BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                     (s32 *)&selectedPosition);
          if (conditionWindow != 0)
            UiWork_Finalize(conditionWindow, 1);
          switch (kind) {
          case 7: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 4;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            if (System_GetLanguage() == 4) {
              if (anchor + 6 > 29)
                helpColumn = 20;
              conditionWindow = UiWindow_Create(helpColumn, 8, 10, 3, 6);
            } else if (System_GetLanguage() == 3) {
              if (anchor + 7 > 29)
                helpColumn = 19;
              conditionWindow = UiWindow_Create(helpColumn, 8, 11, 3, 6);
            } else {
              if (anchor + 5 > 29)
                helpColumn = 21;
              conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
            }
#elif (LANGUAGE == LANGUAGE_FRENCH)
            if (anchor + 5 > 29)
              helpColumn = 21;
            conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
#elif (LANGUAGE == LANGUAGE_JAPANESE)
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = UiWindow_Create(helpColumn, 8, 8, 3, 6);
#else
            if (anchor + 4 > 29)
              helpColumn = 22;
            conditionWindow = UiWindow_Create(helpColumn, 8, 9, 3, 6);
#endif
          }
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetSetDjinni, conditionWindow, 0,
                                         0);
            UiWork_SetParamNibble(15);
            break;
          case 1: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = UiWindow_Create(helpColumn, 8, 13, 3, 6);
          }
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
            switch (System_GetLanguage()) {
            case 1:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_KpString,
                                        conditionWindow, 0, 0);
              break;
            case 2:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString,
                                        conditionWindow, 0, 0);
              break;
            case 3:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PvString,
                                        conditionWindow, 0, 0);
              break;
            case 4:
              UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString,
                                        conditionWindow, 0, 0);
              break;
            }
#else
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_HpString,
                                      conditionWindow, 0, 0);
#endif
            UiText_DrawNumberInWindow(unit->hp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      conditionWindow, 48, 0);
            UiText_DrawNumberInWindow(unit->maxHp, 4, conditionWindow, 56, 0);
            break;
          case 2: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 6 > 29)
              helpColumn = 17;
            conditionWindow = UiWindow_Create(helpColumn, 8, 13, 3, 6);
          }
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_PpString,
                                      conditionWindow, 0, 0);
            UiText_DrawNumberInWindow(unit->pp, 4, conditionWindow, 16, 0);
            UiText_DrawStringAtOffset((u8 *)gOwnerStatus_SlashString,
                                      conditionWindow, 48, 0);
            UiText_DrawNumberInWindow(unit->maxPp, 4, conditionWindow, 56, 0);
            break;
          case 5: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = UiWindow_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->hp != 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetDowned, conditionWindow, 0,
                                         0);
            break;
          case 3: {
            s32 anchor;

            anchor = selectedPosition.x / 8;
            helpColumn = anchor - 7;
            if (anchor + 5 > 29)
              helpColumn = 18;
            conditionWindow = UiWindow_Create(helpColumn, 8, 12, 3, 6);
          }
            if (unit->poison == 0)
              goto no_condition;
            UiText_DrawCharacterAtOffset(MsgTargetCurePoison, conditionWindow,
                                         0, 0);
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
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            {
              s32 anchor;

              anchor = selectedPosition.x / 8;
              helpColumn = anchor - 7;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  helpColumn = 13;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion,
                                           conditionWindow, 0, 0);
              count = 1;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, conditionWindow,
                                           0, count * 8);
              ++count;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, conditionWindow,
                                           0, count * 8);
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
            helpRow = 9 - count;
            if (helpRow <= 3)
              helpRow = 4;
            {
              s32 anchor;

              anchor = selectedPosition.x / 8;
              helpColumn = anchor - 7;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
              if (System_GetLanguage() == 3) {
                if (anchor + 10 > 29)
                  helpColumn = 13;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 17, count + 2, 6);
              } else {
                if (anchor + 9 > 29)
                  helpColumn = 14;
                conditionWindow =
                    UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
              }
#elif (LANGUAGE == LANGUAGE_JAPANESE)
              if (anchor + 7 > 29)
                helpColumn = 16;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 14, count + 2, 6);
#else
              if (anchor + 9 > 29)
                helpColumn = 14;
              conditionWindow =
                  UiWindow_Create(helpColumn, helpRow, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->poison != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCurePoison, conditionWindow,
                                           0, 0);
              count = 1;
            }
            if (unit->delusion != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureDelusion,
                                           conditionWindow, 0, count * 8);
              count++;
            }
            if (unit->stun != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureStun, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->sleep != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSleep, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->psySeal != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureSeal, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->grimReaper != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureCurse, conditionWindow,
                                           0, count * 8);
              count++;
            }
            if (unit->haunt != 0) {
              UiText_DrawCharacterAtOffset(MsgTargetCureHaunt, conditionWindow,
                                           0, 8 * count);
              count++;
            }
            if (count != 0)
              break;
          no_condition:
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(MsgTargetNoEffect, conditionWindow, 0,
                                         0);
            UiWork_SetParamNibble(15);
            break;
          }
        }
      } else if (spread != ABILITY_RANGE_ALL) {
        target = UnitGetFar(candidateIds[targetIndex]);
        BattleMotion_ProjectConditionalPositionFar(candidateIds[targetIndex],
                                                   (s32 *)&namePosition);
        namePosition.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (target->classId == 125 || target->classId == 122) {
          width = MsgMysteryPersonName;
          if (target->classId == 125)
            width++;
          TARGET_MYSTERY_NAME_COPY(width, targetName,
                                   TARGET_MYSTERY_NAME_CAPACITY);
        } else {
          for (i = 0; i <= 13; i++) {
            targetName[i] = n = target->name[i];
            if (!n)
              break;
          }
          targetName[i] = 0;
        }
        width = UiText_GetWideStringWidth(targetName);
        namePosition.x -= width / 2;
        namePosition.x -= 8;
        if (namePosition.x + width > 224)
          namePosition.x = 224 - width;
        if (namePosition.x < 0)
          namePosition.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(targetName, window, namePosition.x, 4);
      }
      redraw &= ~1;
    }
    if (drawCursors != 0) {
      for (i = 1; i < targetCount; i++) {
        marker = &firstSprite[i];
        track = &cursorTracks[spreadTrackIndices[i]];
        BattleMotion_ProjectConditionalPositionFar(spreadActorIds[i],
                                                   (s32 *)&markerPosition);
        markerPosition.y =
            markerPosition.y + Trig_Sin(gFrameCount << 12) / 32768;
        *marker = *firstSprite;
        if (track->flags & 1) {
          s32 x = (markerPosition.x + track->x) / 2;
          s32 y = (markerPosition.y + track->y) / 2;

          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        } else {
          s32 x = marker->x;
          s32 y = marker->y + 8;

          track->flags = 1;
          markerPosition.x = x;
          track->x = x;
          markerPosition.y = y;
          track->y = y;
        }
        marker->mode = 1;
        marker->x = markerPosition.x - 8;
        marker->y = markerPosition.y - 12;
        if (spread == ABILITY_RANGE_ALL)
          marker->affine = 0;
        else
          marker->affine = 1;
        marker->param = matrix;
        Runtime_PushSlotEntry((s32 *)marker, 240);
      }
    }
    pressed = gKeysPressed;
    repeated = gKeysRepeat;
    if (gLinkCountdownWork->autoEnabled) {
      repeated = 0;
      pressed = 0;
      if (gLinkCountdownWork->autoDelay == 0) {
        gLinkCountdownWork->autoDelay = 60;
        repeated = 1;
        pressed = 1;
      } else {
        gLinkCountdownWork->autoDelay--;
      }
    }
    if (pressed & KEY_A) {
      s32 id;
      s32 k;

      id = candidateIds[targetIndex];
      redraw = 0;
      result = -1;
      for (k = 0; k <= 5 && battle->partyUnits[k] != 0xFF; k++) {
        if (battle->partyUnits[k] == id) {
          result = 0x100 | k;
          break;
        }
      }
      if (result < 0) {
        for (k = 0; k <= 5 && battle->enemyIds[k] != 0xFF; k++) {
          if (battle->enemyIds[k] == id) {
            result = 0x180 | k;
            break;
          }
        }
      }
      targetIndex = result;
    } else if (spread != ABILITY_RANGE_ALL) {
      if (repeated & (KEY_DOWN | KEY_RIGHT)) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        do {
          targetIndex++;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 0xFE);
        redraw |= 1;
      }
      if (repeated & (KEY_UP | KEY_LEFT)) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        do {
          targetIndex = targetIndex + candidateCount - 1;
          targetIndex %= candidateCount;
        } while (candidateIds[targetIndex] == 0xFE);
        redraw |= 1;
      }
    }
    if (gLinkCountdownWork->active == 0 || (pressed & KEY_B)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      targetIndex = -1;
      break;
    }
    WaitFrames(1);
    if (redraw == 0)
      break;
  }
  WaitFrames(1);
  Resource_ResetEntry(cursorSlot);
  if (conditionWindow != 0)
    UiWork_Finalize(conditionWindow, 1);
  UiWork_Finalize(window, 1);
  BattlePres_SetActorModesFar(candidateIds, 0);
  gLinkCountdownWork->slideOffset = 0;
  WaitFrames(1);
  return targetIndex;
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

// Advance the link command-entry timer, slide the battle panels, and redraw
// the remaining seconds. Pause flags and peer signatures stop the timer.
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

    // Local battles still animate the UI, but do not run the link timer.
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

    // Round remaining frames upward so zero appears only after time expires.
    seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

    if (state->displayHandle != 0)
      UiText_DrawNumberInWindow(seconds, 2,
                                (struct UiWindow *)state->displayHandle, 16, 8);
  }
}

// Test whether this battle forbids escape. Show the reason until A/B or the
// link timer expires, returning 1 when escape is forbidden and 0 otherwise.
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
    BATTLE_MENU_MESSAGE_COPY(MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
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
  s32 signedEffect;
  s32 abilityKind;
  s32 targetKind;
  u8 effect;

  targetKind = 0;
  abilityKind = act->flags & ABILITY_KIND_MASK;
  if (abilityKind == ABILITY_KIND_HEAL_HP)
    targetKind = 1;
  if (abilityKind == ABILITY_KIND_HEAL_PP)
    targetKind = 2;
  signedEffect = (s8)act->effect;
  effect = signedEffect;
  if (effect == ABILITY_EFFECT_CURE_POISON)
    targetKind = 3;
  if (effect == ABILITY_EFFECT_RESTORE)
    targetKind = 4;
  if (effect == ABILITY_EFFECT_CURE_ALL)
    targetKind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive
    targetKind = 5;
  return targetKind;
}

// Collect commands in party order, including target, cost and randomized
// priority. Cancelling returns to the preceding choice; linked battles
// exchange completion signatures before releasing the command UI.
s32 Battle_CollectPartyCommands(struct BattleActionRecord *commands,
                                u16 *actors, s32 commandCount) {
  struct LinkRoundState *state;
  struct BattleWork *battle;
  struct BattleWork *linkBattle;
  u16 *peerSignature;
  u16 *transferWords;
  struct BattleActionRecord *command;
  struct BattleUnit *unit;
  struct UiWindow *commandWindow;
  struct MenuSprite *iconSprite;
  u16 signatureHeader[2];
  s32 target;
  s32 actorId;
  s32 mode;
  s32 commandKind;
  s32 parameter;
  s32 largeIconSlot;
  s32 smallIconSlot;
  s32 missedLinkFrames;
  s32 commandIndex;
  s32 k;
  s32 priority;

  // An empty ID list dims every party panel before the next actor is chosen.
  void BattleMenu_ClearPartyHighlights(void) {
    u16 data[2];
    data[0] = 0xff;
    BattleLayout_HighlightPartyPanels(data);
    BattlePres_SetActorModesFar(data, 1);
  }

  // Set up the shared command UI before the optional link handshake.
  target = 256;
  commandKind = 0;
  parameter = 0;
  largeIconSlot = VramCache_AllocEntry(1024);
  smallIconSlot = VramCache_AllocEntry(512);
  state = Runtime_AllocateBlock(57, 260);
  Resource_LoadIndexedIntoBuffer(largeIconSlot, 0);
  Graphics_ExpandVramTilesByColorTable(0x06006000);
  Link_DrawShiftedTilePair(0x06006680);
  Menu_BuildLocalizedPatternTiles();
  battle = gBattleWork;
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
  // Both peers announce the command-entry phase before accepting commands.
  if (battle->twoSided != 0) {
    state->enabled = 1;
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'V';
    transferWords[5] = 'S';
    transferWords[6] = 'S';
    transferWords[7] = 'T';
    missedLinkFrames = 0;
    for (;;) {
      peerSignature = (u16 *)&gLinkPeerSignatures[battle->linkSide ^ 1];
      if (battle->linkPaused != 0) {
        commandCount = -1;
        state->enabled = 0;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        if (++missedLinkFrames > 24) {
          commandCount = -1;
          state->enabled = 0;
          break;
        }
      } else {
        missedLinkFrames = 0;
        if (peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
            peerSignature[6] == 'S' && peerSignature[7] == 'T') {
          break;
        }
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S') {
          break;
        }
        if (!(peerSignature[0] == 'E' && peerSignature[1] == 'X' &&
              peerSignature[2] == 'E' && peerSignature[3] == 'C')) {
          commandCount = -1;
          state->enabled = 0;
          break;
        }
      }
      WaitFrames(1);
    }
  }
  Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

  while (1) {
    BattleMenu_ClearPartyHighlights();
    state->entryActive[2] = 0;
    state->actorId = -1;
    state->hintCount = 0;
    if (GameFlag_TestFar(366) != 0) {
      state->hintCount = 1;
      state->hintTimer = 60;
    }
    if (commandCount > 0) {
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
      commandCount = 1;
      command = commands;
      command->unitId = actors[0];
      command->priority = 0x7ffe;
      command->command = 99;
      command->parameter = 0;
      command->target = 256;
      break;
    }
    if (mode != 14) {
      break;
    }
    AudioCommand_PlayFar(154);
    commandIndex = 0;
    if (commandIndex >= commandCount) {
      break;
    }

    for (;;) {
      if (commandIndex == 0) {
        BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
      } else {
        s32 k;

        for (k = 0; k < 4; k++) {
          state->cost[commandIndex][k] = state->cost[commandIndex - 1][k];
        }
      }

      command = &commands[commandIndex];
      actorId = actors[commandIndex];
      // UB: UnitGetFar returns a pointer, not the cast's doubleword result.
#ifdef UBFIX
      unit = UnitGetFar(actorId);
#else
      /* FAKEMATCH: instruction order only: called through a cast as
         returning a doubleword, the call also sets r1, so the r1
         reload for classId no longer depends on the in reload and
         sched2 loads i * 2 before in, as native (by its prototype,
         differ 2). */
      unit = (struct BattleUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
#endif
      COUNTDOWN->actorId = actorId;
      COUNTDOWN->unk40 = 0;
      iconSprite = &COUNTDOWN->entries[2];
      iconSprite->oam.word.attr01 = 0x80000400;
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.f.tile =
          Ui_LoadEntryForKind(unit->classId, smallIconSlot);
      iconSprite->oam.f.x = BATTLE_CLASS_ICON_X;
      iconSprite->oam.f.y = 0x80;
      iconSprite->oam.f.palette = 14;
      state->entryActive[2] = 1;

    mark_visible:
      UiWindow_MarkVisibleTileAttributes();
      COUNTDOWN->entryActive[0] = 0;
      COUNTDOWN->selected[commandIndex] = 0x80000000;
      /* FAKEMATCH: twelve dead stores restore the native frame's spill slots (i
       * << 1, i << 2 and i << 4 at sp+32 to sp+24, above &hdr at sp+20);
       * without them differ 50 in every edition, every row a spill offset. */
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = 0;
      signatureHeader[0] = (u16)actorId;
      signatureHeader[1] = 255;
      BattlePres_SetActorModesFar(signatureHeader, 1);
      BattleLayout_HighlightPartyPanels(signatureHeader);
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
      BattlePres_SetActorModesFar(signatureHeader, 0);
      if (mode == -1) {
        if (commandIndex == 0) {
          break;
        }
        commandIndex--;
        continue;
      }

      if (COUNTDOWN->timer == 0) {
        mode = 3;
      }
      iconSprite = &COUNTDOWN->entries[0];
      iconSprite->oam.word.attr23 = 0;
      iconSprite->oam.word.attr01 = 0x80002400;
      iconSprite->oam.f.tile =
          Resource_LoadIndexedIntoBuffer(largeIconSlot, mode);
      if (mode == 15) {
        iconSprite->oam.f.x = 128;
      } else {
        iconSprite->oam.f.x = 96;
      }
      iconSprite->oam.f.y = 136;
      state->entryActive[0] = 1;
      switch (mode) {
      case 0:
        commandWindow = UiWindow_Create(11, 17, 11, 3, 6);
        UiText_DrawCharacterAtOffset(MsgCommandName, commandWindow,
                                     BATTLE_COMMAND_NAME_X, 0);
        state->entries[0].oam.f.x = 64;
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 0;
        target = mode;
        command->range = 1;
        break;

      case 1: {
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        struct UiRenderWork *ui;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 entryValue;

        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      psynergy_menu:
        list = state->list;
        pick = state->pick;
        state->entries[0].oam.f.x = 48;
        cnt = 0;
        k = 0;
        entryValue = unit->actionSlots[k].encodedAction & ACTION_ID_MASK;
        if (entryValue != 0) {
          do {
            if ((BattleActGet(entryValue)->flags & 0x80) != 0) {
              pick[cnt] = k;
              list[cnt] = entryValue;
              cnt++;
            }
            k++;
            if (k == 32) {
              break;
            }
            entryValue = unit->actionSlots[k].encodedAction & ACTION_ID_MASK;
          } while (entryValue != 0);
        }
        pick[cnt] = 0;
        list[cnt] = 0;
        mode = BattleMenu_RunActionSelection(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        parameter =
            unit->actionSlots[pick[mode]].encodedAction & ACTION_ID_MASK;
        ability = BattleActGet(parameter);
        cost = ability->range;
        iconSprite = &state->entries[1];
        handle = VramCache_AllocEntry(128);
        ui = UI_WORK;
        commandWindow = UiWindow_Create(8, 17, 18, 3, 6);
        state->entries[0].oam.f.x = 40;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
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
        iconSprite->oam.f.tile =
            Resource_LoadIndexedEntryToBuffer(parameter, handle);
        state->entryActive[1] = 1;
        ui->level = 5;
        if ((s32)ability->ppCost > (s32)unit->pp) {
          UiWork_SetParamNibble(2);
        } else if (unit->psySeal != 0) {
          UiWork_SetParamNibble(9);
        }
        UiText_DrawCharacterAtOffset(parameter + MsgAbilityName, commandWindow,
                                     16, 0);
        UiText_DrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
        ui->level = 15;
        UiWork_SetParamNibble(15);
        UiWindow_SetTilemapEntry(commandWindow, 61471, 11, 0, 0);
        UiWindow_SetTilemapEntry(commandWindow, PSYNERGY_COST_SECOND_TILE, 12,
                                 0, 0);
        if (ability->element != 4) {
          UiWindow_SetTilemapEntry(commandWindow, ability->element + 20481, 15,
                                   0, 0);
        }
        command->range = cost;
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto psynergy_menu;
        }
        commandKind = 1;
        target = mode;
        break;
      }

      case 16: {
        const struct SummonDefinition *summon;
        struct BattleAct *ability;
        struct MenuSprite *iconSprite;
        s32 handle;
        s32 ok;
        s32 j;
        s32 n;

        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        MenuCell_SetCursor(0, 0, 0);
      djinn_menu:
        state->entries[0].oam.f.x = BATTLE_SUMMON_ICON_X;
        BattleLayout_HighlightPartyPanels(signatureHeader);
        mode = SummonMenu_SelectSummon(0, 0, state->cost[commandIndex]);
        BattleLayout_HighlightPartyPanels(signatureHeader);
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 6;
        parameter = mode;
        WaitFrames(1);
        iconSprite = &state->entries[1];
        summon = SummonDefinition_Get(mode);
        ability = BattleActGet(summon->nameMessageId);
        handle = VramCache_AllocEntry(128);
        commandWindow =
            UiWindow_Create(10, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3, 6);
        n = 0;
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->cost[commandIndex][j]) {
            break;
          }
        }
        ok = (j == 4);
        state->entries[0].oam.f.x = 56;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
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
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadIndexedEntryToBuffer(
            summon->nameMessageId & 0x3fff, handle);
        state->entryActive[1] = 1;
        if (ok == 0) {
          UiWork_SetParamNibble(2);
        }
        UiText_DrawCharacterAtOffset(SummonDefinition_Get(mode)->nameMessageId +
                                         MsgAbilityName,
                                     commandWindow, 16, 0);
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] != 0) {
            UiWindow_SetTilemapEntry(commandWindow, j + 20481,
                                     n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0,
                                     0);
            UiWindow_PutGlyph(commandWindow, summon->djinnRequired[j] + 48,
                              n * 2 + BATTLE_SUMMON_COST_COLUMN, 0, 0);
            n++;
          }
        }
        if (ok != 0) {
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        } else {
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        }
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, ability->range,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        command->range = ability->range;
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto djinn_menu;
        }
        for (j = 0; j <= 3; j++) {
          if (summon->djinnRequired[j] > state->cost[commandIndex][j]) {
            state->cost[commandIndex][j] = 0;
            break;
          }
          state->cost[commandIndex][j] -= summon->djinnRequired[j];
        }
        target = mode;
        break;
      }

      case 15: {
        struct BattleAct *ability;
        struct UiWindow *hint;
        s32 cost;
        s32 element;
        s32 index;

        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        MenuCell_SetCursor(0, 0, 0);
      summon_menu:
        state->entries[0].oam.f.x = 144;
        COUNTDOWN->selected[commandIndex] = 0x80000000;
        mode = Battle_SelectAbility(actorId, 1);
        command->range = 0;
        if (mode == -1) {
          goto mark_visible;
        }
        commandKind = 5;
        parameter = mode;
        COUNTDOWN->selected[commandIndex] = mode;
        element = (parameter >> 8) & 15;
        index = parameter & 255;
        if (Djinn_IsActiveFar(actorId, element, index) != 0) {
          ability = BattleActGet(Djinn_GetAbilityIdFar(element, index));
          cost = ability->range;
          commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
          state->entries[0].oam.f.x = 64;
          UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
          UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                       commandWindow, 16, 0);
          command->range = cost;
          WaitFrames(1);
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
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
          UiWork_Finalize(commandWindow, 1);
          if (mode == -1) {
            goto summon_menu;
          }
          target = mode;
          break;
        }
        commandWindow = UiWindow_Create(11, 17, 10, 3, 6);
        state->entries[0].oam.f.x = 64;
        UiWork_SetParamNibble(2);
        UiWindow_SetTilemapEntry(commandWindow, element + 20481, 0, 0, 0);
        UiText_DrawCharacterAtOffset(element * 20 + index + MsgDjinnName,
                                     commandWindow, 16, 0);
        UiWork_SetParamNibble(15);
        command->range = 1;
        WaitFrames(1);
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto summon_menu;
        }
        target = mode;
        break;
      }

      case 2: {
        struct MenuSprite *iconSprite;
        struct BattleAct *ability;
        u16 *list;
        u8 *pick;
        s32 cost;
        s32 handle;
        s32 cnt;
        s32 k;
        s32 entryValue;
        s32 selection;

        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = 0;
        MenuCell_SetCursor(mode, mode, mode);
      item_menu:
        state->entries[0].oam.f.x = 96;
        pick = state->pick;
        list = state->list;
        cnt = 0;
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_GetDirectFar(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) == 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        k = 0;
        entryValue = unit->inventory[k];
        if (entryValue != 0) {
          do {
            Item_GetDirectFar(entryValue);
            if (Item_ClassifyUseAbility(actorId, entryValue) != 0) {
              list[cnt] = entryValue;
              pick[cnt++] = k;
            }
            k++;
            if (k == 15) {
              break;
            }
            entryValue = unit->inventory[k];
          } while (entryValue != 0);
        }
        list[cnt] = 0;
        mode = ItemList_SelectEntry(actorId, list, cnt);
        if (mode == -1) {
          goto mark_visible;
        }
        selection = pick[mode];
        parameter = selection;
        ability = BattleActGet(
            Item_GetDirectFar(unit->inventory[selection])->actionId);
        cost = ability->range;
        iconSprite = &state->entries[1];
        handle = VramCache_AllocEntry(128);
        commandWindow = UiWindow_Create(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3, 6);
        state->entries[0].oam.f.x = 48;
        iconSprite->oam.word.attr01 = 0x40000400;
        iconSprite->oam.word.attr23 = 0;
        /* FAKEMATCH: instruction order only (pool placement): counted as 4
         * bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native
         * (constant 48) (differ 10). 2026-10-11: arm_reorg runs before
         * shorten_branches and counts the empty asm at its 4-byte default
         * length while it emits nothing; the halfword constant fix needs that
         * much more estimated length after it, and no C statement or single
         * pattern-length change (strh, str, orr, and tested over all 413
         * English objects) supplies it. */
        __asm__ volatile("");
        iconSprite->oam.f.x = commandWindow->x * 8 + 8;
        iconSprite->oam.f.y = commandWindow->y * 8 + 4;
        iconSprite->oam.f.tile = Resource_LoadKind26EntryToBuffer(
            unit->inventory[selection], handle);
        state->entryActive[1] = 1;
        UiText_DrawCharacterAtOffset((unit->inventory[selection] & 0x1ff) +
                                         MsgItemName,
                                     commandWindow, 24, 0);
        command->range = cost;
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        mode = BattleTarget_RunSelection(
            actorId, ability->targetMode, cost,
            Battle_ClassifyEntryKind((const struct BattleAct *)ability));
        state->entryActive[1] = 0;
        Resource_ResetEntry(handle);
        UiWork_Finalize(commandWindow, 1);
        if (mode == -1) {
          goto item_menu;
        }
        commandKind = 2;
        target = mode;
        break;
      }

      case 3:
        commandKind = 3;
        break;
      }

      AudioCommand_PlayFar(110);
      command->unitId = (u16)actorId;
      // Draw the priority random value once, only for a nonzero agility.
      priority = unit->agi;
      if (priority != 0) {
        priority += (u32)(unit->agi * Random16()) >> 20;
      }
      command->priority = priority;
      if (commandIndex != 0 &&
          actors[commandIndex] == actors[commandIndex - 1]) {
        command->priority = (s16)priority / 2;
      }
      if ((s16)command->priority < 0) {
        command->priority = 2000;
      }
      command->command = commandKind;
      command->parameter = parameter;
      command->target = target;
      commandIndex++;
      if (commandIndex < commandCount) {
        continue;
      }
      break;
    }

    if (commandIndex < commandCount) {
      continue;
    }
    break;
  }

  // Publish completion, then wait for the peer to finish or disconnect.
  if (state->enabled != 0) {
    transferWords = (u16 *)gSerialTransfer.payload;
    transferWords[4] = 'E';
    transferWords[5] = 'D';
    transferWords[6] = 'V';
    transferWords[7] = 'S';
  }
  if (state->displayHandle != 0) {
    UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
  }
  Resource_ResetEntry(smallIconSlot);
  Resource_ResetEntry(largeIconSlot);
  Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
  if (state->enabled != 0) {
    // Keep the completion wait visible while the peer finishes its commands.
    s32 Link_DrawWaitingForOpponent(void) {
      u16 data[64];
      BATTLE_MENU_MESSAGE_COPY(MsgWaitingForOpponent, data,
                               BATTLE_MENU_MESSAGE_CAPACITY);
      UiText_RenderWideStringAtOffset(
          data, (struct UiWindow *)state->displayHandle, 0, 4);
    }

    linkBattle = gBattleWork;
    mode = 0;
    if (state->displayHandle == 0 && linkBattle->linkPaused == 0) {
      state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
      Ui_FillVramBlockPattern();
      Link_DrawWaitingForOpponent();
    } else {
      state->displayHandle = 0;
    }
    for (;;) {
      peerSignature = (u16 *)&gLinkPeerSignatures[linkBattle->linkSide ^ 1];
      if (linkBattle->linkPaused != 0) {
        commandCount = -1;
        break;
      }
      if ((gLinkStatus & 3) != 3) {
        mode++;
        if (mode > 24) {
          commandCount = -1;
          break;
        }
      } else {
        mode = 0;
        if (peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
            peerSignature[6] == 'V' && peerSignature[7] == 'S') {
          break;
        }
        if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
              peerSignature[6] == 'S' && peerSignature[7] == 'T')) {
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
  return commandCount;
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

// Draw the resource-menu icons, pulsing the selected icon with one affine
// matrix. Update the horizontal window spans to reveal the active icon row.
// UB: native pointers traverse scalar link/span fields into adjacent fields
// and records. Use their real owners under UBFIX. Plain OAM field accesses
// and indexed scanlines each shrink the EN body from 0x348 to 0x344 bytes.
void AffineEffect_UpdateFrame(void) {
  struct ResourceMenuWork *menu;
  struct ResourceMenuEntry *entry;
  struct AffineTransform transform;
  s32 pulseScale;
  s32 affineIndex;
  s32 packedSpan;

  s32 spriteY;
  u32 lastLine;
  u32 index;
#ifndef UBFIX
  u16 *spanCursor;
#endif
  struct DisplayTransitionState *transition;

  menu = gMenuSelectWork;
  entry = menu->entries;
  pulseScale = gUi_BounceScales[(gFrameCount * 2) & 31];
  pulseScale = (pulseScale - 256) / 4 + 304;
  transform.scaleX = pulseScale;
  transform.scaleY = pulseScale;
  transform.angle = 0;
  affineIndex = AffineMatrix_BuildForEffect(&transform);
  // Pulse only the selection; the other icons keep their normal OAM shape.
  for (index = 0; index < menu->count; index++, entry++) {
#ifndef UBFIX
    u32 *nativeAttributes;
#endif
    s32 drawLayer;
    s32 x;

    x = entry->x;
    if (x == 0)
      continue;
    if (index == menu->selection) {
      s32 left = x + pulseScale * 7 / 512 - 20;

      if (entry->y != 0)
        spriteY = entry->y + pulseScale * 3 / 256 - 20;
      else
        spriteY = (pulseScale * 15 / 256 - 30) & 255;
#ifdef UBFIX
      entry->link = 0;
      entry->attr01 = (affineIndex << 25) | spriteY | (left << 16) | 0x80002300;
      entry->attr2 = gVramBlockCache[entry->slot].offset >> 5;
#else
      nativeAttributes = &entry->link;
      *nativeAttributes++ = 0;
      *nativeAttributes++ =
          (affineIndex << 25) | spriteY | (left << 16) | 0x80002300;
      *nativeAttributes = gVramBlockCache[entry->slot].offset >> 5;
#endif
      drawLayer = 246;
    } else {
      u32 vramOffset;
      u32 packedAttributes;

#ifndef UBFIX
      nativeAttributes = &entry->link;
      /* FAKEMATCH: needed to match: reading the y halfword through attr
       * keeps the attribute-pointer copy before its load; sprite->y
       * moves the copy two instructions later. */
      spriteY = ((s16 *)nativeAttributes)[7];
      *nativeAttributes++ = 0;
#else
      spriteY = entry->y;
      entry->link = 0;
#endif
      packedAttributes = spriteY | (x << 16) | 0x80002000;
      vramOffset = gVramBlockCache[entry->slot].offset;
#ifdef UBFIX
      entry->attr01 = packedAttributes;
      entry->attr2 = vramOffset >> 5;
#else
      *nativeAttributes++ = packedAttributes;
      *nativeAttributes = vramOffset >> 5;
#endif
      drawLayer = 245;
    }
    Runtime_PushSlotEntry(entry, drawLayer);
  }
  // The top and bottom rows reveal opposite sides of the selected icon.
  if (menu->row == 0) {
    struct WindowBuffer *page;

    s32 halfWidth;

    transition = (struct DisplayTransitionState *)gDisplayTransition;
    if (transition == 0 || menu->count == 0)
      return;
    page = (struct WindowBuffer *)transition->pages[transition->page];
    packedSpan = menu->entries[menu->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    lastLine = menu->entries[menu->selection].y + 24;
    lastLine += (pulseScale * 32 - 7937) / 512 + 1;
#ifndef UBFIX
    spanCursor = &page->lines[24].span;
#endif
    index = 24;
    if (index < lastLine) {
      do {
#ifdef UBFIX
        page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
        *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
        index++;
#ifndef UBFIX
        spanCursor += 2;
#endif
      } while (index < lastLine);
    }
    packedSpan = menu->entries[0].x;
    if (menu->selection == 0)
      packedSpan -= (pulseScale * 12 - 2817) / 256;
    packedSpan <<= 8;
#ifndef UBFIX
    spanCursor = &page->lines[0].span;
#endif
    for (index = 0; index <= 23; index++) {
#ifdef UBFIX
      page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
      *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
#ifndef UBFIX
      spanCursor += 2;
#endif
    }
  } else {
    struct WindowBuffer *page;
    s32 firstLine;

    s32 halfWidth;

    transition = (struct DisplayTransitionState *)gDisplayTransition;
    if (transition == 0 || menu->count == 0)
      return;
    page = (struct WindowBuffer *)transition->pages[transition->page];
    packedSpan = menu->entries[menu->selection].x;
    halfWidth = (pulseScale * 12 - 2817) / 256;
    packedSpan =
        ((packedSpan - halfWidth) << 8) + (packedSpan + halfWidth) + 23;
    firstLine = menu->entries[menu->selection].y;
    firstLine -= (pulseScale * 32 - 7937) / 512 + 1;
#ifndef UBFIX
    spanCursor = &page->lines[firstLine].span;
#endif
    index = firstLine;
    if (index <= 135) {
      do {
#ifdef UBFIX
        page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
        *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
        index++;
#ifndef UBFIX
        spanCursor += 2;
#endif
      } while (index <= 135);
    }
    packedSpan = menu->entries[0].x;
    if (menu->selection == 0)
      packedSpan -= (pulseScale * 12 - 2817) / 256;
    packedSpan <<= 8;
#ifndef UBFIX
    spanCursor = &page->lines[136].span;
#endif
    for (index = 136; index <= 159; index++) {
#ifdef UBFIX
      page->lines[index].span = (page->lines[index].span & 0xff) | packedSpan;
#else
      *spanCursor = (*spanCursor & 0xff) | packedSpan;
#endif
#ifndef UBFIX
      spanCursor += 2;
#endif
    }
  }
}

// Allocate and clear the resource-menu state, then schedule its icon pulse.
// The matching allocation size is the complete six-entry work block.
void *AffineEffect_InitializeWork(void) {
  void *work;
  work = Runtime_AllocateBlock(58, 152);
  Dma_Fill32(0, work, 152);
  Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
  return work;
}

// Stop the icon pulse, close the label window and release each loaded icon
// before freeing the resource-menu state.
void Menu_EndResourceSelection(void) {
  struct UiWindow *labelWindow;
  s32 i;
  struct ResourceMenuWork *work;

  work = gMenuSelectWork;
  Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
  labelWindow = work->window;
  if (labelWindow != 0) {
    UiWork_Finalize(labelWindow, 2);
  }
  for (i = 0; i < work->count; i++) {
    Resource_ResetEntry(work->entries[i].slot);
  }
  Runtime_ReleaseHeapBlock(0x3A);
  WaitFrames(1U);
}

// Select an icon position with wrapped directional input. A returns the
// position; B or Start returns -1. Redraw the label only after a move.
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
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      return work->selection;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
    if (gKeysPressed & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
    if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP)) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN)) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}

// Return the distance between lhs and rhs, reusing their supplied difference.
static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
  if (diff >= 0)
    return diff;
  return rhs - lhs;
}

// Animate a cursor from start to goal with table-driven delays. Draw each
// intervening label, pause at the destination, and return its position.
s32 Menu_SelectResource(s32 start, s32 goal) {
  s16 resourceBase;
  s16 selection;
  s32 diff;
  s32 resourceId;
  s32 distance;
  s32 position;
  s32 step;
  s32 delay;
  const u8 *stepDelays;
  struct ResourceMenuWork *state;

  state = gMenuSelectWork;
  step = 1;
  delay = 12;
  state->selection = start;
  if (goal < start)
    step = -1;
  position = start;

  for (;;) {
    RenderOutput_PrepareForRedraw(state->window);
    resourceBase = state->resourceBase;
    if (resourceBase != 0) {
      resourceId = resourceBase + state->selection;
    } else {
      resourceId = state->resourceIds[state->selection] + MsgCommandName;
    }
    UiText_DrawCharacterAtOffset(resourceId, state->window, 0, 0);

    selection = state->selection;
    stepDelays = gMenu_SelectionStepDelays;
    diff = selection - goal;
    distance = AbsoluteDifference(diff, selection, goal);
    WaitFrames(stepDelays[distance] + delay);

    if (position == goal)
      break;

    state->selection += step;
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    delay = 0;
    position += step;
  }

  WaitFrames(48);
  AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
  return goal;
}

// Decode one command-icon table entry into the requested VRAM cache slot.
void Menu_LoadResourceSlot(s32 slot, s32 index) {
  s32 size = 1024;
  void *buffer = MemAlloc(size);
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  Resource_DecodeByteLz((void *)((u32)offsetTable + offsetTable[index]),
                        buffer);
  VramCache_Load(slot, size, buffer);
  MemFree(buffer);
}

// Append a command icon if fewer than six are present. Its initial position
// is on the bottom icon row; a layout routine places the final row.
void Menu_AppendResourceEntry(s32 icon) {
  struct ResourceMenuWork *work;
  struct ResourceMenuEntry *entry;
  s16 index;
  s32 slot;
  s32 y;

  work = gMenuSelectWork;
  index = work->count;
  if (index <= 5) {
    work->count++;
    entry = &work->entries[index];
    slot = Resource_FindFreeEntry();
    Menu_LoadResourceSlot(slot, icon);
    entry->x = index * 24 + 32;
    y = 136;
    entry->y = y;
    entry->slot = slot;
    work->resourceIds[index] = icon;
  }
}

// Center the icons and their label window together on the given tile row.
// A nonzero resourceBase supplies labels; otherwise use command-name IDs.
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

// Place icons at three-tile intervals from (x, y), then create their label
// window immediately to the right. Coordinates and label width are in tiles.
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

// Read the signed command-position mapping used by the top resource menu.
static __inline__ s32 ResourceMenu_ReadCommandPosition(s8 *table, s32 index) {
  return table[index];
}

// Build the field command menu, omitting Djinn when none are owned. Translate
// between command IDs and visible icon positions using the two lookup rows.
s32 Menu_SelectTopEntry(s32 selection) {
  s32 group = 0;
  s32 offset;
  s32 groupTriple;
  s32 result;
  s8 *commandPositions;

  if (Party_SumDjinnCountsFar(-1) == 0) {
    group = 1;
  }

  groupTriple = group * 3;
  commandPositions = gMenu_TopEntryPositionByCommand;
  offset = groupTriple << 1;
  selection =
      ResourceMenu_ReadCommandPosition(commandPositions, selection + offset) -
      1;
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
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();

  if (result >= 0) {
    result = gMenu_TopEntryCommandByPosition[result + offset + 1];
  }

  return result;
}

#if TBS_PATCH_1
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

// Open the field command menu and animate from start to the command's icon.
// Return the final icon position after closing the menu.
s32 Menu_AnimateSelectionToEntry(s32 start, s32 command) {
  UiWindow_OpenMode1AndWaitFrame();
  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(1);
  Menu_AppendResourceEntry(0xF);
  Menu_AppendResourceEntry(2);
  Menu_AppendResourceEntry(7);
  Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
  command = Menu_SelectResource(start, command - 1);
  Menu_EndResourceSelection();
  UiWork_CloseAndRelease();
  return command;
}

// Build the title menu from the save summaries and optional send flags.
// Translate the chosen visible position back to its save-action code.
s32 Menu_SelectSaveSlotAction(void) {
  s32 fileState;
  s32 result;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  fileState = SaveState_ScanRecordFlags();
  if (fileState < 0) {
    return -1;
  }
  if (fileState == 0) {
    return 0;
  }
  if (fileState == 3) {
    group = 1;
  } else if (fileState == 0x67) {
    group = 2;
  } else if (fileState > 0x64) {
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
  result = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (result >= 0) {
    result = gMenu_SaveSlotActionByPosition[result + (group * 6)];
  }
  return result;
}

#if TBS_PATCH_1
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

// Redraw the password-level explanation when the selected level changes.
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

#if TBS_PATCH_1
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)

// Redraw password/cable transfer help when the chosen method changes.
// The French layout keeps its distinct line splitting and redraw path.
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
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgPasswordTransferHelp + 1, state->lowerWindow,
                              TRANSFER_PASSWORD_HELP_X,
                              TRANSFER_PASSWORD_HELP_LAST_Y);
    } else {
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_FIRST_Y);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 1, state->lowerWindow, 0,
                              16);
      TRANSFER_MODE_DRAW_HELP(MsgCableTransferHelp + 2, state->lowerWindow, 0,
                              TRANSFER_CABLE_HELP_LAST_Y);
    }
  }
}
#endif

// Select either the transfer method (mode 0) or password level. Schedule the
// appropriate help redraw while the icon menu is active, then close both
// help windows and the icon menu.
s32 Menu_SelectResourceLayout(s32 mode) {
  struct ResourceMenuWork *work;
  struct UiWindow *window;
  s32 message;
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
#if (LANGUAGE == LANGUAGE_SPANISH)
    Menu_CenterResourceEntries(17, 5, 0);
#endif
  }
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  Menu_CenterResourceEntries(17, 7, 0);
#elif (LANGUAGE == LANGUAGE_SPANISH)
#elif (LANGUAGE == LANGUAGE_FRENCH)
  Menu_CenterResourceEntries(17, 9, 0);
#else
  Menu_CenterResourceEntries(17, 5, 0);
#endif
  if (mode != 0) {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
    work->previousMode = 0xffff;
    window = UiWindow_Create(RESOURCE_MODE0_WINDOW_X, 0,
                             RESOURCE_MODE0_WINDOW_WIDTH, 4, 2);
    message = MsgPasswordSelection;
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(message, window, 0, 4);
#if TBS_PATCH_1
    window = UiWindow_Create(RESOURCE_MODE1_WINDOW_X, 4,
                             RESOURCE_MODE1_WINDOW_WIDTH, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(message + 1, window, 8, 0);
    UiText_DrawResource(message + 2, work->lowerWindow, 8, 11);
    message += 3;
    UiText_DrawResource(message, work->lowerWindow, 8, 22);
#else
    window = UiWindow_Create(5, 4, 21, 12, 2);
    work->lowerWindow = window;
    UiText_DrawResource(message + 1, window, 0, 4);
    message += 2;
    UiText_DrawResource(message, work->lowerWindow, 0, 16);
#endif
  } else {
    Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
    work->previousMode = 0xffff;
    window = UiWindow_Create(TRANSFER_METHOD_WINDOW_X, 0,
                             TRANSFER_METHOD_WINDOW_WIDTH, 4, 2);
    upper = &work->upperWindow;
    *upper = window;
    UiText_DrawResource(MsgTransferMethod, window, TRANSFER_METHOD_LABEL_X, 4);
    work->lowerWindow = UiWindow_Create(TRANSFER_HELP_WINDOW_X, 5,
                                        TRANSFER_HELP_WINDOW_WIDTH, 7, 2);
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
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x11);
  Menu_AppendResourceEntry(0x12);
  Menu_AppendResourceEntry(0x13);
  Menu_AppendResourceEntry(0x14);
  Menu_CenterResourceEntries(17, ENTRY11_COMMAND_LABEL_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

//--------------------------------------------------------------------------
// Select icons 0x19-0x1c centered on row 17.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 selection) {
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x19);
  Menu_AppendResourceEntry(0x1A);
  Menu_AppendResourceEntry(0x1B);
  Menu_AppendResourceEntry(0x1C);
  Menu_CenterResourceEntries(17, ENTRY19_COMMAND_LABEL_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

//--------------------------------------------------------------------------
// Select Yes/No (icons 5/6); cancel returns 1 (No).
// low: 0 places the menu on row 0; otherwise row 17.
// message: first name message, or 0 for command names.
// w: name window width, or 0 for width 3.
// selection: initial position
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 bottom, s32 message, s32 labelWidth,
                             s32 selection) {
  s32 row;

  row = 0;

  AffineEffect_InitializeWork();

  if (labelWidth == 0) {
    labelWidth = 3;
  }
  if (bottom != 0) {
    row = 17;
  }

  Menu_AppendResourceEntry(5);
  Menu_AppendResourceEntry(6);
  Menu_CenterResourceEntries(row, labelWidth, message);

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
#if !TBS_PATCH_1
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// Select icons 0x20 and 0x21 centered on row 17.
// selection: initial position; return the selected position.
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 selection) {
  s32 result;

  AffineEffect_InitializeWork();
  Menu_AppendResourceEntry(0x20);
  Menu_AppendResourceEntry(0x21);
  Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
  result = Menu_RunResourceSelectionLoop(selection);
  Menu_EndResourceSelection();
  return result;
}

// Schedule the debug scene/entrance selector; 0xc80 is a scheduler order,
// not a number of frames to wait.
void Scheduler_ScheduleCallbackCAfterFrames(void) {
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(Menu_RunSelection, 0xC80);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
#endif
}

// Draw the debug scene and entrance numbers with their category/area labels.
void Menu_DrawSelectionRow(struct UiWindow *work, s32 scene,
                           const s16 *entrance) {
  s16 sceneIndex = scene;
  s32 label =
      Scene_GetAreaNameIndexFar(sceneIndex, *entrance) + MsgDebugEntryName;
  RenderOutput_PrepareForRedraw(work);
  UiText_DrawNumber(sceneIndex, 3, work, 0, 14);
  UiText_DrawNumber(*entrance, 3, work, MENU_LABEL_X, 14);
  UiText_DrawString(gMenu_ColonString, work, MENU_TEXT_X, 0);
  UiText_DrawResource(sceneIndex + MsgDebugCategoryName, work, 0, 0);
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
  s32 cursorSlot;
  s16 selectedField;
  s16 entrance;
  struct MenuSprite cursor;
  s16 scene;
  struct UiWindow *window;
  s16 result;

  window = 0;
  selectedField = 0;
  scene = gGameState.defaultScene;
  entrance = gGameState.defaultEntrance;
  window = UiWindow_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(window, scene, &entrance);
  UiTextResource_Initialize(&cursor, &cursorSlot);

  while (gKeysHeld != 0)
    WaitFrames(1);

  for (;;) {
    result =
        Menu_HandleSelectionRowInput(window, scene, &entrance, &selectedField);
    if (result == -1) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
      Event_SetDestinationFar(scene, entrance);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }
    if (result == -2) {
      UiTextResource_Release(cursorSlot);
      UiWork_Finalize(window, 2);
#ifdef UBFIX
      return;
#else
      return result;
#endif
    }

    UiTextResource_SetPosition(&cursor, MENU_TEXT_X,
                               selectedField * 14 + MENU_TEXT_Y);
    scene = result;
    WaitFrames(1);
  }
}

// Edit the debug scene or entrance, returning -1 to warp and -2 to cancel.
// Shoulder buttons step by ten; keep the original 99-step entrance wrap.
s32 Menu_HandleSelectionRowInput(struct UiWindow *window, s16 scene,
                                 s16 *entrance, s16 *selectedField) {
  if (gKeysRepeat & KEY_A)
    return -1;
  if (gKeysRepeat & KEY_B)
    return -2;
  if (gKeysRepeat & KEY_DOWN) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_UP) {
    *selectedField ^= 1;
  } else if (gKeysRepeat & KEY_RIGHT) {
    if (*selectedField == 0) {
      scene++;
    } else {
      (*entrance)++;
      if (*entrance > 99)
        *entrance = 0;
    }
    if (scene > 200)
      scene = 0;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_LEFT) {
    if (*selectedField == 0) {
      scene--;
    } else {
      (*entrance)--;
      if (*entrance < 0)
        *entrance = 99;
    }
    if (scene < 0)
      scene = 200;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_R) {
    if (*selectedField == 0) {
      *entrance = 0;
      scene += 10;
    } else {
      *entrance += 10;
      if (*entrance > 99)
        *entrance -= 99;
    }
    if (scene > 200)
      scene = 0;
    Menu_DrawSelectionRow(window, scene, entrance);
  } else if (gKeysRepeat & KEY_L) {
    if (*selectedField == 0) {
      *entrance = 0;
      scene -= 10;
    } else {
      *entrance -= 10;
      if (*entrance < 0)
        *entrance += 99;
    }
    if (scene < 0)
      scene = 200;
    Menu_DrawSelectionRow(window, scene, entrance);
  }
  return scene;
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
  struct MenuSprite cursorSprite; // Cursor object
  s32 cursor[2];                  // Cursor column and row
  s32 cursorSlot;
  s32 page;
  struct UiWindow *window;
  s32 change;

  page = 8;
  window = 0;
  cursor[0] = 0;
  cursor[1] = 0;
  window = UiWindow_Create(1, 0, 28, 20, 2);
  Menu_DrawFlagBitTable(window, page);
  UiTextResource_Initialize(&cursorSprite, &cursorSlot);
  for (;;) {
    WaitFrames(1);
    change = Menu_HandleFlagGridInput(window, &page, cursor);
    if (change == -1)
      break; // Close
    if (change == 1)
      Menu_DrawFlagBitTable(window, page); // Flag or page changed
    UiTextResource_SetPosition(&cursorSprite, cursor[0] * 8 + 58,
                               cursor[1] * 8 + 20);
  }
  UiTextResource_Release(cursorSlot);
  UiWork_Finalize(window, 2);
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
  u8 digits[8];
  u32 i;

  if (count > 5)
    count = 5;

  for (i = 0; i != count; i++) {
    u32 digit = value & 15;
    if (digit <= 9)
      digits[i] = digit + '0';
    else
      digits[i] = digit + 'A' - 10;
    value >>= 4;
  }

  for (i = count - 1; (s32)i >= 0; i--)
    *out++ = digits[i];
}

// Draw one page of 256 flags as sixteen rows of hexadecimal labels and bits.
void Menu_DrawFlagBitTable(struct UiWindow *window, s32 page) {
  s32 row;
  s32 y;
  s32 flag;
  char label[5];
  char bits[17];

  RenderOutput_PrepareForRedraw(window);
  UiText_DrawStringInWindow((u8 *)gMenu_HexDigitsString, window, 48, 0);

  flag = page << 8;
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
      s32 set = GameFlag_TestFar(flag);
      bits[i] = (set != 0) + 48;
      flag++;
    }
    bits[i] = 0;
    UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
  }
}

// Move within the flag grid, change pages, or toggle the selected flag.
// Return 1 when flags/page need redrawing, -1 to close, and 0 otherwise.
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

// Reserved menu entry; it intentionally performs no work.
void Menu_ReservedNoOp(void) {}

// Set the Djinni's collected flag, then add it to the specified owner.
void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index) {
  GameFlag_SetBitFar((element * 0x14) + index + FLAG_DJINNI_0);
  Djinn_AddToOwnerFar(owner, element, index);
}

// Handle Save, Sleep and Options from the field workspace. A cancelled save
// or options screen reopens the workspace; cancelling the workspace returns -1.
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

// Browse pages of item, Psynergy and status-pattern icons. Suspend the menu
// controller while the browser owns its window and cache entries.
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
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else if (mode == 1) {
          UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        } else {
          slot = Resource_FindFreeEntry();
          Ui_BuildPatternToSlot(i, 0, slot);
          RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
        }
      }
    }
    WaitFrames(1);
  }
  UiWork_Finalize(window, 2);
  gMenuCtrlWork->suspended = 0;
  return 0;
}

#if !TBS_PATCH_2
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

// Browse the character and sprite portrait maps as one circular list. Show
// each entry's glyph and label, then release both windows on A or B.
s32 DebugMenu_BrowseEntryGlyphs(void) {
  struct UiRenderWork *work;
  struct UiWindow *portrait;
  s32 redraw;
  struct UiWindow *window;
  s32 characterCount;
  s32 entryCount;
  s32 index;
  s32 i;
  s32 glyph;
  s32 tile;
  s32 slot;
  s32 cancelKey;
  u32 slotOffset;

  work = UI_WORK;
  redraw = 1;
  portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
  window = UiWindow_Create(10, 10, 14, 3, 2);
  index = 0;
  for (i = 0; gPortrait_CharacterMap[i * 2] != -1; i++) {
  }
  characterCount = i;
  for (i = 0; gPortrait_SpriteMap[i * 2] != -1; i++) {
  }
  entryCount = characterCount + i;

  /* FAKEMATCH: the count-derived mask is needed to match the native mask
   * in r9 and work in fp; a direct constant swaps them (four instructions).
   * 2026-10-13: cancel = KEY_B differs 12 of 179, KEY_B tested directly
   * 20, cancel = KEY_B set first in the function 48. */
  cancelKey = ((u32)(u16)characterCount >> 16) | KEY_B;
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
    if (gKeysRepeat & cancelKey)
      break;
    if (redraw) {
      redraw = 0;
      index = (index + entryCount) % entryCount;
      RenderOutput_PrepareForRedraw(window);
      if (index < characterCount)
        glyph = gPortrait_CharacterMap[index * 2 + 1];
      else
        glyph = gPortrait_SpriteMap[(index - characterCount) * 2 + 1] + 128;
      {
        /* FAKEMATCH: the word temporary is needed to match palette's
         * r2 through the slot store; direct assignment chooses r3.
         * 2026-10-13: slot assigned directly with 15 passed as a
         * literal differs 4 of 179 (the 15 takes r3 after the slot
         * store); so do the loaded temporary alone, the load
         * written as an index and the assignment inside the
         * argument list; pal alone, without loaded, differs 69. */
        s32 palette;
        s32 loadedSlot = *(u16 *)((u8 *)work + slotOffset);
        palette = 15;
        slot = loadedSlot;
        UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, palette, 1);
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
