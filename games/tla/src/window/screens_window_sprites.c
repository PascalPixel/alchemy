/// @file
/// Window sprites, portraits and the window spans.
// Part of screens.c.

#include "window_internal.h"
#include "window/screens.h"

/// The bits of a sprite's OAM x (nine) and y (eight), which wrap round the
/// screen.
#define OAM_X_MASK 0x1ff
#define OAM_Y_MASK 0xff

/// Creates a window sprite for the VRAM cache slot `slot` and attaches it to
/// `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
/// (one tile in from its top-left border). `oamFlags` holds the OAM shape and
/// size bits. Releases the slot and returns NULL when no sprite is free.
struct WindowSprite *WindowSprite_Create(s32 slot, s32 oamFlags,
                                         struct Window *window, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct WindowSprite *sprite;
  s32 y;

  sprite = WindowSprite_AcquireFree();
  if (sprite == NULL) {
    VramCache_FreeEntry((u32)slot);
    return NULL;
  }
  // Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y.
  x = offsetX + (s16)window->x * 8 + 8;
  y = offsetY + (s16)window->y * 8 + 8;
  x &= OAM_X_MASK;
  y &= OAM_Y_MASK;
  *(u32 *)&sprite->sprite.oam = OBJ_X(x) | y | oamFlags;
  // Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
  ((u32 *)&sprite->sprite.oam)[1] = gVramCacheEntries[slot].offset >> 5;
  sprite->depth = OAM_DEPTH_FRONT - 1;
  sprite->x = x;
  sprite->y = (s16)y;
  sprite->next = NULL;
  sprite->index = (s8)slot;
  // An icon owns its VRAM cache slot, which is freed with it.
  sprite->kind = WINDOW_SPRITE_ICON;
  sprite->motion = WINDOW_SPRITE_MOTION_STILL;
  WindowSprite_AppendToList((struct WindowSpriteList *)window, sprite);
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

/// Unused: builds the 16x16 command icon of overlay `icon` over base 1
/// (Icon_BuildCommandIconTiles) and shows it at (x, y) in `window`, in front
/// of the other window sprites. Nothing calls it.
UNUSED struct WindowSprite *
WindowSprite_CreateCommandIcon(s32 icon, struct Window *window, s32 x, s32 y) {
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

/// Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
/// slot `side` (0 or 1, or any other value for the first free one; the second
/// is tried first), in PORTRAIT_PALETTE + side.
struct WindowSprite *Portrait_Create(s32 character, s32 position, s32 side,
                                     struct Window *window, s32 x, s32 y) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct WindowSprite *portrait = 0;
  s32 slot;
  s32 tile;
  s32 icon;
  s32 palette;

  icon = Portrait_GetForSpeaker(character);
  if (icon == -1)
    return portrait;

  // No side given: the second slot if it is free, else the first.
  if ((u32)side > 1) {
    u32 current;

    current = work->portraitPictures[1];
    side = 1;
    if (current != PORTRAIT_SLOT_EMPTY) {
      side = 0;
      if (work->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
        return portrait;
    }
  }

  palette = PORTRAIT_PALETTE + side;
  Icon_LoadWithPalette(icon, position, &slot, &tile, palette, 0);
  portrait = WindowSprite_Create(slot, OUTPUT_SIZE_32X32,
                                 (struct Window *)window, x, y);
  if (portrait != 0) {
    portrait->sprite.oam.palette = palette;
    portrait->kind = WINDOW_SPRITE_PORTRAIT;
  }

  // Recorded even when no sprite was free and WindowSprite_Create released
  // the VRAM slot.
  work->portraitPictures[side] = icon;
  work->portraitSprites[side] = slot;
  return portrait;
}

/// Reloads the portrait picture for the key into a portrait slot with that
/// slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache
/// entry. A slot of 2 or more means whichever slot already shows the picture,
/// the second first; when neither does, nothing is loaded.
void Portrait_Reload(u32 slot, s32 key, s32 unused) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 id = Portrait_GetForSpeaker(key);
  s32 tile;
  s32 entry;

  if (id == -1)
    return;
  if (slot > 1) {
    if (work->portraitPictures[1] == id)
      slot = 1;
    else if (work->portraitPictures[0] == id)
      slot = 0;
    else
      return;
  }
  entry = work->portraitSprites[slot];
  Icon_LoadWithPalette(id, unused, &entry, &tile, PORTRAIT_PALETTE + slot, 1);
}

/// Sets a window sprite's motion and starts it from its first step; does
/// nothing for NULL.
void WindowSprite_SetMotion(struct WindowSprite *out, s32 mode) {
  if (out != NULL) {
    out->motion = mode;
    out->animationStep = 0;
  }
}

/// Sets the depth of the window sprite `sprite` to ~priority, which is
/// OAM_DEPTH_FRONT - `priority`: a higher priority draws further back. Does
/// nothing when `sprite` is NULL.
void WindowSprite_SetInvertedSentinel(void *sprite, s32 priority) {
  if (sprite != NULL)
    ((struct WindowSprite *)sprite)->depth = ~priority;
}

/// A line's window span with no window: left and right both at 224.
#define WINDOW_SPAN_EMPTY WIN_RANGE(224, 224)

/// The ARM routine ScanlineWindow_FillSpans copies to RAM and runs; it returns
/// the end of the spans it wrote.
typedef u16 *(*WindowSpanRoutine)(u16 *spans, u16 *tilemap);

/// Fills `spans`, a page of the screen transition's window edges past its WININ
/// and WINOUT, with the windows' horizontal extent on each of the 160 scan
/// lines: (left << 8) | right in the second halfword (WIN1H) of each line's
/// pair, read from the UI tilemap by the ARM routine Tile_BuildCellPictures
/// (cell_picture.c), copied into RAM for the call. With no window work, every
/// line gets an empty span.
void ScanlineWindow_FillSpans(void *spans) {
  struct WindowWork *work = Ram_HeapSlots->windowWork;

  if (work == NULL) {
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

    RamCode_Load(routine, Tile_BuildCellPicturesCode,
                 Tile_BuildCellPicturesCodeSize);
    routine(spans, work->tilemap);
    Heap_Release(routine);
  }
}

/// Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, a
/// BG_SCREEN_WIDTH-tile map, with the tilemap entry `value`.
void BgScreen_FillRect(s32 unusedA, s32 unusedB, u32 width, u32 height,
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
      dst += BG_SCREEN_WIDTH - width;
    } while (row < height);
  }
}
