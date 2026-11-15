/// @file
/// Window sprites, portraits and the window spans.
// Part of window.c.

#include "window_internal.h"
#include "window/screens.h"

/// Takes a free window output, writes its OAM attributes at the window's
/// position offset by (offsetX, offsetY) pixels and appends it to the window's
/// list. Releases the resource slot and returns NULL when no output is free.
struct WindowSprite *WindowSprite_Create(s32 slot, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct WindowSprite *output;
  s32 y;

  output = WindowSprite_AcquireFree();
  if (output == NULL) {
    VramCache_FreeEntry((u32)slot);
    return NULL;
  }
  x = offsetX + (s16)input->x * 8 + 8;
  y = offsetY + (s16)input->y * 8 + 8;
  x &= 0x1ff;
  y &= 0xff;
  *(u32 *)&output->sprite.oam = (x << 16) | y | flags;
  ((u32 *)&output->sprite.oam)[1] = gVramCacheEntries[slot].offset >> 5;
  output->sentinel = 254;
  output->x = x;
  output->y = (s16)y;
  output->next = NULL;
  output->index = (s8)slot;
  output->kind = WINDOW_SPRITE_ICON;
  output->motion = 1;
  WindowSprite_AppendToList((struct WindowSpriteList *)input, output);
  /* FAKEMATCH(dead): an empty once-only block after the call. */
  do {
  } while (0);
  return output;
}

/// Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
/// it as a 16x16 sprite at (x, y) in `window`.
struct WindowSprite *WindowSprite_CreateFromResource(s32 mode, s32 resource,
                                                     struct RenderInput *window,
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
                                               struct RenderInput *window,
                                               s32 x, s32 y) {
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
struct WindowSprite *
WindowSprite_CreateWithTransform(s32 icon, struct RenderInput *window, s32 x,
                                 s32 y) {
  s32 count;
  s32 unused;
  struct WindowSprite *result;

  count = VramCache_FindFreeEntry();
  if (count == 0x60) {
    return NULL;
  }
  Icon_BuildCommandIconTiles(icon, 1, &count, &unused, 1);
  result = WindowSprite_Create(count, OUTPUT_SIZE_16X16, window, x, y);
  result->sentinel = 251;
  return result;
}

/// Loads the character entry for the key into the given side, or into a free
/// one (resource id 999) when the side is 2 or more, and creates its output
/// with that side's palette (14 + side).
struct WindowSprite *Portrait_Create(s32 objectKind, s32 position, s32 side,
                                     s32 window, s32 x, s32 y) {
  struct WindowWork *state = (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct WindowSprite *object = 0;
  s32 first;
  s32 second;
  s32 id;
  s32 slot;

  id = Portrait_GetForSpeaker(objectKind);
  if (id == -1)
    return object;

  if ((u32)side > 1) {
    u32 current;

    current = state->resourceIds[1];
    side = 1;
    if (current != PORTRAIT_SLOT_EMPTY) {
      side = 0;
      if (state->resourceIds[0] != PORTRAIT_SLOT_EMPTY)
        return object;
    }
  }

  slot = PORTRAIT_PALETTE + side;
  Icon_LoadWithPalette(id, position, &first, &second, slot, 0);
  object = WindowSprite_Create(first, OUTPUT_SIZE_32X32,
                               (struct RenderInput *)window, x, y);
  if (object != 0) {
    object->sprite.oam.palette = slot;
    object->kind = WINDOW_SPRITE_PORTRAIT;
  }

  state->resourceIds[side] = id;
  state->outputIds[side] = first;
  return object;
}

/// Reloads the portrait picture for the key into a portrait slot with that
/// slot's palette (14 + slot), reusing the slot's VRAM cache entry. A slot of
/// 2 or more means whichever slot already shows the picture, the second
/// first; when neither does, nothing is loaded.
void Portrait_Reload(u32 slot, s32 key, s32 unused) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  s32 id = Portrait_GetForSpeaker(key);
  s32 offset;
  s32 palette;
  s32 cached;
  s32 tile;
  s32 entry;

  if (id == -1)
    return;
  if (slot > 1) {
    if (work->resourceIds[1] == id)
      slot = 1;
    else if (work->resourceIds[0] == id)
      slot = 0;
    else
      return;
  }
  offset = offsetof(struct WindowWork, outputIds);
  palette = slot;
  cached = *(u16 *)((u8 *)work + (slot * 2 + offset));
  entry = cached;
  Icon_LoadWithPalette(id, unused, &entry, &tile, palette + 14, 1);
}

/// Sets active to mode and resets the animation step (animStep) to zero.
/// out: sprite output; nothing happens if NULL.
void WindowSprite_SetMotion(struct WindowSprite *out, s32 mode) {
  if (out != NULL) {
    out->motion = mode;
    out->animStep = 0;
  }
}

/// Sets the draw layer of the window sprite `sprite` to 255 - `priority`, so a
/// higher priority draws on a lower layer. Does nothing when `sprite` is NULL.
void WindowSprite_SetInvertedSentinel(u8 *window, s32 value) {
  if (window != NULL)
    window[15] = ~value;
}

/// Fills the 0x140-byte row buffer with blank tiles, or builds it from the
/// window tiles' map through the cell picture routine copied to RAM.
void ScanlineWindow_FillSpans(void *dst) {
  u8 *scene = Ram_HeapSlots->windowWork;

  if (scene == 0) {
    Dma_Fill16(0xe0e0, dst, 0x140);
  } else {
    void (*code)(void *, void *);

    RamCode_Load(code, Tile_BuildCellPicturesCode,
                 Tile_BuildCellPicturesCodeSize);
    code(dst, scene + 8);
    /* FAKEMATCH(dead): an empty once-only block after the call. */
    do {
    } while (0);
    Heap_Release(code);
  }
}

/// Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, 32 tiles
/// wide, with the tilemap entry `value`.
void BgScreen_FillRect(s32 unusedA, s32 unusedB, u32 width, u32 height,
                       s32 value) {
  u32 row = 0;
  s16 *dst = (s16 *)BG_SCREEN_BLOCK(4);

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
