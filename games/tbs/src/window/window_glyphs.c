/* Characters rendered into tiles, string widths and number formatting. Part of
   window.c. */

/* mnTextDrawGlyphPair draws its glyphs into a buffer of one byte per pixel
   (syDecompressBitRuns writes a glyph's rows there in a colour), in rows 16
   bytes apart, then packs its top 16 rows into 4bpp tiles. A glyph is drawn
   from row 2, its shadow a pixel right of and below it, and a kana's mark at
   the top right. */
#define GLYPH_BUFFER_WIDTH 16
#define GLYPH_PIXEL(x, y)  ((y) * GLYPH_BUFFER_WIDTH + (x))
/* The pixels across a tile, which the packing reads a row at a time. */
#define GLYPH_TILE_WIDTH   8

/* The first character of a pair, in the low byte of its code, and the second,
   in bits 8 to 23. */
#define GLYPH_PAIR_FIRST(code)	((code) & 0xff)
#define GLYPH_PAIR_SECOND(code) (((code) << 8) >> 16)

/* The code a character past the font is measured as: '@' in the font's
   Latin-1. */
#define TEXT_PAST_FONT_CODE 64

/* The colour of Mind Read's text, which has no shadow. */
#define MIND_READ_TEXT_COLOR 8
/* The colour of a glyph's shadow, a pixel right of and below it. */
#define GLYPH_SHADOW_COLOR	 1

#if LANGUAGE != LANGUAGE_JAPANESE
/* Draws a character, and a second one when the code carries it in bits 8 to 15,
   into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour over a
   shadow, or outlined in TEXT_STYLE_OUTLINED. Returns the pixels the pair
   takes. */
Sint32 mnTextDrawGlyphPair(Sint32 code, Uint32 *out)
{
	/* 24 rows, of which the top 16 are packed. */
	Uint8		 pixels[24 * GLYPH_BUFFER_WIDTH];
	WIN_STATE	*state;
	const Uint8 *glyph;
	Sint32		 width;
	Sint32		 colour;
	Sint32		 shadowColour;
	Sint32		 firstIdx;
	Sint32		 secondIdx;
	Uint16		 secondWidth;
	Uint8		*pixel;
	Sint32		 tileColumn;
	Sint32		 tileRow;
	Sint32		 line;
	Sint32		 pixelsLeft;
	Uint32		 tileLine;

	state = WINDOW_STATE;
	secondIdx = GLYPH_PAIR_SECOND(code);
	code = GLYPH_PAIR_FIRST(code);
	syWordsClear(pixels, sizeof(pixels));
	if(state->mindRead != 0) {
		shadowColour = 0;
		colour = MIND_READ_TEXT_COLOR;
	} else {
		shadowColour = GLYPH_SHADOW_COLOR;
		colour = state->colour;
	}
	firstIdx = code - TEXT_SPACE;
	glyph = firstIdx * sizeof(GLYPH) + (const Uint8 *)FontGlyphs;
	width = ((const GLYPH *)glyph)->width;
	glyph = (const Uint8 *)((const GLYPH *)glyph)->rows;
	/* Outlined text is the glyph drawn twice a pixel apart, over its shadow
	   drawn the same way: one pixel wider. */
	if(state->outline == TEXT_STYLE_OUTLINED) {
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
		width++;
	} else {
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
	}
	/* The second character follows the first, starting width pixels along. */
	if((Uint16)secondIdx != 0) {
#if FIX_POINTER_OUTSIDE_OBJECT
		/* Form only the actual glyph address: &glyphs[-TEXT_SPACE] is already
		   outside the array before the later byte offset is added. */
		glyph = (const Uint8 *)&FontGlyphs[(Uint16)secondIdx - TEXT_SPACE];
#else
		glyph =
			(const Uint8 *)&FontGlyphs[-TEXT_SPACE] + ((Uint16)secondIdx << 5);
#endif
		secondWidth = *(const Uint16 *)glyph;
		glyph += 2;
		if(state->outline == TEXT_STYLE_OUTLINED) {
			Uint8 *dst = pixels + width;
			syDecompressBitRuns(glyph, dst + GLYPH_PIXEL(1, 3), shadowColour);
			syDecompressBitRuns(glyph, dst + GLYPH_PIXEL(2, 3), shadowColour);
			syDecompressBitRuns(glyph, dst + GLYPH_PIXEL(0, 2), colour);
			syDecompressBitRuns(glyph, dst + GLYPH_PIXEL(1, 2), colour);
			secondWidth += 1;
		} else {
			Uint8 *dst = pixels + width;
			Uint8 *p;
			p = dst + GLYPH_PIXEL(1, 3);
			syDecompressBitRuns(glyph, p, shadowColour);
			p = dst + GLYPH_PIXEL(0, 2);
			syDecompressBitRuns(glyph, p, colour);
		}
		width += secondWidth;
	}

#else
/* Draws a character into a 16x16-pixel cell of four 4bpp tiles at out: in the
   text colour over a shadow, or outlined in TEXT_STYLE_OUTLINED, with the kana
   mark in the code's top two bits above its top right. Returns its width. */
Sint32 mnTextDrawGlyphPair(Uint32 code, Uint32 *out)
{
	Uint8		 pixels[GLYPH_BUFFER_WIDTH * GLYPH_BUFFER_WIDTH];
	WIN_STATE	*state;
	const Uint8 *glyph;
	const Uint8 *markGlyph;
	Sint32		 width;
	Sint32		 colour;
	Sint32		 shadowColour;
	Uint32		 markCode;
	Uint8		*pixel;
	Sint32		 tileColumn;
	Sint32		 tileRow;
	Sint32		 line;
	Sint32		 pixelsLeft;
	Uint32		 tileLine;

	state = WINDOW_STATE;
	markGlyph = 0;
	markCode = FontMarkGlyphCodes[code >> TEXT_CODE_MARK_SHIFT];
	if(markCode != 0)
		markGlyph =
			(const Uint8 *)((const GLYPH *)((const Uint8 *)FontGlyphs +
								(markCode - TEXT_SPACE) * sizeof(GLYPH)))
				->rows;
	code &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
	if(code <= TEXT_LAST_FONT_CODE) {
		glyph = (const Uint8 *)FontGlyphs + (code - TEXT_SPACE) * sizeof(GLYPH);
		width = ((const GLYPH *)glyph)->width;
		glyph = (const Uint8 *)((const GLYPH *)glyph)->rows;
	} else {
		glyph = FontKanjiGlyphs +
			(code - (TEXT_LAST_FONT_CODE + 1)) * KANJI_GLYPH_SIZE;
		width = KANJI_WIDTH;
	}
	syWordsClear(pixels, sizeof(pixels));
	if(state->mindRead != 0) {
		shadowColour = 0;
		colour = MIND_READ_TEXT_COLOR;
	} else {
		shadowColour = GLYPH_SHADOW_COLOR;
		colour = state->colour;
	}
	if(state->outline == TEXT_STYLE_OUTLINED) {
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
		if(markGlyph != 0) {
			syDecompressBitRuns(
				markGlyph, pixels + GLYPH_PIXEL(8, 1), shadowColour);
			syDecompressBitRuns(
				markGlyph, pixels + GLYPH_PIXEL(9, 1), shadowColour);
		}
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
		if(markGlyph != 0) {
			pixel = pixels + GLYPH_PIXEL(7, 0);
			syDecompressBitRuns(markGlyph, pixel, colour);
			syDecompressBitRuns(markGlyph, pixel + 1, colour);
		}
		width++;
	} else {
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
		if(markGlyph != 0)
			syDecompressBitRuns(
				markGlyph, pixels + GLYPH_PIXEL(8, 1), shadowColour);
		syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
		if(markGlyph != 0) {
			pixel = pixels + GLYPH_PIXEL(7, 0);
			syDecompressBitRuns(markGlyph, pixel, colour);
		}
	}
#endif

	/* Pack either font into four tiles, left to right and then down, with the
	   leftmost pixel in the low nibble: each tile row is read right to left, so
	   the next starts a buffer row on and a tile's width back. */
	pixel = pixels + GLYPH_TILE_WIDTH - 1;
	tileRow = 0;
	do {
		tileColumn = 0;
		do {
			line = 0;
			do {
				tileLine = 0;
				for(pixelsLeft = 0; pixelsLeft < GLYPH_TILE_WIDTH;
					pixelsLeft++) {
					tileLine = (tileLine << 4) + *pixel--;
				}
				*out++ = tileLine;
				line++;
				pixel += GLYPH_BUFFER_WIDTH + GLYPH_TILE_WIDTH;
			} while(line <= 7);
			tileColumn++;
			/* Back up to the top row, at the next tile's right edge. */
			pixel -= GLYPH_TILE_WIDTH * GLYPH_BUFFER_WIDTH - GLYPH_TILE_WIDTH;
		} while(tileColumn <= 1);
		tileRow++;
		/* Down to the next row of tiles, at the first tile's right edge. */
		pixel += GLYPH_TILE_WIDTH * GLYPH_BUFFER_WIDTH - 2 * GLYPH_TILE_WIDTH;
	} while(tileRow <= 1);
	return width;
}

/* The main font's glyph n, counted from the space character. */
#define FONT_GLYPH(n) \
	((const GLYPH *)((n) * sizeof(GLYPH) + (const Uint8 *)FontGlyphs))

/* Returns the width in pixels of a zero-terminated string of 16-bit character
   codes: 4 for a space, and for other codes up to TEXT_LAST_FONT_CODE the font
   width, but nothing for the kana marks; 10 for a kanji (any code above it). */
Sint32 mnTextGetWideStringWidth(Uint16 *text)
{
	Sint32	width;
	Uint16 *cursor;
	Uint32	code;
#if !WIDE_STRING_FIXED_WIDTH
	Uint32 glyphIdx;
#endif

	code = *text;
	width = 0;
	cursor = text + 1;
	while(code != 0) {
		if(code == TEXT_SPACE) {
			width += 4;
		} else if(code <= TEXT_LAST_FONT_CODE) {
#if WIDE_STRING_FIXED_WIDTH
			/* From the German release on, every font code counts as 10 pixels,
			   and the codes above them as nothing. */
			width += 10;
#else
			glyphIdx = code - TEXT_MARK_VOICED;
			if(glyphIdx > 1U) {
				/* The font starts at the space. */
				glyphIdx += TEXT_MARK_VOICED - TEXT_SPACE;
				width += FONT_GLYPH(glyphIdx)->width;
			}
		} else {
			width += KANJI_WIDTH;
#endif
		}
		code = *cursor;
		cursor += 1;
	}
	return width;
}

/* Draws a zero-terminated string of 16-bit character codes into a text canvas
   window at pixel (x, y), running its colour, outline, line spacing and new
   line codes; other control codes are skipped with their arguments. */
void mnTextDrawWideStringAtOffset(Uint16 *text, WINDOW *win, Sint32 x, Sint32 y)
{
	WIN_STATE *state;
	Uint32	   code;
#if LANGUAGE != LANGUAGE_JAPANESE
	Uint32 nextCode;
#endif
	Sint16 lineStartX;

	state = (WIN_STATE *)WindowStateWork[0];
	code = 0;
	lineStartX = x;
	if(text == NULL) {
		text = state->textBuf;
		state->textBuf[state->textBufEnd] = code;
		state->textBufEnd = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
	}
#if LANGUAGE != LANGUAGE_JAPANESE
	for(;;)
#else
	while((code = *text++) != 0)
#endif
	{
#if LANGUAGE != LANGUAGE_JAPANESE
		code = *text++;
		if(code > TEXT_LAST_FONT_CODE)
			code = TEXT_PAST_FONT_CODE;
		if(code == 0)
			break;
#endif
		/* Colour, style, line spacing and new lines take effect; the other
		   control codes are skipped with their operands. */
		if(code <= TEXT_END_ALT) {
			switch(code) {
			case TEXT_COLOR:
				state->colour = *text;
				text++;
				break;
			case TEXT_STYLE:
				state->outline = *text;
				text++;
				break;
			case TEXT_LINE_SPACING:
				state->lineSpacing = *text;
				text++;
				break;
			case TEXT_RESET_STYLE:
				mnTextResetStyle();
				break;
			case TEXT_NEWLINE:
				x = lineStartX;
				y += TEXT_LINE_HEIGHT;
				break;
			case TEXT_UNUSED_14:
			case TEXT_WINDOW_ANIMATION:
			case TEXT_ACTOR_NAME_START:
				text++;
				/* Skip the second operand too. Fall through. */
			case TEXT_UNUSED_11:
			case TEXT_UNUSED_12:
			case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
			case TEXT_ARTICLE_CLASS:
#endif
				text++;
				break;
			}
		} else {
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Pair two narrow characters into one cell, as mnMessageType
			   does. */
			if((win->flg & WINDOW_FLAG_TEXT_CANVAS) == 0) {
				nextCode = *text;
#if OUTLINED_GLYPH_PAIRS_FIT_CELL
				if(code > TEXT_SPACE && nextCode > TEXT_SPACE) {
					Sint16 width = FontGlyphs[code - TEXT_SPACE].width +
						FontGlyphs[nextCode - TEXT_SPACE].width;

					if(state->outline == TEXT_STYLE_OUTLINED) {
						if((Uint16)width <= GLYPH_PAIR_OUTLINED_WIDTH_MAX) {
							code |= nextCode << 8;
							text++;
						}
					} else if((Uint16)width <= GLYPH_PAIR_WIDTH_MAX) {
						code |= nextCode << 8;
						text++;
					}
				}
#else
				if(code > TEXT_SPACE && nextCode > TEXT_SPACE &&
					(Uint16)(FontGlyphs[code - TEXT_SPACE].width +
						FontGlyphs[nextCode - TEXT_SPACE].width) <=
						GLYPH_PAIR_WIDTH_MAX) {
					code |= nextCode << 8;
					text++;
				}
#endif
			}
#else
			if(*text == TEXT_MARK_VOICED) {
				code |= TEXT_CODE_VOICED;
				text++;
			} else if(*text == TEXT_MARK_SEMIVOICED) {
				code |= TEXT_CODE_SEMIVOICED;
				text++;
			}
#endif
			x += mnGlyphDraw(win, code, x, y, GLYPH_DRAW_CHARACTER);
		}
	}
}

/* Unused: draws a string of 16-bit character codes as tiles in a window at tile
   (ofsX, ofsY) of its interior, as mnTextDrawMessageAsTiles draws a
   message; a NULL string draws the decoded text in WIN_STATE.textBuf. */
UNUSED void mnTextDrawWideStringAsTiles(
	Uint16 *buf, const WINDOW *win, Sint32 ofsX, Sint32 ofsY)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   cell;

	if(buf == NULL) {
		buf = state->textBuf;
		state->textBuf[state->textBufEnd] = 0;
		state->textBufEnd = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
	}

	cell = ((win->y + ofsY + 1) << 5) + (win->x + ofsX) + 1;
	if(cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
		/* Inside the tilemap. */
		Sint32 src;
		Sint32 dst;

		cell *= 2;
		dst = (Sint32)WINDOW_TILEMAP_VRAM + cell;
		src = (Sint32)state->tilemap + cell;

		mnTextDrawStringAsTiles(
			buf, (Uint16 *)src, (Uint16 *)dst, (Sint32)WINDOW_TILEMAP_VRAM);
	}
}

/* Draws a zero-terminated string of 16-bit character codes into a window's
   tilemap at tile (x, y) of its interior, one tile per character, skipping
   control codes; a NULL text draws the decoded text in WIN_STATE.textBuf. */
void mnTextDrawWideStringInWindow(Sint16 *text, WINDOW *win, Sint32 x, Sint32 y)
{
	WIN_STATE *state;
	Uint16	   code;
	Sint16	   lineStartX;

	state = WINDOW_STATE;
	lineStartX = x;
	if(text == NULL) {
		text = state->textBuf;
		state->textBuf[state->textBufEnd] = 0;
		state->textBufEnd = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
	}
	code = *text++;
	if(code != 0)
		do {
			if(code <= TEXT_END_ALT) {
				switch(code) {
				case TEXT_NEWLINE:
					x = lineStartX;
					y++;
					break;
				case TEXT_UNUSED_14:
				case TEXT_WINDOW_ANIMATION:
				case TEXT_ACTOR_NAME_START:
					text++;
					/* Skip the second operand too. Fall through. */
				case TEXT_RESET_STYLE:
				case TEXT_COLOR:
				case TEXT_STYLE:
				case TEXT_LINE_SPACING:
				case TEXT_UNUSED_11:
				case TEXT_UNUSED_12:
				case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
				case TEXT_ARTICLE_CLASS:
#endif
					text++;
					break;
				}
			} else {
				mnWindowPutGlyph(win, code, x, y, GLYPH_DRAW_CHARACTER);
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
				x++;
#else
				/* The kana marks share a tile. */
				if((Uint16)(code - TEXT_MARK_VOICED) > 1)
					x++;
#endif
			}
			code = *text++;
		} while(code != 0);
	state->dirty = 1;
}

/* The digits mnStringFormatNum writes, in buffer[1] to [12]. */
#define NUMBER_DIGITS 12

/* Writes value as decimal text into buffer (NUMBER_DIGITS + 2 bytes) and
   returns where the text starts. */
Uint8 *mnStringFormatNum(Uint8 *buf, Sint32 input, Sint32 width)
{
	Sint32 ofs;
	Sint32 value;
	Sint32 negative;
	Sint32 spaceCharacter;
	Sint32 minusCharacter;
	Uint8 *leading;
	Uint8 *trim;

	value = input;
	negative = 0;
	if(value < 0) {
		if(width == 0)
			negative = 1;
		value = 0U - value;
	}

	/* The digits, then spaces for the leading zeros (the last digit stays) and
	   the sign in front; the byte after them ends the text. */
	buf[0] = ' ';
	for(ofs = NUMBER_DIGITS; ofs != 0; ofs--) {
		buf[ofs] = value % 10 + '0';
		value = value / 10;
	}

	ofs = 0;
	buf[NUMBER_DIGITS + 1] = ofs;
	spaceCharacter = ' ';
	ofs = 1;
	minusCharacter = '-';
	for(leading = buf; ofs != NUMBER_DIGITS + 1; leading++, ofs++) {
		if(leading[1] == '0') {
			if(ofs != NUMBER_DIGITS)
				leading[1] = spaceCharacter;
		} else {
			if(negative != 0)
				leading[0] = minusCharacter;
			break;
		}
	}

	if(width == 0) {
		ofs = 0;
		if(buf[0] == ' ') {
			trim = buf;
			do {
				ofs++;
				if(ofs == NUMBER_DIGITS)
					break;
				trim++;
			} while(*trim == ' ');
		}
		return buf + ofs;
	}

	if(width < 0 || width > NUMBER_DIGITS)
		width = NUMBER_DIGITS;
	return buf + NUMBER_DIGITS + 1 - width;
}
