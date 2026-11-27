/* Sizing windows to their text and fitting them on the screen. Part of
   window.c. */

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

/* Sizes a window for the decoded text at entryIndex, as flags (WINDOW_FIT_)
   ask, and moves (*windowX, *windowY) so the window fits on the screen; spacing
   receives each line's extra gap between characters. In the Japanese and
   English releases, under Mind Read, a measured window is 2 tiles wider and
   kept off the screen's last column. */
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

	if(!(flags & WINDOW_FIT_GIVEN_SIZE)) {
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
	if(!(flags & WINDOW_FIT_GIVEN_SIZE)) {
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

/* Sizes and places a window for a message's first page. */
DEFINE_GET_RESOURCE_DIMENSIONS(mnTextGetMessageSize, 0)
/* Sizes and places a window for all of a message's pages. */
DEFINE_GET_RESOURCE_DIMENSIONS(
	mnTextGetMessageSizeAllPages, WINDOW_FIT_ALL_PAGES)

/* Measures decoded text from entryIndex to its first page's end: returns the
   width of the longest line and the height (TEXT_LINE_HEIGHT a line) in
   pixels. */
void mnTextMeasureDecoded(
	Sint32 entryIndex, Uint32 *outWidth, Uint32 *outHeight, Uint16 *spacing)
{
	WIN_STATE *state;
	Uint32	   lines;
	Uint32	   width;
	Uint32	   height;
	Uint32	   lineWidth;
	Uint32	   count;
	Uint32	   code;
	Uint32	   glyph;
	Uint32	   lineIndex;
	Uint16	   lineSpacingCounts[MESSAGE_LINE_COUNT];
	Uint16	   lineWidths[MESSAGE_LINE_COUNT];
	Sint32	   gap;
	Uint32	   style;
#if LANGUAGE == LANGUAGE_JAPANESE
	Sint32 trailing;
#endif

	state = WINDOW_STATE;
#if LANGUAGE == LANGUAGE_JAPANESE
	trailing = 0;
#endif
	width = 0;
	height = TEXT_LINE_HEIGHT;
	lines = 0;
	lineWidth = 0;
	count = 0;
	for(;;) {
		code = state->textBuf[entryIndex];
		entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
		/* A character adds its width. */
		if(code >= TEXT_SPACE) {
#if LANGUAGE != LANGUAGE_JAPANESE
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
#else
			if(code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
				if(code == TEXT_SPACE) {
					lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
				} else {
					/* Punctuation at the end of a line takes less room than its
					   cell. */
					if(code == TEXT_JAPANESE_MIDDLE_DOT)
						trailing = -1;
					else if(code == '!')
						trailing = -3;
					else if(code == TEXT_JAPANESE_FULL_STOP ||
						code == TEXT_JAPANESE_COMMA)
						trailing = -6;
					else
						trailing = 0;
					glyph = KANJI_WIDTH;
					if(code <= TEXT_LAST_FONT_CODE)
						glyph = FontGlyphs[code - TEXT_SPACE].width;
					style = state->outline;
					if(style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
						glyph++;
					lineWidth += glyph;
				}
			}
			count++;
#endif
			continue;
		}
		switch(code) {
		case TEXT_END:
#if !PAGE_BREAK_RESETS_STYLE
		case TEXT_PAGE:
#endif
			goto done;
		case TEXT_NEWLINE:
			/* Outside Japanese, count is the line's spaces plus one, its words:
			   the gap is spread over the spaces. */
#if LANGUAGE != LANGUAGE_JAPANESE
			count++;
#else
			lineWidth += trailing;
#endif
			lineSpacingCounts[lines] = count;
			lineWidths[lines] = lineWidth;
			if(width < lineWidth)
				width = lineWidth;
			if(lines < MESSAGE_LINE_COUNT - 1)
				lines++;
			count = 0;
			lineWidth = 0;
			height += TEXT_LINE_HEIGHT;
			break;
#if PAGE_BREAK_RESETS_STYLE
		case TEXT_PAGE:
			/* The French, Spanish and Italian releases end the style at a page
			   break, as their messages do (mnMessageType). */
			state->outline = TEXT_STYLE_PLAIN;
			goto done;
#endif
		case TEXT_UNUSED_14:
		case TEXT_ACTOR_NAME_START:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			/* Skip the second operand too. Fall through. */
		case TEXT_COLOR:
		case TEXT_LINE_SPACING:
		case TEXT_WINDOW_ANIMATION:
		case TEXT_ACTOR_NAME:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		case TEXT_STYLE:
			state->outline = state->textBuf[entryIndex];
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		}
	}
done:
#if LANGUAGE != LANGUAGE_JAPANESE
	count++;
#else
	lineWidth += trailing;
#endif
	lineSpacingCounts[lines] = count;
	lineWidths[lines] = lineWidth;
	if(width < lineWidth)
		width = lineWidth;
#if !MIND_READ_FIRST_PAGE_NOT_MEASURED_WIDER
	if(state->mindRead)
		width = width + 2;
#endif
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
				gap = ((gap << 8) / (lineSpacingCounts[lineIndex] - 1));
#if LANGUAGE != LANGUAGE_JAPANESE
				if(gap < 0 || gap > Q_8_8(LINE_GAP_MAX))
					gap = Q_8_8(2);
#else
				if(gap < 0 || gap > Q_8_8(8))
					gap = 0;
				if(gap < 0 || gap > Q_8_8(1))
					gap = Q_8_8(1);
#endif
				*spacing++ = gap;
			}
			lineIndex++;
		}
	}
}

/* Measures decoded text from start over all its pages: the width of the first
   page's longest line and the height of its tallest page, in pixels, with each
   line's extra character gap in spacing as mnTextMeasureDecoded. */
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
#if LANGUAGE == LANGUAGE_JAPANESE
	Sint32 trailing;

	trailing = 0;
#endif
	state = WINDOW_STATE;
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
	maxWidth = 0;
	pageHeights[13] = TEXT_LINE_HEIGHT;
	pageHeights[14] = TEXT_LINE_HEIGHT;
	pageHeights[15] = TEXT_LINE_HEIGHT;
	line = 0;
	width = 0;
	count = 0;
	page = 0;
	for(;;) {
		code = state->textBuf[entryIndex];
		entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
		if(code >= TEXT_SPACE) {
#if LANGUAGE != LANGUAGE_JAPANESE
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
#else
			if(code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
				if(code == TEXT_SPACE) {
					width += WINDOW_SPRITE_SPACE_WIDTH;
				} else {
					if(code == TEXT_JAPANESE_MIDDLE_DOT)
						trailing = -1;
					else if(code == '!')
						trailing = -3;
					else if(code == TEXT_JAPANESE_FULL_STOP ||
						code == TEXT_JAPANESE_COMMA)
						trailing = -6;
					else
						trailing = 0;
					glyph = KANJI_WIDTH;
					if(code <= TEXT_LAST_FONT_CODE)
						glyph = FontGlyphs[code - TEXT_SPACE].width;
					outline = state->outline;
					if(outline == TEXT_STYLE_OUTLINED ||
						outline == TEXT_STYLE_SPIN)
						glyph++;
					width += glyph;
				}
			}
			count++;
#endif
			continue;
		}
		switch(code) {
		case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
			lineSpacingCounts[line] = ++count;
#else
			width += trailing;
			lineSpacingCounts[line] = count;
#endif
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
			lineSpacingCounts[line] = ++count;
#else
			width += trailing;
			lineSpacingCounts[line] = count;
#endif
			lineWidths[line] = width;
			if(page == 0 && maxWidth < width)
				maxWidth = width;
			page++;
			goto done;
		case TEXT_PAGE:
#if LANGUAGE != LANGUAGE_JAPANESE
			lineSpacingCounts[line] = ++count;
#else
			width += trailing;
			lineSpacingCounts[line] = count;
#endif
			lineWidths[line] = width;
			if(page == 0 && maxWidth < width)
				maxWidth = width;
#if PAGE_BREAK_RESETS_STYLE
			state->outline = TEXT_STYLE_PLAIN;
#endif
			page++;
			break;
		case TEXT_UNUSED_14:
		case TEXT_ACTOR_NAME_START:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			/* Skip the second operand too. Fall through. */
		case TEXT_COLOR:
		case TEXT_LINE_SPACING:
		case TEXT_WINDOW_ANIMATION:
		case TEXT_ACTOR_NAME:
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		case TEXT_STYLE:
			state->outline = state->textBuf[entryIndex];
			entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
			break;
		}
	}
done:
	/* Under Mind Read the pages measure 2 pixels wider. The German, Spanish,
	   French and Italian editions dropped Mind Read's widening from the
	   first-page measure, the fit and the pen; the Spanish and Italian editions
	   put it back here alone. */
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
				gap = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE == LANGUAGE_JAPANESE
				if(gap < 0 || gap > Q_8_8(8))
					gap = 0;
				if(gap < 0 || gap > Q_8_8(1))
					gap = Q_8_8(1);
#endif
				*spacing++ = gap;
			}
		}
	}
}

#undef WINDOW_BORDER_WIDTH
#undef LINE_GAP_MAX
#undef WINDOW_PAD_X
#undef WINDOW_PAD_Y
