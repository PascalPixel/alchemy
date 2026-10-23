/// @file
/// Window sprites, portraits and the window spans.
// Part of battle_command.c.

#include "../battle_command_internal.h"

/// Creates a window sprite for the VRAM cache slot `slot` and attaches it to
/// `window`, placed at (offsetX, offsetY) pixels from the window's inner
/// corner (one tile in from its top-left border). `oamFlags` holds the OAM
/// shape and size bits. The sprite draws on layer 255 until the caller changes
/// it. Returns the sprite, or NULL with the VRAM slot released when all
/// window sprites are in use.
struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct WindowSprite *output;
  s32 y;

  output = WindowSprite_AcquireFree();
  if (output == NULL) {
    VramCache_FreeEntry(slot);
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
        ((struct VramCacheEntry *)Ram_VramCacheEntries)[slot].offset >> 5;
    __builtin_memcpy(&output->oam, words, sizeof(words));
  }
#else
  *(u32 *)&output->oam = (x << 16) | y | oamFlags;
  // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
  ((u32 *)&output->oam)[1] =
      ((struct VramCacheEntry *)Ram_VramCacheEntries)[slot].offset >> 5;
#endif
  output->sentinel = 0xff;
  output->next = NULL;
  output->x = x;
  output->y = y;
  output->index = slot;
  // Kind 1: the sprite owns the slot, which is freed with it. Its motion is
  // WINDOW_SPRITE_MOTION_STILL.
  output->kind = WINDOW_SPRITE_ICON;
  output->motion = 1;
  WindowSprite_AppendToList(&window->sprites, output);
  return output;
}

/// Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
/// it as a 16x16 sprite at (x, y) in `window`. Returns NULL when it cannot be
/// loaded. The field menus' item and Psynergy icons come through here, by far
/// call (menu/menu).
struct WindowSprite *WindowSprite_CreateFromResource(s32 mode, s32 resource,
                                                     struct Window *window,
                                                     s32 x, s32 y) {
  s32 slot;

  slot = Resource_LoadByMode(mode, resource);
  if (slot < 0) {
    return NULL;
  }
  return WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
}

/// Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
/// slot and shows it as a 16x16 sprite at (x, y) in `window`. Returns NULL
/// when no slot is free. The shops' item icons come through here, by far
/// call.
struct WindowSprite *WindowSprite_CreateLoaded(s32 icon, s32 layers,
                                               struct Window *window, s32 x,
                                               s32 y) {
  s32 slot;
  struct WindowSprite *output;

  slot = VramCache_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Icon_CopyResourceToSlot(icon, layers, slot);
    output = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return output;
}

/// Builds a 16x16 icon from frame pattern 1 with overlay pattern `icon` drawn
/// over it, and shows it at (x, y) in `window` on draw layer 251. Returns NULL
/// when no VRAM slot is free. Nothing calls it.
struct WindowSprite *WindowSprite_CreateWithTransform(s32 icon,
                                                      struct Window *window,
                                                      s32 x, s32 y) {
  s32 slot;
  s32 unusedTile;
  struct WindowSprite *output;

  slot = VramCache_FindFreeEntry();
  if (slot == VRAM_CACHE_ENTRY_COUNT) {
    return NULL;
  }
  Ui_BuildPairedPatternsToSlot(icon, 1, &slot, &unusedTile, 1);
  output = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  // Not checked: with every sprite in use, this stores through NULL.
  output->sentinel = 251;
  return output;
}

/// Loads the pattern at the start of gHeadingPictures into a
/// free VRAM slot and shows it as a 16x16 sprite at (x, y) in `window`.
/// `tableEntry` is passed on to the loader, which loads that same pattern for
/// every value. Returns NULL when no slot is free.
struct WindowSprite *WindowSprite_CreateFromTable(s32 tableEntry,
                                                  struct Window *window, s32 x,
                                                  s32 y) {
  s32 slot;
  struct WindowSprite *output;

  slot = VramCache_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    RenderResource_LoadTableEntry(tableEntry, 0, slot);
    output = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return output;
}

/// Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
/// slot `side` (0 or 1, or any other value for the first free one; the second
/// is tried first). Each slot has its own palette, 14 or 15. In the prologue,
/// characters 0 and 1 show portraits 18 and 19 instead. `unused` is
/// passed on to the loader, which ignores it. Returns the sprite, or NULL when
/// the character has no portrait or both slots are taken. The slot records
/// the portrait and its VRAM slot (WindowWork.portraitPictures and
/// portraitSprites); the message windows' speaker portraits come through here
/// (window/messages.c).
struct WindowSprite *CreateSideObject(s32 character, s32 unused, s32 side,
                                      struct Window *window, s32 x, s32 y) {
  struct WindowWork *work = WINDOW_WORK;
  struct WindowSprite *portrait = NULL;
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

  icon = Portrait_GetForSpeaker(character);
  if (icon == -1)
    return portrait;

  // No side given: the second slot if it is free, else the first.
  if ((u32)side > 1) {
    side = 1;
    if (work->portraitPictures[1] != PORTRAIT_SLOT_EMPTY) {
      side = 0;
      if (work->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
        return portrait;
    }
  }

  palette = PORTRAIT_PALETTE + side;
  UiGlyph_LoadEntryWithPalette(icon, unused, &slot, &tile, palette, 0);
  portrait = WindowSprite_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
  if (portrait != NULL) {
    portrait->oam.palette = palette;
    portrait->kind = WINDOW_SPRITE_PORTRAIT;
  }

  // Recorded even when no sprite was free and WindowSprite_Create released
  // the VRAM slot.
  work->portraitPictures[side] = icon;
  work->portraitSprites[side] = slot;
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
  struct WindowWork *work;

  work = WINDOW_WORK;

  if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
    if (character == 0)
      character = 18;
    if (character == 1)
      character = 19;
  }

  icon = Portrait_GetForSpeaker(character);
  if (icon != -1U) {
    if (side > 1U) {
      if (work->portraitPictures[1] == icon) {
        side = 1;
      } else if (work->portraitPictures[0] == icon) {
        side = 0;
      } else {
        return;
      }
    }
    slot = work->portraitSprites[side];
    UiGlyph_LoadEntryWithPalette(icon, unused, &slot, &tile,
                                 PORTRAIT_PALETTE + side, 1);
  }
}

/// Switches the window sprite `out` to animation `mode` from its first step.
/// Does nothing when `out` is NULL.
void WindowSprite_SetMotion(struct WindowSprite *out, s32 mode) {
  if (out != NULL) {
    out->motion = mode;
    out->animationStep = 0;
  }
}

/// Sets the draw layer of the window sprite `output` to 255 - `priority`, so a
/// higher priority draws on a lower layer. Does nothing when `output` is NULL.
void WindowSprite_SetInvertedSentinel(void *output, s32 priority) {
  if (output != NULL)
    ((struct WindowSprite *)output)->sentinel = ~priority;
}

typedef void (*WindowSpanRoutine)(u16 *spans, u16 *tilemap);

/// Fills `spans`, a page of the screen transition's window edges past its
/// WININ and WINOUT, with the window frames' horizontal extent on each of the
/// 160 scan lines: (left << 8) | right in the second halfword (WIN1H) of each
/// line's pair, read from the UI tilemap by the ARM routine
/// Tile_BuildMetatiles, copied into RAM for the call. Each frame of a window
/// transition calls it (ScreenTransition_UpdateScanlineTable). With no UI
/// work it writes 0xe0e0, an empty extent, over 0x140 bytes: both halfwords
/// of the first 80 lines only.
/// XXX: likely meant to cover all 160 lines, because each line takes 4 bytes;
/// what the lower half then shows is not established.
void Win_FillScanlineSpans(void *spans) {
  void *tilemap = gWindowWork[0];

  if (tilemap == NULL) {
    Dma_Fill16(0xe0e0, spans, 0x140);
  } else {
    WindowSpanRoutine routine;

    RamCode_Load(routine, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
    routine(spans, tilemap);
    Free(routine);
  }
}

/// Fills the top-left `width` x `height` tiles of the 32-tile-wide BG screen
/// block at 0x06002000 with the tilemap entry `value`. The first two
/// parameters are ignored.
void Win_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                             s32 value) {
  u32 row = 0;
  s16 *dst = (s16 *)WINDOW_TILEMAP_VRAM;

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
