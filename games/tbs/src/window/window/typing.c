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
/// top 8 rows, and clears the rows this uncovers. A typing channel calls it
/// with step 1 on each of the two frames of a line scroll
/// (UiText_StartCanvasScroll).
void UiText_ScrollCanvasUp(s32 step) {
  s32 shift = step * 6;
  u32 *base = UI_CANVAS_VRAM;
  s32 n;

  for (n = 0; n < CANVAS_COLUMNS; n++) {
    u32 *column = base + n * CANVAS_COLUMN_WORDS;

    Dma_Set(column + CANVAS_KEPT_ROWS + shift, column + CANVAS_KEPT_ROWS,
            DMA_COPY32(CANVAS_COLUMN_WORDS - CANVAS_KEPT_ROWS - shift),
            REG_DMA3);
    Iwram_FillWords(column + (CANVAS_COLUMN_WORDS - shift), shift * 4, 0);
  }
}

/// What UiWork_StepChannelScript reports.
enum {
  CHANNEL_STEP_RUNNING = 0, ///< the text goes on next frame
  CHANNEL_STEP_ENDED = 8,   ///< the text has ended; the window stays open
  CHANNEL_STEP_CLOSE = 9,   ///< the text is dismissed: close the window
};

/// The per-frame message update: each message channel whose window is fully
/// open runs its window effect or types more text, and on the text's end marks
/// the window done (state 1) or closes it. A channel whose window has closed
/// is freed here. UiWork_ProcessAll runs it each frame after the windows'
/// zooms.
void UiWork_ProcessRenderChannels(void) {
  struct UiChannelSlot *channel = UI_WORK->channels;
  s32 channelNo = 0;

  do {
    struct UiWindow *current = channel->work;

    // A zooming window is left alone until its zoom ends.
    if (current != NULL && current->frame == 0 && current->duration == 0) {
      u16 flags = current->flags;

      // Flags 0 with no zoom left: the window has closed.
      if (flags == 0) {
        channel->work = NULL;
      } else {
        s32 pending = current->effect;
        s32 step;

        if (pending != 0) {
          UiWindow_StepSwellEffect(channel);
        } else {
          step = UiWork_StepChannelScript(channel);
          switch (step) {
          case CHANNEL_STEP_ENDED:
            channel->work->state = 1;
            break;
          case CHANNEL_STEP_CLOSE: {
            struct UiWindow *window = channel->work;

            UiWork_Finalize(window, window->flags & UI_WINDOW_INSTANT);
            // pending is 0 here: it clears the channel.
            channel->x = pending;
            channel->y = pending;
            channel->entry = pending;
            channel->countdown = pending;
            channel->colour = pending;
            channel->outline = pending;
            channel->lineSpacing = pending;
            channel->work->state = 1;
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

/// Types one frame's share of a channel's text: runs control codes (new line,
/// new page, pauses, colours) and draws characters, playing the speaker's voice
/// blips. Returns a CHANNEL_STEP_ result. The share is the message speed's
/// glyphs a frame (gUiText_SpeedSteps) outside battle, where a space or a
/// control code also ends it; in battle it is 8 glyphs, or 13 after a frame
/// that took two VBlanks or more.
s32 UiWork_StepChannelScript(struct UiChannelSlot *channel) {
  struct UiRenderWork *work = UI_WORK;
  // The ROM reads gKeysHeld here once and never uses the value.
  u32 keys = gKeysHeld;
  s32 glyphsLeft = gUiText_SpeedSteps[gGameState.messageSpeed];
  s32 lag;
  u32 code;
  u32 nextCode;
  struct UiWindow *window;
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
  if (work->menuState != 0) {
    lag = gLagFramesShown;
    CLAMP(lag, 0, 2);
    glyphsLeft = lag * 5 + 3;
  }

  // Finish scrolling the canvas up a line before any more text.
  if (channel->transition != 0) {
    UiText_ScrollCanvasUp(1);
    channel->transition = (s16)channel->transition - 1;
    return CHANNEL_STEP_RUNNING;
  }
  // With no button held, wait out the delay after the last glyph.
  if (gKeysHeld == 0 && channel->glyphDelay != 0) {
    channel->glyphDelay--;
    return CHANNEL_STEP_RUNNING;
  }

  // Run control codes and draw characters until this frame's share is done.
  do {
    code = TEXT_END;
    if (channel->ended == 0)
      code = work->entries[channel->entry];

    if (code < TEXT_COMMANDS) {
      switch (code) {
      case TEXT_NEWLINE:
        // Canvas lines are 13 pixels apart, and from the second line on the
        // canvas scrolls up instead; other windows' lines are 15 apart.
        channel->x = channel->startX;
        if (channel->work->flags & UI_WINDOW_TEXT_CANVAS) {
          if (channel->y >= 13 << 8) {
            UiText_StartCanvasScroll(channel);
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
        // The countdown only holds the channel on this code while it waits: it
        // is set to 919 each frame and counted down once, so it is below 900
        // only on the wait's first frame. Then, under Mind Read, presses
        // latched before the wait are dropped.
        if (work->mode != 0 && channel->countdown < 900)
          gKeysPressedLatch = 0;
        channel->countdown = 919;
        if (UiText_CheckTextDismissed(channel) == 0) {
          // Until the button: show the next-page arrow in the bottom-right
          // corner. Outside Japanese, a window with no size (as
          // UiText_ShowCenteredMessage opens with CENTERED_MESSAGE_BARE) shows
          // none.
          struct UiWindow *pane = channel->work;

#if TBS_PATCH_1
          if (pane->width == 0 || pane->height == 0 ||
              work->pageArrowShown != 0)
            break;
#else
          if (work->pageArrowShown != 0)
            break;
#endif
          UiText_DrawGlyph(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16,
                           GLYPH_DRAW_PAGE_ARROW);
          work->pageArrowShown = 1;
          break;
        }
        // Clear the window, and refit it to the next page's text. A channel
        // started with flags set keeps the window's size; the frame is drawn
        // again only when text follows the break.
        window = channel->work;
        x = window->x;
        y = window->y;
        oldWidth = window->width;
        oldHeight = window->height;
        entryIndex = channel->entry;
        work->pageArrowShown = 0;
        RenderOutput_PrepareForRedraw(window);
        if (channel->flags == 0 && (window->width | window->height) != 0)
          UiWindow_EraseBorderRect(window->x, window->y, window->width,
                                   window->height);
        entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
        if (work->entries[entryIndex] != 0 &&
            (window->width | window->height) != 0) {
          if (channel->flags != 0) {
            UiWindow_EraseBorderRect(window->x, window->y, window->width,
                                     window->height);
          } else {
            UiWindow_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                 channel->spacing, 0);
            if (window->flags & UI_WINDOW_ANCHOR_BOTTOM) {
              if (oldHeight != newHeight)
                y -= newHeight - oldHeight;
              if (y < 0)
                y = 0;
            }
            if (!(window->flags & UI_WINDOW_FIXED_X)) {
              // Shift the window by a quarter of the change in width (tiles),
              // part way to keeping its centre, which a half would keep; then
              // fit it on the screen again at its new size (flags 2).
              x += (s32)(oldWidth - newWidth) / 4;
              UiWindow_FitOnScreen(entryIndex, &x, &y, &newWidth, &newHeight,
                                   channel->spacing, 2);
            }
            window->x = x;
            window->y = y;
            window->width = newWidth;
            window->height = newHeight;
          }
          UiWindow_DrawFrame(window->x, window->y, window->width,
                             window->height);
        }
        channel->x = channel->startX;
        channel->y = 0;
        channel->line = 0;
        // The French, Spanish and Italian releases also start each page in the
        // default style.
#if TBS_PATCH_3
        channel->colour = 15;
        channel->outline = TEXT_STYLE_PLAIN;
        channel->lineSpacing = 10;
        UiText_ApplyChannelStyle(channel);
#endif
        Resource_ResetEntry(work->glyphResource);
        work->glyphResource = NO_GLYPH_RESOURCE;
        break;

      case TEXT_WAIT_KEY:
        // The same hold as a page break's.
        if (work->mode != 0 && channel->countdown < 900)
          gKeysPressedLatch = 0;
        if (UiText_CheckTextDismissed(channel) != 0)
          return CHANNEL_STEP_CLOSE;
        channel->countdown = 919;
        break;

      case TEXT_PAUSE_SHORT:
        if (channel->countdown == 0)
          channel->countdown = 20;
        work->speakerVoiceCountdown = 0;
        UiText_CheckPauseSkipped(channel);
        break;

      case TEXT_PAUSE_LONGEST:
        if (channel->countdown == 0)
          channel->countdown = 120;
        work->speakerVoiceCountdown = 0;
        UiText_CheckPauseSkipped(channel);
        break;

      case TEXT_PAUSE_LONG:
        if (channel->countdown == 0)
          channel->countdown = 60;
        work->speakerVoiceCountdown = 0;
        break;

      case TEXT_COLOUR:
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        channel->colour = work->entries[channel->entry];
        UiText_ApplyChannelStyle(channel);
        break;

      case TEXT_OUTLINE:
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        channel->outline = work->entries[channel->entry];
        UiText_ApplyChannelStyle(channel);
        break;

      case TEXT_LINE_SPACING:
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        channel->lineSpacing = work->entries[channel->entry];
        UiText_ApplyChannelStyle(channel);
        break;

      case TEXT_COLOURS_RESET:
        channel->colour = 15;
        channel->outline = TEXT_STYLE_PLAIN;
        channel->lineSpacing = 10;
        UiText_ApplyChannelStyle(channel);
        break;

      case TEXT_WINDOW_STATE:
        // The effect lasts while the countdown of 10 runs out
        // (UiWindow_StepSwellEffect counts it down); typing then resumes after
        // the operand.
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        channel->work->effect = work->entries[channel->entry];
        channel->countdown = 10;
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        break;

      case TEXT_END:
      case TEXT_END_ALT:
        channel->ended = 1;
        return CHANNEL_STEP_ENDED;
      }

      // Outside battle a control code ends this frame's typing.
      if (work->menuState == 0)
        glyphsLeft = 1;
    } else {
      // A character: draw it at the pen position, rounded to whole pixels.
      pixelX = (channel->x + 128) / 256;
      pixelY = (channel->y + 128) / 256;
      voiceGap = gUiText_SpeedCueGaps[gGameState.messageSpeed];
      nextEntryIndex = channel->entry;
      // Under Mind Read the text starts 8 pixels further in (Japanese and
      // English), in the window UiWindow_FitOnScreen made 2 tiles wider.
#if !TBS_PATCH_2
      if (work->mode != 0)
        pixelX += 8;
#endif
      nextEntryIndex = (nextEntryIndex + 1) & RENDER_ENTRY_MASK;
      nextCode = work->entries[nextEntryIndex];
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
      // A kana mark after the character is drawn with it.
      if (nextCode == TEXT_VOICED_MARK) {
        code |= TEXT_CODE_VOICED;
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
      } else if (nextCode == TEXT_SEMIVOICED_MARK) {
        code |= TEXT_CODE_SEMIVOICED;
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
      }
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
      // Two narrow characters that fit in one 16-pixel cell together are drawn
      // as one pair, the second in the code's high byte: 15 pixels of glyph
      // widths at most, and in Spanish 14 with an outline (style 1), which
      // widens each glyph by one.
#if (LANGUAGE == LANGUAGE_SPANISH)
      if (!(channel->work->flags & UI_WINDOW_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE) {
        s16 pair = gUiText_Glyphs[code - TEXT_SPACE].width +
                   gUiText_Glyphs[nextCode - TEXT_SPACE].width;

        if (work->outline == TEXT_STYLE_OUTLINED) {
          if ((u16)pair <= 14) {
            code |= nextCode << 8;
            channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
          }
        } else if ((u16)pair <= 15) {
          code |= nextCode << 8;
          channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
        }
      }
#else
      if (!(channel->work->flags & UI_WINDOW_TEXT_CANVAS) &&
          code > TEXT_SPACE && nextCode > TEXT_SPACE &&
          (u16)(gUiText_Glyphs[code - TEXT_SPACE].width +
                gUiText_Glyphs[nextCode - TEXT_SPACE].width) <= 15) {
        code |= nextCode << 8;
        channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
      }
#endif
#endif
      glyphWidth = UiText_DrawGlyph(channel->work, code, pixelX, pixelY,
                                    GLYPH_DRAW_CHARACTER);
      channel->glyphDelay = gUiText_SpeedDelays[gGameState.messageSpeed];
      if (glyphWidth != 0) {
        // The speaker's voice blip (speakerVoiceSound, the first of four
        // sounds, one picked by the code's low two bits) every few characters
        // (gUiText_SpeedCueGaps), and the pen moves on by the glyph's width.
        if (work->speakerVoiceSound != 0) {
          if (work->speakerVoiceCountdown == 0) {
            if (code != TEXT_SPACE) {
              Sound_PlaySongFar(work->speakerVoiceSound + (code & 3));
              work->speakerVoiceCountdown = voiceGap;
            }
          } else {
            work->speakerVoiceCountdown--;
          }
        }
        glyphAdvance = glyphWidth << 8;
#if (LANGUAGE != LANGUAGE_JAPANESE)
        if (code == TEXT_SPACE)
          glyphAdvance += channel->spacing[channel->line];
#else
        glyphAdvance += channel->spacing[channel->line];
#endif
        channel->x += glyphAdvance;
      }
      // In Japanese a character that advances nothing (UiText_DrawGlyph
      // returns 0, as it does when no render output is free) closes the
      // window.
#if (LANGUAGE == LANGUAGE_JAPANESE)
      else
        return CHANNEL_STEP_CLOSE;
#endif
      // Outside battle a space ends this frame's typing.
      if (code == TEXT_SPACE && work->menuState == 0)
        glyphsLeft = 1;
    }

    // A pause holds the channel on this entry until its countdown ends.
    if (channel->countdown != 0) {
      channel->countdown--;
      if (channel->countdown != 0)
        continue;
    }
    channel->entry = (channel->entry + 1) & RENDER_ENTRY_MASK;
  } while (--glyphsLeft != 0);

  return CHANNEL_STEP_RUNNING;
}
