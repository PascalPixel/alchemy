/// @file
/// The quantity bar, the price digits and their sprites, and the hand cursor.
// Part of shop.c.

#include "shop_internal.h"
#include "graphics/palette_buffers.h"
#include "shop/shop.h"
#include "window/cursor.h"

/// The frames the field's colours take to fade to a tint and back.
#define SHOP_TINT_FRAMES 16

/// Paints the first `segments` segments of the quantity bar in `color` (a
/// QUANTITY_BAR_ colour), over what was painted before.
void QuantityBar_Fill(s32 segments, s32 color, u8 *tileBuffer) {
  u32 shifted = color << 4;
  const u16 *offset;

  // A segment is one byte, two pixels: colour 1 on the left (the low nibble)
  // and `color` on the right.
  color = shifted + 1;

  if (segments > 0) {
    offset = gShop_SelectorTileOffsets;
    do {
      // The segment's byte in rows 1 to 6.
      u8 *column = tileBuffer + *offset++;

      column[1 * TILE_ROW_SIZE_4BPP] = color;
      column[2 * TILE_ROW_SIZE_4BPP] = color;
      column[3 * TILE_ROW_SIZE_4BPP] = color;
      column[4 * TILE_ROW_SIZE_4BPP] = color;
      column[5 * TILE_ROW_SIZE_4BPP] = color;
      column[6 * TILE_ROW_SIZE_4BPP] = color;
      segments--;
    } while (segments != 0);
  }
}

/// Copies decimal glyph `digit` into its price-tile position, stopping at zero
/// source bytes: two bytes of each of its four rows in the digit's tile, and
/// two more in the same row of the next.
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition) {
  u8 *src = gShop_GlyphBytes + digit * TILE_SIZE_4BPP;
  u8 *dst = tileBuffer + gShop_PriceDigitTileOffsets[digitPosition] + 2;
  s32 count;

  for (count = 0; count < 4; count++) {
    if (*src != 0) {
      dst[0] = *src++;
      if (*src != 0) {
        dst[1] = *src++;
        if (*src != 0) {
          dst[TILE_SIZE_4BPP - 2] = *src++;
          if (*src != 0)
            dst[TILE_SIZE_4BPP - 1] = *src++;
        }
      }
    }
    dst += TILE_ROW_SIZE_4BPP;
  }
}

/// Builds a price of up to five digits as a sprite on window at (x, y). Returns
/// the sprite, or NULL with no free slot.
struct WindowSprite *Shop_CreatePriceSprite(s32 value, struct Window *window,
                                            s32 x, s32 y) {
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

/// Saves the field's colours and fades them to `tint` (a PALETTE_TINT_ mode)
/// over SHOP_TINT_FRAMES frames: the grey of the sanctum's healing, and the
/// element's colour as a Djinni joins the party.
void Shop_SaveAndTintFieldPalette(s32 tint) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct PaletteFade *paletteBuffers = Ram_HeapSlots->paletteFade;
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(savedPalette, work->savedFadeTarget,
          DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * 3 * sizeof(u16) / 4),
          REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          DMA_COPY_WORDS(sizeof(paletteBuffers->current) / 4), REG_DMA3);
  PaletteFade_SetTargetFar(tint, 1);
  PaletteFade_StartInterpolationFar(SHOP_TINT_FRAMES);
}

/// Fades back to the colours Shop_SaveAndTintFieldPalette saved, over
/// SHOP_TINT_FRAMES frames.
void Shop_RestoreFieldPalette(void) {
  PaletteFade_SetTargetFar((s32)gFieldWork->savedFadeTarget, 1);
  PaletteFade_StartInterpolationFar(SHOP_TINT_FRAMES);
}

/// Moves the cursor a frame along its straight line toward its target.
void MenuCursor_Advance(struct MenuCursor *cursor) {
  struct WindowSprite *sprite;
  s32 durationFrames;
  s32 frame;
  s32 cursorX;
  s32 cursorY;

  if (cursor == NULL)
    return;
  durationFrames = cursor->frames;
  if (durationFrames == 0)
    return;
  sprite = cursor->anchor;
  frame = ++cursor->frame;
  cursorX =
      cursor->x + (cursor->targetX - (s16)cursor->x) * frame / durationFrames;
  sprite->x = cursorX;
  sprite->sprite.oam.x = cursorX & OBJ_X_MASK;
  cursorY =
      cursor->y + (cursor->targetY - (s16)cursor->y) * frame / durationFrames;
  sprite->y = cursorY;
  sprite->sprite.oam.y = cursorY;
  if (frame == durationFrames) {
    cursor->frames = 0;
    cursor->frame = 0;
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
/// `frames` frames, for MenuCursor_Advance to run.
void MenuCursor_Set(struct MenuCursor *cursor, s32 targetX, s32 targetY,
                    s8 frames) {
  struct WindowSprite *anchor = cursor->anchor;

  cursor->x = (u16)anchor->x;
  cursor->y = (u16)anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->frames = frames;
  cursor->frame = 0;
}

/// Places the cursor and its sprite at (x, y) at once, with a one-frame glide
/// that has already arrived.
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y) {
  {
    struct WindowSprite *sprite = cursor->anchor;

    cursor->targetX = x;
    cursor->frames = 1;
    cursor->frame = 0;
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
/// when it is NULL), over the frames shop->cursorFrames holds.
void MenuCursor_Place(struct Window *window, s32 x, s32 y) {
  struct ShopWork *shop = Ram_HeapSlots->menuWork;

  // A window's position is in tiles, and its interior starts one tile (its
  // frame) in.
  if (window != NULL) {
    x += (s16)window->x * 8 + 8;
    y += (s16)window->y * 8 + 8;
  }
  MenuCursor_Set(&shop->cursor, x, y, (s8)shop->cursorFrames);
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
  Decompress_ByteLz(source, buffer);
  VramCache_Load(entryId, size, buffer);
  Heap_Release(buffer);
}

#undef SHOP_TINT_FRAMES
