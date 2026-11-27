/* Characters drawn into windows, as glyph sprites, into a text canvas or as
   tilemap tiles, and the per-frame update that moves and shows the sprites
   windows carry. Part of window.c. */

typedef Sint32 (*GLYPH_ROUTINE)(
	WINDOW *, Uint32, Uint32, Uint32, const Uint8 *);

/* A glyph sprite's size in its OAM attribute 1: 16 by 16, four tiles. */
#define GLYPH_SPRITE_SIZE (OBJ_SIZE_16X16 >> 16)

/* The glyph function RAM_CODE_LOAD copies to RAM to run, and its size, as this
   symbol's address. */
extern const Uint8 mnGlyphDrawCode[];
extern Uint8	   mnGlyphDrawCodeSize[];

Sint32 mnTextDrawGlyphPair(Sint32 character, Uint32 *tiles);

/* The page arrow's offset from the window's last inner tile, and a sprite
   glyph's from its text position, in pixels; the later editions moved both. */
#define ARROW_X JAPANESE_OR(3, 4)
#define ARROW_Y JAPANESE_OR(1, (-1))
#define GLYPH_X JAPANESE_OR(1, 2)
#define GLYPH_Y JAPANESE_OR(0, (-2))

/* Draws a character of a window's text with its top-left corner at pixel (x, y)
   of the window's interior and returns how far the text advances. A text canvas
   window draws it into the canvas; other windows show it as a sprite carried by
   the window. GLYPH_DRAW_PAGE_ARROW shows the next-page arrow instead. A sprite
   glyph takes a window sprite and its four tiles from the glyph sheet
   (WIN_STATE.glyphTiles, four for each sprite); with no sprite free nothing is
   drawn and it returns 0. The sprite stays until its window is cleared or
   closed. */
Sint32 mnGlyphDraw(
	WINDOW *window, Sint32 character, Sint32 x, Sint32 y, Sint32 mode)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   width;
	Uint32	   spacingX = state->letterSpacing;
	Uint32	   spacingY = state->lineSpacing;

	if(mode != GLYPH_DRAW_PAGE_ARROW &&
		(window->flg & WINDOW_FLAG_TEXT_CANVAS) != 0) {
#if LANGUAGE == LANGUAGE_JAPANESE
		Uint8		 *font = NULL;
		GLYPH_ROUTINE draw;
		Sint32		  result;
		Sint16		  mark;

		/* The small font, and the kana's voicing mark drawn as a second glyph
		   after it. */
		if(Ram_HeapSlots->battleMsg->win == window) {
			font = syResourceGetTableEntry(RESOURCE_SMALL_FONT);
			width = 3;
			if(character == ' ')
				goto done;
		}
		mark = Font_MarkGlyphCodes[(Uint32)character >> TEXT_MARK_SHIFT];
		character &= (1 << TEXT_MARK_SHIFT) - 1;
		width = 4;
		if(character != ' ') {
			RAM_CODE_LOAD(draw, mnGlyphDrawCode, mnGlyphDrawCodeSize);
			result = draw(window, character, x, y, font);
			{
				Uint16 markCode = mark;

				if(markCode != 0)
					result += draw(window, markCode, x + result, y, font);
			}
			syHeapRelease(draw);
			return result;
		}
#else
		Uint8		 *font;
		GLYPH_ROUTINE draw;
		Sint32		  result;

		if(Ram_HeapSlots->battleMsg->win == window) {
			syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
			width = 3;
			if(character == ' ')
				goto done;
		}
		font = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
		width = 4;
		if(character != ' ') {
			RAM_CODE_LOAD(draw, mnGlyphDrawCode, mnGlyphDrawCodeSize);
			result = draw(window, character, x, y, font);
			syHeapRelease(draw);
			return result;
		}
#endif
	} else if(character == ' ') {
		width = WINDOW_SPRITE_SPACE_WIDTH;
	} else {
		Uint32		tiles[32];
		WINDOW_SPR *output;
		QUEUED_SPR *sprite;
		Sint32		index;

		output = mnWindowSprAcquireFree();
		if(output == NULL)
			return 0;
		index = (output - state->sprs) * 4;
		output->motion = WINDOW_SPRITE_MOTION_STILL;
		output->kind = WINDOW_SPRITE_PLAIN;
		if(mode == GLYPH_DRAW_PAGE_ARROW) {
			width = 1;
			output->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
		} else {
			switch(state->outline) {
			case TEXT_STYLE_SHAKE:
				output->motion = WINDOW_SPRITE_MOTION_SHAKE;
				break;
			case TEXT_STYLE_POP:
				output->motion = WINDOW_SPRITE_MOTION_POP;
				output->anmStep = 8;
				break;
			case TEXT_STYLE_SPIN:
				output->motion = WINDOW_SPRITE_MOTION_SPIN;
				output->anmStep = 0;
				break;
			case TEXT_STYLE_WOBBLE:
				output->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				output->anmStep = 0;
				break;
			}
			{
				Sint32 drawn = mnTextDrawGlyphPair(character, tiles);

				/* The Japanese edition keeps a width of 0. */
#if ZERO_WIDTH_GLYPH_ADVANCES
				if(drawn == 0)
					drawn = 1;
#endif
				width = drawn;
			}
		}

		if((Uint8)output->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
			sprite = mnWindowSprGetSpr(output);
			if(state->glyphResource == NO_GLYPH_RESOURCE)
				state->glyphResource = syVRAMCacheFindFreeEntry();
			sprite->oam.x =
				((Uint16)window->x << 3) + ((window->width - 2) << 3) + ARROW_X;
			sprite->oam.y = ((Uint16)window->y << 3) +
				((window->height - 2) << 3) + ARROW_Y;
		} else {
			Uint16 *attr;

			syDMAXCopy(tiles,
				(void *)(OBJ_VRAM0 +
					(state->glyphTiles + index) * TILE_SIZE_4BPP),
				DMA_COPY_WORDS(32), REG_DMA3);
			attr = (Uint16 *)&output->spr.oam;
			if(state->battleMode & WINDOW_GLYPHS_BLENDED)
				*attr++ =
					(y + (spacingY >> 1) + (Uint16)window->y * 8 + GLYPH_Y) |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			else
				*attr++ = y + (spacingY >> 1) + (Uint16)window->y * 8 + GLYPH_Y;
			*attr = (x + (spacingX >> 1) + (Uint16)window->x * 8 + GLYPH_X) |
				GLYPH_SPRITE_SIZE;
			attr[1] = state->glyphTiles + index;
			sprite = mnWindowSprGetSpr(output);
		}
		output->depth = OAM_DEPTH_FRONT - 2;
		output->x = sprite->oam.x;
		output->y = sprite->oam.y;
		output->idx = index;
		output->next = NULL;
		mnWindowSprAppendToList(&window->sprs, output);
	}
done:
	return width;
}

/* Where the glyph's sprite sits against the text position, in pixels: the
   Japanese font's glyphs are drawn into their tiles differently (see
   mnTextDrawGlyphPair). */
#define GLYPH_SPRITE_OFFSET_X JAPANESE_OR(1, 2)
#define GLYPH_SPRITE_OFFSET_Y JAPANESE_OR(0, (-2))

Sint32 mnTextDrawGlyphPair(Sint32 code, Uint32 *tiles);

/* Draws character as a glyph sprite on the window at x, y and returns its
   width, or 0 when no window sprite is free. */
Sint32 mnGlyphDrawAsSpr(WINDOW *window, Sint32 character, Sint32 x, Sint32 y)
{
	WIN_STATE  *state = Ram_HeapSlots->winState;
	Uint32		tiles[32];
	Uint32		spacingX = state->letterSpacing;
	Uint32		spacingY = state->lineSpacing;
	WINDOW_SPR *output;
	Sint32		index;
	Sint32		width;

	output = mnWindowSprAcquireFree();
	if(output == NULL)
		return 0;
	index = (output - state->sprs) * 4;
	output->motion = WINDOW_SPRITE_MOTION_STILL;
	output->kind = WINDOW_SPRITE_PLAIN;
	width = mnTextDrawGlyphPair(character, tiles);
	syDMAXCopy(tiles,
		(void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP),
		DMA_COPY_WORDS(32), REG_DMA3);
	((Uint16 *)&output->spr.oam)[0] =
		y + (spacingY >> 1) + (Uint16)window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
	((Uint16 *)&output->spr.oam)[1] =
		(x + (spacingX >> 1) + (Uint16)window->x * 8 + GLYPH_SPRITE_OFFSET_X) |
		GLYPH_SPRITE_SIZE;
	((Uint16 *)&output->spr.oam)[2] = state->glyphTiles + index;
	output->depth = OAM_DEPTH_FRONT - 2;
	output->x = mnWindowSprGetSpr(output)->oam.x;
	output->y = mnWindowSprGetSpr(output)->oam.y;
	output->idx = index;
	output->next = NULL;
	mnWindowSprAppendToList(&window->sprs, output);
	return width;
}

/* Draws the character source names into a 16x16 tile pair and copies it to
   destination. */
void mnGlyphDrawToTiles(const void *source, void *destination)
{
	Uint8 buffer[128];

	mnTextDrawGlyphPair((Sint32)source, (Uint32 *)buffer);
	syDMAXCopy(buffer, destination, DMA_COPY_WORDS(32), REG_DMA3);
}

/* Places a glyph sprite on the window at x, y as mnGlyphDrawAsSpr does, but
   draws nothing, and returns the VRAM address of its tiles for the caller to
   fill, or NULL when no window sprite is free. */
void *mnGlyphAddBlankSpr(WINDOW *window, Sint32 character, Sint32 x, Sint32 y)
{
	WIN_STATE *state = Ram_HeapSlots->winState;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Uint32		tiles[32];
	Uint32		spacingX = state->letterSpacing;
	Uint32		spacingY = state->lineSpacing;
	WINDOW_SPR *output;
	Sint32		index;
	void	   *vram;

	output = mnWindowSprAcquireFree();
	if(output == NULL)
		return NULL;
	index = (output - state->sprs) * 4;
	output->motion = WINDOW_SPRITE_MOTION_STILL;
	output->kind = WINDOW_SPRITE_PLAIN;
	vram = (void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP);
	((Uint16 *)&output->spr.oam)[0] =
		y + (spacingY >> 1) + (Uint16)window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
	((Uint16 *)&output->spr.oam)[1] =
		(x + (spacingX >> 1) + (Uint16)window->x * 8 + GLYPH_SPRITE_OFFSET_X) |
		GLYPH_SPRITE_SIZE;
	((Uint16 *)&output->spr.oam)[2] = state->glyphTiles + index;
	output->depth = OAM_DEPTH_FRONT - 2;
	output->x = mnWindowSprGetSpr(output)->oam.x;
	output->y = mnWindowSprGetSpr(output)->oam.y;
	output->idx = index;
	output->next = NULL;
	/* FAKEMATCH(dead): motion stored a second time with the 1 it already
	 * holds. */
	output->motion = WINDOW_SPRITE_MOTION_STILL;
	mnWindowSprAppendToList(&window->sprs, output);
	return vram;
}

/* Where the next-page arrow sits on the interior's last tile, in pixels right
   and down from its corner. It moves with the text: the Japanese glyph sprites
   also sit 1 pixel further left and 2 lower (mnGlyphDrawAsSpr). */
#define PAGE_ARROW_OFFSET_X JAPANESE_OR(3, 4)
#define PAGE_ARROW_OFFSET_Y JAPANESE_OR(1, (-1))

#if (LANGUAGE == LANGUAGE_JAPANESE)
/* The tiles a voiced mark takes in place of its own over the frame's top edge
   and over a divider; a semi-voiced mark takes the next. */
#define MARK_OVER_BORDER_TILE  30
#define MARK_OVER_DIVIDER_TILE 14
#endif

/* Puts a character into a window's tilemap as a tile at tile (x, y) of its
   interior; GLYPH_DRAW_PAGE_ARROW instead shows the next-page arrow sprite on
   the interior's last tile. In Japanese, a kana mark goes in the tile above the
   character before it. */
void mnWindowPutGlyph(WINDOW *win, Uint32 tile, Uint32 x, Uint32 y, Sint32 mode)
{
	WIN_STATE  *base = WINDOW_STATE;
	Uint16	   *tilemap = base->tilemap[0];
	WINDOW_SPR *out;
	Sint32		idx;
	QUEUED_SPR *sprite;
	Uint32		pos;

	if(y > (win->height - 2))
		return;
	if(x > (win->width - 2))
		return;
	if(mode == GLYPH_DRAW_PAGE_ARROW) {
		out = mnWindowSprAcquireFree();
		if(out == NULL)
			return;
		idx = (out - base->sprs) * 4;
		out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
		sprite = mnWindowSprGetSpr(out);
		if(base->glyphResource == NO_GLYPH_RESOURCE)
			base->glyphResource = syVRAMCacheFindFreeEntry();
		sprite->oam.x =
			(win->x << 3) + ((win->width - 2) << 3) + PAGE_ARROW_OFFSET_X;
		sprite->oam.y =
			(win->y << 3) + ((win->height - 2) << 3) + PAGE_ARROW_OFFSET_Y;
		out->x = sprite->oam.x;
		out->y = sprite->oam.y;
		out->idx = idx;
		out->next = NULL;
		if((Uint8)out->motion == 0)
			out->motion = mode;
		mnWindowSprAppendToList(&win->sprs, out);
	} else if(tile <= TEXT_LAST_FONT_CODE) {
#if (LANGUAGE == LANGUAGE_JAPANESE)
		if(tile == TEXT_MARK_VOICED || tile == TEXT_MARK_SEMIVOICED) {
			/* One up and one left of where a character at (x, y) goes. Over the
			   frame's top edge or a divider, the mark takes another tile. */
			Uint32 line;

			line = (win->y + y) * BG_SCREEN_WIDTH;
			/* XXX (no evidence): whether a mark ever lands on a top edge or a
			   divider, where this puts tiles 30 and 31 (in
			   window_tiles_tla_ja.png the HP label's second tile and the PP
			   label) or 14 and 15 (the dividers themselves); the render probe
			   stages scenes only, and cannot open a tla-ja window drawn by
			   tiles with a voiced kana on its top line. */
			switch(tilemap[line + (win->x + x)]) {
			case WINDOW_TILE_BORDER_TOP:
				tile -= TEXT_MARK_VOICED - MARK_OVER_BORDER_TILE;
				break;
			case WINDOW_TILE_DIVIDER_HORIZONTAL:
				tile -= TEXT_MARK_VOICED - MARK_OVER_DIVIDER_TILE;
				break;
			}
		} else {
			/* Past the border's top-left tile, and only inside the tilemap. */
			x++;
			y++;
		}
#else
		/* Past the border's top-left tile, and only inside the tilemap. */
		x++;
		y++;
#endif
		pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
		if(pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
			*(Uint16 *)((Uint32)tilemap + (pos << 1)) = WINDOW_TILE(tile);
	}
}

/* A window tilemap's width in tiles, and its entries over the screen's rows. */
#define TILEMAP_WIDTH	BG_SCREEN_WIDTH
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * DISPLAY_TILE_HEIGHT)

/* Writes value into a window's tilemap at tile (x, y) of its interior, with the
   palette mode names (TILEMAP_ENTRY_), or as it is for TILEMAP_ENTRY_PLAIN;
   TILEMAP_ENTRY_NONE writes nothing. y -1 is the border's top row, where the
   menus put their page numbers and arrows (menu/menu_shared.c); a place beyond
   the bottom or right frame, or outside the tilemap, is ignored. */
void mnWindowSetTilemapEntry(
	WINDOW *window, Sint32 value, Sint32 x, Sint32 y, Uint32 mode)
{
	Uint16 *map = WINDOW_STATE->tilemap[0];
	Sint32	palette;
	Sint32	index;

	y += 1;
	x += 1;
	if((Uint32)y > (Uint32)(window->height - 1))
		return;
	if((Uint32)x > (Uint32)(window->width - 1))
		return;

	switch(mode) {
	case TILEMAP_ENTRY_WINDOW:
		palette = BG_TILE_PALETTE(WINDOW_PALETTE);
		break;
	case TILEMAP_ENTRY_HIGHLIGHT:
		palette = BG_TILE_PALETTE(HIGHLIGHT_PALETTE);
		break;
	case TILEMAP_ENTRY_ICON:
		palette = BG_TILE_PALETTE(1);
		break;
	default:
		palette = 0;
		break;
	}

	/* Both entries are stored at the entry's byte offset from map. */
	switch(mode) {
	case TILEMAP_ENTRY_NONE:
		return;
	case TILEMAP_ENTRY_HIGHLIGHT:
	case TILEMAP_ENTRY_WINDOW:
	case TILEMAP_ENTRY_ICON:
		index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
		if((Uint32)index >= TILEMAP_ENTRIES)
			return;
		*(Uint16 *)((Uint8 *)map + (index << 1)) = palette | value;
		return;
	case TILEMAP_ENTRY_PLAIN:
	default:
		index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
		if((Uint32)index >= TILEMAP_ENTRIES)
			return;
		*(Uint16 *)((Uint8 *)map + (index << 1)) = value;
		return;
	}
}

extern const Uint16 WindowSprite_BounceScales[];

/* The steps of WindowSprite_BounceScales: a swell to 1.5 times and back. */
#define BOUNCE_SCALE_STEPS 32

/* Steps a scaling sprite's animation (WINDOW_SPRITE_MOTION_SWELL to
   WINDOW_SPRITE_MOTION_SHRINK_HALF) and sets its affine scale for this
   frame. */
void mnWindowSprUpdateScaleAnimation(WINDOW_SPR *windowSprite)
{
	Sint32			 scale = Q_8_8(1);
	QUEUED_SPR		*sprite = mnWindowSprGetSpr(windowSprite);
	AFFINE_TRANSFORM effect;

	switch((Uint8)windowSprite->motion) {
	case WINDOW_SPRITE_MOTION_SWELL:
		scale = WindowSprite_BounceScales[windowSprite->anmStep++ &
			(BOUNCE_SCALE_STEPS - 1)];
		break;
	case WINDOW_SPRITE_MOTION_SWELL_HALF:
		scale = WindowSprite_BounceScales[windowSprite->anmStep++ &
					(BOUNCE_SCALE_STEPS - 1)] >>
			1;
		break;
	/* The shrinks play the bounce's second half at twice its pace. */
	case WINDOW_SPRITE_MOTION_SHRINK:
		if(windowSprite->anmStep < BOUNCE_SCALE_STEPS / 4)
			scale = WindowSprite_BounceScales[windowSprite->anmStep++ * 2 +
				BOUNCE_SCALE_STEPS / 2];
		break;
	case WINDOW_SPRITE_MOTION_SHRINK_HALF:
		if(windowSprite->anmStep < BOUNCE_SCALE_STEPS / 4)
			scale = WindowSprite_BounceScales[windowSprite->anmStep++ * 2 +
						BOUNCE_SCALE_STEPS / 2] >>
				1;
		break;
	}
	if(scale == Q_8_8(1)) {
		sprite->oam.matrix = 0;
		sprite->oam.affineMode = OAM_AFFINE_OFF;
		sprite->oam.x = windowSprite->x;
		sprite->oam.y = windowSprite->y;
	} else {
		effect.scaleX = scale;
		effect.scaleY = scale;
		effect.ang = 0;
		sprite->oam.matrix = syOAMMatrixAllocFromTransform(&effect);
		if(scale > Q_8_8(1)) {
			sprite->oam.affineMode = OAM_AFFINE_DOUBLE_SIZE;
			/* Drawn at double size, so set 8 pixels up and left. */
			sprite->oam.x = windowSprite->x - 8;
			sprite->oam.y = windowSprite->y - 8;
		} else {
			sprite->oam.affineMode = OAM_AFFINE_ON;
			sprite->oam.x = windowSprite->x;
			sprite->oam.y = windowSprite->y;
		}
	}
}

/* A window sprite's queued OAM attributes, as mnWindowSprUpdateAll writes
   them. */
typedef struct ANIM_SPR {
	struct ANIM_SPR *next;
	Uint8			 y;
	Uint8			 affine : 2;
	Uint8			 mode : 2;
	Uint8			 mosaic : 1;
	Uint8			 cols : 1;
	Uint8			 shape : 2;
	Uint16			 x : 9;
	Uint16			 affineIdx : 5;
	Uint16			 size : 2;
	Uint16			 tile : 10;
	Uint16			 pri : 2;
	Uint16			 pal : 4;
} ANIM_SPR;

/* A window sprite as mnWindowSprUpdateAll reads it, its kind and motion
   unsigned. */
typedef struct ANIM_OUTPUT {
	struct ANIM_OUTPUT *next;
	Uint8				kind;
	Uint8				motion;
	Uint16				x;
	Uint16				y;
	Uint16				unused;
	Uint16				anmStep;
	Uint8				idx;
	Uint8				depth;
	ANIM_SPR			attr;
} ANIM_OUTPUT;

/* A pop's affine transform as mnWindowSprUpdateAll packs it: x and y scale and
   the angle. */
typedef struct POP_TRANSFORM {
	Uint32 scaleX : 16;
	Uint32 scaleY : 16;
	Uint32 ang : 16;
	Uint32 padding : 16;
} POP_TRANSFORM;

/* The page arrow's pictures, a frame each. */
extern const Uint8 WindowSprite_PageArrowTiles[];

extern const Uint8 WindowSprite_CursorBobOffsets[];
extern const Sint8 WindowSprite_WobbleOffsets[][2];
extern const Sint8 WindowSprite_BounceOffsets[];

/* A frame of the next-page arrow: an 8 by 16 sprite of 256 colours. */
#define PAGE_ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/* The steps of WindowSprite_WobbleOffsets and WindowSprite_BounceOffsets. */
#define WOBBLE_STEPS		   20
#define HOP_STEPS			   16

/* The per-frame sprite update: moves each sprite that an open window carries by
   its animation mode (WINDOW_SPRITE_MOTION_) and queues it for drawing, at its
   depth (WINDOW_SPR.depth). */
void mnWindowSprUpdateAll(void)
{
	WIN_STATE *state = WINDOW_STATE;
	WINDOW	  *window = state->wins;
	Sint32	   no;

	union {
		AFFINE_TRANSFORM plain;
		POP_TRANSFORM	 packed;
	} effect;

	for(no = 0; no != WINDOW_COUNT; window++, no++) {
		ANIM_OUTPUT *output;
		Uint32		 phase;

		if(!(window->flg & WINDOW_FLAG_OPEN))
			continue;
		output = (ANIM_OUTPUT *)window->sprs.head;
		while(phase = (VBlankCount >> 2) & 7, output != NULL) {
			ANIM_SPR *sprite = &output->attr;

			switch(output->motion) {
			case WINDOW_SPRITE_MOTION_PAGE_ARROW:
				if(state->glyphResource != VRAM_CACHE_ENTRY_COUNT) {
					const Uint8 *tiles = &WindowSprite_PageArrowTiles[phase *
						PAGE_ARROW_FRAME_BYTES];

					sprite->tile = syVRAMCacheLoad(
						state->glyphResource, PAGE_ARROW_FRAME_BYTES, tiles);
					output->idx = sprite->tile;
					sprite->mode = OBJ_MODE_NORMAL;
					sprite->mosaic = 0;
					sprite->cols = OBJ_COLORS_256;
					sprite->size = 0;
					sprite->shape = OBJ_SHAPE_VERTICAL;
					sprite->y = output->y +
						WindowSprite_CursorBobOffsets[VBlankCount % 80] + 2;
					sprite->affine = OAM_AFFINE_OFF;
					sprite->affineIdx = 0;
				}
				break;
			case WINDOW_SPRITE_MOTION_SHAKE:
				if(VBlankCount & 1) {
					sprite->x = output->x +
						(Sint32)(((syRandomBelow(3)) + (syRandomBelow(3))) >>
							1) -
						1;
					sprite->y = output->y +
						(Sint32)(((syRandomBelow(3)) + (syRandomBelow(3))) >>
							1) -
						1;
				}
				break;
			case WINDOW_SPRITE_MOTION_POP:
				if(output->anmStep != 0) {
					effect.packed.scaleX = Q_8_8(2);
					effect.packed.scaleY = Q_8_8(2);
					effect.packed.ang = 0;
					sprite->affineIdx =
						syOAMMatrixAllocFromTransform(&effect.plain);
					sprite->affine = OAM_AFFINE_DOUBLE_SIZE;
					sprite->x = output->x - 5;
					sprite->y = output->y - 5;
					output->anmStep--;
				} else {
					sprite->affineIdx = 0;
					sprite->affine = OAM_AFFINE_OFF;
					sprite->x = output->x;
					sprite->y = output->y;
				}
				break;
			case WINDOW_SPRITE_MOTION_SPIN:
				/* animationStep is the angle, three 256ths of a turn a frame, a
				   turn in about 85 frames, while the sprite circles its place
				   four pixels out. */
				effect.plain.scaleX = Q_8_8(1);
				effect.plain.scaleY = Q_8_8(1);
				output->anmStep += TRIG_TURN * 3 / 256;
				effect.plain.ang = output->anmStep;
				sprite->affineIdx =
					syOAMMatrixAllocFromTransform(&effect.plain);
				sprite->affine = OAM_AFFINE_ON;
				sprite->x = output->x -
					FIXEDtoINT_FOUR_TIMES(
						sySin(effect.plain.ang + TRIG_TURN * 29 / 32)) -
					2;
				sprite->y = output->y -
					FIXEDtoINT_FOUR_TIMES(
						syCos(effect.plain.ang + TRIG_TURN * 13 / 32)) -
					2;
				break;
			case WINDOW_SPRITE_MOTION_WOBBLE:
				if(VBlankCount & 1)
					output->anmStep++;
				sprite->x = output->x +
					WindowSprite_WobbleOffsets[output->anmStep % WOBBLE_STEPS]
											  [0];
				sprite->y = output->y +
					WindowSprite_WobbleOffsets[output->anmStep % WOBBLE_STEPS]
											  [1] -
					2;
				break;
			case WINDOW_SPRITE_MOTION_HOP_UP:
				output->anmStep++;
				sprite->y = output->y -
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE:
				output->anmStep++;
				sprite->y = output->y +
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_LEFT:
				output->anmStep++;
				sprite->x = output->x -
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_RIGHT:
				output->anmStep++;
				sprite->x = output->x +
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT:
				output->anmStep++;
				sprite->x = output->x -
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				sprite->y = output->y +
					WindowSprite_BounceOffsets[output->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_SWELL:
			case WINDOW_SPRITE_MOTION_SWELL_HALF:
			case WINDOW_SPRITE_MOTION_SHRINK:
			case WINDOW_SPRITE_MOTION_SHRINK_HALF:
				mnWindowSprUpdateScaleAnimation((WINDOW_SPR *)output);
				break;
			}
			if(output->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
				if(state->glyphResource != VRAM_CACHE_ENTRY_COUNT)
					syOAMAddSprAtDepth(sprite, OAM_DEPTH_FRONT);
			} else if(output->motion == WINDOW_SPRITE_MOTION_ON_SCREEN) {
				Sint32 x = (Sint16)output->x;
				Sint32 y = (Sint16)output->y;

				if((Uint32)(x + 64) < DISPLAY_WIDTH + 64 &&
					(Uint32)(y + 64) < DISPLAY_HEIGHT + 64)
					syOAMAddSprAtDepth(sprite, output->depth);
			} else if(output->motion != WINDOW_SPRITE_MOTION_HIDDEN) {
				syOAMAddSprAtDepth(sprite, output->depth);
			}
			output = output->next;
		}
	}
}

#undef ARROW_X
#undef ARROW_Y
#undef GLYPH_X
#undef GLYPH_Y
#undef GLYPH_SPRITE_OFFSET_X
#undef GLYPH_SPRITE_OFFSET_Y
#undef PAGE_ARROW_OFFSET_X
#undef PAGE_ARROW_OFFSET_Y
#if (LANGUAGE == LANGUAGE_JAPANESE)
#undef MARK_OVER_BORDER_TILE
#undef MARK_OVER_DIVIDER_TILE
#endif
#undef TILEMAP_WIDTH
#undef TILEMAP_ENTRIES
