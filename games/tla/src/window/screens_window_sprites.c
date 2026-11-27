/* Window sprites, portraits and the window spans. Part of screens.c. */

/* The bits of a sprite's OAM x (nine) and y (eight), which wrap round the
   screen. */
#define OAM_X_MASK 0x1ff
#define OAM_Y_MASK 0xff

/* Creates a window sprite for the VRAM cache slot `slot` and attaches it to
   `win`, placed at (ofsX, ofsY) pixels from the window's inner corner
   (one tile in from its top-left border). `oamFlg` holds the OAM shape and
   size bits. Releases the slot and returns NULL when no sprite is free. */
WINDOW_SPR *mnWindowSprCreate(
	Sint32 slot, Sint32 oamFlg, WINDOW *win, Sint32 ofsX, Sint32 ofsY)
{
	Sint32		x;
	WINDOW_SPR *spr;
	Sint32		y;

	spr = mnWindowSprAcquireFree();
	if(spr == NULL) {
		syVRAMCacheFreeEntry((Uint32)slot);
		return NULL;
	}
	/* Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y. */
	x = ofsX + win->x * 8 + 8;
	y = ofsY + win->y * 8 + 8;
	x &= OAM_X_MASK;
	y &= OAM_Y_MASK;
	*(Uint32 *)&spr->spr.oam = OBJ_X(x) | y | oamFlg;
	/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a
	   tile). */
	((Uint32 *)&spr->spr.oam)[1] = VramCacheEntries[slot].ofs >> 5;
	spr->depth = OAM_DEPTH_FRONT - 1;
	spr->x = x;
	spr->y = (Sint16)y;
	spr->next = NULL;
	spr->idx = (Sint8)slot;
	/* An icon owns its VRAM cache slot, which is freed with it. */
	spr->kind = WINDOW_SPRITE_ICON;
	spr->motion = WINDOW_SPRITE_MOTION_STILL;
	mnWindowSprAppendToList((WINDOW_SPR_LIST *)win, spr);
	return spr;
}

#include "window/window_spr_create.c"

/* Unused: builds the 16x16 command icon of overlay `icon` over base 1
   (mnIconBuildCommandIconTiles) and shows it at (x, y) in `win`, in front of
   the other window sprites. Nothing calls it. */
UNUSED WINDOW_SPR *mnWindowSprCreateCommandIcon(
	Sint32 icon, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	Sint32		unusedTile;
	WINDOW_SPR *spr;

	slot = syVRAMCacheFindFreeEntry();
	if(slot == VRAM_CACHE_ENTRY_COUNT) {
		return NULL;
	}
	mnIconBuildCommandIconTiles(icon, 1, &slot, &unusedTile, 1);
	spr = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	/* Not checked: with every sprite in use, this stores through NULL. */
	spr->depth = OAM_DEPTH_FRONT - 4;
	return spr;
}

/* Shows the 32x32 portrait of `character` at (x, y) in `win`, in portrait
   slot `side` (0 or 1, or any other value for the first free one; the second is
   tried first), in PORTRAIT_PALETTE + side. */
WINDOW_SPR *mnPortraitCreate(
	Sint32 character, Sint32 pos, Sint32 side, WINDOW *win, Sint32 x, Sint32 y)
{
	WIN_STATE  *state = (WIN_STATE *)RamHeapSlots->winState;
	WINDOW_SPR *portrait = 0;
	Sint32		slot;
	Sint32		tile;
	Sint32		icon;
	Sint32		palette;

	icon = mnPortraitGetForSpeaker(character);
	if(icon == -1)
		return portrait;

	/* No side given: the second slot if it is free, else the first. */
	if((Uint32)side > 1) {
		Uint32 cur;

		cur = state->portraitPictures[1];
		side = 1;
		if(cur != PORTRAIT_SLOT_EMPTY) {
			side = 0;
			if(state->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
				return portrait;
		}
	}

	palette = PORTRAIT_PALETTE + side;
	mnIconLoadWithPalette(icon, pos, &slot, &tile, palette, 0);
	portrait =
		mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X32, (WINDOW *)win, x, y);
	if(portrait != 0) {
		portrait->spr.oam.pal = palette;
		portrait->kind = WINDOW_SPRITE_PORTRAIT;
	}

	/* Recorded even when no sprite was free and mnWindowSprCreate released the
	   VRAM slot. */
	state->portraitPictures[side] = icon;
	state->portraitSprs[side] = slot;
	return portrait;
}

/* Reloads the portrait picture for the key into a portrait slot with that
   slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache
   entry. A slot of 2 or more means whichever slot already shows the picture,
   the second first; when neither does, nothing is loaded. */
void mnPortraitReload(Uint32 slot, Sint32 key, Sint32 unused)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;
	Sint32	   id = mnPortraitGetForSpeaker(key);
	Sint32	   tile;
	Sint32	   entry;

	if(id == -1)
		return;
	if(slot > 1) {
		if(state->portraitPictures[1] == id)
			slot = 1;
		else if(state->portraitPictures[0] == id)
			slot = 0;
		else
			return;
	}
	entry = state->portraitSprs[slot];
	mnIconLoadWithPalette(
		id, unused, &entry, &tile, PORTRAIT_PALETTE + slot, 1);
}

/* Sets a window sprite's motion and starts it from its first step; does nothing
   for NULL. */
void mnWindowSprSetMotion(WINDOW_SPR *out, Sint32 mode)
{
	if(out != NULL) {
		out->motion = mode;
		out->anmStep = 0;
	}
}

#include "window/window_spr_set_priority.c"

/* A line's window span with no window: left and right both at 224. */
#define WINDOW_SPAN_EMPTY WIN_RANGE(224, 224)

/* The ARM function mnScanlineWindowFillSpans copies to RAM and runs; it returns
   the end of the spans it wrote. */
typedef Uint16 *(*WINDOW_SPAN_ROUTINE)(Uint16 *spans, Uint16 *tilemap);

/* Fills `spans`, a page of the screen transition's window edges past its WININ
   and WINOUT, with the windows' horizontal extent on each of the 160 scan
   lines: (left << 8) | right in the second halfword (WIN1H) of each line's
   pair, read from the UI tilemap by the ARM function mnScanlineWindowBuildSpans
   (window_spans.c), copied into RAM for the call. With no window state, every
   line gets an empty span. */
void mnScanlineWindowFillSpans(void *spans)
{
	WIN_STATE *state = RamHeapSlots->winState;

	if(state == NULL) {
		/* BUG: the fill is one halfword for each of the DISPLAY_HEIGHT lines,
		   but each line takes two, so the lower 80 lines keep the window 1
		   edges this page held when it was last built. */
#if FIX_EMPTY_WINDOW_SPAN_FILL
		DMA_FILL16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 4);
#else
		DMA_FILL16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 2);
#endif
	} else {
		WINDOW_SPAN_ROUTINE routine;

		RAM_CODE_LOAD(routine, mnScanlineWindowBuildSpansCode,
			mnScanlineWindowBuildSpansCodeSize);
		routine(spans, state->tilemap);
		syHeapRelease(routine);
	}
}

/* Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, a
   BG_SCREEN_WIDTH-tile map, with the tilemap entry `value`. */
void mnBgScrFillRect(
	Sint32 unusedA, Sint32 unusedB, Uint32 width, Uint32 height, Sint32 value)
{
	Uint32	row = 0;
	Sint16 *dst = (Sint16 *)WINDOW_TILEMAP_VRAM;

	if(row < height) {
		do {
			Uint32 column = 0;
			if(column < width) {
				do {
					column++;
					*dst = value;
					dst++;
				} while(column < width);
			}
			row++;
			dst += BG_SCREEN_WIDTH - width;
		} while(row < height);
	}
}
