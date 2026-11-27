/* Sizing windows to their text and fitting them on the screen. Part of
   window.c. */

void mnTextMeasureAllPages(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing);
void mnTextMeasureDecoded(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing);

/* The pixels of a window's two border tiles, left and right. */
#define WINDOW_BORDER_WIDTH 16
/* The widest gap between characters that spreads a line across its window, in
   pixels, outside Japanese. */
#define LINE_GAP_MAX		12

/* What a window adds to its text's size in pixels before it is cut to tiles:
   across, its border and 3, so a width rounds up to the next tile only past 4
   pixels into it; down, 15, so a height rounds up and takes one tile more. */
#define WINDOW_PAD_X (WINDOW_BORDER_WIDTH + 3)
#define WINDOW_PAD_Y 15

/* Sizes a window for decoded text entryIndex and moves (*windowX, *windowY) so
   the window fits on the screen, as flags (WINDOW_FIT_FLAG) say. In the
   Japanese release, while a mind-read window is open, a measured window is 2
   tiles wider and kept off the screen's last column. */
void mnWindowFitOnScreen(Sint32 entryIndex, Sint32 *windowX, Sint32 *windowY,
	Uint32 *windowWidth, Uint32 *windowHeight, Uint16 *spacing, Uint32 flags)
{
#if !MIND_READ_WINDOW_NOT_WIDENED
	WIN_STATE *state;
#endif
	Sint32 x;
	Sint32 y;
	Sint32 rightLimit;
	Sint32 right;
	Sint32 bottom;
	Sint32 overflow;
	Sint32 fittedPos;

#if !MIND_READ_WINDOW_NOT_WIDENED
	state = WINDOW_STATE;
#endif
	x = *windowX;
	y = *windowY;
	rightLimit = DISPLAY_TILE_WIDTH;

	if(!(flags & WINDOW_FIT_SIZE_GIVEN)) {
		if(flags & WINDOW_FIT_ALL_PAGES)
			mnTextMeasureAllPages(
				entryIndex, windowWidth, windowHeight, spacing);
		else
			mnTextMeasureDecoded(
				entryIndex, windowWidth, windowHeight, spacing);
	}

	if(*windowWidth == 0 && *windowHeight == 0)
		return;

	/* Pixels to tiles, with room for the border and a margin. */
	if(!(flags & WINDOW_FIT_SIZE_GIVEN)) {
		*windowWidth = (*windowWidth + WINDOW_PAD_X) >> 3;
		*windowHeight = (*windowHeight + WINDOW_PAD_Y) >> 3;
#if !MIND_READ_WINDOW_NOT_WIDENED
		if(state->mindRead != 0) {
			*windowWidth += 2;
			rightLimit = DISPLAY_TILE_WIDTH - 1;
		}
#endif
	}

	/* Slide the window back onto the screen, rightLimit columns by
	   DISPLAY_TILE_HEIGHT rows. */
	right = x + *windowWidth;
	if(right > rightLimit) {
		overflow = right - rightLimit;
		fittedPos = x - overflow;
		if(fittedPos >= 0)
			x = fittedPos;
		else
			x = 0;
	}
	bottom = y + *windowHeight;
	if(bottom > DISPLAY_TILE_HEIGHT) {
		overflow = bottom - DISPLAY_TILE_HEIGHT;
		fittedPos = y - overflow;
		if(fittedPos >= 0)
			y = fittedPos;
		else
			y = 0;
	}
	if(x < 0)
		x = 0;
	if(y < 0)
		y = 0;
	if(x > rightLimit - *windowWidth)
		x = rightLimit - *windowWidth;
	if(y > DISPLAY_TILE_HEIGHT - *windowHeight)
		y = DISPLAY_TILE_HEIGHT - *windowHeight;
	*windowX = x;
	*windowY = y;
}

/* Decodes message and returns its text's width and height in pixels. */
void mnTextMeasureMessage(Sint32 msg, Sint32 *x, Sint32 *y)
{
	mnTextMeasureDecoded(mnMessageDecode(msg, 0), (Uint32 *)x, (Uint32 *)y, 0);
}

/* Defines a function that decodes message and sizes its window to fit on the
   screen near tile (*x, *y), as mnWindowFitOnScreen does with flags; it returns
   0 when the message is empty. */
#define DEFINE_MESSAGE_SIZE(name, flags) \
	Sint32 name( \
		Sint32 msg, Sint32 *x, Sint32 *y, Uint32 *width, Uint32 *height) \
	{ \
		WIN_STATE *state; \
		Sint32	   entry; \
\
		state = WINDOW_STATE; \
		entry = mnMessageDecode(msg, 0); \
		if(state->textBuf[entry] == 0) { \
			return 0; \
		} \
		mnWindowFitOnScreen(entry, x, y, width, height, 0, flags); \
		return 1; \
	}

/* Sizes a message's window by its first page. */
DEFINE_MESSAGE_SIZE(mnTextGetMessageSize, 0)
/* Sizes a message's window to hold every page. */
DEFINE_MESSAGE_SIZE(mnTextGetMessageSizeAllPages, WINDOW_FIT_ALL_PAGES)

/* Measures decoded text from entryIndex to its first page's end: returns the
   width of the longest line and the height (TEXT_LINE_HEIGHT a line) in pixels.
   With spacing set, fills one entry per line, MESSAGE_LINE_COUNT at most, with
   the extra gap between characters, in 1/256 pixel, that spreads the line
   across the window. It leaves WIN_STATE.outline at the last style it
   passed. */
void mnTextMeasureDecoded(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing)
{
	WIN_STATE *state;
	Uint32	   height;
	Uint32	   lines;
	Uint32	   width;
	Uint32	   lineWidth;
	Uint32	   count;
	Uint32	   code;
	Uint32	   glyph;
	Uint32	   lineIndex;
	Uint16	   lineSpacingCounts[MESSAGE_LINE_COUNT];
	Uint16	   lineWidths[MESSAGE_LINE_COUNT];
	Sint32	   gap;
	Uint32	   step;
	Uint32	   style;

	state = WINDOW_STATE;
	width = 0;
	height = TEXT_LINE_HEIGHT;
	lines = 0;
	lineWidth = 0;
	count = 0;
	for(;;) {
		code = state->textBuf[entryIndex];
		entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
		if(code >= TEXT_SPACE && code != TEXT_EM_DASH) {
			if(code == TEXT_SPACE) {
				lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
				count++;
			} else {
				glyph = FontGlyphs[code - TEXT_SPACE].width;
				style = state->outline;
				if(style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
					glyph++;
				lineWidth += glyph;
			}
			continue;
		}
#else
		/* Japanese counts every character as a point the line can be stretched
		   at. The kana marks (TEXT_MARK_VOICED and TEXT_MARK_SEMIVOICED) sit
		   over the character before them; codes above TEXT_LAST_FONT_CODE are
		   kanji, outside FontGlyphs. */
		if(code >= TEXT_SPACE) {
			if(code - TEXT_MARK_VOICED <= 1) {
				lineWidth += 1;
			} else if(code == TEXT_JAPANESE_MIDDLE_DOT) {
				lineWidth += 6;
			} else if(code == TEXT_SPACE) {
				lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
			} else {
				glyph = KANJI_WIDTH;
				if(code <= TEXT_LAST_FONT_CODE)
					glyph = FontGlyphs[code - TEXT_SPACE].width;
				style = state->outline;
				if(style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
					glyph++;
				lineWidth += glyph;
			}
			count++;
			continue;
		}
#endif
		switch(code) {
		case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Outside Japanese, count is the line's spaces plus one, its
			   words. */
			count++;
#endif
#if LINE_SPACING_COUNTS_ALWAYS_STORED
			lineSpacingCounts[lines] = count;
#else
			/* UB: with FLAG_SPREAD_LINES clear the line's count is never
			   stored, and the spacing below reads what the stack held there. */
			if(syGameFlagGet(FLAG_SPREAD_LINES))
				lineSpacingCounts[lines] = count;
#if FIX_UNINITIALIZED_READ
			else
				lineSpacingCounts[lines] = 0;
#endif
#endif
			lineWidths[lines] = lineWidth;
			if(width < lineWidth)
				width = lineWidth;
			if(lines < MESSAGE_LINE_COUNT - 1)
				lines++;
			count = 0;
			lineWidth = 0;
			height += TEXT_LINE_HEIGHT;
			break;
		case TEXT_PAGE:
			mnTextResetStyle();
			/* Fall through. */
		case TEXT_END:
			goto done;
		case 14:
		case TEXT_ACTOR_NAME_START:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
		case TEXT_COLOR:
		case TEXT_LINE_SPACING:
		case TEXT_WINDOW_ANIMATION:
		case TEXT_ACTOR_NAME:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		case TEXT_OUTLINE:
			state->outline = state->textBuf[entryIndex];
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		}
	}
done:
#if LANGUAGE != LANGUAGE_JAPANESE
	count++;
#endif
	lineSpacingCounts[lines] = count;
	lineWidths[lines] = lineWidth;
	if(width < lineWidth)
		width = lineWidth;
	/* While a mind-read window is open the text is 2 pixels wider, except in
	   the German release and the French and Italian after it (Spanish keeps
	   them). */
#if !MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER
	if(state->mindRead)
		width = width + 2;
#endif
	/* In textMode a message of three lines or more is squeezed into 32 pixels,
	   and spacing's last entry tells mnMessageStart so
	   (MESSAGE.compressLines). */
	if(state->textMode != 0) {
		if(height > 44) {
			spacing[3] = 1;
			height = 32;
		} else {
			spacing[3] = 0;
		}
	}
	*outWidth = width;
	*outHeight = height;
	/* Each line's extra gap between characters, in 1/256 pixel, to spread it
	   across the window's inner width: the window's width in tiles, as
	   mnWindowFitOnScreen makes it, less the tick, in pixels, and 4 pixels of
	   margin. */
	width = (width + WINDOW_PAD_X) >> 3;
	width = (width << 3) - WINDOW_BORDER_WIDTH;
	if(spacing != NULL) {
		lineIndex = 0;
		while(lineIndex <= lines) {
			if(lineSpacingCounts[lineIndex] <= 1) {
				*spacing++ = 0;
			} else {
				gap = width - lineWidths[lineIndex];
				gap -= 4;
				if(gap < 0)
					gap = 0;
				/* Outside Japanese a gap over LINE_GAP_MAX pixels falls back to
				   2; in Japanese one over 8 is dropped and the rest held to at
				   most 1. */
				step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE != LANGUAGE_JAPANESE
				if(step > Q_8_8(LINE_GAP_MAX))
					step = Q_8_8(2);
#else
				if(step > Q_8_8(8))
					step = 0;
				if(step > Q_8_8(1))
					step = Q_8_8(1);
#endif
				*spacing++ = step;
			}
			lineIndex++;
		}
	}
	mnTextResetStyle();
}

/* Measures decoded text from entryIndex over all its pages: the width of the
   first page's longest line and the height of its tallest page, in pixels, with
   each line's extra character gap in spacing as mnTextMeasureDecoded. The text
   must fit 16 pages. Outside Japanese the gaps have no ceiling, and past the
   first page they do not fit their lines; its one caller,
   mnTextGetMessageSizeAllPages, passes no spacing. */
void mnTextMeasureAllPages(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing)
{
	Uint32	   pageHeights[16];
	Uint16	   lineSpacingCounts[MESSAGE_LINE_COUNT];
	Uint16	   lineWidths[MESSAGE_LINE_COUNT];
	WIN_STATE *state;
	Uint32	   maxWidth;
	Uint32	   line;
	Uint32	   width;
	Uint32	   count;
	Uint32	   page;
	Uint32	   code;
	Uint32	   glyph;
	Uint32	   outline;
	Uint32	   pageIndex;
	Uint32	   lineIndex;
	Sint32	   gap;
	Uint32	   step;

	state = WINDOW_STATE;
#if MAX_WIDTH_CLEAR_FIRST
	maxWidth = 0;
#endif
	/* Each of the 16 pages starts a line tall. */
	pageHeights[0] = TEXT_LINE_HEIGHT;
	pageHeights[1] = TEXT_LINE_HEIGHT;
	pageHeights[2] = TEXT_LINE_HEIGHT;
	pageHeights[3] = TEXT_LINE_HEIGHT;
	pageHeights[4] = TEXT_LINE_HEIGHT;
	pageHeights[5] = TEXT_LINE_HEIGHT;
	pageHeights[6] = TEXT_LINE_HEIGHT;
	pageHeights[7] = TEXT_LINE_HEIGHT;
	pageHeights[8] = TEXT_LINE_HEIGHT;
	pageHeights[9] = TEXT_LINE_HEIGHT;
	pageHeights[10] = TEXT_LINE_HEIGHT;
	pageHeights[11] = TEXT_LINE_HEIGHT;
	pageHeights[12] = TEXT_LINE_HEIGHT;
	pageHeights[13] = TEXT_LINE_HEIGHT;
	pageHeights[14] = TEXT_LINE_HEIGHT;
	pageHeights[15] = TEXT_LINE_HEIGHT;
#if !MAX_WIDTH_CLEAR_FIRST
	maxWidth = 0;
#endif
	line = 0;
	width = 0;
	count = 0;
	page = 0;
	for(;;) {
		code = state->textBuf[entryIndex];
		entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
		if(code >= TEXT_SPACE && code != TEXT_EM_DASH) {
			if(code == TEXT_SPACE) {
				width += WINDOW_SPRITE_SPACE_WIDTH;
				count++;
			} else {
				glyph = FontGlyphs[code - TEXT_SPACE].width;
				outline = state->outline;
				if(outline == TEXT_STYLE_OUTLINED || outline == TEXT_STYLE_SPIN)
					glyph++;
				width += glyph;
			}
			continue;
		}
#else
		/* Japanese counts every character as a point the line can be stretched
		   at. The kana marks (TEXT_MARK_VOICED and TEXT_MARK_SEMIVOICED) sit
		   over the character before them; codes above TEXT_LAST_FONT_CODE are
		   kanji, outside FontGlyphs. */
		if(code >= TEXT_SPACE) {
			if(code - TEXT_MARK_VOICED <= 1) {
				width += 1;
			} else if(code == TEXT_JAPANESE_MIDDLE_DOT) {
				width += 6;
			} else if(code == TEXT_SPACE) {
				width += WINDOW_SPRITE_SPACE_WIDTH;
			} else {
				glyph = KANJI_WIDTH;
				if(code <= TEXT_LAST_FONT_CODE)
					glyph = FontGlyphs[code - TEXT_SPACE].width;
				outline = state->outline;
				if(outline == TEXT_STYLE_OUTLINED || outline == TEXT_STYLE_SPIN)
					glyph++;
				width += glyph;
			}
			count++;
			continue;
		}
#endif
		switch(code) {
		case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Outside Japanese, count is the line's spaces plus one, its
			   words. */
			count++;
#endif
			lineSpacingCounts[line] = count;
			lineWidths[line] = width;
			if(page == 0 && maxWidth < width)
				maxWidth = width;
			if(line < MESSAGE_LINE_COUNT - 1)
				line++;
			count = 0;
			width = 0;
			pageHeights[page] += TEXT_LINE_HEIGHT;
			break;
		case TEXT_END:
#if LANGUAGE != LANGUAGE_JAPANESE
			count++;
#endif
			lineSpacingCounts[line] = count;
			lineWidths[line] = width;
			if(page == 0 && maxWidth < width)
				maxWidth = width;
			page++;
			goto done;
		case TEXT_PAGE:
#if LANGUAGE != LANGUAGE_JAPANESE
			count++;
#endif
			lineSpacingCounts[line] = count;
			lineWidths[line] = width;
			if(page == 0 && maxWidth < width)
				maxWidth = width;
			page++;
			mnTextResetStyle();
			break;
		case 14:
		case TEXT_ACTOR_NAME_START:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
		case TEXT_COLOR:
		case TEXT_LINE_SPACING:
		case TEXT_WINDOW_ANIMATION:
		case TEXT_ACTOR_NAME:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		case TEXT_OUTLINE:
			state->outline = state->textBuf[entryIndex];
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		}
	}
done:
	/* From the German release on, a mind-read window is no longer 2 pixels
	   wider. */
#if !MIND_READ_ALL_PAGES_NOT_MEASURED_WIDER
	if(state->mindRead != 0)
		maxWidth += 2;
#endif
	for(pageIndex = 0; pageIndex < page; pageIndex++) {
		if(pageIndex == 0)
			*outHeight = pageHeights[0];
		else if(*outHeight < pageHeights[pageIndex])
			*outHeight = pageHeights[pageIndex];
	}
	*outWidth = maxWidth;
	maxWidth = (((maxWidth + WINDOW_PAD_X) >> 3) << 3) - WINDOW_BORDER_WIDTH;
	if(spacing != NULL) {
		for(lineIndex = 0; lineIndex <= line; lineIndex++) {
			if(lineSpacingCounts[lineIndex] <= 1) {
				*spacing++ = 0;
			} else {
				gap = maxWidth - lineWidths[lineIndex];
				gap -= 4;
				if(gap < 0)
					gap = 0;
				/* The gap between characters, in 1/256 pixel: in Japanese one
				   over 8 pixels is dropped and the rest held to at most 1. */
				step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE == LANGUAGE_JAPANESE
				if(step > Q_8_8(8))
					step = 0;
				if(step > Q_8_8(1))
					step = Q_8_8(1);
#endif
				*spacing++ = step;
			}
		}
	}
	mnTextResetStyle();
}

#undef DEFINE_MESSAGE_SIZE
