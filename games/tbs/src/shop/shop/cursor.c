/// @file
/// The selection frame and price digit tiles, and the hand cursor.
/// Part of shop.c, the shop object, compiled only through it.

#include "../shop_internal.h"

/// Selection frame tile positions
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

/// Fills six selector bytes at each requested offset in the quantity tile
/// buffer.
void Shop_FillSelector(s32 itemCount, s32 paletteSelector, u8 *tileBuffer) {
  const u16 *tileOffset;

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

/// Number transfer destinations
// clang-format off
static const u16 sPriceDigitTileOffsets[5] = {
    0xc0, 0x90, 0x80, 0x50, 0x40,
};
// clang-format on

/// Copies one decimal glyph into its price-tile position, stopping at zero
/// source bytes.
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

/// Builds up to five decimal digits in scratch tiles, creates their sprite and
/// frees the scratch block.
struct RenderOutput *Shop_CreatePriceSprite(s32 value, struct UiWindow *window,
                                            s32 x, s32 y) {
  u8 *tileBuffer;
  s32 resourceSlot;
  struct RenderOutput *priceSprite;

  tileBuffer = Runtime_AllocateBlock(14, 0x400);
  priceSprite = 0;
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
  Runtime_ReleaseHeapBlock(14);
  return priceSprite;
}

/// Restores the saved field color buffers and starts their interpolation.
void Shop_RestoreSceneTiles(s32 layer) {
  // UB: slot 32 lies past scalar gEventWork. Its named load adds eight
  // bytes in the native EN trial (0x54 to 0x5c).
#ifdef UBFIX
  struct BattleEffectBuffers *paletteBuffers = gPaletteBufferWork;
  u8 *eventBytes = (u8 *)gEventWork;
#else
  u32 *workCells = (u32 *)&gEventWork;
  struct BattleEffectBuffers *paletteBuffers =
      (struct BattleEffectBuffers *)workCells[5];
  u8 *eventBytes = (u8 *)workCells[0];
#endif
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(savedPalette, eventBytes + 0x236,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x150, REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2a0, REG_DMA3);
  BattleFx_ApplyColorToTargetBufferFar(layer, 1);
  BattleFx_StartBufferInterpolationFar(16);
}

/// Starts restoring the field color buffer after the menu effect.
void Shop_InitEffect(void) {
  BattleFx_ApplyColorToTargetBufferFar((s32)gEventWork + 0x236, 1);
  BattleFx_StartBufferInterpolationFar(0x10);
}

/// Interpolates the cursor from its saved position to its target over the
/// requested duration.
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
/// one-pixel step.
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

/// Saves the cursor's current position, assigns its target and duration, and
/// resets progress.
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

/// Places the cursor and its OAM coordinates immediately, preserving the
/// one-frame state.
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

/// Converts window-relative pixel coordinates to the cursor's absolute target.
void Shop_PlaceCursor(struct UiWindow *window, s32 x, s32 y) {
  s32 cursorX;
  s32 cursorY;
  struct ShopRuntime *shop;

  cursorX = x;
  cursorY = y;
  shop = gMenuWork;
  if (window != NULL) {
    cursorX = cursorX + (window->x * 8) + 8;
    cursorY = cursorY + (window->y * 8) + 8;
  }
  Shop_SetCursor(&shop->cursor, cursorX, cursorY, (s8)shop->mode);
}
