/* Window sprites, portraits and the window spans. Part of screens.c. */

/* Attribute 0's eight bits of y: a sprite above the screen is at 256 less its
   distance. */
#define OBJ_Y_MASK 0xff

/* A line's window edges with nothing between them: 224 up to 224, so the window
   covers none of the line. */
#define WINDOW_SPAN_EMPTY WIN_RANGE(224, 224)

/* Creates a window sprite for the VRAM cache slot `slot` and attaches it to
   `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
   (one tile in from its top-left border). `oamFlags` holds the OAM shape and
   size bits. */
WINDOW_SPR *mnWindowSpriteCreate(Sint32 slot, Sint32 oamFlags, WINDOW *window,
	Sint32 offsetX, Sint32 offsetY)
{
	Sint32		x;
	WINDOW_SPR *sprite;
	Sint32		y;

	sprite = mnWindowSpriteAcquireFree();
	if(sprite == NULL) {
		syVRAMCacheFreeEntry(slot);
		return NULL;
	}

	/* Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y. */
	x = offsetX + (window->x * 8) + 8;
	y = offsetY + (window->y * 8) + 8;
	x &= OBJ_X_MASK;
	y &= OBJ_Y_MASK;
	/* UB: attributes 0 and 1, then attribute 2, are written as words through
	   the bitfields; UBFIX copies the words. */
#if FIX_TYPE_PUNNING
	{
		Uint32 words[2];

		words[0] = OBJ_X(x) | y | oamFlags;
		/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a
		   tile). */
		words[1] = ((VRAM_CACHE_ENTRY *)RamVramCacheEntries)[slot].offset >> 5;
		__builtin_memcpy(&sprite->sprite.oam, words, sizeof(words));
	}
#else
	*(Uint32 *)&sprite->sprite.oam = OBJ_X(x) | y | oamFlags;
	/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a
	   tile). */
	((Uint32 *)&sprite->sprite.oam)[1] =
		((VRAM_CACHE_ENTRY *)RamVramCacheEntries)[slot].offset >> 5;
#endif
	sprite->depth = OAM_DEPTH_FRONT;
	sprite->next = NULL;
	sprite->x = x;
	sprite->y = y;
	sprite->index = slot;
	/* The sprite owns the slot, which is freed with it. */
	sprite->kind = WINDOW_SPRITE_ICON;
	sprite->motion = WINDOW_SPRITE_MOTION_STILL;
	mnWindowSpriteAppendToList(&window->sprites, sprite);
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

/* Builds a 16x16 icon from tick pattern 1 with overlay pattern `icon` drawn
   over it, and shows it at (x, y) in `window` on draw layer 251. */
WINDOW_SPR *mnWindowSpriteCreateOverlaidIcon(
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

/* Loads the pattern at the start of MenuTitlePictures into a free VRAM slot and
   shows it as a 16x16 sprite at (x, y) in `window`. `tableEntry` is passed on
   to the loader, which loads that same pattern for every value. */
WINDOW_SPR *mnWindowSpriteCreateFromTable(
	Sint32 tableEntry, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *sprite;

	slot = syVRAMCacheFindFreeEntry();
	sprite = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnTitleLoadFirstTile(tableEntry, 0, slot);
		sprite =
			mnWindowSpriteCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
	return sprite;
}

/* Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait
   slot `side` (0 or 1, or any other value for the first free one; the second is
   tried first). */
WINDOW_SPR *mnPortraitCreate(Sint32 character, Sint32 unused, Sint32 side,
	WINDOW *window, Sint32 x, Sint32 y)
{
	WIN_STATE  *state = WINDOW_STATE;
	WINDOW_SPR *portrait = NULL;
	Sint32		slot;
	Sint32		tile;
	Sint32		icon;
	Sint32		palette;

	if(gmFlagGetStub(FLAG_CHILDHOOD) != 0) {
		if(character == ISAAC)
			character = SPRITE_ISAAC_CHILD;
		if(character == GARET)
			character = SPRITE_GARET_CHILD;
	}

	icon = mnPortraitGetForSpeaker(character);
	if(icon == -1)
		return portrait;

	/* No side given: the second slot if it is free, else the first. */
	if((Uint32)side > 1) {
		side = 1;
		if(state->portraitPictures[1] != PORTRAIT_SLOT_EMPTY) {
			side = 0;
			if(state->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
				return portrait;
		}
	}

	palette = PORTRAIT_PALETTE + side;
	mnIconLoadWithPalette(icon, unused, &slot, &tile, palette, 0);
	portrait = mnWindowSpriteCreate(slot, OUTPUT_SIZE_32X32, window, x, y);
	if(portrait != NULL) {
		portrait->sprite.oam.palette = palette;
		portrait->kind = WINDOW_SPRITE_PORTRAIT;
	}

	/* Recorded even when no sprite was free and mnWindowSpriteCreate released
	   the VRAM slot. */
	state->portraitPictures[side] = icon;
	state->portraitSprites[side] = slot;
	return portrait;
}

/* Reloads the portrait image of `character` into portrait slot `side` (0 or 1),
   keeping its sprite: used to change the picture of a portrait already on
   screen. */
void mnPortraitReload(Uint32 side, Sint32 character, Sint32 unused)
{
	Sint32	   tile;
	Sint32	   slot;
	Uint32	   icon;
	WIN_STATE *state;

	state = WINDOW_STATE;

	if(gmFlagGetStub(FLAG_CHILDHOOD) != 0) {
		if(character == ISAAC)
			character = SPRITE_ISAAC_CHILD;
		if(character == GARET)
			character = SPRITE_GARET_CHILD;
	}

	icon = mnPortraitGetForSpeaker(character);
	if(icon != -1U) {
		if(side > 1U) {
			if(state->portraitPictures[1] == icon) {
				side = 1;
			} else if(state->portraitPictures[0] == icon) {
				side = 0;
			} else {
				return;
			}
		}
		slot = state->portraitSprites[side];
		mnIconLoadWithPalette(
			icon, unused, &slot, &tile, PORTRAIT_PALETTE + side, 1);
	}
}

/* Switches `sprite` to motion `mode` (WINDOW_SPRITE_MOTION_) from its first
   step. Does nothing when `sprite` is NULL. */
void mnWindowSpriteSetMotion(WINDOW_SPR *sprite, Sint32 mode)
{
	if(sprite != NULL) {
		sprite->motion = mode;
		sprite->animationStep = 0;
	}
}

/* Sets the draw layer of the window sprite `sprite` to 255 - `priority`, so a
   higher priority draws on a lower layer. Does nothing when `sprite` is
   NULL. */
void mnWindowSpriteSetPriority(void *sprite, Sint32 priority)
{
	if(sprite != NULL)
		((WINDOW_SPR *)sprite)->depth = ~priority;
}

typedef void (*WINDOW_SPAN_ROUTINE)(Uint16 *spans, Uint16 *tilemap);

/* Fills `spans`, a page of the screen transition's window edges past its WININ
   and WINOUT, with the windows' horizontal extent on each of the 160 scan
   lines: (left << 8) | right in the second halfword (WIN1H) of each line's
   pair, read from the UI tilemap by the ARM function
   mnScanlineWindowBuildEdges, copied into RAM for the call. */
void mnScanlineWindowFillSpans(void *spans)
{
	void *tilemap = WindowStateWork[0];

	if(tilemap == NULL) {
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

		RAM_CODE_LOAD(routine, mnScanlineWindowBuildEdges,
			mnScanlineWindowBuildEdgesCodeSize);
		routine(spans, tilemap);
		syHeapRelease(routine);
	}
}

/* Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, 32 tiles
   wide, with the tilemap entry `value`. */
void mnBgScreenFillRect(
	Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 value)
{
	Uint32	row, column;
	Sint16 *dest = (Sint16 *)WINDOW_TILEMAP_VRAM;

	for(row = 0; row < height; row++) {
		for(column = 0; column < width; column++)
			*dest++ = value;
		dest += BG_SCREEN_WIDTH - width;
	}
}
