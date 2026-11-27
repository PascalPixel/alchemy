/* The per-tick message update: typing text and running its control codes. Part
   of window.c. */

/* The text canvas: 30 columns of tiles, 32 words to a column. */
#define CANVAS_COLUMNS		30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS	8 /* the top pixel rows, which stay put */

/* What mnMessageType reports. */
enum {
	MESSAGE_STEP_RUNNING = 0, /* the text goes on next tick */
	MESSAGE_STEP_ENDED = 8,	  /* the text has ended; the window stays open */
	MESSAGE_STEP_CLOSE = 9,	  /* the text is dismissed: close the window */
};

#include "window/text_scroll_canvas_up.c"

Sint32 mnMessageType(MESSAGE *msg);
void   mnWindowStepSwellEffect(MESSAGE *msg);

/* The per-tick message update: each message whose window is fully open runs
   its window animation or types more text, and on the text's end marks the
   window typed out (state 1) or closes it. */
void mnMessageUpdateAll(void)
{
	MESSAGE *msg = WINDOW_STATE->msgs;
	Sint32	 i = 0;

	do {
		WINDOW *cur = msg->win;

		/* A zooming window is left alone until its zoom ends. */
		if(cur != NULL && *(Sint32 *)&cur->zoomStage == 0) {
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
					case MESSAGE_STEP_ENDED:
						msg->win->state = 1;
						break;
					case MESSAGE_STEP_CLOSE: {
						WINDOW *win = msg->win;

						mnWindowClose(
							win, (Uint16)(win->flg & WINDOW_FLAG_INSTANT));
						/* pending is 0 here: it clears the message. */
						msg->x = pending;
						msg->y = pending;
						msg->textIdx = pending;
						msg->countdown = pending;
						msg->colour = pending;
						msg->outline = pending;
						msg->lineSpacing = pending;
						msg->win->state = 1;
						break;
					}
					}
				}
			}
		}
		i++;
		msg++;
	} while(i != MESSAGE_COUNT);
}

void mnTextStartCanvasScroll(MESSAGE *msg);
void mnMessageApplyStyle(MESSAGE *msg);
Bool mnTextCheckPauseSkipped(MESSAGE *msg);
Bool mnTextCheckTextDismissed(MESSAGE *msg);

#define TEXT_SPEED MainRoutineStateWork.msgSpeed

/* Types one tick's share of a message's text: runs control codes (new line,
   new page, pauses, colours) and draws characters, playing the speaker's voice
   blips. Returns a MESSAGE_STEP_ result. The share is the message speed's
   glyphs a tick (MessageSpeedSteps) outside battle, where a space or a
   control code also ends it; in battle it is 8 glyphs, 13 after a tick that
   took two frames and 18 after one that took three or more. */
Sint32 mnMessageType(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	/* DEAD: the held keys are read here and never used; the ROM keeps the
	   load. */
	Uint32	keys = PerWork.data;
	Sint32	glyphsLeft = MessageSpeedSteps[TEXT_SPEED];
	Sint32	tickFrames;
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
	   most three. The count is read as the interrupt may change it. */
	if(state->battleMode != WINDOW_FIELD) {
		tickFrames = *(volatile Uint16 *)&LastTickFrames;
		CLAMP(tickFrames, 0, 3);
		glyphsLeft = tickFrames * 5 + 3;
	}

	if(msg->scrollFrm != 0) {
		mnTextScrollCanvasUp(1);
		msg->scrollFrm = (Sint16)msg->scrollFrm - 1;
		return MESSAGE_STEP_RUNNING;
	}
	if(PerWork.data == 0 && msg->glyphDelay != 0) {
		msg->glyphDelay--;
		return MESSAGE_STEP_RUNNING;
	}

	do {
		code = TEXT_END;
		if(msg->ended == 0)
			code = state->textBuf[msg->textIdx];

#if LANGUAGE != LANGUAGE_JAPANESE
		if(code < TEXT_COMMANDS || code == TEXT_EM_DASH) {
#else
		if(code < TEXT_COMMANDS) {
#endif
			switch(code) {
			case TEXT_NEWLINE:
				msg->x = msg->startX;
				if(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) {
					if(msg->y >= 13 << 8) {
						mnTextStartCanvasScroll(msg);
						glyphsLeft = 1;
					} else {
						msg->y += 13 << 8;
					}
				} else {
					msg->y += 15 << 8;
					if(msg->line <= 2)
						msg->line++;
				}
				break;

			case TEXT_PAGE:
				if(msg->countdown == 0) {
					msg->countdown = MessageSpeedHeldPageDelays[TEXT_SPEED];
					if(state->mindRead != 0)
						PerWork.pendingPush = 0;
				}
				if(mnTextCheckTextDismissed(msg) == 0) {
					WINDOW *pane = msg->win;

					if(pane->width == 0 || pane->height == 0 ||
						state->pageArrowShown != 0)
						break;
					mnGlyphDraw(pane, 1, pane->width * 4 - 8,
						pane->height * 8 - 16, GLYPH_DRAW_PAGE_ARROW);
					state->pageArrowShown = 1;
					break;
				}
				win = msg->win;
				entryIdx = msg->textIdx;
				x = win->x;
				y = win->y;
				oldWidth = win->width;
				oldHeight = win->height;
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
							x += (Sint32)(oldWidth - newWidth) / 4;
							mnWindowFitOnScreen(entryIdx, &x, &y, &newWidth,
								&newHeight, msg->spacing,
								WINDOW_FIT_SIZE_GIVEN);
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
				msg->colour = TEXT_COLOR_WHITE;
				msg->outline = TEXT_STYLE_PLAIN;
				msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(msg);
				syVRAMCacheFreeEntry(state->glyphResource);
				state->glyphResource = NO_GLYPH_RESOURCE;
				break;

			case TEXT_WAIT_KEY:
				if(msg->countdown == 0) {
					msg->countdown = MessageSpeedHeldPageDelays[TEXT_SPEED];
					if(state->mindRead != 0)
						PerWork.pendingPush = 0;
				}
				if(mnTextCheckTextDismissed(msg) != 0)
					return MESSAGE_STEP_CLOSE;
				break;

			case TEXT_PAUSE_SHORT:
				if(msg->countdown == 0)
					msg->countdown = 20;
				state->speakerVoiceCountdown = 0;
				mnTextCheckPauseSkipped(msg);
				break;

			case TEXT_PAUSE_LONGEST:
				if(msg->countdown == 0)
					msg->countdown = 120;
				state->speakerVoiceCountdown = 0;
				mnTextCheckPauseSkipped(msg);
				break;

			case TEXT_PAUSE_LONG:
				if(msg->countdown == 0)
					msg->countdown = 60;
				state->speakerVoiceCountdown = 0;
				break;

			case TEXT_COLOR:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->colour = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_OUTLINE:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->outline = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_LINE_SPACING:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->lineSpacing = state->textBuf[msg->textIdx];
				mnMessageApplyStyle(msg);
				break;

			case TEXT_COLORS_RESET:
				msg->colour = TEXT_COLOR_WHITE;
				msg->outline = TEXT_STYLE_PLAIN;
				msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
				mnMessageApplyStyle(msg);
				break;

			case TEXT_WINDOW_ANIMATION:
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				msg->win->anm = state->textBuf[msg->textIdx];
				msg->countdown = 11;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				break;

			case TEXT_END:
			case TEXT_END_ALT:
				msg->ended = 1;
				return MESSAGE_STEP_ENDED;
			}

			if(state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		} else {
			pixelX = (msg->x + 128) / 256;
			pixelY = (msg->y + 128) / 256;
			voiceGap = MessageSpeedVoiceGaps[TEXT_SPEED];
			nextEntryIdx = msg->textIdx;
#if !MIND_READ_TEXT_NOT_INDENTED
			if(state->mindRead != 0)
				pixelX += 8;
#endif
			nextEntryIdx = (nextEntryIdx + 1) & TEXT_BUFFER_MASK;
			nextCode = state->textBuf[nextEntryIdx];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
			if(nextCode == TEXT_MARK_VOICED) {
				code |= TEXT_CODE_VOICED;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			} else if(nextCode == TEXT_MARK_SEMIVOICED) {
				code |= TEXT_CODE_SEMIVOICED;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
#if OUTLINED_GLYPH_PAIRS_FIT_CELL
			if(!(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
				code > TEXT_SPACE && nextCode > TEXT_SPACE) {
				Sint16 pairWidth = FontGlyphs[code - TEXT_SPACE].width +
					FontGlyphs[nextCode - TEXT_SPACE].width;

				if(state->outline == TEXT_STYLE_OUTLINED) {
					if((Uint16)pairWidth <= 14) {
						code |= nextCode << 8;
						msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
					}
				} else if((Uint16)pairWidth <= 15) {
					code |= nextCode << 8;
					msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
				}
			}
#else
			if(!(msg->win->flg & WINDOW_FLAG_TEXT_CANVAS) &&
				code > TEXT_SPACE && nextCode > TEXT_SPACE &&
				(Uint16)(FontGlyphs[code - TEXT_SPACE].width +
					FontGlyphs[nextCode - TEXT_SPACE].width) <= 15) {
				code |= nextCode << 8;
				msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
			}
#endif
#endif
			if(state->textMode != 0 && msg->compressLines != 0)
				pixelY = (msg->y + 128) / 320 - 3;
			glyphWidth = mnGlyphDraw(
				msg->win, code, pixelX, pixelY, GLYPH_DRAW_CHARACTER);
			msg->glyphDelay = MessageSpeedDelays[TEXT_SPEED];
			if(glyphWidth == 0)
				return MESSAGE_STEP_CLOSE;
			if(state->speakerVoiceSnd != 0) {
				if(state->speakerVoiceCountdown == 0) {
					if(code != TEXT_SPACE) {
						sySndPlayStub(state->speakerVoiceSnd + (code & 3));
						state->speakerVoiceCountdown = voiceGap;
					}
				} else {
					state->speakerVoiceCountdown--;
				}
			}
			glyphAdvance = glyphWidth << 8;
#if LANGUAGE == LANGUAGE_JAPANESE
			glyphAdvance += msg->spacing[msg->line];
#elif !RAGGED_LINES
			if(code == TEXT_SPACE)
				glyphAdvance += msg->spacing[msg->line];
#endif
			msg->x += glyphAdvance;
			if(code == TEXT_SPACE && state->battleMode == WINDOW_FIELD)
				glyphsLeft = 1;
		}

		if(msg->countdown != 0) {
			msg->countdown--;
			if(msg->countdown != 0)
				continue;
		}
		msg->textIdx = (msg->textIdx + 1) & TEXT_BUFFER_MASK;
	} while(--glyphsLeft != 0);

	return MESSAGE_STEP_RUNNING;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef TEXT_SPEED
