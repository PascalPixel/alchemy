/// @file
/// The selection frame and price digit tiles, and the hand cursor.
// Part of shop.c.

#include "../shop_internal.h"

/// Where each of the quantity bar's 30 segments starts in its tile buffer: a
/// byte column of a 4-bit tile, so four segments to a tile and the next tile
/// 0x20 bytes on, eight tiles in all.
// clang-format off
static const u16 sSelectorTileOffsets[30] = {
    0x00, 0x01, 0x02, 0x03,
    0x20, 0x21, 0x22, 0x23,
    0x40, 0x41, 0x42, 0x43,
    0x60, 0x61, 0x62, 0x63,
    0x80, 0x81, 0x82, 0x83,
    0xa0, 0xa1, 0xa2, 0xa3,
    0xc0, 0xc1, 0xc2, 0xc3,
    0xe0, 0xe1,
};
// clang-format on

/// Colours the first `itemCount` segments of the quantity bar in
/// `tileBuffer`: each segment is two pixels wide on rows 1 to 6 of its tile,
/// the left pixel colour 1 and the right one colour `paletteSelector` (a
/// colour number, not a palette). Called by the shop's quantity choice and,
/// through SHOP.S, by the item menu's quantity bar, which draws its bar in
/// four colours this way.
void Shop_FillSelector(s32 itemCount, s32 paletteSelector, u8 *tileBuffer) {
  const u16 *tileOffset;

  // One byte holds two 4-bit pixels, the left one in the low nibble
  paletteSelector = (paletteSelector << 4) + 1;

  if (itemCount > 0) {
    tileOffset = sSelectorTileOffsets;
    do {
      struct SelectorRecord *record =
          (struct SelectorRecord *)(tileBuffer + *tileOffset++);
      record->values[0] = paletteSelector;
      record->values[4] = paletteSelector;
      record->values[8] = paletteSelector;
      record->values[12] = paletteSelector;
      record->values[16] = paletteSelector;
      record->values[20] = paletteSelector;
      itemCount--;
    } while (itemCount != 0);
  }
}

/// Where each of a price's five digits starts in its tile buffer, ones first.
// clang-format off
static const u16 sPriceDigitTileOffsets[5] = {
    0xc0, 0x90, 0x80, 0x50, 0x40,
};
// clang-format on

/// Copies decimal glyph `digit` into its price-tile position, stopping at
/// zero source bytes. Called by Shop_CreatePriceSprite.
void Shop_CopyGlyphs(s32 digit, u8 *tileBuffer, u32 digitPosition) {
  u8 *src = gShop_GlyphBytes + (digit << 5);
  u8 *dst = tileBuffer + sPriceDigitTileOffsets[digitPosition] + 2;
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

/// Builds a price of up to five digits in scratch tiles
/// (HEAP_SLOT_TILE_SCRATCH), loads them into a free VRAM cache slot and
/// attaches them to `window` as one sprite at (x, y). Returns the sprite, or
/// NULL with no free slot. Called by Shop_DrawStock for each item on sale. A
/// price of 100000 or more shows its last five digits. The scratch block is
/// released before returning; the cache entry is not freed here.
struct RenderOutput *Shop_CreatePriceSprite(s32 value, struct UiWindow *window,
                                            s32 x, s32 y) {
  u8 *tileBuffer;
  s32 resourceSlot;
  struct RenderOutput *priceSprite;

  tileBuffer = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
  priceSprite = 0;
  // 0x40 words: the blank price's eight tiles, 0x100 bytes of the block
  Dma_Set(gShop_PriceTiles, tileBuffer, (DMA_ENABLE | DMA_32BIT) << 16 | 0x40,
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
  resourceSlot = Resource_FindFreeEntry();
  if (resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(resourceSlot, 0x100, tileBuffer);
    priceSprite =
        RenderOutput_CreateFar(resourceSlot, 0x80008000, window, x, y);
  }
  FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  return priceSprite;
}

/// Saves the field colours in event work and fades to `layer` over 16 frames.
/// Called before sanctum and Djinn capture effects and Altmiller's wall burst;
/// Shop_RestoreFieldPalette restores the saved colours afterward.
void Shop_SaveAndTintFieldPalette(s32 layer) {
  // UB: slot 32 lies past scalar gFieldWork. Its named load adds eight
  // bytes in the native EN trial (0x54 to 0x5c).
#ifdef UBFIX
  struct ColorBuffer *paletteBuffers = gPaletteFadeWork;
  u8 *eventBytes = (u8 *)gFieldWork;
#else
  u32 *workCells = (u32 *)&gFieldWork;
  struct ColorBuffer *paletteBuffers =
      (struct ColorBuffer *)workCells[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_FIELD];
  u8 *eventBytes = (u8 *)workCells[0];
#endif
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(savedPalette, eventBytes + 0x236,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x150, REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2a0, REG_DMA3);
  ColorBuffer_SetTargetFar(layer, 1);
  ColorBuffer_StartInterpolationFar(16);
}

/// Fades back to the colours Shop_SaveAndTintFieldPalette saved, over 16
/// frames. Called when the sanctum, Djinn capture or Altmiller wall effect
/// ends.
void Shop_RestoreFieldPalette(void) {
  ColorBuffer_SetTargetFar((s32)gFieldWork + 0x236, 1);
  ColorBuffer_StartInterpolationFar(0x10);
}

/// Moves the cursor from its saved position toward its target, in a straight
/// line over `kind` frames, one frame a call; `active` counts the frames and
/// both go back to 0 on arrival. Runs every frame from Shop_StepCursor, and
/// from the option, name entry and save menus' cursor tasks.
void ShopCursor_Advance(struct ShopCursor *cursor) {
  struct RenderOutput *sprite;
  s32 durationFrames;
  s32 frame;
  s32 cursorX;
  s32 cursorY;

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
  sprite->oam.x = cursorX;
  cursorY =
      cursor->y + (cursor->targetY - (s16)cursor->y) * frame / durationFrames;
  sprite->y = cursorY;
  sprite->oam.y = cursorY;
  if (frame == durationFrames) {
    cursor->kind = 0;
    cursor->active = 0;
  }
}

/// Moves each cursor axis one quarter of the remaining distance, with a minimum
/// one-pixel step. Called each frame by the option menu and name entry.
void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor) {
  struct RenderOutput *sprite;
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
      sprite->oam.x = sprite->x;
    } else if (delta < 0) {
      if (step != 0)
        sprite->x = sprite->x + step;
      else
        sprite->x = sprite->x + 1;
      sprite->oam.x = sprite->x;
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
      sprite->oam.y = sprite->y;
    } else if (delta < 0) {
      if (step != 0)
        sprite->y = sprite->y + step;
      else
        sprite->y = sprite->y + 1;
      sprite->oam.y = sprite->y;
    }
  }
}

/// Starts a glide of the cursor from where it is to (targetX, targetY) over
/// `kind` frames, for ShopCursor_Advance to run. Called by Shop_PlaceCursor
/// and, through SHOP.S, by name entry and the Colosso choice.
void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s32 kind) {
  struct RenderOutput *anchor = cursor->anchor;

  cursor->x = anchor->x;
  cursor->y = anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->kind = kind;
  cursor->active = 0;
}

/// Places the cursor and its sprite at (x, y) at once, with a one-frame glide
/// that has already arrived. Called when a counter, the option and system
/// menus or name entry opens.
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y) {

  {
    struct RenderOutput *sprite = cursor->anchor;

    cursor->targetX = x;
    cursor->kind = 1;
    cursor->active = 0;
    cursor->x = x;
    sprite->oam.x = sprite->x = x;
  }
  {
    struct RenderOutput *sprite;

    cursor->targetY = y;
    cursor->y = y;
    sprite = cursor->anchor;
    sprite->oam.y = sprite->y = y;
  }
}

/// Starts the hand cursor's glide to (x, y) pixels inside `window` (the
/// screen when it is NULL), over the frames shop->mode holds. Called by every
/// counter's lists.
void Shop_PlaceCursor(struct UiWindow *window, s32 x, s32 y) {
  s32 cursorX;
  s32 cursorY;
  struct ShopWork *shop;

  cursorX = x;
  cursorY = y;
  shop = gMenuWork;
  // A window's position is in tiles, and its interior starts one tile (its
  // frame) in
  if (window != NULL) {
    cursorX = cursorX + (window->x * 8) + 8;
    cursorY = cursorY + (window->y * 8) + 8;
  }
  Shop_SetCursor(&shop->cursor, cursorX, cursorY, (s8)shop->mode);
}
