/* @file */
/* Window sprites, portraits and the window spans. */
/* Part of screens.c. */

/* Attribute 0's eight bits of y: a sprite above the screen is at 256 less its
 */
/* distance. */
#define OBJ_Y_MASK 0xff

/* A line's window edges with nothing between them: 224 up to 224, so the */
/* window covers none of the line. */
#define WINDOW_SPAN_EMPTY WIN_RANGE(224, 224)

/* Creates a window sprite for the VRAM cache slot `slot` and attaches it to */
/* `window`, placed at (offsetX, offsetY) pixels from the window's inner corner
 */
/* (one tile in from its top-left border). `oamFlags` holds the OAM shape and */
/* size bits. */
WINDOW_SPR *mnWindowSprCreate(s32 slot, s32 oamFlags, WINDOW *window,
							  s32 offsetX, s32 offsetY) {
	s32 x;
	WINDOW_SPR *sprite;
	s32 y;

	sprite = mnWindowSprAcquireFree();
	if(sprite == NULL) {
		syVramCacheFreeEntry(slot);
		return NULL;
	}

	/* Window tiles to screen pixels, wrapped to OAM's 9-bit X and 8-bit Y. */
	x = offsetX + (window->x * 8) + 8;
	y = offsetY + (window->y * 8) + 8;
	x &= OBJ_X_MASK;
	y &= OBJ_Y_MASK;
	/* UB: attributes 0 and 1, then attribute 2, are written as words through */
	/* the bitfields; UBFIX copies the words. */
#ifdef UBFIX
	{
		u32 words[2];

		words[0] = OBJ_X(x) | y | oamFlags;
		/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a
		 * tile). */
		words[1] = ((VRAM_CACHE_ENTRY *)Ram_VramCacheEntries)[slot].ofs >> 5;
		__builtin_memcpy(&sprite->spr.oam, words, sizeof(words));
	}
#else
	*(u32 *)&sprite->spr.oam = OBJ_X(x) | y | oamFlags;
	/* Attribute 2: the first tile of the slot's VRAM block (32 bytes a tile).
	 */
	((u32 *)&sprite->spr.oam)[1] =
		((VRAM_CACHE_ENTRY *)Ram_VramCacheEntries)[slot].ofs >> 5;
#endif
	sprite->depth = OAM_DEPTH_FRONT;
	sprite->next = NULL;
	sprite->x = x;
	sprite->y = y;
	sprite->idx = slot;
	/* The sprite owns the slot, which is freed with it. */
	sprite->kind = WINDOW_SPRITE_ICON;
	sprite->motion = WINDOW_SPRITE_MOTION_STILL;
	mnWindowSprAppendToList(&window->sprs, sprite);
	return sprite;
}

/* Loads resource `resource` in loading mode `mode` into a VRAM slot and shows
 */
/* it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSprCreateFromResource(s32 mode, s32 resource,
										  WINDOW *window, s32 x, s32 y) {
	s32 slot;

	slot = mnSelectionIconLoad(mode, resource);
	if(slot < 0) {
		return NULL;
	}
	return mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
}

/* Copies icon `icon` (with `layers` passed on to the copier) into a free VRAM
 */
/* slot and shows it as a 16x16 sprite at (x, y) in `window`. */
WINDOW_SPR *mnWindowSprCreateLoaded(s32 icon, s32 layers, WINDOW *window, s32 x,
									s32 y) {
	s32 slot;
	WINDOW_SPR *sprite;

	slot = syVramCacheFindFreeEntry();
	sprite = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnItemIconLoadLayersIntoSlot(icon, layers, slot);
		sprite =
			mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
	return sprite;
}

/* Builds a 16x16 icon from frame pattern 1 with overlay pattern `icon` drawn */
/* over it, and shows it at (x, y) in `window` on draw layer 251. */
WINDOW_SPR *mnWindowSprCreateOverlaidIcon(s32 icon, WINDOW *window, s32 x,
										  s32 y) {
	s32 slot;
	s32 unusedTile;
	WINDOW_SPR *sprite;

	slot = syVramCacheFindFreeEntry();
	if(slot == VRAM_CACHE_ENTRY_COUNT) {
		return NULL;
	}
	mnIconBuildCommandIconTiles(icon, 1, &slot, &unusedTile, 1);
	sprite = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	/* Not checked: with every sprite in use, this stores through NULL. */
	sprite->depth = OAM_DEPTH_FRONT - 4;
	return sprite;
}

/* Loads the pattern at the start of MenuTitlePictures into a free VRAM slot */
/* and shows it as a 16x16 sprite at (x, y) in `window`. `tableEntry` is passed
 */
/* on to the loader, which loads that same pattern for every value. */
WINDOW_SPR *mnWindowSprCreateFromTable(s32 tableEntry, WINDOW *window, s32 x,
									   s32 y) {
	s32 slot;
	WINDOW_SPR *sprite;

	slot = syVramCacheFindFreeEntry();
	sprite = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnTitleLoadFirstTile(tableEntry, 0, slot);
		sprite =
			mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
	return sprite;
}

/* Shows the 32x32 portrait of `character` at (x, y) in `window`, in portrait */
/* slot `side` (0 or 1, or any other value for the first free one; the second */
/* is tried first). */
WINDOW_SPR *mnPortraitCreate(s32 character, s32 unused, s32 side,
							 WINDOW *window, s32 x, s32 y) {
	WIN_STATE *state = WINDOW_STATE;
	WINDOW_SPR *portrait = NULL;
	s32 slot;
	s32 tile;
	s32 icon;
	s32 palette;

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
	if((u32)side > 1) {
		side = 1;
		if(state->portraitPictures[1] != PORTRAIT_SLOT_EMPTY) {
			side = 0;
			if(state->portraitPictures[0] != PORTRAIT_SLOT_EMPTY)
				return portrait;
		}
	}

	palette = PORTRAIT_PALETTE + side;
	mnIconLoadWithPalette(icon, unused, &slot, &tile, palette, 0);
	portrait = mnWindowSprCreate(slot, OUTPUT_SIZE_32X32, window, x, y);
	if(portrait != NULL) {
		portrait->spr.oam.pal = palette;
		portrait->kind = WINDOW_SPRITE_PORTRAIT;
	}

	/* Recorded even when no sprite was free and mnWindowSprCreate released */
	/* the VRAM slot. */
	state->portraitPictures[side] = icon;
	state->portraitSprs[side] = slot;
	return portrait;
}

/* Reloads the portrait image of `character` into portrait slot `side` (0 or */
/* 1), keeping its sprite: used to change the picture of a portrait already on
 */
/* screen. */
void mnPortraitReload(u32 side, s32 character, s32 unused) {
	s32 tile;
	s32 slot;
	u32 icon;
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
			}
			else if(state->portraitPictures[0] == icon) {
				side = 0;
			}
			else {
				return;
			}
		}
		slot = state->portraitSprs[side];
		mnIconLoadWithPalette(icon, unused, &slot, &tile,
							  PORTRAIT_PALETTE + side, 1);
	}
}

/* Switches `sprite` to motion `mode` (WINDOW_SPRITE_MOTION_) from its first */
/* step. Does nothing when `sprite` is NULL. */
void mnWindowSprSetMotion(WINDOW_SPR *sprite, s32 mode) {
	if(sprite != NULL) {
		sprite->motion = mode;
		sprite->anmStep = 0;
	}
}

/* Sets the draw layer of the window sprite `sprite` to 255 - `priority`, so a
 */
/* higher priority draws on a lower layer. Does nothing when `sprite` is NULL.
 */
void mnWindowSprSetPriority(void *sprite, s32 priority) {
	if(sprite != NULL)
		((WINDOW_SPR *)sprite)->depth = ~priority;
}

typedef void (*WINDOW_SPAN_ROUTINE)(u16 *spans, u16 *tilemap);

/* Fills `spans`, a page of the screen transition's window edges past its WININ
 */
/* and WINOUT, with the windows' horizontal extent on each of the 160 scan */
/* lines: (left << 8) | right in the second halfword (WIN1H) of each line's */
/* pair, read from the UI tilemap by the ARM function */
/* mnScanlineWindowBuildEdges, copied into RAM for the call. */
void mnScanlineWindowFillSpans(void *spans) {
	void *tilemap = WindowStateWork[0];

	if(tilemap == NULL) {
		/* BUG: the fill is one halfword for each of the DISPLAY_HEIGHT lines,
		 * but */
		/* each line takes two, so the lower 80 lines keep the window 1 edges */
		/* this page held when it was last built. */
#ifdef BUGFIX
		btDmaFill16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 4);
#else
		btDmaFill16(WINDOW_SPAN_EMPTY, spans, DISPLAY_HEIGHT * 2);
#endif
	}
	else {
		WINDOW_SPAN_ROUTINE routine;

		syRamCodeLoad(routine, mnScanlineWindowBuildEdges,
					  mnScanlineWindowBuildEdgesCodeSize);
		routine(spans, tilemap);
		syHeapRelease(routine);
	}
}

/* Fills the top-left `width` x `height` tiles of WINDOW_TILEMAP_VRAM, 32 tiles
 */
/* wide, with the tilemap entry `value`. */
void mnBgScrFillRect(s32 x, s32 y, u32 width, u32 height, s32 value) {
	u32 row, column;
	s16 *dst = (s16 *)WINDOW_TILEMAP_VRAM;

	for(row = 0; row < height; row++) {
		for(column = 0; column < width; column++)
			*dst++ = value;
		dst += BG_SCREEN_WIDTH - width;
	}
}
