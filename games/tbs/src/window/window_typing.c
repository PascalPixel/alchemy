/* The per-tick message update: typing text and running its control codes. Part
   of window.c. */

/* The battle message canvas: 30 columns of four tiles, each column 32 pixel
   rows of a word each. */
#define CANVAS_COLUMNS		30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS	8 /* the top 8 pixel rows stay put */

/* A message waiting for a button holds its countdown here, set again each tick
   and counted down once, so it reads below MESSAGE_KEY_WAIT_STARTED only on the
   wait's first tick. */
#define MESSAGE_KEY_WAIT		 919
#define MESSAGE_KEY_WAIT_STARTED 900

/* Scrolls the battle message canvas up by 6 pixel rows per step, below its top
   8 rows, and clears the rows this uncovers. */
void mnTextScrollCanvasUp(Sint32 step)
{
	Sint32	shift = step * 6;
	Uint32 *base = TEXT_CANVAS_VRAM;
	Sint32	n;

	for(n = 0; n < CANVAS_COLUMNS; n++) {
		Uint32 *column = base + n * CANVAS_COLUMN_WORDS;

		syDMAXCopy(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
			DMA_COPY_WORDS(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
			REG_DMA3);
		syWordsFill(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
	}
}

/* What mnMessageType reports. */
enum {
	TYPING_GOES_ON = 0,		  /* the text goes on next tick */
	TYPING_ENDED = 8,		  /* the text has ended; the window stays open */
	TYPING_CLOSES_WINDOW = 9, /* the text is dismissed: close the window */
};

/* The per-tick message update: each message whose window is fully open runs
   its window animation or types more text, and on the text's end marks the
   window typed (WINDOW_TYPED) or closes it. */
void mnMessageUpdateAll(void)
{
	MESSAGE *msg = WINDOW_STATE->msgs;
	Sint32	 msgIdx = 0;

	do {
		WINDOW *cur = msg->win;

		/* A zooming window is left alone until its zoom ends. */
		if(cur != NULL && cur->zoomStage == 0 && cur->duration == 0) {
			Uint16 flg = cur->flg;

			/* Flags 0 with no zoom left: the window has closed. */
			if(flg == 0) {
				msg->win = NULL;
			} else {
				Sint32 pending = cur->anm;
				Sint32 step;

				if(pending != 0) {
					mnWindowStepSwellEffect(msg);
				} else {
					step = mnMessageType(msg);
					switch(step) {
					case TYPING_ENDED:
						msg->win->state = WINDOW_TYPED;
						break;
					case TYPING_CLOSES_WINDOW: {
						WINDOW *win = msg->win;

						mnWindowClose(win, win->flg & WINDOW_FLAG_INSTANT);
						/* pending is 0 here: it clears the message. */
						msg->x = pending;
						msg->y = pending;
						msg->textIdx = pending;
						msg->countdown = pending;
						msg->colour = pending;
						msg->outline = pending;
						msg->lineSpacing = pending;
						msg->win->state = WINDOW_TYPED;
						break;
					}
					}
				}
			}
		}
		msgIdx++;
		msg++;
	} while(msgIdx != MESSAGE_COUNT);
}

/* Types one tick's share of a message's text: runs control codes (new line,
   new page, pauses, colours) and draws characters, playing the speaker's voice
   blips. */
Sint32 mnMessageType(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	/* DEAD: PerData read and not used; the ROM keeps the load. */
	Uint32	keys = PerData;
	Sint32	glyphsLeft = MessageSpeedSteps[MainRoutineStateWork.msgSpeed];
	Sint32	frmFrames;
	Uint32	code;
	Uint32	nextCode;
	WINDOW *win;
	Sint32	x;
	Sint32	y;
	Uint32	newWidth;
	Uint32	newHeight;
	Sint32	oldWidth;
	Sint32	oldHeight;
	Sint32	entryIdx;
	Sint32	nextEntryIdx;
	Sint32	pixelX;
	Sint32	pixelY;
	Sint32	voiceGap;
	Sint32	glyphWidth;
	Sint32	glyphAdvance;

	(void)keys;

	/* In battle, text runs at 3 glyphs a tick and 5 more for each VBlank the
	   last tick took (LastTickFrames, 1 for a tick on time), counting at
	   most two. */
	if(state->battleMode != WINDOW_FIELD) {
		frmFrames = LastTickFrames;
		CLAMP(frmFrames, 0, 2);
		glyphsLeft = frmFrames * 5 + 3;
	}

	/* Finish scrolling the canvas up a line before any more text. */
	if(msg->transition != 0) {
		mnTextScrollCanvasUp(1);
		msg->transition = (Sint16)msg->transition - 1;
		return TYPING_GOES_ON;
	}
	/* With no button held, wait out the delay after the last glyph. */
	if(PerData == 0 && msg->glyphDelay != 0) {
		msg->glyphDelay--;
		return TYPING_GOES_ON;
	}

	/* Run control codes and draw characters until this tick's share is
	   done. */
	do {
		code = TEXT_END;
		if(msg->ended == 0)
			code = state->textBuf[msg->textIdx];

		if(code < TEXT_COMMANDS) {
			switch(code) {
			case TEXT_NEWLINE:
				/* On a canvas the text scrolls up from the second line on. */
				msg->x = msg->startX;
				if(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) {
					if(msg->y >= BATTLE_MESSAGE_LINE_HEIGHT << 8) {
						mnTextStartCanvasScroll(msg);
						glyphsLeft = 1;
					} else {
						msg->y += BATTLE_MESSAGE_LINE_HEIGHT << 8;
					}
				} else {
					msg->y += TEXT_LINE_HEIGHT << 8;
					if(msg->line < MESSAGE_LINE_COUNT - 1)
						msg->line++;
				}
				break;

			case TEXT_PAGE:
				/* Under Mind Read a press from before the wait does not
				   count. */
				if(state->mindRead != 0 &&
					msg->countdown < MESSAGE_KEY_WAIT_STARTED)
					PerPushLatch = 0;
				msg->countdown = MESSAGE_KEY_WAIT;
				if(mnTextCheckTextDismissed(msg) == 0) {
					/* Until the button: show the next-page arrow in the
					   bottom-right corner. */
					WINDOW *arrowWin = msg->win;

#if PAGE_ARROW_SKIPS_EMPTY_WINDOW
					if(arrowWin->width == 0 || arrowWin->height == 0 ||
						state->pageArrowShown != 0)
						break;
#else
					if(state->pageArrowShown != 0)
						break;
#endif
					mnGlyphDraw(arrowWin, 1, arrowWin->width * 4 - 8,
						arrowWin->height * 8 - 16, GLYPH_DRAW_PAGE_ARROW);
					state->pageArrowShown = 1;
					break;
				}
				/* Clear the window, and refit it to the next page's text. */
				win = msg->win;
				x = win->x;
				y = win->y;
				oldWidth = win->width;
				oldHeight = win->height;
				entryIdx = msg->textIdx;
				state->pageArrowShown = 0;
				mnWindowSprPrepareForRedraw(win);
				if(msg->flg == 0 && (win->width | win->height) != 0)
					mnWindowEraseBorderRect(
						win->x, win->y, win->width, win->height);
				entryIdx = (entryIdx + 1) & TEXT_BUFFER_MASK;
				if(state->textBuf[entryIdx] != 0 &&
					(win->width | win->height) != 0) {
					if(msg->flg != 0) {
						mnWindowEraseBorderRect(
							win->x, win->y, win->width, win->height);
					} else {
						mnWindowFitOnScreen(entryIdx, &x, &y, &newWidth,
							&newHeight, msg->spacing, 0);
						if(win->flg & WINDOW_FLAG_ANCHOR_BOTTOM) {
							if(oldHeight != newHeight)
								y -= newHeight - oldHeight;
							if(y < 0)
								y = 0;
						}
						if(!(win->flg & WINDOW_FLAG_FIXED_X)) {
							/* Shift the window by a quarter of the change in
							   width (tiles), part way to keeping its centre,
							   which a half would keep; then fit it on the
							   screen again at its new size. */
							x += (Sint32)(oldWidth - newWidth) / 4;
							mnWindowFitOnScreen(entryIdx, &x, &y, &newWidth,
								&newHeight, msg->spacing,
								WINDOW_FIT_GIVEN_SIZE);
						}
						win->x = x;
						win->y = y;
						win->width = newWidth;
						win->height = newHeight;
					}
					mnWindowDrawBorder(win->x, win->y, win->width, win->height);
				}
				msg->x = msg->startX;
				msg->y = 0;
				msg->line = 0;
				/* The French, Spanish and Italian releases also start each page
				   in the default style. */
#if PAGE_BREAK_RESETS_STYLE
				msg->colour = TEXT_DEFAULT_COLOR;
				msg->outline = TEXT_STYLE_PLAIN;
				msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(msg);
#endif
				syVRAMCacheFreeEntry(state->glyphResource);
				state->glyphResource = NO_GLYPH_RESOURCE;
				break;

			case TEXT_WAIT_KEY:
				/* The same hold as a page break's. */
				if(state->mindRead != 0 &&
					msg->countdown < MESSAGE_KEY_WAIT_STARTED)
					PerPushLatch = 0;
				if(mnTextCheckTextDismissed(msg) != 0)
					return TYPING_CLOSES_WINDOW;
				msg->countdown = MESSAGE_KEY_WAIT;
				break;

			case TEXT_PAUSE_SHORT:
				if(msg->countdown == 0)
					msg->countdown = 20;
				state->voiceCountdown = 0;
				mnTextCheckPauseSkipped(msg);
				break;

			case TEXT_PAUSE_LONGEST:
				if(msg->countdown == 0)
					msg->countdown = 120;
				state->voiceCountdown = 0;
				mnTextCheckPauseSkipped(msg);
				break;

			case TEXT_PAUSE_LONG:
				if(msg->countdown == 0)
					msg->countdown = 60;
				state->voiceCountdown = 0;
				break;

			case TEXT_COLOR:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->colour = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_STYLE:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->outline = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_LINE_SPACING:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->lineSpacing = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_RESET_STYLE:
				msg->colour = TEXT_DEFAULT_COLOR;
				msg->outline = TEXT_STYLE_PLAIN;
				msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(msg);
				break;

			case TEXT_WINDOW_ANIMATION:
				/* The animation lasts while the countdown of 10 runs out
				   (mnWindowStepSwellEffect counts it down); typing then resumes
				   after the operand. */
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->win->anm = state->textBuf[msg->textIdx];
				msg->countdown = 10;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				break;

			case TEXT_END:
			case TEXT_END_ALT:
				msg->ended = 1;
				return TYPING_ENDED;
			}

			/* Outside battle a control code ends this tick's typing. */
			if(state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		} else {
			/* A character: draw it at the pen position, rounded to whole
			   pixels. */
			pixelX = (msg->x + 128) / 256;
			pixelY = (msg->y + 128) / 256;
			voiceGap = MessageSpeedVoiceGaps[MainRoutineStateWork.msgSpeed];
			nextEntryIdx = msg->textIdx;
			/* Under Mind Read the text starts 8 pixels further in (Japanese and
			   English), in the window mnWindowFitOnScreen made 2 tiles
			   wider. */
#if !MIND_READ_TEXT_NOT_INDENTED
			if(state->mindRead != 0)
				pixelX += 8;
#endif
			nextEntryIdx = (nextEntryIdx + 1) & TEXT_BUFFER_MASK;
			nextCode = state->textBuf[nextEntryIdx];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
			/* A kana mark after the character is drawn with it. */
			if(nextCode == TEXT_MARK_VOICED) {
				code |= TEXT_CODE_VOICED;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			} else if(nextCode == TEXT_MARK_SEMIVOICED) {
				code |= TEXT_CODE_SEMIVOICED;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Two narrow characters that fit in one cell together are drawn as
			   one pair, the second in the code's high byte. */
#if OUTLINED_GLYPH_PAIRS_FIT_CELL
			if(!(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
				code > TEXT_SPACE && nextCode > TEXT_SPACE) {
				Sint16 pair = FontGlyphs[code - TEXT_SPACE].width +
					FontGlyphs[nextCode - TEXT_SPACE].width;

				if(state->outline == TEXT_STYLE_OUTLINED) {
					if((Uint16)pair <= GLYPH_PAIR_OUTLINED_WIDTH_MAX) {
						code |= nextCode << 8;
						msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
					}
				} else if((Uint16)pair <= GLYPH_PAIR_WIDTH_MAX) {
					code |= nextCode << 8;
					msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				}
			}
#else
			if(!(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
				code > TEXT_SPACE && nextCode > TEXT_SPACE &&
				(Uint16)(FontGlyphs[code - TEXT_SPACE].width +
					FontGlyphs[nextCode - TEXT_SPACE].width) <=
					GLYPH_PAIR_WIDTH_MAX) {
				code |= nextCode << 8;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#endif
			glyphWidth = mnGlyphDraw(
				msg->win, code, pixelX, pixelY, GLYPH_DRAW_CHARACTER);
			msg->glyphDelay = MessageSpeedDelays[MainRoutineStateWork.msgSpeed];
			if(glyphWidth != 0) {
				/* The speaker's voice blip (voiceSnd, the first of four sounds,
				   one picked by the code's low two bits) every few characters
				   (MessageSpeedVoiceGaps), and the pen moves on by the glyph's
				   width. */
				if(state->voiceSnd != 0) {
					if(state->voiceCountdown == 0) {
						if(code != TEXT_SPACE) {
							sySndPlayStub(state->voiceSnd + (code & 3));
							state->voiceCountdown = voiceGap;
						}
					} else {
						state->voiceCountdown--;
					}
				}
				glyphAdvance = glyphWidth << 8;
#if LANGUAGE != LANGUAGE_JAPANESE
				if(code == TEXT_SPACE)
					glyphAdvance += msg->spacing[msg->line];
#else
				glyphAdvance += msg->spacing[msg->line];
#endif
				msg->x += glyphAdvance;
			}
			/* In Japanese a character that advances nothing (mnGlyphDraw
			   returns 0, as it does when no window sprite is free) closes the
			   window. */
#if LANGUAGE == LANGUAGE_JAPANESE
			else
				return TYPING_CLOSES_WINDOW;
#endif
			/* Outside battle a space ends this tick's typing. */
			if(code == TEXT_SPACE && state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}

		/* A pause holds the message on this entry until its countdown ends. */
		if(msg->countdown != 0) {
			msg->countdown--;
			if(msg->countdown != 0)
				continue;
		}
		msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
	} while(--glyphsLeft != 0);

	return TYPING_GOES_ON;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef MESSAGE_KEY_WAIT
#undef MESSAGE_KEY_WAIT_STARTED
