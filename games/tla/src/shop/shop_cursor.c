/* The quantity bar, the price digits and their sprites, and the hand cursor.
   Part of shop.c. */

#include "graphics/palette_buffers.h"

/* The ticks the field's colours take to fade to a tint and back. */
#define SHOP_TINT_TICKS 16

/* Paints the first `segments` segments of the quantity bar in `col` (a
   QUANTITY_BAR_ colour), over what was painted before. */
void mnQuantityBarFill(Sint32 segments, Sint32 col, Uint8 *tileBuf)
{
	Uint32		  shifted = col << 4;
	const Uint16 *ofs;

	/* A segment is one byte, two pixels: colour 1 on the left (the low nibble)
	   and `col` on the right. */
	col = shifted + 1;

	if(segments > 0) {
		ofs = ShopQuantityBarSegmentOffsets;
		do {
			/* The segment's byte in rows 1 to 6. */
			Uint8 *column = tileBuf + *ofs++;

			column[1 * TILE_ROW_SIZE_4BPP] = col;
			column[2 * TILE_ROW_SIZE_4BPP] = col;
			column[3 * TILE_ROW_SIZE_4BPP] = col;
			column[4 * TILE_ROW_SIZE_4BPP] = col;
			column[5 * TILE_ROW_SIZE_4BPP] = col;
			column[6 * TILE_ROW_SIZE_4BPP] = col;
			segments--;
		} while(segments != 0);
	}
}

/* Copies decimal glyph `digit` into its price-tile position, stopping at zero
   source bytes: two bytes of each of its four rows in the digit's tile, and two
   more in the same row of the next. */
void mnShopDrawPriceDigit(Sint32 digit, Uint8 *tileBuf, Uint32 digitPos)
{
	Uint8 *src = ShopGlyphBytes + digit * TILE_SIZE_4BPP;
	Uint8 *dst = tileBuf + ShopPriceDigitTileOffsets[digitPos] + 2;
	Sint32 cnt;

	for(cnt = 0; cnt < 4; cnt++) {
		if(*src != 0) {
			dst[0] = *src++;
			if(*src != 0) {
				dst[1] = *src++;
				if(*src != 0) {
					dst[TILE_SIZE_4BPP - 2] = *src++;
					if(*src != 0)
						dst[TILE_SIZE_4BPP - 1] = *src++;
				}
			}
		}
		dst += TILE_ROW_SIZE_4BPP;
	}
}

/* Builds a price of up to five digits as a sprite on window at (x, y). Returns
   the sprite, or NULL with no free slot. */
WINDOW_SPR *mnShopCreatePriceSpr(Sint32 value, WINDOW *win, Sint32 x, Sint32 y)
{
	Uint8	   *tileBuf;
	Sint32		resourceSlot;
	WINDOW_SPR *priceSpr;

	tileBuf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	priceSpr = 0;
	/* the blank price's eight tiles (8 * TILE_SIZE_4BPP bytes) */
	syDMAXCopy(ShopPriceTiles, tileBuf, DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4),
		REG_DMA3);
	mnShopDrawPriceDigit(value % 10, tileBuf, 0);
	value = value / 10;
	if(value != 0) {
		mnShopDrawPriceDigit(value % 10, tileBuf, 1);
		value = value / 10;
		if(value != 0) {
			mnShopDrawPriceDigit(value % 10, tileBuf, 2);
			value = value / 10;
			if(value != 0) {
				Sint32 remainingDigits;

				mnShopDrawPriceDigit(value % 10, tileBuf, 3);
				remainingDigits = value / 10;
				if(remainingDigits != 0)
					mnShopDrawPriceDigit(remainingDigits % 10, tileBuf, 4);
			}
		}
	}
	resourceSlot = syVRAMCacheFindFreeEntry();
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuf);
		priceSpr = mnWindowSprCreateStub(
			resourceSlot, WINDOW_SPRITE_SIZE_16X32, win, x, y);
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return priceSpr;
}

/* Saves the field's colours and fades them to `tint` (a PALETTE_TINT_ mode)
   over SHOP_TINT_TICKS ticks: the grey of the sanctum's healing, and the
   element's colour as a Djinni joins the party. */
void mnShopSaveAndTintFieldPalette(Sint32 tint)
{
	FIELD_STATE	 *state = RamHeapSlots->fieldState;
	PALETTE_FADE *paletteBuffers = RamHeapSlots->palFade;
	Uint16		 *savedPalette = paletteBuffers->tgt;

	syDMAXCopy(savedPalette, state->savedFadeTgt,
		DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * 3 * sizeof(Uint16) / 4),
		REG_DMA3);
	syDMAXCopy(savedPalette, paletteBuffers->cur,
		DMA_COPY_WORDS(sizeof(paletteBuffers->cur) / 4), REG_DMA3);
	fdPaletteFadeSetTargetStub(tint, 1);
	fdPaletteFadeStartInterpolationStub(SHOP_TINT_TICKS);
}

/* Fades back to the colours mnShopSaveAndTintFieldPalette saved, over
   SHOP_TINT_TICKS ticks. */
void mnShopRestoreFieldPalette(void)
{
	fdPaletteFadeSetTargetStub((Sint32)FieldStateWork->savedFadeTgt, 1);
	fdPaletteFadeStartInterpolationStub(SHOP_TINT_TICKS);
}

/* Moves the cursor a tick along its straight line toward its target. */
void mnCursorAdvance(MENU_CURSOR *cursor)
{
	WINDOW_SPR *spr;
	Sint32		durationTicks;
	Sint32		frm;
	Sint32		cursorX;
	Sint32		cursorY;

	if(cursor == NULL)
		return;
	durationTicks = cursor->ticks;
	if(durationTicks == 0)
		return;
	spr = cursor->anchor;
	frm = ++cursor->frm;
	cursorX =
		cursor->x + (cursor->tgtX - (Sint16)cursor->x) * frm / durationTicks;
	spr->x = cursorX;
	spr->spr.oam.x = cursorX & OBJ_X_MASK;
	cursorY =
		cursor->y + (cursor->tgtY - (Sint16)cursor->y) * frm / durationTicks;
	spr->y = cursorY;
	spr->spr.oam.y = cursorY;
	if(frm == durationTicks) {
		cursor->ticks = 0;
		cursor->frm = 0;
	}
}

/* Moves each cursor axis one quarter of the remaining distance, with a minimum
   one-pixel step. */
void mnCursorMoveTowardTarget(MENU_CURSOR *cursor)
{
	WINDOW_SPR *spr;
	Sint32		delta;
	Sint32		step;

	spr = cursor->anchor;
	if(spr != NULL) {
		delta = spr->x - cursor->tgtX;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				spr->x = spr->x - step;
			else
				spr->x = spr->x - 1;
			spr->spr.oam.x = spr->x;
		} else if(delta < 0) {
			if(step != 0)
				spr->x = spr->x + step;
			else
				spr->x = spr->x + 1;
			spr->spr.oam.x = spr->x;
		}

		delta = spr->y - cursor->tgtY;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				spr->y = spr->y - step;
			else
				spr->y = spr->y - 1;
			spr->spr.oam.y = spr->y;
		} else if(delta < 0) {
			if(step != 0)
				spr->y = spr->y + step;
			else
				spr->y = spr->y + 1;
			spr->spr.oam.y = spr->y;
		}
	}
}

/* Starts a glide of the cursor from where it is to (tgtX, tgtY) over
   `ticks` ticks, for mnCursorAdvance to run. */
void mnCursorSet(MENU_CURSOR *cursor, Sint32 tgtX, Sint32 tgtY, Sint8 ticks)
{
	WINDOW_SPR *anchor = cursor->anchor;

	cursor->x = (Uint16)anchor->x;
	cursor->y = (Uint16)anchor->y;
	cursor->tgtX = tgtX;
	cursor->tgtY = tgtY;
	cursor->ticks = ticks;
	cursor->frm = 0;
}

/* Places the cursor and its sprite at (x, y) at once, with a one-tick glide
   that has already arrived. */
void mnCursorSetPosImmediate(MENU_CURSOR *cursor, Sint32 x, Sint32 y)
{
	{
		WINDOW_SPR *spr = cursor->anchor;

		cursor->tgtX = x;
		cursor->ticks = 1;
		cursor->frm = 0;
		cursor->x = x;
		spr->spr.oam.x = spr->x = x;
	}
	{
		WINDOW_SPR *spr;

		cursor->tgtY = y;
		cursor->y = y;
		spr = cursor->anchor;
		spr->spr.oam.y = spr->y = y;
	}
}

/* Starts the hand cursor's glide to (x, y) pixels inside `win` (the screen
   when it is NULL), over the ticks shop->cursorFrm holds. */
void mnCursorPlace(WINDOW *win, Sint32 x, Sint32 y)
{
	SHOP_STATE *shop = RamHeapSlots->menuState;

	/* A window's position is in tiles, and its interior starts one tile (its
	   tick) in. */
	if(win != NULL) {
		x += win->x * 8 + 8;
		y += win->y * 8 + 8;
	}
	mnCursorSet(&shop->cursor, x, y, (Sint8)shop->cursorFrm);
}

/* Decompresses resource `resourceId` and loads its first `size` bytes into VRAM
   cache entry `entryId`; does nothing for an entry past the cache. */
void mnVRAMBlockLoadResource(Sint32 entryId, Sint32 size, Sint32 resourceId)
{
	void *src;
	void *buf;

	if(entryId >= VRAM_CACHE_ENTRY_COUNT)
		return;
	src = syResourceGetTableEntry(resourceId);
	buf = syHeapAlloc(size);
	syDecompressByteLZ(src, buf);
	syVRAMCacheLoad(entryId, size, buf);
	syHeapRelease(buf);
}

#undef SHOP_TINT_TICKS
