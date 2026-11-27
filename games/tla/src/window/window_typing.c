// The per-tick message update: typing text and running its control codes. Part of window.c.

// The text canvas: 30 columns of tiles, 32 words to a column.
#define CANVAS_COLUMNS      30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS    8 // the top pixel rows, which stay put

// What mnMessageType reports.
enum
{
    MESSAGE_STEP_RUNNING = 0, // the text goes on next tick
    MESSAGE_STEP_ENDED   = 8, // the text has ended; the window stays open
    MESSAGE_STEP_CLOSE   = 9, // the text is dismissed: close the window
};

// Scrolls the message canvas up by 6 pixel rows per step, below its top CANVAS_KEPT_ROWS, and
// clears the rows this uncovers. A typing message calls it with step 1 on each tick of a line
// scroll.
void mnTextScrollCanvasUp(Sint32 step)
{
    Sint32  shift = step * 6;
    Uint32 *base  = TEXT_CANVAS_VRAM;
    Sint32  n;

    for (n = 0; n < CANVAS_COLUMNS; n++)
    {
        Uint32 *column = base + n * CANVAS_COLUMN_WORDS;

        syDMAXCopy(column + CANVAS_KEPT_ROWS + shift,
                   column + CANVAS_KEPT_ROWS,
                   DMA_COPY_WORDS(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
                   REG_DMA3);
        syWordsFill(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
    }
}

Sint32 mnMessageType(MESSAGE *msg);
void   mnWindowStepSwellEffect(MESSAGE *msg);

// The per-tick message update: each message whose window is fully open runs its window animation or
// types more text, and on the text's end marks the window typed out (state 1) or closes it.
void   mnMessageUpdateAll(void)
{
    MESSAGE *msg = WINDOW_STATE->msgs;
    Sint32   i   = 0;

    do
    {
        WINDOW *current = msg->window;

        // A zooming window is left alone until its zoom ends.
        if (current != NULL && *(Sint32 *)&current->zoomStage == 0)
        {
            Uint16 flags = current->flags;

            // Flags 0 with no zoom left: the window has closed.
            if (flags == 0)
            {
                msg->window = NULL;
            }
            else
            {
                Sint32 pending = current->animation;
                Sint32 step;

                if (pending != 0)
                {
                    mnWindowStepSwellEffect(msg);
                }
                else
                {
                    step = mnMessageType(msg);
                    switch (step)
                    {
                        case MESSAGE_STEP_ENDED: msg->window->state = 1; break;
                        case MESSAGE_STEP_CLOSE:
                        {
                            WINDOW *window = msg->window;

                            mnWindowClose(window, (Uint16)(window->flags & WINDOW_FLAG_INSTANT));
                            // pending is 0 here: it clears the message.
                            msg->x             = pending;
                            msg->y             = pending;
                            msg->textIndex     = pending;
                            msg->countdown     = pending;
                            msg->colour        = pending;
                            msg->outline       = pending;
                            msg->lineSpacing   = pending;
                            msg->window->state = 1;
                            break;
                        }
                    }
                }
            }
        }
        i++;
        msg++;
    } while (i != MESSAGE_COUNT);
}

void mnTextStartCanvasScroll(MESSAGE *msg);
void mnMessageApplyStyle(MESSAGE *msg);
Bool mnTextCheckPauseSkipped(MESSAGE *msg);
Bool mnTextCheckTextDismissed(MESSAGE *msg);

#define TEXT_SPEED MainRoutineStateWork.msgSpeed

// Types one tick's share of a message's text: runs control codes (new line, new page, pauses,
// colours) and draws characters, playing the speaker's voice blips. Returns a MESSAGE_STEP_ result.
// The share is the message speed's glyphs a tick (MessageSpeedSteps) outside battle, where a space
// or a control code also ends it; in battle it is 8 glyphs, 13 after a tick that took two frames
// and 18 after one that took three or more.
Sint32 mnMessageType(MESSAGE *msg)
{
    WIN_STATE *state      = WINDOW_STATE;
    // DEAD: the held keys are read here and never used; the ROM keeps the load.
    Uint32     keys       = PerWork.data;
    Sint32     glyphsLeft = MessageSpeedSteps[TEXT_SPEED];
    Sint32     tickFrames;
    Uint32     code;
    Uint32     nextCode;
    WINDOW    *window;
    Sint32     x;
    Sint32     y;
    Uint32     newWidth;
    Uint32     newHeight;
    Sint32     oldWidth;
    Sint32     oldHeight;
    Sint32     entryIndex;
    Sint32     nextEntryIndex;
    Sint32     pixelX;
    Sint32     pixelY;
    Sint32     voiceGap;
    Sint32     glyphWidth;
    Sint32     glyphAdvance;

    (void)keys;

    // In battle, text runs at 3 glyphs a tick and 5 more for each VBlank the last tick took
    // (LastTickFrames, 1 for a tick on time), counting at most three. The count is read as the
    // interrupt may change it.
    if (state->battleMode != WINDOW_FIELD)
    {
        tickFrames = *(volatile Uint16 *)&LastTickFrames;
        CLAMP(tickFrames, 0, 3);
        glyphsLeft = tickFrames * 5 + 3;
    }

    if (msg->scrollFrame != 0)
    {
        mnTextScrollCanvasUp(1);
        msg->scrollFrame = (Sint16)msg->scrollFrame - 1;
        return MESSAGE_STEP_RUNNING;
    }
    if (PerWork.data == 0 && msg->glyphDelay != 0)
    {
        msg->glyphDelay--;
        return MESSAGE_STEP_RUNNING;
    }

    do
    {
        code = TEXT_END;
        if (msg->ended == 0)
            code = state->textBuf[msg->textIndex];

#if LANGUAGE != LANGUAGE_JAPANESE
        if (code < TEXT_COMMANDS || code == TEXT_EM_DASH)
        {
#else
        if (code < TEXT_COMMANDS)
        {
#endif
            switch (code)
            {
                case TEXT_NEWLINE:
                    msg->x = msg->startX;
                    if (msg->window->flags & WINDOW_FLAG_TEXT_CANVAS)
                    {
                        if (msg->y >= 13 << 8)
                        {
                            mnTextStartCanvasScroll(msg);
                            glyphsLeft = 1;
                        }
                        else
                        {
                            msg->y += 13 << 8;
                        }
                    }
                    else
                    {
                        msg->y += 15 << 8;
                        if (msg->line <= 2)
                            msg->line++;
                    }
                    break;

                case TEXT_PAGE:
                    if (msg->countdown == 0)
                    {
                        msg->countdown = MessageSpeedHeldPageDelays[TEXT_SPEED];
                        if (state->mindRead != 0)
                            PerWork.pendingPush = 0;
                    }
                    if (mnTextCheckTextDismissed(msg) == 0)
                    {
                        WINDOW *pane = msg->window;

                        if (pane->width == 0 || pane->height == 0 || state->pageArrowShown != 0)
                            break;
                        mnGlyphDraw(pane,
                                    1,
                                    pane->width * 4 - 8,
                                    pane->height * 8 - 16,
                                    GLYPH_DRAW_PAGE_ARROW);
                        state->pageArrowShown = 1;
                        break;
                    }
                    window                = msg->window;
                    entryIndex            = msg->textIndex;
                    x                     = window->x;
                    y                     = window->y;
                    oldWidth              = window->width;
                    oldHeight             = window->height;
                    state->pageArrowShown = 0;
                    mnWindowSpritePrepareForRedraw(window);
                    if (msg->flags == 0 && (window->width | window->height) != 0)
                        mnWindowEraseBorderRect(
                            window->x, window->y, window->width, window->height);
                    entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
                    if (state->textBuf[entryIndex] != 0 && (window->width | window->height) != 0)
                    {
                        if (msg->flags != 0)
                        {
                            mnWindowEraseBorderRect(
                                window->x, window->y, window->width, window->height);
                        }
                        else
                        {
                            mnWindowFitOnScreen(
                                entryIndex, &x, &y, &newWidth, &newHeight, msg->spacing, 0);
                            if (window->flags & WINDOW_FLAG_ANCHOR_BOTTOM)
                            {
                                if (oldHeight != newHeight)
                                    y -= newHeight - oldHeight;
                                if (y < 0)
                                    y = 0;
                            }
                            if (!(window->flags & WINDOW_FLAG_FIXED_X))
                            {
                                x += (Sint32)(oldWidth - newWidth) / 4;
                                mnWindowFitOnScreen(entryIndex,
                                                    &x,
                                                    &y,
                                                    &newWidth,
                                                    &newHeight,
                                                    msg->spacing,
                                                    WINDOW_FIT_SIZE_GIVEN);
                            }
                            window->x      = x;
                            window->y      = y;
                            window->width  = newWidth;
                            window->height = newHeight;
                        }
                        mnWindowDrawBorder(window->x, window->y, window->width, window->height);
                    }
                    msg->x           = msg->startX;
                    msg->y           = 0;
                    msg->line        = 0;
                    msg->colour      = TEXT_COLOR_WHITE;
                    msg->outline     = TEXT_STYLE_PLAIN;
                    msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
                    mnMessageApplyStyle(msg);
                    syVRAMCacheFreeEntry(state->glyphResource);
                    state->glyphResource = NO_GLYPH_RESOURCE;
                    break;

                case TEXT_WAIT_KEY:
                    if (msg->countdown == 0)
                    {
                        msg->countdown = MessageSpeedHeldPageDelays[TEXT_SPEED];
                        if (state->mindRead != 0)
                            PerWork.pendingPush = 0;
                    }
                    if (mnTextCheckTextDismissed(msg) != 0)
                        return MESSAGE_STEP_CLOSE;
                    break;

                case TEXT_PAUSE_SHORT:
                    if (msg->countdown == 0)
                        msg->countdown = 20;
                    state->speakerVoiceCountdown = 0;
                    mnTextCheckPauseSkipped(msg);
                    break;

                case TEXT_PAUSE_LONGEST:
                    if (msg->countdown == 0)
                        msg->countdown = 120;
                    state->speakerVoiceCountdown = 0;
                    mnTextCheckPauseSkipped(msg);
                    break;

                case TEXT_PAUSE_LONG:
                    if (msg->countdown == 0)
                        msg->countdown = 60;
                    state->speakerVoiceCountdown = 0;
                    break;

                case TEXT_COLOR:
                    msg->textIndex = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    msg->colour    = state->textBuf[msg->textIndex];
                    mnMessageApplyStyle(msg);
                    break;

                case TEXT_OUTLINE:
                    msg->textIndex = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    msg->outline   = state->textBuf[msg->textIndex];
                    mnMessageApplyStyle(msg);
                    break;

                case TEXT_LINE_SPACING:
                    msg->textIndex   = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    msg->lineSpacing = state->textBuf[msg->textIndex];
                    mnMessageApplyStyle(msg);
                    break;

                case TEXT_COLORS_RESET:
                    msg->colour      = TEXT_COLOR_WHITE;
                    msg->outline     = TEXT_STYLE_PLAIN;
                    msg->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
                    mnMessageApplyStyle(msg);
                    break;

                case TEXT_WINDOW_ANIMATION:
                    msg->textIndex         = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    msg->window->animation = state->textBuf[msg->textIndex];
                    msg->countdown         = 11;
                    msg->textIndex         = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    break;

                case TEXT_END:
                case TEXT_END_ALT: msg->ended = 1; return MESSAGE_STEP_ENDED;
            }

            if (state->battleMode == WINDOW_FIELD)
                glyphsLeft = 1;
        }
        else
        {
            pixelX         = (msg->x + 128) / 256;
            pixelY         = (msg->y + 128) / 256;
            voiceGap       = MessageSpeedVoiceGaps[TEXT_SPEED];
            nextEntryIndex = msg->textIndex;
#if !MIND_READ_TEXT_NOT_INDENTED
            if (state->mindRead != 0)
                pixelX += 8;
#endif
            nextEntryIndex = (nextEntryIndex + 1) & TEXT_BUFFER_MASK;
            nextCode       = state->textBuf[nextEntryIndex];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
            if (nextCode == TEXT_MARK_VOICED)
            {
                code           |= TEXT_CODE_VOICED;
                msg->textIndex  = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
            }
            else if (nextCode == TEXT_MARK_SEMIVOICED)
            {
                code           |= TEXT_CODE_SEMIVOICED;
                msg->textIndex  = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
            }
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
#    if OUTLINED_GLYPH_PAIRS_FIT_CELL
            if (!(msg->window->flags & WINDOW_FLAG_TEXT_CANVAS) && code > TEXT_SPACE
                && nextCode > TEXT_SPACE)
            {
                Sint16 pairWidth =
                    FontGlyphs[code - TEXT_SPACE].width + FontGlyphs[nextCode - TEXT_SPACE].width;

                if (state->outline == TEXT_STYLE_OUTLINED)
                {
                    if ((Uint16)pairWidth <= 14)
                    {
                        code           |= nextCode << 8;
                        msg->textIndex  = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                    }
                }
                else if ((Uint16)pairWidth <= 15)
                {
                    code           |= nextCode << 8;
                    msg->textIndex  = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
                }
            }
#    else
            if (!(msg->window->flags & WINDOW_FLAG_TEXT_CANVAS) && code > TEXT_SPACE
                && nextCode > TEXT_SPACE
                && (Uint16)(FontGlyphs[code - TEXT_SPACE].width
                            + FontGlyphs[nextCode - TEXT_SPACE].width)
                       <= 15)
            {
                code           |= nextCode << 8;
                msg->textIndex  = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
            }
#    endif
#endif
            if (state->textMode != 0 && msg->compressLines != 0)
                pixelY = (msg->y + 128) / 320 - 3;
            glyphWidth      = mnGlyphDraw(msg->window, code, pixelX, pixelY, GLYPH_DRAW_CHARACTER);
            msg->glyphDelay = MessageSpeedDelays[TEXT_SPEED];
            if (glyphWidth == 0)
                return MESSAGE_STEP_CLOSE;
            if (state->speakerVoiceSound != 0)
            {
                if (state->speakerVoiceCountdown == 0)
                {
                    if (code != TEXT_SPACE)
                    {
                        sySoundPlayStub(state->speakerVoiceSound + (code & 3));
                        state->speakerVoiceCountdown = voiceGap;
                    }
                }
                else
                {
                    state->speakerVoiceCountdown--;
                }
            }
            glyphAdvance = glyphWidth << 8;
#if LANGUAGE == LANGUAGE_JAPANESE
            glyphAdvance += msg->spacing[msg->line];
#elif !RAGGED_LINES
            if (code == TEXT_SPACE)
                glyphAdvance += msg->spacing[msg->line];
#endif
            msg->x += glyphAdvance;
            if (code == TEXT_SPACE && state->battleMode == WINDOW_FIELD)
                glyphsLeft = 1;
        }

        if (msg->countdown != 0)
        {
            msg->countdown--;
            if (msg->countdown != 0)
                continue;
        }
        msg->textIndex = (msg->textIndex + 1) & TEXT_BUFFER_MASK;
    } while (--glyphsLeft != 0);

    return MESSAGE_STEP_RUNNING;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef TEXT_SPEED
