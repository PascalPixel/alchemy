/// @file
/// Sprite outputs, portraits and the window spans.
/// Part of battle_command.c, the menu object, compiled only through it.

#include "../battle_command_internal.h"

/// Creates a sprite output for the VRAM cache slot `slot` and attaches it to
/// `window`, placed at (offsetX, offsetY) pixels from the window's inner
/// corner (one tile in from its top-left border). `oamFlags` holds the OAM
/// shape and size bits. The sprite draws on layer 255 until the caller changes
/// it. Returns the output, or NULL with the VRAM slot released when all
/// outputs are in use.
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
  // UB: attributes 0 and 1, then attribute 2, are written as words through
  // the bitfields; UBFIX copies the words.
#ifdef UBFIX
  {
    u32 words[2];

    words[0] = (x << 16) | y | oamFlags;
    // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
    words[1] =
        ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
    __builtin_memcpy(&output->oam, words, sizeof(words));
  }
#else
  *(u32 *)&output->oam = (x << 16) | y | oamFlags;
  // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
  ((u32 *)&output->oam)[1] =
      ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
#endif
  output->sentinel = 0xff;
  output->next = NULL;
  output->x = x;
  output->y = y;
  output->index = slot;
  // Kind 1: the output owns the slot, which is freed with it. Its motion is
  // UI_SPRITE_STILL.
  output->kind = 1;
  output->active = 1;
  RenderOutput_AppendToList(&window->output, output);
  return output;
}

/// Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
/// it as a 16x16 sprite at (x, y) in `window`. Returns NULL when it cannot be
/// loaded. The field menus' item and Psynergy icons come through here, by far
/// call (menu/menu).
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

/// Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
/// slot and shows it as a 16x16 sprite at (x, y) in `window`. Returns NULL
/// when no slot is free. The shops' item icons come through here, by far
/// call.
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

/// Builds a 16x16 icon from frame pattern 1 with overlay pattern `icon` drawn
/// over it, and shows it at (x, y) in `window` on draw layer 251. Returns NULL
/// when no VRAM slot is free. Nothing calls it.
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
  // Not checked: with every output in use, this stores through NULL.
  output->sentinel = 251;
  return output;
}

/// Loads the pattern at the start of gRenderResource_PairSourceTable into a
/// free VRAM slot and shows it as a 16x16 sprite at (x, y) in `window`.
/// `tableEntry` is passed on to the loader, which loads that same pattern for
/// every value. Returns NULL when no slot is free.
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

/// Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
/// slot `side` (0 or 1, or any other value for the first free one; the second
/// is tried first). Each slot has its own palette, 14 or 15. In the prologue,
/// characters 0 and 1 show portraits 18 and 19 instead. `unused` is
/// passed on to the loader, which ignores it. Returns the sprite, or NULL when
/// the character has no portrait or both slots are taken. The slot records
/// the portrait and its VRAM slot (UiRenderWork.resourceIds and outputIds);
/// the message windows' speaker portraits come through here
/// (window/messages.c).
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

  // No side given: the second slot if it is free, else the first.
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

  // Recorded even when no output was free and RenderOutput_Create released
  // the VRAM slot.
  work->resourceIds[side] = icon;
  work->outputIds[side] = slot;
  return portrait;
}

/// Reloads the portrait image of `character` into portrait slot `side` (0 or
/// 1), keeping its sprite: used to change the picture of a portrait already on
/// screen. With any other `side`, the slot already showing that character's
/// portrait is used, and nothing happens when none does. `unused` is passed on
/// to the loader, which ignores it. The field menus reload portrait slot 0
/// with it by far call (menu/menu/common.c).
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

/// Switches the sprite output `out` to animation `mode` from its first step.
/// Does nothing when `out` is NULL.
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode) {
  if (out != NULL) {
    out->active = mode;
    out->animStep = 0;
  }
}

/// Sets the draw layer of the sprite output `output` to 255 - `priority`, so a
/// higher priority draws on a lower layer. Does nothing when `output` is NULL.
void RenderOutput_SetInvertedSentinel(void *output, s32 priority) {
  if (output != NULL)
    ((struct RenderOutput *)output)->sentinel = ~priority;
}

typedef void (*WindowSpanRoutine)(u16 *spans, u16 *tilemap);

/// Fills `spans`, a page of the display transition's window edges past its
/// WININ and WINOUT, with the window frames' horizontal extent on each of the
/// 160 scan lines: (left << 8) | right in the second halfword (WIN1H) of each
/// line's pair, read from the UI tilemap by the ARM routine
/// Tile_BuildMetatiles, copied into RAM for the call. Each frame of a window
/// transition calls it (DisplayTransition_UpdateScanlineTable). With no UI
/// work it writes 0xe0e0, an empty extent, over 0x140 bytes: both halfwords
/// of the first 80 lines only.
/// XXX: likely meant to cover all 160 lines, because each line takes 4 bytes;
/// what the lower half then shows is not established.
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

/// Fills the top-left `width` x `height` tiles of the 32-tile-wide BG screen
/// block at 0x06002000 with the tilemap entry `value`. The first two
/// parameters are ignored.
void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width,
                                  u32 height, s32 value) {
  u32 row = 0;
  s16 *dst = (s16 *)UI_TILEMAP_VRAM;

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
