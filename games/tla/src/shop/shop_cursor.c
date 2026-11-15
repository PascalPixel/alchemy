/// @file
/// The quantity bar, the price digits and their sprites, and the hand cursor.
// Part of shop.c.

#include "shop_internal.h"
#include "graphics/palette_buffers.h"
#include "module/shop/shop.h"
#include "module/tla.h"

/// Colours the first itemCount segments of the quantity bar, each two pixels
/// wide.
void QuantityBar_Fill(s32 itemCount, s32 paletteSelector, u8 *tileBuffer) {
  u32 shifted = paletteSelector << 4;
  const u16 *offset;

  // One byte holds two 4-bit pixels, the left one in the low nibble.
  paletteSelector = shifted + 1;

  if (itemCount > 0) {
    offset = gShop_SelectorTileOffsets;
    do {
      // A tile row is 4 bytes; the segment's byte in rows 1 to 6.
      u8 *column = tileBuffer + *offset++;

      column[4] = paletteSelector;
      column[8] = paletteSelector;
      column[12] = paletteSelector;
      column[16] = paletteSelector;
      column[20] = paletteSelector;
      column[24] = paletteSelector;
      itemCount--;
    } while (itemCount != 0);
  }
}

/// Copies decimal glyph `digit` into its price-tile position, stopping at zero
/// source bytes.
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition) {
  u8 *src = gShop_GlyphBytes + ((u32)digit << 5);
  u8 *dst = tileBuffer + gShop_PriceDigitTileOffsets[digitPosition] + 2;
  s32 count = 3;

  do {
    if (*src != 0) {
      dst[0] = *src++;
      if (*src != 0) {
        dst[1] = *src++;
        if (*src != 0) {
          dst[30] = *src++;
          if (*src != 0)
            dst[31] = *src++;
        }
      }
    }
    dst += 4;
    count--;
  } while (count >= 0);
}

/// Builds a price of up to five digits as a sprite on window at (x, y). Returns
/// the sprite, or NULL with no free slot.
struct WindowSprite *
Shop_CreatePriceSprite(s32 value, struct RenderInput *window, s32 x, s32 y) {
  u8 *tileBuffer;
  s32 resourceSlot;
  struct WindowSprite *priceSprite;

  tileBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  priceSprite = 0;
  // the blank price's eight tiles (8 * TILE_SIZE_4BPP bytes)
  Dma_Set(gShop_PriceTiles, tileBuffer, DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4),
          REG_DMA3);
  Shop_CopyGlyphs(value % 10, tileBuffer, 0);
  value = value / 10;
  if (value != 0) {
    Shop_CopyGlyphs(value % 10, tileBuffer, 1);
    value = value / 10;
    if (value != 0) {
      Shop_CopyGlyphs(value % 10, tileBuffer, 2);
      value = value / 10;
      if (value != 0) {
        s32 remainingDigits;

        Shop_CopyGlyphs(value % 10, tileBuffer, 3);
        remainingDigits = value / 10;
        if (remainingDigits != 0)
          Shop_CopyGlyphs(remainingDigits % 10, tileBuffer, 4);
      }
    }
  }
  resourceSlot = VramCache_FindFreeEntry();
  if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuffer);
    priceSprite =
        WindowSprite_CreateFar(resourceSlot, OUTPUT_SIZE_16X32, window, x, y);
  }
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  return priceSprite;
}

/// Saves the field colours in the field work and fades to `layer` over 16
/// frames.
void Shop_SaveAndTintFieldPalette(s32 layer) {
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  struct PaletteFade *paletteBuffers = Ram_HeapSlots->paletteFade;
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(savedPalette, field->savedFadeTarget, DMA_COPY_WORDS(336), REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          DMA_COPY_WORDS(sizeof(paletteBuffers->current) / 4), REG_DMA3);
  PaletteFade_SetTargetFar(layer, 1);
  PaletteFade_StartInterpolationFar(16);
}

/// Fades back to the colours Shop_SaveAndTintFieldPalette saved, over 16
/// frames.
void Shop_RestoreFieldPalette(void) {
  PaletteFade_SetTargetFar((s32)gFieldWork->savedFadeTarget, 1);
  PaletteFade_StartInterpolationFar(16);
}

/// Moves the cursor a frame along its straight line toward its target.
void MenuCursor_Advance(struct MenuCursor *cursor) {
  struct WindowSprite *sprite;
  s32 durationFrames;
  s32 frame;
  s32 cursorX;
  s32 cursorY;
  u32 mask;

  if (cursor == NULL)
    return;
  durationFrames = cursor->kind;
  if (durationFrames == 0)
    return;
  sprite = cursor->anchor;
  frame = ++cursor->active;
  cursorX =
      cursor->x + (cursor->targetX - (s16)cursor->x) * frame / durationFrames;
  sprite->x = cursorX;
  mask = 0x1ff;
  mask &= cursorX;
  sprite->sprite.oam.x = mask;
  cursorY =
      cursor->y + (cursor->targetY - (s16)cursor->y) * frame / durationFrames;
  sprite->y = cursorY;
  sprite->sprite.oam.y = cursorY;
  if (frame == durationFrames) {
    cursor->kind = 0;
    cursor->active = 0;
  }
}

/// Moves each cursor axis one quarter of the remaining distance, with a minimum
/// one-pixel step.
void MenuCursor_MoveTowardTarget(struct MenuCursor *cursor) {
  struct WindowSprite *sprite;
  s32 delta;
  s32 step;

  sprite = cursor->anchor;
  if (sprite != NULL) {
    delta = sprite->x - cursor->targetX;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->x = sprite->x - step;
      else
        sprite->x = sprite->x - 1;
      sprite->sprite.oam.x = sprite->x;
    } else if (delta < 0) {
      if (step != 0)
        sprite->x = sprite->x + step;
      else
        sprite->x = sprite->x + 1;
      sprite->sprite.oam.x = sprite->x;
    }

    delta = sprite->y - cursor->targetY;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->y = sprite->y - step;
      else
        sprite->y = sprite->y - 1;
      sprite->sprite.oam.y = sprite->y;
    } else if (delta < 0) {
      if (step != 0)
        sprite->y = sprite->y + step;
      else
        sprite->y = sprite->y + 1;
      sprite->sprite.oam.y = sprite->y;
    }
  }
}

/// Starts a glide of the cursor from where it is to (targetX, targetY) over
/// `kind` frames, for MenuCursor_Advance to run.
void MenuCursor_Set(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind) {
  struct WindowSprite *anchor = cursor->anchor;

  cursor->x = (u16)anchor->x;
  cursor->y = (u16)anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->kind = kind;
  cursor->active = 0;
}

/// Places the cursor and its sprite at (x, y) at once, with a one-frame glide
/// that has already arrived.
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y) {
  {
    struct WindowSprite *sprite = cursor->anchor;

    cursor->targetX = x;
    cursor->kind = 1;
    cursor->active = 0;
    cursor->x = x;
    sprite->sprite.oam.x = sprite->x = x;
  }
  {
    struct WindowSprite *sprite;

    cursor->targetY = y;
    cursor->y = y;
    sprite = cursor->anchor;
    sprite->sprite.oam.y = sprite->y = y;
  }
}

/// Starts the hand cursor's glide to (x, y) pixels inside `window` (the screen
/// when it is NULL), over the frames shop->mode holds.
void MenuCursor_Place(struct RenderInput *window, s32 x, s32 y) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;

  // A window's position is in tiles, and its interior starts one tile (its
  // frame) in
  if (window != NULL) {
    x += (s16)window->x * 8 + 8;
    y += (s16)window->y * 8 + 8;
  }
  MenuCursor_Set(&shop->cursor, x, y, (s8)shop->mode);
}

/// Decompresses resource `resourceId` and loads its first `size` bytes into
/// VRAM cache entry `entryId`; does nothing for an entry past the cache.
void VramBlock_LoadResource(s32 entryId, s32 size, s32 resourceId) {
  void *source;
  void *buffer;

  if (entryId >= VRAM_CACHE_ENTRY_COUNT)
    return;
  source = Resource_GetTableEntry(resourceId);
  buffer = Heap_Alloc(size);
  Resource_DecodeByteLzInRam(source, buffer);
  VramCache_Load(entryId, size, buffer);
  Heap_Release(buffer);
}
