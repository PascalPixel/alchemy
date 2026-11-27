/* @file */
/* The per-frame message update: typing text and running its control codes. */
/* Part of window.c. */

/* The text canvas: 30 columns of tiles, 32 words to a column. */
#define CANVAS_COLUMNS		30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS	8 /* the top pixel rows, which stay put */

/* What mnMessageType reports. */
enum {
	MESSAGE_STEP_RUNNING = 0, /* the text goes on next frame */
	MESSAGE_STEP_ENDED = 8,	  /* the text has ended; the window stays open */
	MESSAGE_STEP_CLOSE = 9,	  /* the text is dismissed: close the window */
};

/* Scrolls the message canvas up by 6 pixel rows per step, below its top */
/* CANVAS_KEPT_ROWS, and clears the rows this uncovers. A typing message calls
 */
/* it with step 1 on each frame of a line scroll. */
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

s32 mnMessageType(MESSAGE *message);
void mnWindowStepSwellEffect(MESSAGE *message);

/* The per-frame message update: each message whose window is fully open runs */
/* its window animation or types more text, and on the text's end marks the */
/* window typed out (state 1) or closes it. */
void mnMessageUpdateAll(void) {
	MESSAGE *message = WINDOW_STATE->msgs;
	s32 i = 0;

	do {
		WINDOW *current = message->win;

		/* A zooming window is left alone until its zoom ends. */
		if(current != NULL && *(s32 *)&current->zoomStage == 0) {
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
					case MESSAGE_STEP_ENDED:
						message->win->state = 1;
						break;
					case MESSAGE_STEP_CLOSE: {
						WINDOW *window = message->win;

						mnWindowClose(window,
									  (u16)(window->flg & WINDOW_FLAG_INSTANT));
						/* pending is 0 here: it clears the message. */
						message->x = pending;
						message->y = pending;
						message->textIdx = pending;
						message->countdown = pending;
						message->colour = pending;
						message->outline = pending;
						message->lineSpacing = pending;
						message->win->state = 1;
						break;
					}
					}
				}
			}
		}
		i++;
		message++;
	} while(i != MESSAGE_COUNT);
}

void mnTextStartCanvasScroll(MESSAGE *message);
void mnMessageApplyStyle(MESSAGE *message);
s32 mnTextCheckPauseSkipped(MESSAGE *message);
s32 mnTextCheckTextDismissed(MESSAGE *message);

#define TEXT_SPEED MainRoutineStateWork.msgSpeed

/* Types one frame's share of a message's text: runs control codes (new */
/* line, new page, pauses, colours) and draws characters, playing the speaker's
 */
/* voice blips. Returns a MESSAGE_STEP_ result. The share is the message */
/* speed's glyphs a frame (MessageSpeed_Steps) outside battle, where a space */
/* or a control code also ends it; in battle it is 8 glyphs, 13 after a frame */
/* that took two VBlanks and 18 after one that took three or more. */
s32 mnMessageType(MESSAGE *message) {
	WIN_STATE *state = WINDOW_STATE;
	/* DEAD: the held keys are read here and never used; the ROM keeps the load.
	 */
	u32 keys = InputWork.held;
	s32 glyphsLeft = MessageSpeed_Steps[TEXT_SPEED];
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
	/* most three. The count is read as the interrupt may change it. */
	if(state->battleMode != WINDOW_FIELD) {
		frameVBlanks = *(volatile u16 *)&LastFrameVBlanks;
		CLAMP(frameVBlanks, 0, 3);
		glyphsLeft = frameVBlanks * 5 + 3;
	}

	if(message->scrollFrm != 0) {
		mnTextScrollCanvasUp(1);
		message->scrollFrm = (s16)message->scrollFrm - 1;
		return MESSAGE_STEP_RUNNING;
	}
	if(InputWork.held == 0 && message->glyphDelay != 0) {
		message->glyphDelay--;
		return MESSAGE_STEP_RUNNING;
	}

	do {
		code = TEXT_END;
		if(message->ended == 0)
			code = state->textBuf[message->textIdx];

#if LANGUAGE != LANGUAGE_JAPANESE
		if(code < TEXT_COMMANDS || code == TEXT_EM_DASH) {
#else
		if(code < TEXT_COMMANDS) {
#endif
			switch(code) {
			case TEXT_NEWLINE:
				message->x = message->startX;
				if(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) {
					if(message->y >= 13 << 8) {
						mnTextStartCanvasScroll(message);
						glyphsLeft = 1;
					}
					else {
						message->y += 13 << 8;
					}
				}
				else {
					message->y += 15 << 8;
					if(message->line <= 2)
						message->line++;
				}
				break;

			case TEXT_PAGE:
				if(message->countdown == 0) {
					message->countdown =
						MessageSpeed_HeldPageDelays[TEXT_SPEED];
					if(state->mindRead != 0)
						InputWork.pendingPressed = 0;
				}
				if(mnTextCheckTextDismissed(message) == 0) {
					WINDOW *pane = message->win;

					if(pane->width == 0 || pane->height == 0 ||
					   state->pageArrowShown != 0)
						break;
					mnGlyphDraw(pane, 1, pane->width * 4 - 8,
								pane->height * 8 - 16, GLYPH_DRAW_PAGE_ARROW);
					state->pageArrowShown = 1;
					break;
				}
				window = message->win;
				entryIndex = message->textIdx;
				x = window->x;
				y = window->y;
				oldWidth = window->width;
				oldHeight = window->height;
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
							x += (s32)(oldWidth - newWidth) / 4;
							mnWindowFitOnScr(entryIndex, &x, &y, &newWidth,
											 &newHeight, message->spacing,
											 WINDOW_FIT_SIZE_GIVEN);
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
				message->colour = TEXT_COLOR_WHITE;
				message->outline = TEXT_STYLE_PLAIN;
				message->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(message);
				syVramCacheFreeEntry(state->glyphResource);
				state->glyphResource = NO_GLYPH_RESOURCE;
				break;

			case TEXT_WAIT_KEY:
				if(message->countdown == 0) {
					message->countdown =
						MessageSpeed_HeldPageDelays[TEXT_SPEED];
					if(state->mindRead != 0)
						InputWork.pendingPressed = 0;
				}
				if(mnTextCheckTextDismissed(message) != 0)
					return MESSAGE_STEP_CLOSE;
				break;

			case TEXT_PAUSE_SHORT:
				if(message->countdown == 0)
					message->countdown = 20;
				state->speakerVoiceCountdown = 0;
				mnTextCheckPauseSkipped(message);
				break;

			case TEXT_PAUSE_LONGEST:
				if(message->countdown == 0)
					message->countdown = 120;
				state->speakerVoiceCountdown = 0;
				mnTextCheckPauseSkipped(message);
				break;

			case TEXT_PAUSE_LONG:
				if(message->countdown == 0)
					message->countdown = 60;
				state->speakerVoiceCountdown = 0;
				break;

			case TEXT_COLOR:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->colour = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_OUTLINE:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->outline = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_LINE_SPACING:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->lineSpacing = state->textBuf[message->textIdx];
				mnMessageApplyStyle(message);
				break;

			case TEXT_COLORS_RESET:
				message->colour = TEXT_COLOR_WHITE;
				message->outline = TEXT_STYLE_PLAIN;
				message->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(message);
				break;

			case TEXT_WINDOW_ANIMATION:
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				message->win->anm = state->textBuf[message->textIdx];
				message->countdown = 11;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
				break;

			case TEXT_END:
			case TEXT_END_ALT:
				message->ended = 1;
				return MESSAGE_STEP_ENDED;
			}

			if(state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}
		else {
			pixelX = (message->x + 128) / 256;
			pixelY = (message->y + 128) / 256;
			voiceGap = MessageSpeed_VoiceGaps[TEXT_SPEED];
			nextEntryIndex = message->textIdx;
#if !TLA_PATCH_2
			if(state->mindRead != 0)
				pixelX += 8;
#endif
			nextEntryIndex = (nextEntryIndex + 1) & TEXT_BUFFER_MASK;
			nextCode = state->textBuf[nextEntryIndex];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
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
#if TLA_PATCH_4
			if(!(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
			   code > TEXT_SPACE && nextCode > TEXT_SPACE) {
				s16 pairWidth = Font_Glyphs[code - TEXT_SPACE].width +
								Font_Glyphs[nextCode - TEXT_SPACE].width;

				if(state->outline == TEXT_STYLE_OUTLINED) {
					if((u16)pairWidth <= 14) {
						code |= nextCode << 8;
						message->textIdx =
							(message->textIdx + 1) & TEXT_BUFFER_MASK;
					}
				}
				else if((u16)pairWidth <= 15) {
					code |= nextCode << 8;
					message->textIdx =
						(message->textIdx + 1) & TEXT_BUFFER_MASK;
				}
			}
#else
			if(!(message->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
			   code > TEXT_SPACE && nextCode > TEXT_SPACE &&
			   (u16)(Font_Glyphs[code - TEXT_SPACE].width +
					 Font_Glyphs[nextCode - TEXT_SPACE].width) <= 15) {
				code |= nextCode << 8;
				message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#endif
			if(state->textMode != 0 && message->compressLines != 0)
				pixelY = (message->y + 128) / 320 - 3;
			glyphWidth = mnGlyphDraw(message->win, code, pixelX, pixelY,
									 GLYPH_DRAW_CHARACTER);
			message->glyphDelay = MessageSpeed_Delays[TEXT_SPEED];
			if(glyphWidth == 0)
				return MESSAGE_STEP_CLOSE;
			if(state->speakerVoiceSnd != 0) {
				if(state->speakerVoiceCountdown == 0) {
					if(code != TEXT_SPACE) {
						sySndPlayStub(state->speakerVoiceSnd + (code & 3));
						state->speakerVoiceCountdown = voiceGap;
					}
				}
				else {
					state->speakerVoiceCountdown--;
				}
			}
			glyphAdvance = glyphWidth << 8;
#if LANGUAGE == LANGUAGE_JAPANESE
			glyphAdvance += message->spacing[message->line];
#elif LANGUAGE != LANGUAGE_GERMAN
			if(code == TEXT_SPACE)
				glyphAdvance += message->spacing[message->line];
#endif
			message->x += glyphAdvance;
			if(code == TEXT_SPACE && state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}

		if(message->countdown != 0) {
			message->countdown--;
			if(message->countdown != 0)
				continue;
		}
		message->textIdx = (message->textIdx + 1) & TEXT_BUFFER_MASK;
	} while(--glyphsLeft != 0);

	return MESSAGE_STEP_RUNNING;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef TEXT_SPEED
