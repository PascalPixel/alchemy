/// @file
/// The quantity bar and the price sprites, the field's tint behind a counter,
/// and the hand cursor's moves.
// Part of shop.c.

#include "shop_internal.h"

/// The frames the field's colours take to fade to a shop's tint and back.
#define SHOP_TINT_FRAMES 16

/// Where each of the quantity bar's 30 segments starts in its tile buffer: a
/// byte column of a 4-bit tile, so four segments to a tile and the next tile
/// TILE_SIZE_4BPP bytes on, eight tiles in all.
// clang-format off
static const u16 sQuantityBarSegmentOffsets[30] = {
    0, 1, 2, 3,
    32, 33, 34, 35,
    64, 65, 66, 67,
    96, 97, 98, 99,
    128, 129, 130, 131,
    160, 161, 162, 163,
    192, 193, 194, 195,
    224, 225,
};
// clang-format on

/// Colours the first itemCount segments of the quantity bar in `color` (enum
/// QuantityBarColor).
void QuantityBar_Fill(s32 itemCount, s32 color, u8 *tileBuffer) {
  s32 segment;

  // One byte holds two 4-bit pixels, the left one in the low nibble: a black
  // pixel (colour 1), then one of the segment's colour.
  color = (color << 4) + 1;
  for (segment = 0; segment < itemCount; segment++) {
    // The segment's byte in rows 1 to 6.
    u8 *column = tileBuffer + sQuantityBarSegmentOffsets[segment];

    column[1 * TILE_ROW_SIZE_4BPP] = color;
    column[2 * TILE_ROW_SIZE_4BPP] = color;
    column[3 * TILE_ROW_SIZE_4BPP] = color;
    column[4 * TILE_ROW_SIZE_4BPP] = color;
    column[5 * TILE_ROW_SIZE_4BPP] = color;
    column[6 * TILE_ROW_SIZE_4BPP] = color;
  }
}

/// Where each of a price's five digits starts in its tile buffer, ones first.
// clang-format off
static const u16 sPriceDigitTileOffsets[5] = {
    192, 144, 128, 80, 64,
};
// clang-format on

/// Draws one digit of a price at its place (0 for the ones) over the blank
/// price's tiles. A digit is up to four rows of eight pixels, the right half of
/// one tile and the left half of the next; its glyph's bytes end at the first
/// 0.
void Shop_DrawPriceDigit(s32 digit, u8 *tileBuffer, u32 digitPosition) {
  u8 *src = gShop_GlyphBytes + digit * TILE_SIZE_4BPP;
  u8 *dst = tileBuffer + sPriceDigitTileOffsets[digitPosition] + 2;
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
  Shop_DrawPriceDigit(value % 10, tileBuffer, 0);
  value = value / 10;
  if (value != 0) {
    Shop_DrawPriceDigit(value % 10, tileBuffer, 1);
    value = value / 10;
    if (value != 0) {
      Shop_DrawPriceDigit(value % 10, tileBuffer, 2);
      value = value / 10;
      if (value != 0) {
        s32 remainingDigits;

        Shop_DrawPriceDigit(value % 10, tileBuffer, 3);
        remainingDigits = value / 10;
        if (remainingDigits != 0)
          Shop_DrawPriceDigit(remainingDigits % 10, tileBuffer, 4);
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

/// Saves the field's background colours in the field state and fades them to
/// `tint` (PALETTE_TINT_CUSTOM with a colour) over SHOP_TINT_FRAMES frames.
void Shop_SaveAndTintFieldPalette(s32 tint) {
  // UB: slot 32 lies past scalar gFieldState.
#ifdef UBFIX
  struct PaletteFade *paletteBuffers = gPaletteFadeState;
  u8 *fieldBytes = (u8 *)gFieldState;
#else
  u32 *stateCells = (u32 *)&gFieldState;
  struct PaletteFade *paletteBuffers = (struct PaletteFade *)
      stateCells[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_FIELD];
  u8 *fieldBytes = (u8 *)stateCells[0];
#endif
  u16 *savedPalette = paletteBuffers->target;

  Dma_Set(
      savedPalette, fieldBytes + offsetof(struct FieldState, savedFadeTarget),
      DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * 3 * sizeof(u16) / 4), REG_DMA3);
  Dma_Set(savedPalette, paletteBuffers->current,
          DMA_COPY_WORDS(sizeof(paletteBuffers->current) / 4), REG_DMA3);
  PaletteFade_SetTargetFar(tint, 1);
  PaletteFade_StartInterpolationFar(SHOP_TINT_FRAMES);
}

/// Fades back to the colours Shop_SaveAndTintFieldPalette saved, over
/// SHOP_TINT_FRAMES frames.
void Shop_RestoreFieldPalette(void) {
  PaletteFade_SetTargetFar(
      (s32)gFieldState + offsetof(struct FieldState, savedFadeTarget), 1);
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
  sprite->sprite.oam.x = cursorX;
  cursorY =
      cursor->y + (cursor->targetY - (s16)cursor->y) * frame / durationFrames;
  sprite->y = cursorY;
  sprite->sprite.oam.y = cursorY;
  if (frame == durationFrames) {
    cursor->frames = 0;
    cursor->frame = 0;
  }
}

/// Eases a cursor toward its target, a quarter of the way on each axis a frame
/// and at least a pixel; the settings and name-entry screens move their markers
/// with it.
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
                    s32 frames) {
  struct WindowSprite *anchor = cursor->anchor;

  cursor->x = anchor->x;
  cursor->y = anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->frames = frames;
  cursor->frame = 0;
}

/// Places the cursor and its sprite at (x, y) at once, with a one-frame glide
/// that has already arrived.
void MenuCursor_SetPositionImmediate(struct MenuCursor *cursor, s32 x, s32 y) {
  cursor->targetX = x;
  cursor->frames = 1;
  cursor->frame = 0;
  cursor->x = x;
  cursor->anchor->sprite.oam.x = cursor->anchor->x = x;
  cursor->targetY = y;
  cursor->y = y;
  cursor->anchor->sprite.oam.y = cursor->anchor->y = y;
}

/// Starts the hand cursor's glide to (x, y) pixels inside `window` (the screen
/// when it is NULL), over the frames shop->cursorFrames holds.
void MenuCursor_Place(struct Window *window, s32 x, s32 y) {
  s32 cursorX;
  s32 cursorY;
  struct ShopState *shop;

  cursorX = x;
  cursorY = y;
  shop = gMenuState;
  // A window's position is in tiles, and its interior starts one tile (its
  // frame) in
  if (window != NULL) {
    cursorX = cursorX + (window->x * 8) + 8;
    cursorY = cursorY + (window->y * 8) + 8;
  }
  MenuCursor_Set(&shop->cursor, cursorX, cursorY, (s8)shop->cursorFrames);
}

#undef SHOP_TINT_FRAMES
