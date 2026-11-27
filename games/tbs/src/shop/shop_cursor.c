// The quantity bar and the price sprites, the field's tint behind a counter, and the hand cursor's
// moves. Part of shop.c.

#include "shop_internal.h"

// The ticks the field's colours take to fade to a shop's tint and back.
#define SHOP_TINT_TICKS 16

// Where each of the quantity bar's 30 segments starts in its tile buffer: a byte column of a 4-bit
// tile, so four segments to a tile and the next tile TILE_SIZE_4BPP bytes on, eight tiles in all.
// clang-format off
static const Uint16 QuantityBarSegmentOffsets[30] = {
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

// Colours the first itemCount segments of the quantity bar in `color` (enum QUANTITY_BAR_COL).
void mnQuantityBarFill(Sint32 itemCount, Sint32 color, Uint8 *tileBuf)
{
    Sint32 segment;

    // One byte holds two 4-bit pixels, the left one in the low nibble: a black pixel (colour 1),
    // then one of the segment's colour.
    color = (color << 4) + 1;
    for (segment = 0; segment < itemCount; segment++)
    {
        // The segment's byte in rows 1 to 6.
        Uint8 *column                  = tileBuf + QuantityBarSegmentOffsets[segment];

        column[1 * TILE_ROW_SIZE_4BPP] = color;
        column[2 * TILE_ROW_SIZE_4BPP] = color;
        column[3 * TILE_ROW_SIZE_4BPP] = color;
        column[4 * TILE_ROW_SIZE_4BPP] = color;
        column[5 * TILE_ROW_SIZE_4BPP] = color;
        column[6 * TILE_ROW_SIZE_4BPP] = color;
    }
}

// Where each of a price's five digits starts in its tile buffer, ones first.
// clang-format off
static const Uint16 PriceDigitTileOffsets[5] = {
    192, 144, 128, 80, 64,
};
// clang-format on

// Draws one digit of a price at its place (0 for the ones) over the blank price's tiles. A digit is
// up to four rows of eight pixels, the right half of one tile and the left half of the next; its
// glyph's bytes end at the first 0.
void mnShopDrawPriceDigit(Sint32 digit, Uint8 *tileBuf, Uint32 digitPos)
{
    Uint8 *src  = ShopGlyphBytes + digit * TILE_SIZE_4BPP;
    Uint8 *dest = tileBuf + PriceDigitTileOffsets[digitPos] + 2;
    Sint32 count;

    for (count = 0; count < 4; count++)
    {
        if (*src != 0)
        {
            dest[0] = *src++;
            if (*src != 0)
            {
                dest[1] = *src++;
                if (*src != 0)
                {
                    dest[TILE_SIZE_4BPP - 2] = *src++;
                    if (*src != 0)
                        dest[TILE_SIZE_4BPP - 1] = *src++;
                }
            }
        }
        dest += TILE_ROW_SIZE_4BPP;
    }
}

// Builds a price of up to five digits as a sprite on window at (x, y). Returns the sprite, or NULL
// with no free slot.
WINDOW_SPR *mnShopCreatePriceSprite(Sint32 value, WINDOW *window, Sint32 x, Sint32 y)
{
    Uint8      *tileBuf;
    Sint32      resourceSlot;
    WINDOW_SPR *priceSprite;

    tileBuf     = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
    priceSprite = 0;
    // the blank price's eight tiles (8 * TILE_SIZE_4BPP bytes)
    syDMAXCopy(ShopPriceTiles, tileBuf, DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
    mnShopDrawPriceDigit(value % 10, tileBuf, 0);
    value = value / 10;
    if (value != 0)
    {
        mnShopDrawPriceDigit(value % 10, tileBuf, 1);
        value = value / 10;
        if (value != 0)
        {
            mnShopDrawPriceDigit(value % 10, tileBuf, 2);
            value = value / 10;
            if (value != 0)
            {
                Sint32 remainingDigits;

                mnShopDrawPriceDigit(value % 10, tileBuf, 3);
                remainingDigits = value / 10;
                if (remainingDigits != 0)
                    mnShopDrawPriceDigit(remainingDigits % 10, tileBuf, 4);
            }
        }
    }
    resourceSlot = syVRAMCacheFindFreeEntry();
    if (resourceSlot != VRAM_CACHE_ENTRY_COUNT)
    {
        syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuf);
        priceSprite = mnWindowSpriteCreateStub(resourceSlot, OUTPUT_SIZE_16X32, window, x, y);
    }
    syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
    return priceSprite;
}

// Saves the field's background colours in the field state and fades them to `tint`
// (PALETTE_TINT_CUSTOM with a colour) over SHOP_TINT_TICKS ticks.
void mnShopSaveAndTintFieldPalette(Sint32 tint)
{
    // UB: slot 32 lies past scalar FieldStateWork.
#if FIX_POINTER_OUTSIDE_OBJECT
    PALETTE_FADE *paletteBuffers = PaletteFadeState;
    Uint8        *fieldBytes     = (Uint8 *)FieldStateWork;
#else
    Uint32       *stateCells = (Uint32 *)&FieldStateWork;
    PALETTE_FADE *paletteBuffers =
        (PALETTE_FADE *)stateCells[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_FIELD];
    Uint8 *fieldBytes = (Uint8 *)stateCells[0];
#endif
    Uint16 *savedPalette = paletteBuffers->target;

    syDMAXCopy(savedPalette,
               fieldBytes + offsetof(FIELD_STATE, savedFadeTarget),
               DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * 3 * sizeof(Uint16) / 4),
               REG_DMA3);
    syDMAXCopy(savedPalette,
               paletteBuffers->current,
               DMA_COPY_WORDS(sizeof(paletteBuffers->current) / 4),
               REG_DMA3);
    fdPaletteFadeSetTargetStub(tint, 1);
    fdPaletteFadeStartInterpolationStub(SHOP_TINT_TICKS);
}

// Fades back to the colours mnShopSaveAndTintFieldPalette saved, over SHOP_TINT_TICKS ticks.
void mnShopRestoreFieldPalette(void)
{
    fdPaletteFadeSetTargetStub((Sint32)FieldStateWork + offsetof(FIELD_STATE, savedFadeTarget), 1);
    fdPaletteFadeStartInterpolationStub(SHOP_TINT_TICKS);
}

// Moves the cursor a tick along its straight line toward its target.
void mnCursorAdvance(MENU_CURSOR *cursor)
{
    WINDOW_SPR *sprite;
    Sint32      durationTicks;
    Sint32      frame;
    Sint32      cursorX;
    Sint32      cursorY;

    if (cursor == NULL)
        return;
    durationTicks = cursor->ticks;
    if (durationTicks == 0)
        return;
    sprite    = cursor->anchor;
    frame     = ++cursor->frame;
    cursorX   = cursor->x + (cursor->targetX - (Sint16)cursor->x) * frame / durationTicks;
    sprite->x = cursorX;
    sprite->sprite.oam.x = cursorX;
    cursorY   = cursor->y + (cursor->targetY - (Sint16)cursor->y) * frame / durationTicks;
    sprite->y = cursorY;
    sprite->sprite.oam.y = cursorY;
    if (frame == durationTicks)
    {
        cursor->ticks = 0;
        cursor->frame = 0;
    }
}

// Eases a cursor toward its target, a quarter of the way on each axis a tick and at least a pixel;
// the settings and name-entry screens move their markers with it.
void mnCursorMoveTowardTarget(MENU_CURSOR *cursor)
{
    WINDOW_SPR *sprite;
    Sint32      delta;
    Sint32      step;

    sprite = cursor->anchor;
    if (sprite != NULL)
    {
        delta = sprite->x - cursor->targetX;
        step  = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0)
        {
            if (step != 0)
                sprite->x = sprite->x - step;
            else
                sprite->x = sprite->x - 1;
            sprite->sprite.oam.x = sprite->x;
        }
        else if (delta < 0)
        {
            if (step != 0)
                sprite->x = sprite->x + step;
            else
                sprite->x = sprite->x + 1;
            sprite->sprite.oam.x = sprite->x;
        }

        delta = sprite->y - cursor->targetY;
        step  = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0)
        {
            if (step != 0)
                sprite->y = sprite->y - step;
            else
                sprite->y = sprite->y - 1;
            sprite->sprite.oam.y = sprite->y;
        }
        else if (delta < 0)
        {
            if (step != 0)
                sprite->y = sprite->y + step;
            else
                sprite->y = sprite->y + 1;
            sprite->sprite.oam.y = sprite->y;
        }
    }
}

// Starts a glide of the cursor from where it is to (targetX, targetY) over `ticks` ticks, for
// mnCursorAdvance to run.
void mnCursorSet(MENU_CURSOR *cursor, Sint32 targetX, Sint32 targetY, Sint32 ticks)
{
    WINDOW_SPR *anchor = cursor->anchor;

    cursor->x          = anchor->x;
    cursor->y          = anchor->y;
    cursor->targetX    = targetX;
    cursor->targetY    = targetY;
    cursor->ticks      = ticks;
    cursor->frame      = 0;
}

// Places the cursor and its sprite at (x, y) at once, with a one-tick glide that has already
// arrived.
void mnCursorSetPosImmediate(MENU_CURSOR *cursor, Sint32 x, Sint32 y)
{
    cursor->targetX              = x;
    cursor->ticks                = 1;
    cursor->frame                = 0;
    cursor->x                    = x;
    cursor->anchor->sprite.oam.x = cursor->anchor->x = x;
    cursor->targetY                                  = y;
    cursor->y                                        = y;
    cursor->anchor->sprite.oam.y = cursor->anchor->y = y;
}

// Starts the hand cursor's glide to (x, y) pixels inside `window` (the screen when it is NULL),
// over the ticks shop->cursorFrame holds.
void mnCursorPlace(WINDOW *window, Sint32 x, Sint32 y)
{
    Sint32      cursorX;
    Sint32      cursorY;
    SHOP_STATE *shop;

    cursorX = x;
    cursorY = y;
    shop    = MenuState;
    // A window's position is in tiles, and its interior starts one tile (its tick) in
    if (window != NULL)
    {
        cursorX = cursorX + (window->x * 8) + 8;
        cursorY = cursorY + (window->y * 8) + 8;
    }
    mnCursorSet(&shop->cursor, cursorX, cursorY, (Sint8)shop->cursorFrame);
}

#undef SHOP_TINT_TICKS
