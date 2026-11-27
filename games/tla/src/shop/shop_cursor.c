/* The quantity bar, the price digits and their sprites, and the hand cursor.
   Part of shop.c. */

#include "graphics/palette_buffers.h"

/* The frames the field's colours take to fade to a tint and back. */
#define SHOP_TINT_FRAMES 16

/* Paints the first `segments` segments of the quantity bar in `color` (a
   QUANTITY_BAR_ colour), over what was painted before. */
void mnQuantityBarFill(Sint32 segments, Sint32 color, Uint8 *tileBuffer)
{
	Uint32		  shifted = color << 4;
	const Uint16 *offset;

	/* A segment is one byte, two pixels: colour 1 on the left (the low nibble)
	   and `color` on the right. */
	color = shifted + 1;

	if(segments > 0) {
		offset = Shop_QuantityBarSegmentOffsets;
		do {
			/* The segment's byte in rows 1 to 6. */
			Uint8 *column = tileBuffer + *offset++;

			column[1 * TILE_ROW_SIZE_4BPP] = color;
			column[2 * TILE_ROW_SIZE_4BPP] = color;
			column[3 * TILE_ROW_SIZE_4BPP] = color;
			column[4 * TILE_ROW_SIZE_4BPP] = color;
			column[5 * TILE_ROW_SIZE_4BPP] = color;
			column[6 * TILE_ROW_SIZE_4BPP] = color;
			segments--;
		} while(segments != 0);
	}
}

/* Copies decimal glyph `digit` into its price-tile position, stopping at zero
   source bytes: two bytes of each of its four rows in the digit's tile, and two
   more in the same row of the next. */
void mnShopDrawPriceDigit(Sint32 digit, Uint8 *tileBuffer, Uint32 digitPosition)
{
	Uint8 *src = Shop_GlyphBytes + digit * TILE_SIZE_4BPP;
	Uint8 *dst = tileBuffer + Shop_PriceDigitTileOffsets[digitPosition] + 2;
	Sint32 count;

	for(count = 0; count < 4; count++) {
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
WINDOW_SPR *mnShopCreatePriceSpr(
	Sint32 value, WINDOW *window, Sint32 x, Sint32 y)
{
	Uint8	   *tileBuffer;
	Sint32		resourceSlot;
	WINDOW_SPR *priceSprite;

	tileBuffer = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	priceSprite = 0;
	/* the blank price's eight tiles (8 * TILE_SIZE_4BPP bytes) */
	syDMAXCopy(Shop_PriceTiles, tileBuffer,
		DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
	mnShopDrawPriceDigit(value % 10, tileBuffer, 0);
	value = value / 10;
	if(value != 0) {
		mnShopDrawPriceDigit(value % 10, tileBuffer, 1);
		value = value / 10;
		if(value != 0) {
			mnShopDrawPriceDigit(value % 10, tileBuffer, 2);
			value = value / 10;
			if(value != 0) {
				Sint32 remainingDigits;

				mnShopDrawPriceDigit(value % 10, tileBuffer, 3);
				remainingDigits = value / 10;
				if(remainingDigits != 0)
					mnShopDrawPriceDigit(remainingDigits % 10, tileBuffer, 4);
			}
		}
	}
	resourceSlot = syVRAMCacheFindFreeEntry();
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuffer);
		priceSprite = mnWindowSprCreateStub(
			resourceSlot, WINDOW_SPRITE_SIZE_16X32, window, x, y);
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return priceSprite;
}

/* Saves the field's colours and fades them to `tint` (a PALETTE_TINT_ mode)
   over SHOP_TINT_FRAMES frames: the grey of the sanctum's healing, and the
   element's colour as a Djinni joins the party. */
void mnShopSaveAndTintFieldPalette(Sint32 tint)
{
	FIELD_STATE	 *state = Ram_HeapSlots->fieldState;
	PALETTE_FADE *paletteBuffers = Ram_HeapSlots->palFade;
	Uint16		 *savedPalette = paletteBuffers->tgt;

	syDMAXCopy(savedPalette, state->savedFadeTgt,
		DMA_COPY_WORDS(PALETTE_FADE_BG_COLORS * 3 * sizeof(Uint16) / 4),
		REG_DMA3);
	syDMAXCopy(savedPalette, paletteBuffers->cur,
		DMA_COPY_WORDS(sizeof(paletteBuffers->cur) / 4), REG_DMA3);
	fdPaletteFadeSetTargetStub(tint, 1);
	fdPaletteFadeStartInterpolationStub(SHOP_TINT_FRAMES);
}

/* Fades back to the colours mnShopSaveAndTintFieldPalette saved, over
   SHOP_TINT_FRAMES frames. */
void mnShopRestoreFieldPalette(void)
{
	fdPaletteFadeSetTargetStub((Sint32)FieldStateWork->savedFadeTgt, 1);
	fdPaletteFadeStartInterpolationStub(SHOP_TINT_FRAMES);
}

/* Moves the cursor a frame along its straight line toward its target. */
void mnCursorAdvance(MENU_CURSOR *cursor)
{
	WINDOW_SPR *sprite;
	Sint32		durationFrames;
	Sint32		frame;
	Sint32		cursorX;
	Sint32		cursorY;

	if(cursor == NULL)
		return;
	durationFrames = cursor->frames;
	if(durationFrames == 0)
		return;
	sprite = cursor->anchor;
	frame = ++cursor->frm;
	cursorX =
		cursor->x + (cursor->tgtX - (Sint16)cursor->x) * frame / durationFrames;
	sprite->x = cursorX;
	sprite->spr.oam.x = cursorX & OBJ_X_MASK;
	cursorY =
		cursor->y + (cursor->tgtY - (Sint16)cursor->y) * frame / durationFrames;
	sprite->y = cursorY;
	sprite->spr.oam.y = cursorY;
	if(frame == durationFrames) {
		cursor->frames = 0;
		cursor->frm = 0;
	}
}

/* Moves each cursor axis one quarter of the remaining distance, with a minimum
   one-pixel step. */
void mnCursorMoveTowardTarget(MENU_CURSOR *cursor)
{
	WINDOW_SPR *sprite;
	Sint32		delta;
	Sint32		step;

	sprite = cursor->anchor;
	if(sprite != NULL) {
		delta = sprite->x - cursor->tgtX;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				sprite->x = sprite->x - step;
			else
				sprite->x = sprite->x - 1;
			sprite->spr.oam.x = sprite->x;
		} else if(delta < 0) {
			if(step != 0)
				sprite->x = sprite->x + step;
			else
				sprite->x = sprite->x + 1;
			sprite->spr.oam.x = sprite->x;
		}

		delta = sprite->y - cursor->tgtY;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				sprite->y = sprite->y - step;
			else
				sprite->y = sprite->y - 1;
			sprite->spr.oam.y = sprite->y;
		} else if(delta < 0) {
			if(step != 0)
				sprite->y = sprite->y + step;
			else
				sprite->y = sprite->y + 1;
			sprite->spr.oam.y = sprite->y;
		}
	}
}

/* Starts a glide of the cursor from where it is to (targetX, targetY) over
   `frames` frames, for mnCursorAdvance to run. */
void mnCursorSet(
	MENU_CURSOR *cursor, Sint32 targetX, Sint32 targetY, Sint8 frames)
{
	WINDOW_SPR *anchor = cursor->anchor;

	cursor->x = (Uint16)anchor->x;
	cursor->y = (Uint16)anchor->y;
	cursor->tgtX = targetX;
	cursor->tgtY = targetY;
	cursor->frames = frames;
	cursor->frm = 0;
}

/* Places the cursor and its sprite at (x, y) at once, with a one-frame glide
   that has already arrived. */
void mnCursorSetPosImmediate(MENU_CURSOR *cursor, Sint32 x, Sint32 y)
{
	{
		WINDOW_SPR *sprite = cursor->anchor;

		cursor->tgtX = x;
		cursor->frames = 1;
		cursor->frm = 0;
		cursor->x = x;
		sprite->spr.oam.x = sprite->x = x;
	}
	{
		WINDOW_SPR *sprite;

		cursor->tgtY = y;
		cursor->y = y;
		sprite = cursor->anchor;
		sprite->spr.oam.y = sprite->y = y;
	}
}

/* Starts the hand cursor's glide to (x, y) pixels inside `window` (the screen
   when it is NULL), over the frames shop->cursorFrm holds. */
void mnCursorPlace(WINDOW *window, Sint32 x, Sint32 y)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;

	/* A window's position is in tiles, and its interior starts one tile (its
	   frame) in. */
	if(window != NULL) {
		x += window->x * 8 + 8;
		y += window->y * 8 + 8;
	}
	mnCursorSet(&shop->cursor, x, y, (Sint8)shop->cursorFrm);
}

/* Decompresses resource `resourceId` and loads its first `size` bytes into VRAM
   cache entry `entryId`; does nothing for an entry past the cache. */
void mnVRAMBlockLoadResource(Sint32 entryId, Sint32 size, Sint32 resourceId)
{
	void *source;
	void *buffer;

	if(entryId >= VRAM_CACHE_ENTRY_COUNT)
		return;
	source = syResourceGetTableEntry(resourceId);
	buffer = syHeapAlloc(size);
	syDecompressByteLZ(source, buffer);
	syVRAMCacheLoad(entryId, size, buffer);
	syHeapRelease(buffer);
}

#undef SHOP_TINT_FRAMES
