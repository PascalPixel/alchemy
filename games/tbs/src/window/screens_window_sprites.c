/// @file
/// Window sprites, portraits and the window spans.
// Part of screens.c.

#include "screens_internal.h"

/// Attribute 0's eight bits of y: a sprite above the screen is at 256 less its
/// distance.
#define OBJ_Y_MASK 0xff

/// A line's window edges with nothing between them: 224 up to 224, so the
/// window covers none of the line.
#define WINDOW_SPAN_EMPTY WIN_RANGE(224, 224)

/// Creates a window sprite for the VRAM cache slot `slot` and attaches it to
/// `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
/// (one tile in from its top-left border). `oamFlags` holds the OAM shape and
/// size bits.
struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct WindowSprite *sprite;
  s32 y;

  sprite = WindowSprite_AcquireFree();
  if (sprite == NULL) {
    VramCache_FreeEntry(slot);
    return NULL;
  }

  // Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y.
  x = offsetX + (window->x * 8) + 8;
  y = offsetY + (window->y * 8) + 8;
  x &= OBJ_X_MASK;
  y &= OBJ_Y_MASK;
  // UB: attributes 0 and 1, then attribute 2, are written as words through
  // the bitfields; UBFIX copies the words.
#ifdef UBFIX
  {
    u32 words[2];

    words[0] = OBJ_X(x) | y | oamFlags;
    // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
    words[1] =
        ((struct VramCacheEntry *)Ram_VramCacheEntries)[slot].offset >> 5;
    __builtin_memcpy(&sprite->oam, words, sizeof(words));
  }
#else
  *(u32 *)&sprite->oam = OBJ_X(x) | y | oamFlags;
  // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
  ((u32 *)&sprite->oam)[1] =
      ((struct VramCacheEntry *)Ram_VramCacheEntries)[slot].offset >> 5;
#endif
  sprite->depth = OAM_DEPTH_FRONT;
  sprite->next = NULL;
  sprite->x = x;
  sprite->y = y;
  sprite->index = slot;
  // The sprite owns the slot, which is freed with it.
  sprite->kind = WINDOW_SPRITE_ICON;
  sprite->motion = WINDOW_SPRITE_MOTION_STILL;
  WindowSprite_AppendToList(&window->sprites, sprite);
  return sprite;
}

/// Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
/// it as a 16x16 sprite at (x, y) in `window`.
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
/// slot and shows it as a 16x16 sprite at (x, y) in `window`.
struct WindowSprite *WindowSprite_CreateLoaded(s32 icon, s32 layers,
                                               struct Window *window, s32 x,
                                               s32 y) {
  s32 slot;
  struct WindowSprite *sprite;

  slot = VramCache_FindFreeEntry();
  sprite = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Icon_CopyResourceToSlot(icon, layers, slot);
    sprite = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return sprite;
}

/// Builds a 16x16 icon from frame pattern 1 with overlay pattern `icon` drawn
/// over it, and shows it at (x, y) in `window` on draw layer 251.
struct WindowSprite *WindowSprite_CreateWithTransform(s32 icon,
                                                      struct Window *window,
                                                      s32 x, s32 y) {
  s32 slot;
  s32 unusedTile;
  struct WindowSprite *sprite;

  slot = VramCache_FindFreeEntry();
  if (slot == VRAM_CACHE_ENTRY_COUNT) {
    return NULL;
  }
  Icon_BuildCommandIconTiles(icon, 1, &slot, &unusedTile, 1);
  sprite = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  // Not checked: with every sprite in use, this stores through NULL.
  sprite->depth = OAM_DEPTH_FRONT - 4;
  return sprite;
}

/// Loads the pattern at the start of gMenuTitlePictures into a free VRAM slot
/// and shows it as a 16x16 sprite at (x, y) in `window`. `tableEntry` is passed
/// on to the loader, which loads that same pattern for every value.
struct WindowSprite *WindowSprite_CreateFromTable(s32 tableEntry,
                                                  struct Window *window, s32 x,
                                                  s32 y) {
  s32 slot;
  struct WindowSprite *sprite;

  slot = VramCache_FindFreeEntry();
  sprite = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadTableEntry(tableEntry, 0, slot);
    sprite = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return sprite;
}

/// Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
/// slot `side` (0 or 1, or any other value for the first free one; the second
/// is tried first).
struct WindowSprite *Portrait_Create(s32 character, s32 unused, s32 side,
                                     struct Window *window, s32 x, s32 y) {
  struct WindowState *state = WINDOW_STATE;
  struct WindowSprite *portrait = NULL;
  s32 slot;
  s32 tile;
  s32 icon;
  s32 palette;

  if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0) {
    if (character == ISAAC)
      character = SPRITE_ISAAC_CHILD;
    if (character == GARET)
      character = SPRITE_GARET_CHILD;
  }

  icon = Portrait_GetForSpeaker(character);
  if (icon == -1)
    return portrait;

  // No side given: the second slot if it is free, else the first.
  if ((u32)side > 1) {
    side = 1;
    if (state->portraitPictures[1] != PORTRAIT_SLOT_EMPTY) {
      side = 0;
      if (state->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
        return portrait;
    }
  }

  palette = PORTRAIT_PALETTE + side;
  Icon_LoadWithPalette(icon, unused, &slot, &tile, palette, 0);
  portrait = WindowSprite_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
  if (portrait != NULL) {
    portrait->oam.palette = palette;
    portrait->kind = WINDOW_SPRITE_PORTRAIT;
  }

  // Recorded even when no sprite was free and WindowSprite_Create released
  // the VRAM slot.
  state->portraitPictures[side] = icon;
  state->portraitSprites[side] = slot;
  return portrait;
}

/// Reloads the portrait image of `character` into portrait slot `side` (0 or
/// 1), keeping its sprite: used to change the picture of a portrait already on
/// screen.
void Portrait_Reload(u32 side, s32 character, s32 unused) {
  s32 tile;
  s32 slot;
  u32 icon;
  struct WindowState *state;

  state = WINDOW_STATE;

  if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0) {
    if (character == ISAAC)
      character = SPRITE_ISAAC_CHILD;
    if (character == GARET)
      character = SPRITE_GARET_CHILD;
  }

  icon = Portrait_GetForSpeaker(character);
  if (icon != -1U) {
    if (side > 1U) {
      if (state->portraitPictures[1] == icon) {
        side = 1;
      } else if (state->portraitPictures[0] == icon) {
        side = 0;
      } else {
        return;
      }
    }
    slot = state->portraitSprites[side];
    Icon_LoadWithPalette(icon, unused, &slot, &tile, PORTRAIT_PALETTE + side,
                         1);
  }
}

/// Switches `sprite` to motion `mode` (WINDOW_SPRITE_MOTION_) from its first
/// step. Does nothing when `sprite` is NULL.
void WindowSprite_SetMotion(struct WindowSprite *sprite, s32 mode) {
  if (sprite != NULL) {
    sprite->motion = mode;
    sprite->animationStep = 0;
  }
}

/// Sets the draw layer of the window sprite `sprite` to 255 - `priority`, so a
/// higher priority draws on a lower layer. Does nothing when `sprite` is NULL.
void WindowSprite_SetInvertedSentinel(void *sprite, s32 priority) {
  if (sprite != NULL)
    ((struct WindowSprite *)sprite)->depth = ~priority;
}

typedef void (*WindowSpanRoutine)(u16 *spans, u16 *tilemap);

/// Fills `spans`, a page of the screen transition's window edges past its WININ
/// and WINOUT, with the windows' horizontal extent on each of the 160 scan
/// lines: (left << 8) | right in the second halfword (WIN1H) of each line's
/// pair, read from the UI tilemap by the ARM routine Tile_BuildCellPictures,
/// copied into RAM for the call.
void ScanlineWindow_FillSpans(void *spans) {
  void *tilemap = gWindowState[0];

  if (tilemap == NULL) {
    // BUG: the fill is one halfword for each of the DISPLAY_HEIGHT lines, but
    // each line takes two, so the lower 80 lines keep the window 1 edges
    // this page held when it was last built.
#ifdef BUGFIX
    Dma_Fill16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 4);
#else
    Dma_Fill16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 2);
#endif
  } else {
    WindowSpanRoutine routine;

    RamCode_Load(routine, Tile_BuildCellPictures,
                 Tile_BuildCellPicturesCodeSize);
    routine(spans, tilemap);
    Heap_Release(routine);
  }
}

/// Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, 32 tiles
/// wide, with the tilemap entry `value`.
void BgScreen_FillRect(s32 x, s32 y, u32 width, u32 height, s32 value) {
  u32 row, column;
  s16 *dst = (s16 *)WINDOW_TILEMAP_VRAM;

  for (row = 0; row < height; row++) {
    for (column = 0; column < width; column++)
      *dst++ = value;
    dst += BG_SCREEN_WIDTH - width;
  }
}
