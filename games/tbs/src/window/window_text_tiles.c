// Text drawn as tiles, text tile ownership and divider lines. Part of window.c.

// A new line in tile text advances by a space's width: tile text is one line.
#define TILE_TEXT_NEWLINE_WIDTH JAPANESE_OR(8, FontGlyphWidths[0])

// Tile text is drawn into a canvas of one byte a pixel, a tile tall, then packed into 4bpp tiles in
// place: row `row` of its tile `tile` is then a word of eight pixels.
#define TILE_TEXT_CANVAS_WIDTH  256
#define TILE_TEXT_CANVAS_SIZE   (TILE_TEXT_CANVAS_WIDTH * 8)
#define TILE_TEXT_PACKED_ROW(tile, row)                                                            \
	(((Uint32 *)canvas)[(tile) + (row) * TILE_TEXT_CANVAS_WIDTH / 4])

// In Japanese, the codes above TILE_TEXT_NARROW_LAST (the kana) are a whole tile wide; a kana's
// mark is drawn from a tile glyph of its own, the last two of the window tiles, three pixels wide
// and three pixels back over its kana.
#define TILE_TEXT_NARROW_LAST           128
#define TILE_TEXT_VOICED_MARK_GLYPH     254
#define TILE_TEXT_SEMIVOICED_MARK_GLYPH 255
#define TILE_TEXT_MARK_WIDTH            3

// A 4bpp pixel: the low four bits of a byte, the colour of its palette.
#define PIXEL_4BPP_MASK                 (PALETTE_COLORS - 1)

// A text tile's bits in WIN_STATE.textTileState; 0 is free.
#define TEXT_TILE_TAKEN                 1 // allocated to some text
#define TEXT_TILE_SHOWN                 2 // seen on the tilemap by mnWindowMarkVisibleTextTiles
// A text tile's place in WIN_STATE.textTileState: the TEXT_TILES tiles from TEXT_TILE_FIRST take
// the first places, those from TEXT_EXTRA_TILE_FIRST the next TEXT_TILES.
#define TEXT_TILE_STATE_INDEX(tile)     (((tile) & 0xFF) ^ TEXT_TILE_FIRST)

// Unused: draws window tile pattern over the tile of tilemap entry *entry, its colours mapped
// through remap (0 leaves a pixel as it was), first giving the entry (and the same entry in mirror)
// a text tile of its own.
UNUSED void mnWindowOverlayTilePattern(Uint16 *entry, Uint16 *mirror, Sint32 pattern, Uint8 *remap)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint8      pixels[128];
	Uint8     *sheet = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
	Uint32     tile  = *(Uint8 *)entry;
	Uint8     *src;
	Uint8     *dest;
	Uint32     n;
	Uint32     i;

	dest = pixels;
	src  = BG_CHAR_BLOCK(0) + TILE_SIZE_4BPP * tile;
	for (i = 0; i < TILE_SIZE_4BPP; i++)
	{
		n       = *src++;
		*dest++ = n & PIXEL_4BPP_MASK;
		*dest++ = n >> 4;
	}

	dest = pixels;
	src  = sheet + pattern * TILE_SIZE_4BPP;
	for (i = 0; i < TILE_SIZE_4BPP; i++)
	{
		Uint32 value = *src++;
		Uint32 color = remap[value & PIXEL_4BPP_MASK];

		if (color != 0)
			*dest = color;
		dest++;
		color = remap[value >> 4];
		if (color != 0)
			*dest = color;
		dest++;
	}

	{
		Uint8 *p;

		dest = pixels;
		for (i = 0, p = dest; i < TILE_SIZE_4BPP; i++)
		{
			Uint32 value  = p[0] | p[1] << 4;

			p            += 2;
			*dest++       = value;
		}
	}

	if ((Sint8)tile >= 0)
	{
		for (n = 0; n < TEXT_TILES; n++)
		{
			Uint32 value        = state->nextTextTile;
			Uint32 following    = (value + 1) % TEXT_TILES;

			tile                = (Uint8)value;
			state->nextTextTile = following;
			if (state->textTileState[tile] == 0)
				break;
		}
		state->textTileState[tile]  = TEXT_TILE_TAKEN;
		tile                       |= TEXT_TILE_FIRST;
		*entry                      = WINDOW_TILE(tile);
		*mirror                     = WINDOW_TILE(tile);
	}
	syDMAXCopy(pixels,
	           BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP,
	           DMA_COPY_WORDS(TILE_SIZE_4BPP / 4),
	           REG_DMA3);
}

// Draws a string of 16-bit character codes with the window font into tiles of their own, starting x
// pixels into the first tile, and points tiles (and the same entries in mirror) at them.
Sint32 mnTextDrawStringAsTiles(Uint16 *text, Uint16 *tiles, Uint16 *mirror, Sint32 x)
{
	// LEFTOVER(frame): an unused local the ROM's frame reserves and never
	// * touches.
	UNUSED Uint32 reserve[8];
	Uint8         pens[16];
	WIN_STATE    *state;
	Uint8        *canvas;
	Uint8        *glyphs;
	Uint32        paletteBits;
	Uint32       *src;
	Uint8        *dest;
	Uint32        glyphCode;
	Uint32        code;
	Sint32        tileCount;
	Uint32        glyphWidth;
	Sint32        index;
	Sint32        column;
	Uint32        bits;

	state       = WINDOW_STATE;
	canvas      = syHeapAllocate(TILE_TEXT_CANVAS_SIZE);
	glyphs      = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
	paletteBits = BG_TILE_PALETTE(state->textPalette);
	syWordsClear(pens, 16);
	// pens maps a font pixel, 1 or 3 (the font's two shades), to a colour; 0 leaves the background.
	if (paletteBits == BG_TILE_PALETTE(WINDOW_PALETTE))
	{
		pens[1] = FontColorPens[state->colour & (PALETTE_COLORS - 1)];
		pens[3] = WINDOW_BLACK_COLOR;
		syWordsFill(canvas, TILE_TEXT_CANVAS_SIZE, PIXELS_8BPP(WINDOW_BACKGROUND_COLOR));
	}
	else
	{
		pens[1] = (Uint8)state->colour & (PALETTE_COLORS - 1);
		pens[3] = 1;
		syWordsFill(canvas, TILE_TEXT_CANVAS_SIZE, PIXELS_8BPP(TEXT_BACKGROUND_COLOR));
	}
	// Expand glyph pixels and control-code colour changes into a byte canvas.
	if (text != 0)
	{
		for (;;)
		{
			glyphCode = *text++;
			code      = glyphCode;
			if (glyphCode == 0)
				break;
			if (glyphCode <= TEXT_END_ALT)
			{
				switch (glyphCode)
				{
					case TEXT_COLOR:
						state->colour = *text++;
						pens[1]       = FontColorPens[state->colour & (PALETTE_COLORS - 1)];
						break;
					// TEXT_RESET_STYLE, TEXT_STYLE and TEXT_LINE_SPACING all reset the colour here,
					// and the last two leave their operand to be read as the next code.
					case TEXT_RESET_STYLE:
					case TEXT_STYLE:
					case TEXT_LINE_SPACING:
						state->colour = TEXT_DEFAULT_COLOR;
						pens[1]       = FontColorPens[state->colour];
						break;
					case TEXT_NEWLINE: x += TILE_TEXT_NEWLINE_WIDTH; break;
					// The other codes are skipped with their operands.
					case TEXT_UNUSED_14:
					case TEXT_WINDOW_ANIMATION:
					case TEXT_ACTOR_NAME_START:
						text++;
						// Skip the second operand too. Fall through.
					case TEXT_UNUSED_11:
					case TEXT_UNUSED_12:
					case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
					case TEXT_ARTICLE_CLASS:
#endif
						text++;
						break;
				}
			}
			else
			{
				// A tilemap code (such as WINDOW_TILE_HP_LABEL) draws window tile glyphCode & 0xff.
				glyphCode &= 0xFF;
#if LANGUAGE == LANGUAGE_JAPANESE
				if (glyphCode == TEXT_MARK_VOICED || glyphCode == TEXT_MARK_SEMIVOICED)
				{
					x         -= TILE_TEXT_MARK_WIDTH;
					glyphCode += TILE_TEXT_VOICED_MARK_GLYPH - TEXT_MARK_VOICED;
				}
#endif
				// Each glyph is an 8x8 tile of the window tiles, a word a row.
				src  = (Uint32 *)(glyphs + glyphCode * TILE_SIZE_4BPP);
				dest = canvas + x;
				for (index = 0; index < 8; index++)
				{
					bits = *src++;
					for (column = 0; column < 4; column++)
					{
						Uint8 pen = pens[bits & PIXEL_4BPP_MASK];

						if (pen != 0)
							*dest = pen;
						dest++;
						bits >>= 4;
						pen    = pens[bits & PIXEL_4BPP_MASK];
						if (pen != 0)
							*dest = pen;
						dest++;
						bits >>= 4;
					}
					dest += TILE_TEXT_CANVAS_WIDTH - 8;
				}
				// The sign tiles mnTextDrawPrefixedNumberAtOffset writes (the HP and PP labels'
				// tiles) advance 8 for a first tile and 3 for a second, in pixels; a character its
				// font width, and any other tile below 32 one.
				if (code == WINDOW_TILE_HP_LABEL || code == WINDOW_TILE_PP_LABEL)
					x += 8;
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
				else if (code == WINDOW_TILE_HP_LABEL_END || code == WINDOW_TILE_PP_LABEL_END)
#else
				else if (code == WINDOW_TILE_HP_LABEL_END)
#endif
					x += 3;
				else
				{
#if LANGUAGE == LANGUAGE_JAPANESE
					if (glyphCode > TILE_TEXT_NARROW_LAST)
					{
						if (glyphCode == TILE_TEXT_VOICED_MARK_GLYPH
						    || glyphCode == TILE_TEXT_SEMIVOICED_MARK_GLYPH)
							glyphWidth = TILE_TEXT_MARK_WIDTH;
						else
							glyphWidth = 8;
					}
					else
#endif
						if (glyphCode >= TEXT_SPACE)
						glyphWidth = FontGlyphWidths[glyphCode - TEXT_SPACE];
					else
						glyphWidth = 1;
					x += glyphWidth;
				}
			}
		}
	}

	// Pack each eight-pixel span back into the tile's four-bit colour fields.
	tileCount = (Uint32)(x + 7) >> 3;
	src       = (Uint32 *)canvas;
	dest      = canvas;
	for (index = 0; index < 8; index++)
	{
		for (column = 0; column < tileCount; column++)
		{
			glyphCode        = src[0];
			bits             = src[1];
			glyphCode       |= glyphCode << 4;
			bits            |= bits >> 4;
			glyphCode       &= 0x0FF00FF0;
			bits            &= 0x00FF00FF;
			glyphCode       |= glyphCode << 8;
			bits            |= bits >> 8;
			src             += 2;
			*(Uint32 *)dest  = (glyphCode << 4) >> 16 | bits << 16;
			dest            += 4;
		}
		dest = dest - tileCount * 4 + TILE_TEXT_CANVAS_WIDTH;
		src  = (Uint32 *)((Uint8 *)src - tileCount * 8 + TILE_TEXT_CANVAS_WIDTH);
	}

	// Reuse private text tiles, allocate any new ones, then upload each tile.
	for (index = 0; index < tileCount; index++)
	{
		Uint32  mask = TEXT_TILES - 1;
		Uint32  tile;
		Uint32 *vramTile;

		if (state->extraTextTiles != 0)
			mask = 2 * TEXT_TILES - 1;
		// Text tile n is tile TEXT_TILE_FIRST + n, and with extraTextTiles set the next TEXT_TILES
		// are tiles from TEXT_EXTRA_TILE_FIRST.
		tile = *tiles & TILE_NUMBER_MASK;
		if ((tile < TEXT_TILE_FIRST || tile > TEXT_TILE_FIRST + TEXT_TILES - 1)
		    && (state->extraTextTiles == 0 || tile < TEXT_EXTRA_TILE_FIRST
		        || tile > TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
		{
			tile = state->nextTextTile & mask;
			for (column = 0; column <= mask && state->textTileState[tile] != 0; column++)
				tile = (tile + 1) & mask;
			state->nextTextTile        = (tile + 1) & mask;
			state->textTileState[tile] = TEXT_TILE_TAKEN;
			if (tile > TEXT_TILES - 1)
				tile += TEXT_EXTRA_TILE_FIRST - TEXT_TILES;
			else
				tile |= TEXT_TILE_FIRST;
			*tiles  = paletteBits | tile;
			*mirror = paletteBits | tile;
		}
		vramTile    = (Uint32 *)(BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 0);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 1);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 2);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 3);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 4);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 5);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 6);
		*vramTile++ = TILE_TEXT_PACKED_ROW(index, 7);
		tiles++;
		mirror++;
	}
	syHeapRelease(canvas);
	return tileCount;
}

// Frees the text tiles under a tilemap rectangle (x, y, width and height in tiles) for reuse: their
// taken and shown bits are cleared.
void mnWindowClearTextTilesInRect(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
	Uint8  *base           = WindowStateWork[0];
	Uint16 *cursor         = (Uint16 *)((y * BG_SCREEN_WIDTH + x) * 2 + (Uint32)base);
	Uint32  alternateTiles = ((WIN_STATE *)base)->extraTextTiles;
	Uint32  row;

	for (row = 0; row < height; row++)
	{
		Uint32 column;

		for (column = 0; column < width; column++)
		{
			Uint32 tileId = *cursor++ & TILE_NUMBER_MASK;

			if ((tileId - TEXT_TILE_FIRST) <= TEXT_TILES - 1
			    || (alternateTiles != 0 && tileId > TEXT_EXTRA_TILE_FIRST - 1
			        && tileId <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
			{
				Uint32 attributeIndex = TEXT_TILE_STATE_INDEX(tileId);
				((WIN_STATE *)base)->textTileState[attributeIndex] &=
					~(TEXT_TILE_TAKEN | TEXT_TILE_SHOWN);
			}
		}
		cursor += BG_SCREEN_WIDTH - width;
	}
}

// Marks the text tiles the tilemap shows, and frees those it no longer shows: a tile taken but not
// marked shown (its state exactly TEXT_TILE_TAKEN).
void mnWindowMarkVisibleTextTiles(void)
{
	WIN_STATE *state          = WINDOW_STATE;
	Uint16    *cursor         = state->tilemap;
	Sint32     width          = DISPLAY_TILE_WIDTH;
	Uint32     alternateTiles = state->extraTextTiles;
	Sint32     row;
	Sint32     x;

	// BUG: the cursor never skips the two hidden columns that end each row of BG_SCREEN_WIDTH
	// entries, so the scan covers the tilemap's first DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT
	// entries in order, and the text tiles of the last row and a half are never marked shown: the
	// loop below frees them while they are still on the screen.
	for (row = DISPLAY_TILE_HEIGHT; row != 0; row--)
	{
		for (x = 0; x < width; x++)
		{
			Uint32 tileId = *cursor++ & TILE_NUMBER_MASK;

			if ((tileId - TEXT_TILE_FIRST) <= TEXT_TILES - 1
			    || (alternateTiles != 0 && tileId > TEXT_EXTRA_TILE_FIRST - 1
			        && tileId <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
			{
				Uint32 attributeIndex                 = TEXT_TILE_STATE_INDEX(tileId);

				state->textTileState[attributeIndex] |= TEXT_TILE_SHOWN;
			}
		}
#if FIX_TEXT_TILE_SCAN_ROW_STRIDE
		cursor += BG_SCREEN_WIDTH - width;
#endif
	}

	for (row = 0; row < 2 * TEXT_TILES; row++)
	{
		if (state->textTileState[row] == TEXT_TILE_TAKEN)
			state->textTileState[row] = 0;
	}
}

// Frees the text tiles of the second range (the TEXT_TILES tiles from TEXT_EXTRA_TILE_FIRST), and
// with allowAlternateTiles set lets text take that range too; the first range's tiles keep their
// state.
void mnTextUseExtraTiles(Sint32 allowAlternateTiles)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32     i;

	if (allowAlternateTiles != 0)
	{
		state->extraTextTiles = 1;
		for (i = TEXT_TILES; i <= 2 * TEXT_TILES - 1; i++)
			state->textTileState[i] = 0;
		return;
	}
	state->extraTextTiles = 0;
	for (i = TEXT_TILES; i <= 2 * TEXT_TILES - 1; i++)
		state->textTileState[i] = 0;
}

// Unused: an empty function between the text tiles and the dividers.
UNUSED void mnWindowReservedNoOperation(void)
{
}

// Draws a straight divider line inside a window from tile (x1, y1) to (x2, y2), horizontal or
// vertical: its ends join the tick or the lines they meet, and it crosses the lines it passes over.
void mnWindowDrawDividerLine(WINDOW *window, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint16    *cursor;
	Uint32     pos;
	Uint32     tile;

	if (x1 == x2)
	{
		if (y1 == y2)
			return;
		if (y1 > y2)
		{
			pos = y1;
			y1  = y2;
			y2  = pos;
		}
		mnWindowClearTextTilesInRect(window->x + x2, window->y + y1, 1, y2 - y1);
		cursor = (Uint16 *)(((window->y + y1) * BG_SCREEN_WIDTH + window->x + x2) * 2
		                    + (Uint32)state->tilemap);
		for (pos = y1; pos <= y2; pos++)
		{
			tile = *cursor;
			if (pos == y1)
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_DOWN:
					case WINDOW_TILE_DIVIDER_TEE_RIGHT:
					case WINDOW_TILE_DIVIDER_TEE_LEFT:
					case WINDOW_TILE_DIVIDER_CROSS:
					case WINDOW_TILE_BORDER_TOP_TEE    : break;
					case WINDOW_TILE_BORDER_TOP        : tile = WINDOW_TILE_BORDER_TOP_TEE; break;
					case WINDOW_TILE_DIVIDER_TEE_UP    : tile = WINDOW_TILE_DIVIDER_CROSS; break;
					case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_TEE_DOWN; break;
					default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
				}
			}
			else if (pos == y2)
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_UP:
					case WINDOW_TILE_DIVIDER_TEE_RIGHT:
					case WINDOW_TILE_DIVIDER_TEE_LEFT:
					case WINDOW_TILE_DIVIDER_CROSS:
					case WINDOW_TILE_BORDER_BOTTOM_TEE : break;
					case WINDOW_TILE_BORDER_BOTTOM     : tile = WINDOW_TILE_BORDER_BOTTOM_TEE; break;
					case WINDOW_TILE_DIVIDER_TEE_DOWN  : tile = WINDOW_TILE_DIVIDER_CROSS; break;
					case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_TEE_UP; break;
					default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
				}
			}
			else
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_RIGHT:
					case WINDOW_TILE_DIVIDER_TEE_LEFT:
					case WINDOW_TILE_DIVIDER_CROSS     : break;
					case WINDOW_TILE_DIVIDER_TEE_DOWN  :
					case WINDOW_TILE_DIVIDER_TEE_UP    :
					case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_CROSS; break;
					default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
				}
			}
			*cursor  = tile;
			cursor  += BG_SCREEN_WIDTH;
		}
	}
	else if (y1 == y2)
	{
		if (x1 == x2)
			return;
		if (x1 > x2)
		{
			pos = x1;
			x1  = x2;
			x2  = pos;
		}
		mnWindowClearTextTilesInRect(window->x + x1, window->y + y1, x2 - x1, 1);
		cursor = (Uint16 *)(((window->y + y1) * BG_SCREEN_WIDTH + window->x + x1) * 2
		                    + (Uint32)state->tilemap);
		for (pos = x1; pos <= x2; pos++)
		{
			tile = *cursor;
			if (pos == x1)
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_DOWN:
					case WINDOW_TILE_DIVIDER_TEE_UP:
					case WINDOW_TILE_DIVIDER_TEE_RIGHT:
					case WINDOW_TILE_DIVIDER_CROSS:
					case WINDOW_TILE_BORDER_LEFT_TEE  : break;
					case WINDOW_TILE_BORDER_LEFT      : tile = WINDOW_TILE_BORDER_LEFT_TEE; break;
					case WINDOW_TILE_DIVIDER_TEE_LEFT : tile = WINDOW_TILE_DIVIDER_CROSS; break;
					case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_TEE_RIGHT; break;
					default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
				}
			}
			else if (pos == x2)
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_DOWN:
					case WINDOW_TILE_DIVIDER_TEE_UP:
					case WINDOW_TILE_DIVIDER_TEE_LEFT:
					case WINDOW_TILE_DIVIDER_CROSS:
					case WINDOW_TILE_BORDER_RIGHT_TEE : break;
					case WINDOW_TILE_BORDER_RIGHT     : tile = WINDOW_TILE_BORDER_RIGHT_TEE; break;
					case WINDOW_TILE_DIVIDER_TEE_RIGHT: tile = WINDOW_TILE_DIVIDER_CROSS; break;
					case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_TEE_LEFT; break;
					default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
				}
			}
			else
			{
				switch (tile)
				{
					case WINDOW_TILE_DIVIDER_TEE_DOWN:
					case WINDOW_TILE_DIVIDER_TEE_UP:
					case WINDOW_TILE_DIVIDER_CROSS    : break;
					case WINDOW_TILE_DIVIDER_TEE_RIGHT:
					case WINDOW_TILE_DIVIDER_TEE_LEFT :
					case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_CROSS; break;
					default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
				}
			}
			*cursor = tile;
			cursor++;
		}
	}
}

// Sets the colour (TEXT_COL) text is drawn in.
void mnTextSetColor(Sint32 color)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour    = color & (PALETTE_COLORS - 1);
}

// Sets the style (TXT_STYLE) text is drawn in.
void mnTextSetStyle(Uint16 style)
{
	WIN_STATE *state = WINDOW_STATE;

	state->outline   = style;
}

// Decodes a message and draws it into a text canvas window at pixel (x, y), all at once.
void mnTextDrawMessage(Sint32 msg, WINDOW *window, Sint32 x, Sint32 y)
{
	WIN_STATE *state  = WINDOW_STATE;
	state->textBufEnd = 0;
	mnMessageDecode(msg, 1);
	state->textBuf[state->textBufEnd] = 0;
	state->textBufEnd                 = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
	mnTextDrawWideStringAtOffset(state->textBuf, window, x, y);
}

// Decodes a message and draws it as tiles in a window at pixel (offsetX, offsetY) of its interior.
// offsetY is rounded down to a tile row; offsetX picks the tile, and its remainder is where the
// text starts in it.
void mnTextDrawMessageAsTiles(Sint32 msg, const WINDOW *window, Uint32 offsetX, Uint32 offsetY)
{
	Sint32     byteOffset;
	Sint32     vramAddr;
	Uint16    *text;
	Uint32     cell;
	WIN_STATE *state;

	state             = WINDOW_STATE;
	state->textBufEnd = 0;
	mnMessageDecode(msg, 1);

	state->textBuf[state->textBufEnd] = 0;
	state->textBufEnd                 = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;

	cell = ((window->y + (offsetY >> 3) + 1) << 5) + (window->x + (offsetX >> 3)) + 1;
	if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
	{
		// Inside the tilemap.
		byteOffset = cell * 2;
		vramAddr   = byteOffset + (Sint32)WINDOW_TILEMAP_VRAM;
		text       = state->textBuf;
		mnTextDrawStringAsTiles(text,
		                        (Uint16 *)((Uint8 *)state->tilemap + byteOffset),
		                        (Uint16 *)vramAddr,
		                        7 & offsetX);
	}
}

// Copies a zero-terminated byte string into the text renderer's halfwords. The destination needs
// one halfword per input byte, including the terminator.
static __inline__ void mnTextWidenByteString(const Uint8 *input, Uint16 *output)
{
	while (*input != 0)
	{
		*output = *input;
		input++;
		output++;
	}
	*output = 0;
}

// Draws a byte string into a text canvas window at pixel (x, y). The 256-halfword scratch requires
// at most 255 input bytes before the terminator.
void mnTextDrawString(const Uint8 *text, WINDOW *window, Sint32 x, Sint32 y)
{
	Sint16 *buf;

	buf = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
	mnTextWidenByteString(text, (Uint16 *)buf);
	mnTextDrawWideStringAtOffset((Uint16 *)buf, window, x, y);
	syHeapRelease(buf);
}

// Draws a byte string as tiles in a window at pixel (offsetX, offsetY) of its interior.
void mnTextDrawStringAtOffset(const Uint8  *text,
                              const WINDOW *window,
                              Sint32        offsetX,
                              Sint32        offsetY)
{
	Sint16    *buf;
	WIN_STATE *state;
	Uint32     cell;
	Sint32     vram;
	Sint32     src;
	Sint32     dest;
	Sint32     phase;

	buf   = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
	state = WINDOW_STATE;
	mnTextWidenByteString(text, (Uint16 *)buf);

	cell =
		((window->y + ((Uint32)offsetY >> 3) + 1) << 5) + (window->x + ((Uint32)offsetX >> 3)) + 1;
	if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
	{
		// Inside the tilemap.
		vram   = (Sint32)WINDOW_TILEMAP_VRAM;
		cell  *= 2;
		dest   = vram + cell;
		src    = (Sint32)state->tilemap + cell;
		phase  = offsetX & 7;
		mnTextDrawStringAsTiles((Uint16 *)buf, (Uint16 *)src, (Uint16 *)dest, phase);
		// The scratch is freed only here: a place outside the tilemap leaves it allocated.
		syHeapRelease(buf);
	}
}

// Draws a byte string into a window's tilemap at pixel (x, y) of its interior, rounded down to
// tiles.
void mnTextDrawStringInWindow(const Uint8 *text, WINDOW *window, Uint32 x, Uint32 y)
{
	Uint16 *buf = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
	mnTextWidenByteString(text, buf);
	x >>= 3;
	y >>= 3;
	mnTextDrawWideStringInWindow((Sint16 *)buf, window, x, y);
	syHeapRelease(buf);
}

// Draws a number (width as mnStringFormatNumber) into a text canvas window at pixel (x, y).
void mnTextDrawNumber(Sint32 value, Sint32 width, WINDOW *window, Sint32 x, Sint32 y)
{
	Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

	mnTextDrawString(mnStringFormatNumber(numberText, value, width), window, x, y);
}

// Draws a number (width as mnStringFormatNumber) as tiles in a window at pixel (x, y) of its
// interior.
void mnTextDrawNumberAtOffset(Sint32 value, Sint32 width, const WINDOW *window, Sint32 x, Sint32 y)
{
	Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

	mnTextDrawStringAtOffset(mnStringFormatNumber(numberText, value, width), window, x, y);
}

// Draws a number (width as mnStringFormatNumber) into a window's tilemap at pixel (x, y) of its
// interior.
void mnTextDrawNumberInWindow(Sint32 value, Sint32 width, WINDOW *window, Sint32 x, Sint32 y)
{
	Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

	mnTextDrawStringInWindow(mnStringFormatNumber(numberText, value, width), window, x, y);
}

// Draws a four-digit number after a two-tile label as tiles in a window at pixel (offsetX, offsetY)
// of its interior: the HP label (WINDOW_TILE_HP_LABEL and WINDOW_TILE_HP_LABEL_END) for variant 0,
// the party status window's HP; the PP label (WINDOW_TILE_PP_LABEL and WINDOW_TILE_PP_LABEL_END)
// for variant 1, its PP. The number is padded to four places with spaces.
void mnTextDrawPrefixedNumberAtOffset(
	Sint32 value, const WINDOW *window, Sint32 offsetX, Sint32 offsetY, Sint32 variant)
{
	Uint8      formatted[TEXT_NUMBER_BUFFER_SIZE];
	Sint16     output[8];
	Uint8     *text;
	Sint32     index;
	Uint32     cell;
	WIN_STATE *state;
	Sint32     vram;
	Sint32     src;
	Sint32     dest;
	Sint32     phase;

	state = WINDOW_STATE;
	text  = mnStringFormatNumber(formatted, value, 4);
	// Each label is two tiles; the PP label's second is the HP label's in English, German and
	// Japanese (WINDOW_TILE_PP_LABEL_END).
	if (variant == 0)
	{
		output[0] = WINDOW_TILE_HP_LABEL;
		output[1] = WINDOW_TILE_HP_LABEL_END;
	}
	else
	{
		output[0] = WINDOW_TILE_PP_LABEL;
		output[1] = WINDOW_TILE_PP_LABEL_END;
	}
	for (index = 0; index <= 4; index++)
	{
		output[index + 2] = text[index];
	}
	output[6] = 0;

	cell =
		((window->y + ((Uint32)offsetY >> 3) + 1) << 5) + (window->x + ((Uint32)offsetX >> 3)) + 1;
	if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
	{
		// Inside the tilemap.
		vram   = (Sint32)WINDOW_TILEMAP_VRAM;
		cell  *= 2;
		dest   = vram + cell;
		src    = (Sint32)state->tilemap + cell;
		phase  = offsetX & 7;
		mnTextDrawStringAsTiles((Uint16 *)output, (Uint16 *)src, (Uint16 *)dest, phase);
	}
}

#undef TILE_TEXT_NEWLINE_WIDTH
#undef TILE_TEXT_CANVAS_WIDTH
#undef TILE_TEXT_CANVAS_SIZE
#undef TILE_TEXT_PACKED_ROW
#undef TILE_TEXT_NARROW_LAST
#undef TILE_TEXT_VOICED_MARK_GLYPH
#undef TILE_TEXT_SEMIVOICED_MARK_GLYPH
#undef TILE_TEXT_MARK_WIDTH
#undef PIXEL_4BPP_MASK
#undef TEXT_TILE_TAKEN
#undef TEXT_TILE_SHOWN
#undef TEXT_TILE_STATE_INDEX
