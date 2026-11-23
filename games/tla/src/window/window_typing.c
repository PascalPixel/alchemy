/// @file
/// The per-frame message update: typing text and running its control codes.
// Part of window.c.

#include "window_internal.h"

/// The text canvas: 30 columns of tiles, 32 words to a column.
#define CANVAS_COLUMNS 30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS 8 ///< the top pixel rows, which stay put

/// What Message_Type reports.
enum {
  CHANNEL_STEP_RUNNING = 0, ///< the text goes on next frame
  CHANNEL_STEP_ENDED = 8,   ///< the text has ended; the window stays open
  CHANNEL_STEP_CLOSE = 9,   ///< the text is dismissed: close the window
};

/// Scrolls the message canvas up by 6 pixel rows per step, below its top
/// CANVAS_KEPT_ROWS, and clears the rows this uncovers. A typing message calls
/// it with step 1 on each frame of a line scroll.
void Text_ScrollCanvasUp(s32 step) {
  s32 shift = step * 6;
  u32 *base = TEXT_CANVAS_VRAM;
  s32 n;

  for (n = 0; n < CANVAS_COLUMNS; n++) {
    u32 *column = base + n * CANVAS_COLUMN_WORDS;

    Dma_Set(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
            DMA_COPY_WORDS(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
            REG_DMA3);
    Iwram_FillWords(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
  }
}

s32 Message_Type(struct Message *channel);
struct Message;
void Window_StepSwellEffect(struct Message *window);

/// The per-frame message update: each message whose window is fully open runs
/// its window animation or types more text, and on the text's end marks the
/// window done (state 1) or closes it.
void Message_UpdateAll(void) {
  struct Message *channel = WINDOW_WORK->messages;
  s32 channelNo = 0;

  do {
    struct Window *current = channel->window;

    // A zooming window is left alone until its zoom ends.
    if (current != NULL && *(s32 *)&current->zoomStage == 0) {
      u16 flags = current->flags;

      // Flags 0 with no zoom left: the window has closed.
      if (flags == 0) {
        channel->window = NULL;
      } else {
        s32 pending = current->animation;
        s32 step;

        if (pending != 0) {
          Window_StepSwellEffect(channel);
        } else {
          step = Message_Type(channel);
          switch (step) {
          case CHANNEL_STEP_ENDED:
            channel->window->state = 1;
            break;
          case CHANNEL_STEP_CLOSE: {
            struct Window *window = channel->window;

            Window_Close(window, (u16)(window->flags & WINDOW_FLAG_INSTANT));
            // pending is 0 here: it clears the channel.
            channel->x = pending;
            channel->y = pending;
            channel->entry = pending;
            channel->countdown = pending;
            channel->colour = pending;
            channel->outline = pending;
            channel->lineSpacing = pending;
            channel->window->state = 1;
            break;
          }
          }
        }
      }
    }
    channelNo++;
    channel++;
  } while (channelNo != 3);
}

void Text_ScrollCanvasUp(s32 step);
void Text_StartCanvasScroll(struct Message *channel);
void Message_ApplyStyle(struct Message *channel);
s32 Text_CheckPauseSkipped(struct Message *channel);
s32 Text_CheckTextDismissed(struct Message *channel);
void WindowSprite_PrepareForRedraw(struct Window *window);

#define TEXT_SPEED gMainRoutineWork.messageSpeed

/// Types one frame's share of a message's text: runs control codes (new
/// line, new page, pauses, colours) and draws characters, playing the speaker's
/// voice blips. Returns a CHANNEL_STEP_ result. The share is the message
/// speed's glyphs a frame (gMessageSpeed_Steps) outside battle, where a space
/// or a control code also ends it; in battle it is 8 glyphs, 13 after a frame
/// that took two VBlanks and 18 after one that took three or more.
s32 Message_Type(struct Message *channel) {
  struct WindowWork *work = WINDOW_WORK;
  // DEAD: the held keys are read here and never used; the ROM keeps the load.
  u32 keys = gInput.held;
  s32 glyphsLeft = gMessageSpeed_Steps[TEXT_SPEED];
  s32 frameVBlanks;
  u32 code;
  u32 nextCode;
  struct Window *window;
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

  // In battle, text runs at 3 glyphs a frame and 5 more for each VBlank the
  // last frame took (gLastFrameVBlanks, 1 for a frame on time), counting at
  // most three. The count is read as the interrupt may change it.
  if (work->battleMode != WINDOW_FIELD) {
    frameVBlanks = *(volatile u16 *)&gLastFrameVBlanks;
    CLAMP(frameVBlanks, 0, 3);
    glyphsLeft = frameVBlanks * 5 + 3;
  }

  if (channel->transition != 0) {
    Text_ScrollCanvasUp(1);
    channel->transition = (s16)channel->transition - 1;
    return CHANNEL_STEP_RUNNING;
  }
  if (gInput.held == 0 && channel->glyphDelay != 0) {
    channel->glyphDelay--;
    return CHANNEL_STEP_RUNNING;
  }

  do {
    code = TEXT_END;
    if (channel->ended == 0)
      code = work->textBuffer[channel->entry];

#if LANGUAGE != LANGUAGE_JAPANESE
    if (code < TEXT_COMMANDS || code == TEXT_EM_DASH) {
#else
    if (code < TEXT_COMMANDS) {
#endif
      switch (code) {
      case TEXT_NEWLINE:
        channel->x = channel->startX;
        if (channel->window->flags & WINDOW_FLAG_TEXT_CANVAS) {
          if (channel->y >= 13 << 8) {
            Text_StartCanvasScroll(channel);
            glyphsLeft = 1;
          } else {
            channel->y += 13 << 8;
          }
        } else {
          channel->y += 15 << 8;
          if (channel->line <= 2)
            channel->line++;
        }
        break;

      case TEXT_PAGE:
        if (channel->countdown == 0) {
          channel->countdown = gMessageSpeed_HeldPageDelays[TEXT_SPEED];
          if (work->mindRead != 0)
            gInput.pendingPressed = 0;
        }
        if (Text_CheckTextDismissed(channel) == 0) {
          struct Window *pane = channel->window;

          if (pane->width == 0 || pane->height == 0 ||
              work->pageArrowShown != 0)
            break;
          Glyph_Draw(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16,
                     GLYPH_DRAW_PAGE_ARROW);
          work->pageArrowShown = 1;
          break;
        }
        window = channel->window;
        entryIndex = channel->entry;
        x = (s16)window->x;
        y = (s16)window->y;
        oldWidth = window->width;
        oldHeight = window->height;
        work->pageArrowShown = 0;
        WindowSprite_PrepareForRedraw(window);
        if (channel->flags == 0 && (window->width | window->height) != 0)
          Window_EraseBorderRect((s16)window->x, (s16)window->y, window->width,
                                 window->height);
        entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
        if (work->textBuffer[entryIndex] != 0 &&
            (window->width | window->height) != 0) {
          if (channel->flags != 0) {
            Window_EraseBorderRect((s16)window->x, (s16)window->y,
                                   window->width, window->height);
          } else {
            Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                               channel->spacing, 0);
            if (window->flags & WINDOW_FLAG_ANCHOR_BOTTOM) {
              if (oldHeight != newHeight)
                y -= newHeight - oldHeight;
              if (y < 0)
                y = 0;
            }
            if (!(window->flags & WINDOW_FLAG_FIXED_X)) {
              x += (s32)(oldWidth - newWidth) / 4;
              Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                 channel->spacing, 2);
            }
            window->x = x;
            window->y = y;
            window->width = newWidth;
            window->height = newHeight;
          }
          Window_DrawBorder((s16)window->x, (s16)window->y, window->width,
                            window->height);
        }
        channel->x = channel->startX;
        channel->y = 0;
        channel->line = 0;
        channel->colour = TEXT_COLOR_WHITE;
        channel->outline = TEXT_STYLE_PLAIN;
        channel->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
        Message_ApplyStyle(channel);
        VramCache_FreeEntry(work->glyphResource);
        work->glyphResource = NO_GLYPH_RESOURCE;
        break;

      case TEXT_WAIT_KEY:
        if (channel->countdown == 0) {
          channel->countdown = gMessageSpeed_HeldPageDelays[TEXT_SPEED];
          if (work->mindRead != 0)
            gInput.pendingPressed = 0;
        }
        if (Text_CheckTextDismissed(channel) != 0)
          return CHANNEL_STEP_CLOSE;
        break;

      case TEXT_PAUSE_SHORT:
        if (channel->countdown == 0)
          channel->countdown = 20;
        work->speakerVoiceCountdown = 0;
        Text_CheckPauseSkipped(channel);
        break;

      case TEXT_PAUSE_LONGEST:
        if (channel->countdown == 0)
          channel->countdown = 120;
        work->speakerVoiceCountdown = 0;
        Text_CheckPauseSkipped(channel);
        break;

      case TEXT_PAUSE_LONG:
        if (channel->countdown == 0)
          channel->countdown = 60;
        work->speakerVoiceCountdown = 0;
        break;

      case TEXT_COLOR:
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        channel->colour = work->textBuffer[channel->entry];
        Message_ApplyStyle(channel);
        break;

      case TEXT_OUTLINE:
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        channel->outline = work->textBuffer[channel->entry];
        Message_ApplyStyle(channel);
        break;

      case TEXT_LINE_SPACING:
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        channel->lineSpacing = work->textBuffer[channel->entry];
        Message_ApplyStyle(channel);
        break;

      case TEXT_COLORS_RESET:
        channel->colour = TEXT_COLOR_WHITE;
        channel->outline = TEXT_STYLE_PLAIN;
        channel->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
        Message_ApplyStyle(channel);
        break;

      case TEXT_WINDOW_STATE:
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        channel->window->animation = work->textBuffer[channel->entry];
        channel->countdown = 11;
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        break;

      case TEXT_END:
      case TEXT_END_ALT:
        channel->ended = 1;
        return CHANNEL_STEP_ENDED;
      }

      if (work->battleMode == WINDOW_FIELD)
        glyphsLeft = 1;
    } else {
      pixelX = (channel->x + 128) / 256;
      pixelY = (channel->y + 128) / 256;
      voiceGap = gMessageSpeed_VoiceGaps[TEXT_SPEED];
      nextEntryIndex = channel->entry;
#if !TLA_PATCH_2
      if (work->mindRead != 0)
        pixelX += 8;
#endif
      nextEntryIndex = (nextEntryIndex + 1) & TEXT_BUFFER_MASK;
      nextCode = work->textBuffer[nextEntryIndex];
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
      if (nextCode == TEXT_MARK_VOICED) {
        code |= TEXT_CODE_VOICED;
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
      } else if (nextCode == TEXT_MARK_SEMIVOICED) {
        code |= TEXT_CODE_SEMIVOICED;
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
      }
#endif
#if LANGUAGE != LANGUAGE_JAPANESE
#if TLA_PATCH_4
      if (!(channel->window->flags & WINDOW_FLAG_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE) {
        s16 pairWidth = gFont_Glyphs[code - TEXT_SPACE].width +
                        gFont_Glyphs[nextCode - TEXT_SPACE].width;

        if (work->outline == TEXT_STYLE_OUTLINED) {
          if ((u16)pairWidth <= 14) {
            code |= nextCode << 8;
            channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
          }
        } else if ((u16)pairWidth <= 15) {
          code |= nextCode << 8;
          channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
        }
      }
#else
      if (!(channel->window->flags & WINDOW_FLAG_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE &&
          (u16)(gFont_Glyphs[code - TEXT_SPACE].width +
                gFont_Glyphs[nextCode - TEXT_SPACE].width) <= 15) {
        code |= nextCode << 8;
        channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
      }
#endif
#endif
      if (work->textMode != 0 && channel->compressLines != 0)
        pixelY = (channel->y + 128) / 320 - 3;
      glyphWidth = Glyph_Draw(channel->window, code, pixelX, pixelY,
                              GLYPH_DRAW_CHARACTER);
      channel->glyphDelay = gMessageSpeed_Delays[TEXT_SPEED];
      if (glyphWidth == 0)
        return CHANNEL_STEP_CLOSE;
      if (work->speakerVoiceSound != 0) {
        if (work->speakerVoiceCountdown == 0) {
          if (code != TEXT_SPACE) {
            Sound_PlayFar(work->speakerVoiceSound + (code & 3));
            work->speakerVoiceCountdown = voiceGap;
          }
        } else {
          work->speakerVoiceCountdown--;
        }
      }
      glyphAdvance = glyphWidth << 8;
#if LANGUAGE == LANGUAGE_JAPANESE
      glyphAdvance += channel->spacing[channel->line];
#elif LANGUAGE != LANGUAGE_GERMAN
      if (code == TEXT_SPACE)
        glyphAdvance += channel->spacing[channel->line];
#endif
      channel->x += glyphAdvance;
      if (code == TEXT_SPACE && work->battleMode == WINDOW_FIELD)
        glyphsLeft = 1;
    }

    if (channel->countdown != 0) {
      channel->countdown--;
      if (channel->countdown != 0)
        continue;
    }
    channel->entry = (channel->entry + 1) & TEXT_BUFFER_MASK;
  } while (--glyphsLeft != 0);

  return CHANNEL_STEP_RUNNING;
}

#undef CANVAS_COLUMNS
#undef CANVAS_COLUMN_WORDS
#undef CANVAS_KEPT_ROWS
#undef TEXT_SPEED
