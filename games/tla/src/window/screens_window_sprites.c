/* Window sprites, portraits and the window spans. Part of screens.c. */

/* The bits of a sprite's OAM x (nine) and y (eight), which wrap round the
   screen. */
#define OAM_X_MASK 0x1ff
#define OAM_Y_MASK 0xff

/* Creates a window sprite for the VRAM cache slot `slot` and attaches it to
   `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
   (one tile in from its top-left border). `oamFlags` holds the OAM shape and
   size bits. Releases the slot and returns NULL when no sprite is free. */
WINDOW_SPR *mnWindowSpriteCreate(Sint32 slot, Sint32 oamFlags, WINDOW *window,
	Sint32 offsetX, Sint32 offsetY)
{
	Sint32		x;
	WINDOW_SPR *sprite;
	Sint32		y;

	sprite = mnWindowSpriteAcquireFree();
	if(sprite == NULL) {
		syVRAMCacheFreeEntry((Uint32)slot);
		return NULL;
	}
	/* Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y. */
	x = offsetX + window->x * 8 + 8;
	y = offsetY + window->y * 8 + 8;
	x &= OAM_X_MASK;
	y &= OAM_Y_MASK;
	*(Uint32 *)&sprite->sprite.oam = OBJ_X(x) | y | oamFlags;
	/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a
	   tile). */
	((Uint32 *)&sprite->sprite.oam)[1] = VramCacheEntries[slot].offset >> 5;
	sprite->depth = OAM_DEPTH_FRONT - 1;
	sprite->x = x;
	sprite->y = (Sint16)y;
	sprite->next = NULL;
	sprite->index = (Sint8)slot;
	/* An icon owns its VRAM cache slot, which is freed with it. */
	sprite->kind = WINDOW_SPRITE_ICON;
	sprite->motion = WINDOW_SPRITE_MOTION_STILL;
	mnWindowSpriteAppendToList((WINDOW_SPR_LIST *)window, sprite);
	return sprite;
}

/* Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
   it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSpriteCreateFromResource(
	Sint32 mode, Sint32 resource, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32 slot;

	slot = mnSelectionIconLoad(mode, resource);
	if(slot < 0) {
		return NULL;
	}
	return mnWindowSpriteCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
}

/* Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
   slot and shows it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSpriteCreateLoaded(
	Sint32 icon, Sint32 layers, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *sprite;

	slot = syVRAMCacheFindFreeEntry();
	sprite = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnItemIconLoadLayersIntoSlot(icon, layers, slot);
		sprite =
			mnWindowSpriteCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
	return sprite;
}

/* Unused: builds the 16x16 command icon of overlay `icon` over base 1
   (mnIconBuildCommandIconTiles) and shows it at (x, y) in `window`, in front of
   the other window sprites. Nothing calls it. */
UNUSED WINDOW_SPR *mnWindowSpriteCreateCommandIcon(
	Sint32 icon, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32		slot;
	Sint32		unusedTile;
	WINDOW_SPR *sprite;

	slot = syVRAMCacheFindFreeEntry();
	if(slot == VRAM_CACHE_ENTRY_COUNT) {
		return NULL;
	}
	mnIconBuildCommandIconTiles(icon, 1, &slot, &unusedTile, 1);
	sprite = mnWindowSpriteCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	/* Not checked: with every sprite in use, this stores through NULL. */
	sprite->depth = OAM_DEPTH_FRONT - 4;
	return sprite;
}

/* Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
   slot `side` (0 or 1, or any other value for the first free one; the second is
   tried first), in PORTRAIT_PALETTE + side. */
WINDOW_SPR *mnPortraitCreate(Sint32 character, Sint32 pos, Sint32 side,
	WINDOW *window, Sint32 x, Sint32 y)
{
	WIN_STATE  *state = (WIN_STATE *)RamHeapSlots->windowState;
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
		Uint32 current;

		current = state->portraitPictures[1];
		side = 1;
		if(current != PORTRAIT_SLOT_EMPTY) {
			side = 0;
			if(state->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
				return portrait;
		}
	}

	palette = PORTRAIT_PALETTE + side;
	mnIconLoadWithPalette(icon, pos, &slot, &tile, palette, 0);
	portrait = mnWindowSpriteCreate(
		slot, WINDOW_SPRITE_SIZE_32X32, (WINDOW *)window, x, y);
	if(portrait != 0) {
		portrait->sprite.oam.palette = palette;
		portrait->kind = WINDOW_SPRITE_PORTRAIT;
	}

	/* Recorded even when no sprite was free and mnWindowSpriteCreate released
	   the VRAM slot. */
	state->portraitPictures[side] = icon;
	state->portraitSprites[side] = slot;
	return portrait;
}

/* Reloads the portrait picture for the key into a portrait slot with that
   slot's palette (PORTRAIT_PALETTE + slot), reusing the slot's VRAM cache
   entry. A slot of 2 or more means whichever slot already shows the picture,
   the second first; when neither does, nothing is loaded. */
void mnPortraitReload(Uint32 slot, Sint32 key, Sint32 unused)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;
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
	entry = state->portraitSprites[slot];
	mnIconLoadWithPalette(
		id, unused, &entry, &tile, PORTRAIT_PALETTE + slot, 1);
}

/* Sets a window sprite's motion and starts it from its first step; does nothing
   for NULL. */
void mnWindowSpriteSetMotion(WINDOW_SPR *out, Sint32 mode)
{
	if(out != NULL) {
		out->motion = mode;
		out->animationStep = 0;
	}
}

/* Sets the depth of the window sprite `sprite` to ~priority, which is
   OAM_DEPTH_FRONT - `priority`: a higher priority draws further back. Does
   nothing when `sprite` is NULL. */
void mnWindowSpriteSetPriority(void *sprite, Sint32 priority)
{
	if(sprite != NULL)
		((WINDOW_SPR *)sprite)->depth = ~priority;
}

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
	WIN_STATE *state = RamHeapSlots->windowState;

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
void mnBgScreenFillRect(
	Sint32 unusedA, Sint32 unusedB, Uint32 width, Uint32 height, Sint32 value)
{
	Uint32	row = 0;
	Sint16 *dest = (Sint16 *)WINDOW_TILEMAP_VRAM;

	if(row < height) {
		do {
			Uint32 column = 0;
			if(column < width) {
				do {
					column++;
					*dest = value;
					dest++;
				} while(column < width);
			}
			row++;
			dest += BG_SCREEN_WIDTH - width;
		} while(row < height);
	}
}
