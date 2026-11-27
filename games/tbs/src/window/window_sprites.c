/* Characters drawn into windows, and the per-tick sprite update. Part of
   window.c. */

/* Pixels added to the page arrow's and the sprite glyphs' screen y, which
   differ by language. */
#define PAGE_ARROW_Y_OFFSET	  JAPANESE_OR(1, (-1))
#define GLYPH_SPRITE_Y_OFFSET JAPANESE_OR(0, (-2))

typedef Sint32 (*GLYPH_ROUTINE)(
	WINDOW *, Uint32, Uint32, Uint32, const Uint8 *);

/* A glyph sprite's size in OAM attribute 1: 16x16, with the square shape. */
#define GLYPH_SPRITE_SIZE (OBJ_SIZE_16X16 >> 16)

/* Draws a character of a window's text with its top-left corner at pixel (x, y)
   of the window's interior and returns how far the text advances. */
Sint32 mnGlyphDraw(
	WINDOW *win, Sint32 character, Sint32 x, Sint32 y, Sint32 mode)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   spacingX = state->letterSpacing;
	Sint32	   width;
	Uint32	   spacingY = state->lineSpacing;

	/* A text canvas window draws the glyph into its canvas. */
	if(mode != GLYPH_DRAW_PAGE_ARROW &&
		(win->flg & WINDOW_FLAG_TEXT_CANVAS) != 0) {
		Uint8		 *font;
		GLYPH_ROUTINE draw;
		Sint32		  result;
#if LANGUAGE == LANGUAGE_JAPANESE
		Sint32 mark;

		font = NULL;
		if(BattleMessageStateWork->win == win) {
			font = syResourceGetTableEntry(RESOURCE_SMALL_FONT);
			width = 3;
			if(character == ' ')
				goto done;
		}
		mark = (Sint16)
			FontMarkGlyphCodes[(Uint32)character >> TEXT_CODE_MARK_SHIFT];
		character &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
		width = 4;
#else
		if(BattleMessageStateWork->win == win) {
			syResourceGetTableEntry(RESOURCE_SMALL_FONT);
			syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
			width = 3;
			if(character == ' ')
				goto done;
		}
		font = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
		width = 4;
#endif
		if(character != ' ') {
			RAM_CODE_LOAD(draw, mnGlyphDrawArm, mnGlyphDrawCodeSize);
			result = draw(win, character, x, y, font);
#if LANGUAGE == LANGUAGE_JAPANESE
			if((Uint16)mark != 0)
				result += draw(win, (Uint16)mark, x + result, y, font);
#endif
			syHeapRelease(draw);
			return result;
		}
	} else if(character == ' ') {
		width = WINDOW_SPRITE_SPACE_WIDTH;
	} else {
		/* Other windows show each glyph as a 16x16 sprite, animated by the text
		   style (the motions of TXT_STYLE) or, with GLYPH_DRAW_PAGE_ARROW, as
		   the page arrow. */
		Uint32		tiles[32];
		WINDOW_SPR *winSpr;
		QUEUED_SPR *spr;
		Sint32		idx;

		winSpr = mnWindowSprAcquireFree();
		if(winSpr == NULL)
			return 0;
		/* The sprite's own four of the glyph tiles. */
		idx = (winSpr - state->sprs) * 4;
		winSpr->motion = WINDOW_SPRITE_MOTION_STILL;
		winSpr->kind = WINDOW_SPRITE_PLAIN;
		if(mode == GLYPH_DRAW_PAGE_ARROW) {
			width = 1;
			winSpr->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
		} else {
			switch(state->outline) {
			case TEXT_STYLE_SHAKE:
				winSpr->motion = WINDOW_SPRITE_MOTION_SHAKE;
				break;
			case TEXT_STYLE_POP:
				winSpr->motion = WINDOW_SPRITE_MOTION_POP;
				winSpr->anmStep = 8;
				break;
			case TEXT_STYLE_SPIN:
				winSpr->motion = WINDOW_SPRITE_MOTION_SPIN;
				winSpr->anmStep = 0;
				break;
			case TEXT_STYLE_WOBBLE:
				winSpr->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				winSpr->anmStep = 0;
				break;
			}
#if !ZERO_WIDTH_GLYPH_ADVANCES
			width = mnTextDrawGlyphPair(character, tiles);
#else
			{
				Sint32 drawn = mnTextDrawGlyphPair(character, tiles);

				if(drawn == 0)
					drawn = 1;
				width = drawn;
			}
#endif
		}

		if(winSpr->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
			/* The page arrow sits in the window's bottom-right corner. */
			spr = &winSpr->spr;
			if(state->glyphResource == NO_GLYPH_RESOURCE)
				state->glyphResource = syVRAMCacheFindFreeEntry();
			spr->oam.x = (win->x << 3) + ((win->width - 2) << 3) + 4;
			spr->oam.y =
				(win->y << 3) + ((win->height - 2) << 3) + PAGE_ARROW_Y_OFFSET;
		} else {
			/* Copy the glyph's tiles to its sprite tiles (sprite VRAM) and
			   place it:
			   half the line spacing below y and half the letter spacing right
			   of x, in screen pixels. */
			syDMAXCopy(tiles,
				(void *)(OBJ_VRAM0 +
					(state->glyphTiles + idx) * TILE_SIZE_4BPP),
				DMA_COPY_WORDS(32), REG_DMA3);
			((Uint16 *)&winSpr->spr.oam)[0] =
				y + (spacingY >> 1) + win->y * 8 + GLYPH_SPRITE_Y_OFFSET;
			((Uint16 *)&winSpr->spr.oam)[1] =
				(x + (spacingX >> 1) + win->x * 8 + 2) | GLYPH_SPRITE_SIZE;
			((Uint16 *)&winSpr->spr.oam)[2] = state->glyphTiles + idx;
			spr = &winSpr->spr;
		}
		winSpr->depth = OAM_DEPTH_FRONT - 1;
		winSpr->x = spr->oam.x;
		winSpr->y = spr->oam.y;
		winSpr->idx = idx;
		winSpr->next = NULL;
		mnWindowSprAppendToList(&win->sprs, winSpr);
	}
done:
	return width;
}

#if LANGUAGE != LANGUAGE_JAPANESE

/* Puts a character into a window's tilemap as a tile at tile (x, y) of its
   interior. */
void mnWindowPutGlyph(WINDOW *win, Uint32 tile, Uint32 x, Uint32 y, Sint32 mode)
{
	void	   *state = WindowStateWork[0];
	WIN_STATE  *base = state;
	WINDOW_SPR *out;
	Sint32		idx;
	Uint16	   *slot;
	QUEUED_SPR *spr;
	Uint32		pos;
	Uint16		row;

	if(y > (win->height - 2))
		return;
	if(x > (win->width - 2))
		return;
	if(mode == GLYPH_DRAW_PAGE_ARROW) {
		Sint32 column = (Uint16)-2;
		/* FAKEMATCH(temporary): state, done with the window state, holds the
		 * acquired sprite until out takes it. */
		state = mnWindowSprAcquireFree();
		if(state == NULL)
			return;
		out = state;
		idx = (out - base->sprs) * 4;
		out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
		spr = &out->spr;
		slot = &base->glyphResource;
		if(*slot == NO_GLYPH_RESOURCE)
			*slot = syVRAMCacheFindFreeEntry();
		/* 0xfffe and 254 are -2: the arrow sits on the interior's last tile, 4
		   pixels right and 1 up. */
		spr->oam.x = (win->x + (column + win->width)) * 8 + 4;
#if FIX_UNSEQUENCED_SIDE_EFFECT
		/* Write row twice, in order. */
		row = (Uint8)win->height + 254;
		row = (Uint8)win->y + row;
#else
		row = (Uint8)win->y + (row = (Uint8)win->height + 254);
#endif
		spr->oam.y = row * 8 - 1;
		out->x = spr->oam.x;
		out->y = spr->oam.y;
		out->next = NULL;
		out->idx = idx;
		if(out->motion == 0)
			out->motion = mode;
		mnWindowSprAppendToList(&win->sprs, out);
	} else if(tile <= TEXT_LAST_FONT_CODE) {
		/* Past the border's top-left tile, and only inside the tilemap. */
		x++;
		y++;
		pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
		if(pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
			((WIN_STATE *)state)->tilemap[pos] = WINDOW_TILE(tile);
	}
}

#else

/* The tiles a voiced mark takes in place of its own over the tick's top edge
   and over a divider; a semi-voiced mark takes the next. */
#define VOICED_MARK_OVER_BORDER_TILE  30
#define VOICED_MARK_OVER_DIVIDER_TILE 14

/* The Japanese mnWindowPutGlyph, which also places the kana marks. */
void mnWindowPutGlyph(WINDOW *win, Uint32 tile, Uint32 x, Uint32 y, Sint32 mode)
{
	Uint8 *state = WindowStateWork[0];
	/* FAKEMATCH(dead): two XORs with tile that cancel. */
	Uint8	   *base = (Uint8 *)((unsigned long)state ^ tile);
	WINDOW_SPR *out;
	Sint32		idx;
	Uint16	   *slot;
	QUEUED_SPR *spr;
	Uint32		pos;
	Uint16		row;

	base = (Uint8 *)((unsigned long)base ^ tile);

	if(y > (win->height - 2))
		return;
	if(x > (win->width - 2))
		return;
	if(mode == GLYPH_DRAW_PAGE_ARROW) {
		Sint32 column = (Uint16)-2;
		/* FAKEMATCH(temporary): left, a copy of win->x for the one sum; as
		 * win->x. */
		Uint16 left;
		out = mnWindowSprAcquireFree();
		if(out == NULL)
			return;
		idx = (out - ((WIN_STATE *)base)->sprs) * 4;
		out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
		spr = &out->spr;
		slot = &((WIN_STATE *)base)->glyphResource;
		if(*slot == NO_GLYPH_RESOURCE)
			*slot = syVRAMCacheFindFreeEntry();
		left = win->x;
		spr->oam.x = (left + (column + win->width)) * 8 + 4;
		/* UB: row is written twice without a sequence point. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
		row = (Uint8)win->height + 254;
		row = (Uint8)win->y + row;
#else
		row = (Uint8)win->y + (row = (Uint8)win->height + 254);
#endif
		spr->oam.y = row * 8 + 1;
		out->x = spr->oam.x;
		out->y = spr->oam.y;
		out->next = NULL;
		out->idx = idx;
		if(out->motion == 0)
			out->motion = mode;
		mnWindowSprAppendToList(&win->sprs, out);
	} else if(tile <= TEXT_LAST_FONT_CODE) {
		if(tile - TEXT_MARK_VOICED <= 1) {
			/* A kana mark goes in the tile above the character before it. Over
			   the tick's top edge or a divider it takes another tile. XXX (no
			   evidence): whether a voiced kana ever lands on a window's top
			   edge or a divider, where it takes VOICED_MARK_OVER_BORDER_TILE
			   (parts of the Japanese HP and PP labels) or
			   VOICED_MARK_OVER_DIVIDER_TILE (the dividers); no scene staged so
			   far opens such a window. */
			Uint32 line;

			line = (win->y + y) * BG_SCREEN_WIDTH;
			switch(*(((WIN_STATE *)base)->tilemap + (line + (win->x + x)))) {
			case WINDOW_TILE_BORDER_TOP:
				tile -= TEXT_MARK_VOICED - VOICED_MARK_OVER_BORDER_TILE;
				break;
			case WINDOW_TILE_DIVIDER_HORIZONTAL:
				tile -= TEXT_MARK_VOICED - VOICED_MARK_OVER_DIVIDER_TILE;
				break;
			}
		} else {
			x++;
			y++;
		}
		pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
		if(pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
			((WIN_STATE *)state)->tilemap[pos] = WINDOW_TILE(tile);
	}
}

#endif

/* The windows' tilemap entries in use: a screen block's rows down the
   screen. */
#define TILEMAP_ENTRIES (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)

/* Writes value into a window's tilemap at tile (x, y) of its interior, with the
   palette mode names (TILEMAP_ENTRY_), or as it is for TILEMAP_ENTRY_PLAIN;
   TILEMAP_ENTRY_NONE writes nothing. y -1 is the border's top row, where the
   menus put their page numbers and arrows (menu/menu_shared.c); a place beyond
   the bottom or right tick, or outside the tilemap, is ignored. */
void mnWindowSetTilemapEntry(
	WINDOW *win, Sint32 value, Sint32 x, Sint32 y, Uint32 mode)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   palette;
	Sint32	   idx;

	y += 1;
	x += 1;
	if(y > (Uint32)(win->height - 1))
		return;
	if(x > (Uint32)(win->width - 1))
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

	switch(mode) {
	case TILEMAP_ENTRY_NONE:
		return;
	case TILEMAP_ENTRY_HIGHLIGHT:
	case TILEMAP_ENTRY_WINDOW:
	case TILEMAP_ENTRY_ICON:
		idx = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
		if((Uint32)idx >= TILEMAP_ENTRIES)
			return;
		state->tilemap[idx] = palette | value;
		return;
	case TILEMAP_ENTRY_PLAIN:
	default:
		idx = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
		if((Uint32)idx >= TILEMAP_ENTRIES)
			return;
		state->tilemap[idx] = value;
		return;
	}
}

/* The steps of WindowSpriteBounceScales: a swell to 1.5 times and back. */
#define BOUNCE_SCALE_STEPS 32

/* Steps a scaling sprite's animation (WINDOW_SPRITE_MOTION_SWELL to
   WINDOW_SPRITE_MOTION_SHRINK_HALF) and sets its affine scale for this
   tick. */
void mnWindowSprUpdateScaleAnimation(WINDOW_SPR *winSpr)
{
	/* Scales are 8.8 fixed point: Q_8_8(1) is the sprite's own size. */
	Sint32			 scale = Q_8_8(1);
	QUEUED_SPR		*spr = &winSpr->spr;
	AFFINE_TRANSFORM effect;

	switch(winSpr->motion) {
	case WINDOW_SPRITE_MOTION_SWELL:
		scale = WindowSpriteBounceScales[winSpr->anmStep++ &
			(BOUNCE_SCALE_STEPS - 1)];
		break;
	case WINDOW_SPRITE_MOTION_SWELL_HALF:
		scale = WindowSpriteBounceScales[winSpr->anmStep++ &
					(BOUNCE_SCALE_STEPS - 1)] >>
			1;
		break;
	/* The shrinks play the bounce's second half at twice its pace. */
	case WINDOW_SPRITE_MOTION_SHRINK:
		if(winSpr->anmStep < BOUNCE_SCALE_STEPS / 4)
			scale = WindowSpriteBounceScales[winSpr->anmStep++ * 2 +
				BOUNCE_SCALE_STEPS / 2];
		break;
	case WINDOW_SPRITE_MOTION_SHRINK_HALF:
		if(winSpr->anmStep < BOUNCE_SCALE_STEPS / 4)
			scale = WindowSpriteBounceScales[winSpr->anmStep++ * 2 +
						BOUNCE_SCALE_STEPS / 2] >>
				1;
		break;
	}
	if(scale == Q_8_8(1)) {
		spr->oam.matrix = 0;
		spr->oam.affineMode = SPRITE_AFFINE_OFF;
		spr->oam.x = winSpr->x;
		spr->oam.y = winSpr->y;
	} else {
		effect.scaleX = scale;
		effect.scaleY = scale;
		effect.ang = 0;
		spr->oam.matrix = syOAMMatrixAllocFromTransform(&effect);
		/* Larger than its size the sprite needs the double-size box; smaller,
		   it fits its own. */
		if(scale > Q_8_8(1)) {
			spr->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
			spr->oam.x = winSpr->x - 8;
			spr->oam.y = winSpr->y - 8;
		} else {
			spr->oam.affineMode = SPRITE_AFFINE_ON;
			spr->oam.x = winSpr->x;
			spr->oam.y = winSpr->y;
		}
	}
}

/* A frame of the next-page arrow: an 8 by 16 sprite of 256 colours. */
#define PAGE_ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/* The steps of WindowSpriteWobbleOffsets and WindowSpriteBounceOffsets. */
#define WOBBLE_STEPS		   20
#define HOP_STEPS			   16

/* The per-tick sprite update: moves each sprite that an open window carries by
   its animation mode (WINDOW_SPRITE_MOTION_) and queues it for drawing, at its
   depth slot (WINDOW_SPR.depth). */
void mnWindowSprUpdateAll(void)
{
	WIN_STATE		*state = WINDOW_STATE;
	WINDOW			*win = state->wins;
	Sint32			 i;
	AFFINE_TRANSFORM effect;

	for(i = 0; i != WINDOW_COUNT; win++, i++) {
		WINDOW_SPR *winSpr;
		Uint32		phase;

		if(!(win->flg & WINDOW_FLAG_OPEN))
			continue;
		winSpr = win->sprs.head;
		/* The page arrow's picture: one of 8, each shown for 4 frames. */
		phase = (FrameCount >> 2) & 7;
		while(winSpr != NULL) {
			QUEUED_SPR *spr = &winSpr->spr;

			if(win->anm == WINDOW_ANIMATION_SWELL) {
				winSpr->anmStep = 2;
				winSpr->motion = WINDOW_SPRITE_MOTION_PULSE;
			}
			switch(winSpr->motion) {
			case WINDOW_SPRITE_MOTION_PAGE_ARROW:
				/* VRAM_CACHE_ENTRY_COUNT means no entry was free for the
				   arrow. */
				if(state->glyphResource != VRAM_CACHE_ENTRY_COUNT) {
					const Uint8 *tiles = &WindowSprite_PageArrowTiles[phase *
						PAGE_ARROW_FRAME_BYTES];

					spr->oam.idx = syVRAMCacheLoad(
						state->glyphResource, PAGE_ARROW_FRAME_BYTES, tiles);
					winSpr->idx = spr->oam.idx;
					spr->oam.objMode = OBJ_MODE_NORMAL;
					spr->oam.mosaic = 0;
					spr->oam.fullCol = 1;
					spr->oam.size = 0;
					spr->oam.shape = SPRITE_SHAPE_TALL;
					spr->oam.y = winSpr->y +
						WindowSpriteCursorBobOffsets[FrameCount % 80] + 2;
					spr->oam.affineMode = SPRITE_AFFINE_OFF;
					spr->oam.matrix = 0;
				}
				break;
			case WINDOW_SPRITE_MOTION_SHAKE:
				/* Every other tick each axis moves -1 to 1 pixels from its
				   place:
				   half the sum of two random 0 to 2, less 1. */
				if(FrameCount & 1) {
					spr->oam.x = winSpr->x +
						(Sint32)((syRandom16Below(3) + syRandom16Below(3)) >>
							1) -
						1;
					spr->oam.y = winSpr->y +
						(Sint32)((syRandom16Below(3) + syRandom16Below(3)) >>
							1) -
						1;
				}
				break;
			case WINDOW_SPRITE_MOTION_POP:
				/* Twice its size for animationStep ticks, which mnGlyphDraw
				   starts at 8. */
				if(winSpr->anmStep == 0) {
					spr->oam.matrix = 0;
					spr->oam.affineMode = SPRITE_AFFINE_OFF;
					spr->oam.x = winSpr->x;
					spr->oam.y = winSpr->y;
					break;
				}
				effect.scaleX = Q_8_8(2);
				effect.scaleY = Q_8_8(2);
				effect.ang = 0;
				spr->oam.matrix = syOAMMatrixAllocFromTransform(&effect);
				spr->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
				spr->oam.x = winSpr->x - 5;
				spr->oam.y = winSpr->y - 5;
				winSpr->anmStep--;
				break;
			case WINDOW_SPRITE_MOTION_SPIN:
				/* animationStep is the angle, three 256ths of a turn a tick, a
				   turn in about 85 ticks, while the sprite circles its place
				   four pixels out (a sine shifted down 14 bits). */
				effect.scaleX = Q_8_8(1);
				effect.scaleY = Q_8_8(1);
				winSpr->anmStep += TRIG_TURN * 3 / 256;
				effect.ang = winSpr->anmStep;
				spr->oam.matrix = syOAMMatrixAllocFromTransform(&effect);
				spr->oam.affineMode = SPRITE_AFFINE_ON;
				spr->oam.x = winSpr->x -
					(sySin(effect.ang + TRIG_TURN * 29 / 32) >> 14) - 2;
				spr->oam.y = winSpr->y -
					(syCos(effect.ang + TRIG_TURN * 13 / 32) >> 14) - 2;
				break;
			case WINDOW_SPRITE_MOTION_WOBBLE:
				/* One step of WindowSpriteWobbleOffsets every other tick. */
				if(FrameCount & 1)
					winSpr->anmStep++;
				spr->oam.x = winSpr->x +
					WindowSpriteWobbleOffsets[winSpr->anmStep % WOBBLE_STEPS]
											 [0];
				spr->oam.y = winSpr->y +
					WindowSpriteWobbleOffsets[winSpr->anmStep % WOBBLE_STEPS]
											 [1] -
					2;
				break;
			case WINDOW_SPRITE_MOTION_HOP_UP:
				winSpr->anmStep++;
				spr->oam.y = winSpr->y -
					WindowSpriteBounceOffsets[winSpr->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_DOWN:
			case WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE:
			case WINDOW_SPRITE_MOTION_HOP_DOWN_UNUSED:
				winSpr->anmStep++;
				spr->oam.y = winSpr->y +
					WindowSpriteBounceOffsets[winSpr->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT:
				winSpr->anmStep++;
				spr->oam.x = winSpr->x -
					WindowSpriteBounceOffsets[winSpr->anmStep &
						(HOP_STEPS - 1)];
				spr->oam.y = winSpr->y +
					WindowSpriteBounceOffsets[winSpr->anmStep &
						(HOP_STEPS - 1)];
				break;
			case WINDOW_SPRITE_MOTION_PULSE:
				/* 1.25 times for animationStep ticks, then its own size. */
				if(winSpr->anmStep != 0) {
					effect.scaleX = Q_8_8(1.25);
					effect.scaleY = Q_8_8(1.25);
					effect.ang = 0;
					spr->oam.matrix = syOAMMatrixAllocFromTransform(&effect);
					spr->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
					spr->oam.x = winSpr->x - 8;
					spr->oam.y = winSpr->y - 8;
					winSpr->anmStep--;
					break;
				}
				spr->oam.matrix = 0;
				spr->oam.affineMode = SPRITE_AFFINE_OFF;
				spr->oam.x = winSpr->x;
				spr->oam.y = winSpr->y;
				break;
			case WINDOW_SPRITE_MOTION_SWELL:
			case WINDOW_SPRITE_MOTION_SWELL_HALF:
			case WINDOW_SPRITE_MOTION_SHRINK:
			case WINDOW_SPRITE_MOTION_SHRINK_HALF:
				mnWindowSprUpdateScaleAnimation((WINDOW_SPR *)winSpr);
				break;
			}
			/* Queue it unless it is hidden, or an arrow with no picture. */
			if(winSpr->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
				if(state->glyphResource != VRAM_CACHE_ENTRY_COUNT)
					syOAMAddSprAtDepth(spr, winSpr->depth);
			} else if(winSpr->motion != WINDOW_SPRITE_MOTION_HIDDEN) {
				syOAMAddSprAtDepth(spr, winSpr->depth);
			}
			winSpr = winSpr->next;
			phase = (FrameCount >> 2) & 7;
		}
	}
}

#undef PAGE_ARROW_Y_OFFSET
#undef GLYPH_SPRITE_Y_OFFSET
#undef GLYPH_SPRITE_SIZE
#undef TILEMAP_ENTRIES
#undef BOUNCE_SCALE_STEPS
#undef PAGE_ARROW_FRAME_BYTES
#undef WOBBLE_STEPS
#undef HOP_STEPS
#undef VOICED_MARK_OVER_BORDER_TILE
#undef VOICED_MARK_OVER_DIVIDER_TILE
