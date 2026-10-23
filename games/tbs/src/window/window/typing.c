/// @file
/// The per-frame message update: typing text and running its control codes.
// Part of window.c.

#include "../window_internal.h"

// The battle message canvas: 30 columns of 4 tiles. In a column each word is
// one 8-pixel row of 4bpp pixels, so a column is 32 pixel rows of one word.
#define CANVAS_COLUMNS 30
#define CANVAS_COLUMN_WORDS 32
#define CANVAS_KEPT_ROWS 8 ///< the top 8 pixel rows stay put

/// Scrolls the battle message canvas up by 6 pixel rows per step, below its
/// top 8 rows, and clears the rows this uncovers. A typing text printer calls
/// it with step 1 on each of the two frames of a line scroll
/// (Text_StartCanvasScroll).
void Text_ScrollCanvasUp(s32 step) {
  s32 shift = step * 6;
  u32 *base = TEXT_CANVAS_VRAM;
  s32 n;

  for (n = 0; n < CANVAS_COLUMNS; n++) {
    u32 *column = base + n * CANVAS_COLUMN_WORDS;

    Dma_Set(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
            DMA_COPY32(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
            REG_DMA3);
    Iwram_FillWords(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
  }
}

/// What TextPrinter_Step reports.
enum {
  CHANNEL_STEP_RUNNING = 0, ///< the text goes on next frame
  CHANNEL_STEP_ENDED = 8,   ///< the text has ended; the window stays open
  CHANNEL_STEP_CLOSE = 9,   ///< the text is dismissed: close the window
};

/// The per-frame message update: each text printer whose window is fully
/// open runs its window effect or types more text, and on the text's end marks
/// the window done (state 1) or closes it. A text printer whose window has
/// closed is freed here. WindowSystem_Update runs it each frame after the
/// windows' zooms.
void TextPrinter_RunAll(void) {
  struct TextPrinter *printer = WINDOW_WORK->printers;
  s32 printerIndex = 0;

  do {
    struct Window *current = printer->window;

    // A zooming window is left alone until its zoom ends.
    if (current != NULL && current->frame == 0 && current->duration == 0) {
      u16 flags = current->flags;

      // Flags 0 with no zoom left: the window has closed.
      if (flags == 0) {
        printer->window = NULL;
      } else {
        s32 pending = current->effect;
        s32 step;

        if (pending != 0) {
          Window_StepSwellEffect(printer);
        } else {
          step = TextPrinter_Step(printer);
          switch (step) {
          case CHANNEL_STEP_ENDED:
            printer->window->state = 1;
            break;
          case CHANNEL_STEP_CLOSE: {
            struct Window *window = printer->window;

            Window_Close(window, window->flags & WINDOW_FLAG_INSTANT);
            // pending is 0 here: it clears the printer.
            printer->x = pending;
            printer->y = pending;
            printer->textIndex = pending;
            printer->countdown = pending;
            printer->colour = pending;
            printer->outline = pending;
            printer->lineSpacing = pending;
            printer->window->state = 1;
            break;
          }
          }
        }
      }
    }
    printerIndex++;
    printer++;
  } while (printerIndex != 3);
}

/// Types one frame's share of a text printer's text: runs control codes (new
/// line, new page, pauses, colours) and draws characters, playing the speaker's
/// voice blips. Returns a CHANNEL_STEP_ result. The share is the message
/// speed's glyphs a frame (gMessageSpeed_Steps) outside battle, where a space
/// or a control code also ends it; in battle it is 8 glyphs, or 13 after a
/// frame that took two VBlanks or more.
s32 TextPrinter_Step(struct TextPrinter *printer) {
  struct WindowWork *work = WINDOW_WORK;
  // The ROM reads gKeysHeld here once and never uses the value.
  u32 keys = gKeysHeld;
  s32 glyphsLeft = gMessageSpeed_Steps[gSaveBlock.messageSpeed];
  s32 lag;
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
  // last frame took (gLagFramesShown, 1 for a frame on time), counting at most
  // two.
  if (work->battleMode != 0) {
    lag = gLagFramesShown;
    CLAMP(lag, 0, 2);
    glyphsLeft = lag * 5 + 3;
  }

  // Finish scrolling the canvas up a line before any more text.
  if (printer->transition != 0) {
    Text_ScrollCanvasUp(1);
    printer->transition = (s16)printer->transition - 1;
    return CHANNEL_STEP_RUNNING;
  }
  // With no button held, wait out the delay after the last glyph.
  if (gKeysHeld == 0 && printer->glyphDelay != 0) {
    printer->glyphDelay--;
    return CHANNEL_STEP_RUNNING;
  }

  // Run control codes and draw characters until this frame's share is done.
  do {
    code = TEXT_END;
    if (printer->ended == 0)
      code = work->textBuffer[printer->textIndex];

    if (code < TEXT_COMMANDS) {
      switch (code) {
      case TEXT_NEWLINE:
        // Canvas lines are 13 pixels apart, and from the second line on the
        // canvas scrolls up instead; other windows' lines are 15 apart.
        printer->x = printer->startX;
        if (printer->window->flags & WINDOW_FLAG_TEXT_CANVAS) {
          if (printer->y >= 13 << 8) {
            Text_StartCanvasScroll(printer);
            glyphsLeft = 1;
          } else {
            printer->y += 13 << 8;
          }
        } else {
          printer->y += 15 << 8;
          if (printer->line <= 2)
            printer->line++;
        }
        break;

      case TEXT_PAGE:
        // The countdown only holds the printer on this code while it waits: it
        // is set to 919 each frame and counted down once, so it is below 900
        // only on the wait's first frame. Then, under Mind Read, presses
        // latched before the wait are dropped.
        if (work->mindRead != 0 && printer->countdown < 900)
          gKeysPressedLatch = 0;
        printer->countdown = 919;
        if (Text_CheckTextDismissed(printer) == 0) {
          // Until the button: show the next-page arrow in the bottom-right
          // corner. Outside Japanese, a window with no size (as
          // FieldMessage_ShowCentered opens with CENTERED_MESSAGE_BARE) shows
          // none.
          struct Window *side = printer->window;

#if TBS_PATCH_1
          if (side->width == 0 || side->height == 0 ||
              work->pageArrowShown != 0)
            break;
#else
          if (work->pageArrowShown != 0)
            break;
#endif
          Glyph_Draw(side, 1, side->width * 4 - 8, side->height * 8 - 16,
                     GLYPH_DRAW_PAGE_ARROW);
          work->pageArrowShown = 1;
          break;
        }
        // Clear the window, and refit it to the next page's text. A text
        // printer started with flags set keeps the window's size; the frame is
        // drawn again only when text follows the break.
        window = printer->window;
        x = window->x;
        y = window->y;
        oldWidth = window->width;
        oldHeight = window->height;
        entryIndex = printer->textIndex;
        work->pageArrowShown = 0;
        WindowSprite_PrepareForRedraw(window);
        if (printer->flags == 0 && (window->width | window->height) != 0)
          Window_EraseBorderRect(window->x, window->y, window->width,
                                 window->height);
        entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
        if (work->textBuffer[entryIndex] != 0 &&
            (window->width | window->height) != 0) {
          if (printer->flags != 0) {
            Window_EraseBorderRect(window->x, window->y, window->width,
                                   window->height);
          } else {
            Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                               printer->spacing, 0);
            if (window->flags & WINDOW_FLAG_ANCHOR_BOTTOM) {
              if (oldHeight != newHeight)
                y -= newHeight - oldHeight;
              if (y < 0)
                y = 0;
            }
            if (!(window->flags & WINDOW_FLAG_FIXED_X)) {
              // Shift the window by a quarter of the change in width (tiles),
              // part way to keeping its centre, which a half would keep; then
              // fit it on the screen again at its new size (flags 2).
              x += (s32)(oldWidth - newWidth) / 4;
              Window_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                 printer->spacing, 2);
            }
            window->x = x;
            window->y = y;
            window->width = newWidth;
            window->height = newHeight;
          }
          Window_DrawFrame(window->x, window->y, window->width, window->height);
        }
        printer->x = printer->startX;
        printer->y = 0;
        printer->line = 0;
        // The French, Spanish and Italian releases also start each page in the
        // default style.
#if TBS_PATCH_3
        printer->colour = 15;
        printer->outline = TEXT_STYLE_PLAIN;
        printer->lineSpacing = 10;
        TextPrinter_ApplyStyle(printer);
#endif
        VramCache_FreeEntry(work->glyphResource);
        work->glyphResource = NO_GLYPH_RESOURCE;
        break;

      case TEXT_WAIT_KEY:
        // The same hold as a page break's.
        if (work->mindRead != 0 && printer->countdown < 900)
          gKeysPressedLatch = 0;
        if (Text_CheckTextDismissed(printer) != 0)
          return CHANNEL_STEP_CLOSE;
        printer->countdown = 919;
        break;

      case TEXT_PAUSE_SHORT:
        if (printer->countdown == 0)
          printer->countdown = 20;
        work->voiceCountdown = 0;
        Text_CheckPauseSkipped(printer);
        break;

      case TEXT_PAUSE_LONGEST:
        if (printer->countdown == 0)
          printer->countdown = 120;
        work->voiceCountdown = 0;
        Text_CheckPauseSkipped(printer);
        break;

      case TEXT_PAUSE_LONG:
        if (printer->countdown == 0)
          printer->countdown = 60;
        work->voiceCountdown = 0;
        break;

      case TEXT_COLOR:
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        printer->colour = work->textBuffer[printer->textIndex];
        TextPrinter_ApplyStyle(printer);
        break;

      case TEXT_OUTLINE:
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        printer->outline = work->textBuffer[printer->textIndex];
        TextPrinter_ApplyStyle(printer);
        break;

      case TEXT_LINE_SPACING:
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        printer->lineSpacing = work->textBuffer[printer->textIndex];
        TextPrinter_ApplyStyle(printer);
        break;

      case TEXT_COLORS_RESET:
        printer->colour = 15;
        printer->outline = TEXT_STYLE_PLAIN;
        printer->lineSpacing = 10;
        TextPrinter_ApplyStyle(printer);
        break;

      case TEXT_WINDOW_STATE:
        // The effect lasts while the countdown of 10 runs out
        // (Window_StepSwellEffect counts it down); typing then resumes after
        // the operand.
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        printer->window->effect = work->textBuffer[printer->textIndex];
        printer->countdown = 10;
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        break;

      case TEXT_END:
      case TEXT_END_ALT:
        printer->ended = 1;
        return CHANNEL_STEP_ENDED;
      }

      // Outside battle a control code ends this frame's typing.
      if (work->battleMode == 0)
        glyphsLeft = 1;
    } else {
      // A character: draw it at the pen position, rounded to whole pixels.
      pixelX = (printer->x + 128) / 256;
      pixelY = (printer->y + 128) / 256;
      voiceGap = gMessageSpeed_VoiceGaps[gSaveBlock.messageSpeed];
      nextEntryIndex = printer->textIndex;
      // Under Mind Read the text starts 8 pixels further in (Japanese and
      // English), in the window Window_FitOnScreen made 2 tiles wider.
#if !TBS_PATCH_2
      if (work->mindRead != 0)
        pixelX += 8;
#endif
      nextEntryIndex = (nextEntryIndex + 1) & TEXT_BUFFER_MASK;
      nextCode = work->textBuffer[nextEntryIndex];
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
      // A kana mark after the character is drawn with it.
      if (nextCode == TEXT_VOICED_MARK) {
        code |= TEXT_CODE_VOICED;
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
      } else if (nextCode == TEXT_SEMIVOICED_MARK) {
        code |= TEXT_CODE_SEMIVOICED;
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
      }
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
      // Two narrow characters that fit in one 16-pixel cell together are drawn
      // as one pair, the second in the code's high byte: 15 pixels of glyph
      // widths at most, and in Spanish 14 with an outline (style 1), which
      // widens each glyph by one.
#if (LANGUAGE == LANGUAGE_SPANISH)
      if (!(printer->window->flags & WINDOW_FLAG_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE) {
        s16 pair = gFont_Glyphs[code - TEXT_SPACE].width +
                   gFont_Glyphs[nextCode - TEXT_SPACE].width;

        if (work->outline == TEXT_STYLE_OUTLINED) {
          if ((u16)pair <= 14) {
            code |= nextCode << 8;
            printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
          }
        } else if ((u16)pair <= 15) {
          code |= nextCode << 8;
          printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
        }
      }
#else
      if (!(printer->window->flags & WINDOW_FLAG_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE &&
          (u16)(gFont_Glyphs[code - TEXT_SPACE].width +
                gFont_Glyphs[nextCode - TEXT_SPACE].width) <= 15) {
        code |= nextCode << 8;
        printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
      }
#endif
#endif
      glyphWidth = Glyph_Draw(printer->window, code, pixelX, pixelY,
                              GLYPH_DRAW_CHARACTER);
      printer->glyphDelay = gMessageSpeed_Delays[gSaveBlock.messageSpeed];
      if (glyphWidth != 0) {
        // The speaker's voice blip (voiceSound, the first of four
        // sounds, one picked by the code's low two bits) every few characters
        // (gMessageSpeed_VoiceGaps), and the pen moves on by the glyph's width.
        if (work->voiceSound != 0) {
          if (work->voiceCountdown == 0) {
            if (code != TEXT_SPACE) {
              Sound_PlaySongFar(work->voiceSound + (code & 3));
              work->voiceCountdown = voiceGap;
            }
          } else {
            work->voiceCountdown--;
          }
        }
        glyphAdvance = glyphWidth << 8;
#if (LANGUAGE != LANGUAGE_JAPANESE)
        if (code == TEXT_SPACE)
          glyphAdvance += printer->spacing[printer->line];
#else
        glyphAdvance += printer->spacing[printer->line];
#endif
        printer->x += glyphAdvance;
      }
      // In Japanese a character that advances nothing (Glyph_Draw
      // returns 0, as it does when no window sprite is free) closes the
      // window.
#if (LANGUAGE == LANGUAGE_JAPANESE)
      else
        return CHANNEL_STEP_CLOSE;
#endif
      // Outside battle a space ends this frame's typing.
      if (code == TEXT_SPACE && work->battleMode == 0)
        glyphsLeft = 1;
    }

    // A pause holds the printer on this entry until its countdown ends.
    if (printer->countdown != 0) {
      printer->countdown--;
      if (printer->countdown != 0)
        continue;
    }
    printer->textIndex = (printer->textIndex + 1) & TEXT_BUFFER_MASK;
  } while (--glyphsLeft != 0);

  return CHANNEL_STEP_RUNNING;
}
