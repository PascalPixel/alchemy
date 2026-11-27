/* @file */
/* The per-frame message update: typing text and running its control codes. */
/* Part of window.c. */

/* The battle message canvas: 30 columns of four tiles, each column 32 pixel */
/* rows of a word each. */
#define CANVAS_COLUMNS		30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS	8 /* the top 8 pixel rows stay put */

/* A message waiting for a button holds its countdown here, set again each */
/* frame and counted down once, so it reads below MESSAGE_KEY_WAIT_STARTED */
/* only on the wait's first frame. */
#define MESSAGE_KEY_WAIT		 919
#define MESSAGE_KEY_WAIT_STARTED 900

/* Scrolls the battle message canvas up by 6 pixel rows per step, below its top
 */
/* 8 rows, and clears the rows this uncovers. */
void mnTextScrollCanvasUp(s32 step) {
	s32 shift = step * 6;
	u32 *base = TEXT_CANVAS_VRAM;
	s32 n;

	for(n = 0; n < CANVAS_COLUMNS; n++) {
		u32 *column = base + n * CANVAS_COLUMN_WORDS;

		syDMAXCopy(
			column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
			DMA_COPY_WORDS(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
			REG_DMA3);
		syWordsFill(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
	}
}

/* What mnMessageType reports. */
enum {
	TYPING_GOES_ON = 0,		  /* the text goes on next frame */
	TYPING_ENDED = 8,		  /* the text has ended; the window stays open */
	TYPING_CLOSES_WINDOW = 9, /* the text is dismissed: close the window */
};

/* The per-frame message update: each message whose window is fully open runs */
/* its window animation or types more text, and on the text's end marks the */
/* window typed (WINDOW_TYPED) or closes it. */
void mnMessageUpdateAll(void) {
	MESSAGE *message = WINDOW_STATE->msgs;
	s32 messageIndex = 0;

	do {
		WINDOW *current = message->win;

		/* A zooming window is left alone until its zoom ends. */
		if(current != NULL && current->zoomStage == 0 &&
		   current->duration == 0) {
			u16 flags = current->flg;

			/* Flags 0 with no zoom left: the window has closed. */
			if(flags == 0) {
				message->win = NULL;
			}
			else {
				s32 pending = current->anm;
				s32 step;

				if(pending != 0) {
					mnWindowStepSwellEffect(message);
				}
				else {
					step = mnMessageType(message);
					switch(step) {
					case TYPING_ENDED:
						message->win->state = WINDOW_TYPED;
						break;
					case TYPING_CLOSES_WINDOW: {
						WINDOW *window = message->win;

						mnWindowClose(window,
									  window->flg & WINDOW_FLAG_INSTANT);
						/* pending is 0 here: it clears the message. */
						message->x = pending;
						message->y = pending;
						message->textIdx = pending;
						message->countdown = pending;
						message->colour = pending;
						message->outline = pending;
						message->lineSpacing = pending;
						message->win->state = WINDOW_TYPED;
						break;
					}
					}
				}
			}
		}
		messageIndex++;
		message++;
	} while(messageIndex != MESSAGE_COUNT);
}

/* Types one frame's share of a message's text: runs control codes (new line, */
/* new page, pauses, colours) and draws characters, playing the speaker's voice
 */
/* blips. */
s32 mnMessageType(MESSAGE *message) {
	WIN_STATE *state = WINDOW_STATE;
	/* DEAD: KeysHeld read and not used; the ROM keeps the load. */
	u32 keys = KeysHeld;
	s32 glyphsLeft = MessageSpeed_Steps[MainRoutineStateWork.msgSpeed];
	s32 frameVBlanks;
	u32 code;
	u32 nextCode;
	WINDOW *window;
	s32 x;
	s32 y;
	u32 newWidth;
	u32 newHeight;
	s32 oldWidth;
	s32 oldHeight;
	s32 entryIndex;
	s32 nextEntryIndex;
	s32 pixelX;
	s32 pixelY;
	s32 voiceGap;
	s32 glyphWidth;
	s32 glyphAdvance;

	(void)keys;

	/* In battle, text runs at 3 glyphs a frame and 5 more for each VBlank the
	 */
	/* last frame took (LastFrameVBlanks, 1 for a frame on time), counting at */
	/* most two. */
	if(state->battleMode != WINDOW_FIELD) {
		frameVBlanks = LastFrameVBlanks;
		CLAMP(frameVBlanks, 0, 2);
		glyphsLeft = frameVBlanks * 5 + 3;
	}

	/* Finish scrolling the canvas up a line before any more text. */
	if(message->transition != 0) {
		mnTextScrollCanvasUp(1);
		message->transition = (s16)message->transition - 1;
		return TYPING_GOES_ON;
	}
	/* With no button held, wait out the delay after the last glyph. */
	if(KeysHeld == 0 && message->glyphDelay != 0) {
		message->glyphDelay--;
		return TYPING_GOES_ON;
	}

	/* Run control codes and draw characters until this frame's share is done.
	 */
	do {
		code = TEXT_END;
		if(message->ended == 0)
			code = state->textBuf[message->textIdx];

		if(code < TEXT_COMMANDS) {
			switch(code) {
			case TEXT_NEWLINE:
				/* On a canvas the text scrolls up from the second line on. */
				message->x = message->startX;
				if(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) {
					if(message->y >= BATTLE_MESSAGE_LINE_HEIGHT << 8) {
						mnTextStartCanvasScroll(message);
						glyphsLeft = 1;
					}
					else {
						message->y += BATTLE_MESSAGE_LINE_HEIGHT << 8;
					}
				}
				else {
					message->y += TEXT_LINE_HEIGHT << 8;
					if(message->line < MESSAGE_LINE_COUNT - 1)
						message->line++;
				}
				break;

			case TEXT_PAGE:
				/* Under Mind Read a press from before the wait does not count.
				 */
				if(state->mindRead != 0 &&
				   message->countdown < MESSAGE_KEY_WAIT_STARTED)
					KeysPressedLatch = 0;
				message->countdown = MESSAGE_KEY_WAIT;
				if(mnTextCheckTextDismissed(message) == 0) {
					/* Until the button: show the next-page arrow in the
					 * bottom-right */
					/* corner. */
					WINDOW *arrowWindow = message->win;

#if TBS_PATCH_1
					if(arrowWindow->width == 0 || arrowWindow->height == 0 ||
					   state->pageArrowShown != 0)
						break;
#else
					if(state->pageArrowShown != 0)
						break;
#endif
					mnGlyphDraw(arrowWindow, 1, arrowWindow->width * 4 - 8,
								arrowWindow->height * 8 - 16,
								GLYPH_DRAW_PAGE_ARROW);
					state->pageArrowShown = 1;
					break;
				}
				/* Clear the window, and refit it to the next page's text. */
				window = message->win;
				x = window->x;
				y = window->y;
				oldWidth = window->width;
				oldHeight = window->height;
				entryIndex = message->textIdx;
				state->pageArrowShown = 0;
				mnWindowSprPrepareForRedraw(window);
				if(message->flg == 0 && (window->width | window->height) != 0)
					mnWindowEraseBorderRect(window->x, window->y, window->width,
											window->height);
				entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
				if(state->textBuf[entryIndex] != 0 &&
				   (window->width | window->height) != 0) {
					if(message->flg != 0) {
						mnWindowEraseBorderRect(window->x, window->y,
												window->width, window->height);
					}
					else {
						mnWindowFitOnScr(entryIndex, &x, &y, &newWidth,
										 &newHeight, message->spacing, 0);
						if(window->flg & WINDOW_FLAG_ANCHOR_BOTTOM) {
							if(oldHeight != newHeight)
								y -= newHeight - oldHeight;
							if(y < 0)
								y = 0;
						}
						if(!(window->flg & WINDOW_FLAG_FIXED_X)) {
							/* Shift the window by a quarter of the change in
							 * width (tiles), */
							/* part way to keeping its centre, which a half
							 * would keep; then */
							/* fit it on the screen again at its new size. */
							x += (s32)(oldWidth - newWidth) / 4;
							mnWindowFitOnScr(entryIndex, &x, &y, &newWidth,
											 &newHeight, message->spacing,
											 WINDOW_FIT_GIVEN_SIZE);
						}
						window->x = x;
						window->y = y;
						window->width = newWidth;
						window->height = newHeight;
					}
					mnWindowDrawBorder(window->x, window->y, window->width,
									   window->height);
				}
				message->x = message->startX;
				message->y = 0;
				message->line = 0;
				/* The French, Spanish and Italian releases also start each page
				 * in the */
				/* default style. */
#if TBS_PATCH_3
				message->colour = TEXT_DEFAULT_COLOR;
				message->outline = TEXT_STYLE_PLAIN;
				message->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(message);
#endif
				syVramCacheFreeEntry(state->glyphResource);
				state->glyphResource = NO_GLYPH_RESOURCE;
				break;

			case TEXT_WAIT_KEY:
				/* The same hold as a page break's. */
				if(state->mindRead != 0 &&
				   message->countdown < MESSAGE_KEY_WAIT_STARTED)
					KeysPressedLatch = 0;
				if(mnTextCheckTextDismissed(message) != 0)
					return TYPING_CLOSES_WINDOW;
				message->countdown = MESSAGE_KEY_WAIT;
				break;

			case TEXT_PAUSE_SHORT:
				if(message->countdown == 0)
					message->countdown = 20;
				state->voiceCountdown = 0;
				mnTextCheckPauseSkipped(message);
				break;

			case TEXT_PAUSE_LONGEST:
				if(message->countdown == 0)
					message->countdown = 120;
				state->voiceCountdown = 0;
				mnTextCheckPauseSkipped(message);
				break;

			case TEXT_PAUSE_LONG:
				if(message->countdown == 0)
					message->countdown = 60;
				state->voiceCountdown = 0;
				break;

			case TEXT_COLOR:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->colour = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_STYLE:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->outline = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_LINE_SPACING:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->lineSpacing = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_RESET_STYLE:
				message->colour = TEXT_DEFAULT_COLOR;
				message->outline = TEXT_STYLE_PLAIN;
				message->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(message);
				break;

			case TEXT_WINDOW_ANIMATION:
				/* The animation lasts while the countdown of 10 runs out */
				/* (mnWindowStepSwellEffect counts it down); typing then resumes
				 * after */
				/* the operand. */
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->win->anm = state->textBuf[message->textIdx];
				message->countdown = 10;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				break;

			case TEXT_END:
			case TEXT_END_ALT:
				message->ended = 1;
				return TYPING_ENDED;
			}

			/* Outside battle a control code ends this frame's typing. */
			if(state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}
		else {
			/* A character: draw it at the pen position, rounded to whole
			 * pixels. */
			pixelX = (message->x + 128) / 256;
			pixelY = (message->y + 128) / 256;
			voiceGap = MessageSpeed_VoiceGaps[MainRoutineStateWork.msgSpeed];
			nextEntryIndex = message->textIdx;
			/* Under Mind Read the text starts 8 pixels further in (Japanese and
			 */
			/* English), in the window mnWindowFitOnScr made 2 tiles wider. */
#if !TBS_PATCH_2
			if(state->mindRead != 0)
				pixelX += 8;
#endif
			nextEntryIndex = (nextEntryIndex + 1) & TEXT_BUFFER_MASK;
			nextCode = state->textBuf[nextEntryIndex];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
			/* A kana mark after the character is drawn with it. */
			if(nextCode == TEXT_MARK_VOICED) {
				code |= TEXT_CODE_VOICED;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
			}
			else if(nextCode == TEXT_MARK_SEMIVOICED) {
				code |= TEXT_CODE_SEMIVOICED;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Two narrow characters that fit in one cell together are drawn as
			 * one */
			/* pair, the second in the code's high byte. */
#if LANGUAGE == LANGUAGE_SPANISH
			if(!(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
			   code > TEXT_SPACE && nextCode > TEXT_SPACE) {
				s16 pair = Font_Glyphs[code - TEXT_SPACE].width +
						   Font_Glyphs[nextCode - TEXT_SPACE].width;

				if(state->outline == TEXT_STYLE_OUTLINED) {
					if((u16)pair <= GLYPH_PAIR_OUTLINED_WIDTH_MAX) {
						code |= nextCode << 8;
						message->textIdx =
							(message->textIdx + 1) & TEXT_BUFFER_MASK;
					}
				}
				else if((u16)pair <= GLYPH_PAIR_WIDTH_MAX) {
					code |= nextCode << 8;
					message->textIdx =
						(message->textIdx + 1) & TEXT_BUFFER_MASK;
				}
			}
#else
			if(!(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
			   code > TEXT_SPACE && nextCode > TEXT_SPACE &&
			   (u16)(Font_Glyphs[code - TEXT_SPACE].width +
					 Font_Glyphs[nextCode - TEXT_SPACE].width) <=
				   GLYPH_PAIR_WIDTH_MAX) {
				code |= nextCode << 8;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#endif
			glyphWidth = mnGlyphDraw(message->win, code, pixelX, pixelY,
									 GLYPH_DRAW_CHARACTER);
			message->glyphDelay =
				MessageSpeed_Delays[MainRoutineStateWork.msgSpeed];
			if(glyphWidth != 0) {
				/* The speaker's voice blip (voiceSnd, the first of four */
				/* sounds, one picked by the code's low two bits) every few
				 * characters */
				/* (MessageSpeed_VoiceGaps), and the pen moves on by the glyph's
				 * width. */
				if(state->voiceSnd != 0) {
					if(state->voiceCountdown == 0) {
						if(code != TEXT_SPACE) {
							sySndPlayStub(state->voiceSnd + (code & 3));
							state->voiceCountdown = voiceGap;
						}
					}
					else {
						state->voiceCountdown--;
					}
				}
				glyphAdvance = glyphWidth << 8;
#if LANGUAGE != LANGUAGE_JAPANESE
				if(code == TEXT_SPACE)
					glyphAdvance += message->spacing[message->line];
#else
				glyphAdvance += message->spacing[message->line];
#endif
				message->x += glyphAdvance;
			}
			/* In Japanese a character that advances nothing (mnGlyphDraw */
			/* returns 0, as it does when no window sprite is free) closes the
			 */
			/* window. */
#if LANGUAGE == LANGUAGE_JAPANESE
			else
				return TYPING_CLOSES_WINDOW;
#endif
			/* Outside battle a space ends this frame's typing. */
			if(code == TEXT_SPACE && state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}

		/* A pause holds the message on this entry until its countdown ends. */
		if(message->countdown != 0) {
			message->countdown--;
			if(message->countdown != 0)
				continue;
		}
		message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
	} while(--glyphsLeft != 0);

	return TYPING_GOES_ON;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef MESSAGE_KEY_WAIT
#undef MESSAGE_KEY_WAIT_STARTED
