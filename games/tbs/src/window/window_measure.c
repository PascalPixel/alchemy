/// @file
/// Sizing windows to their text and fitting them on the screen.
// Part of window.c.

#include "window_internal.h"

/// The pixels of a window's two border tiles, left and right.
#define WINDOW_BORDER_WIDTH 16
/// The widest gap between characters that spreads a line across its window,
/// in pixels, outside Japanese.
#define LINE_GAP_MAX 12

/// What a window adds to its text's size in pixels before it is cut to tiles:
/// across, its border and 3, so a width rounds up to the next tile only past 4
/// pixels into it; down, 15, so a height rounds up and takes one tile more.
#define WINDOW_PAD_X (WINDOW_BORDER_WIDTH + 3)
#define WINDOW_PAD_Y 15

/// Sizes a window for the decoded text at entryIndex, as flags (WINDOW_FIT_)
/// ask, and moves (*windowX, *windowY) so the window fits on the screen;
/// spacing receives each line's extra gap between characters. In the Japanese
/// and English releases, under Mind Read, a measured window is 2 tiles wider
/// and kept off the screen's last column.
void Window_FitOnScreen(s32 entryIndex, s32 *windowX, s32 *windowY,
                        u32 *windowWidth, u32 *windowHeight, u16 *spacing,
                        u32 flags) {
#if !TBS_PATCH_2
  struct WindowState *state;
#endif
  s32 x;
  s32 y;
  s32 rightLimit;
  s32 right;
  s32 bottom;
  s32 overflow;
  s32 fittedPosition;

#if !TBS_PATCH_2
  state = WINDOW_STATE;
#endif
  x = *windowX;
  y = *windowY;
  rightLimit = DISPLAY_TILE_WIDTH;

  if (!(flags & WINDOW_FIT_GIVEN_SIZE)) {
    if (flags & WINDOW_FIT_ALL_PAGES)
      Text_MeasureAllPages(entryIndex, windowWidth, windowHeight, spacing);
    else
      Text_MeasureDecoded(entryIndex, windowWidth, windowHeight, spacing);
  }

  if (*windowWidth == 0 && *windowHeight == 0)
    return;

  // Pixels to tiles, with room for the border and a margin.
  if (!(flags & WINDOW_FIT_GIVEN_SIZE)) {
    *windowWidth = (*windowWidth + WINDOW_PAD_X) >> 3;
    *windowHeight = (*windowHeight + WINDOW_PAD_Y) >> 3;
#if !TBS_PATCH_2
    if (state->mindRead != 0) {
      *windowWidth += 2;
      rightLimit = DISPLAY_TILE_WIDTH - 1;
    }
#endif
  }

  // Slide the window back onto the screen, rightLimit columns by
  // DISPLAY_TILE_HEIGHT rows.
  right = x + *windowWidth;
  if (right > rightLimit) {
    overflow = right - rightLimit;
    fittedPosition = x - overflow;
    if (fittedPosition >= 0)
      x = fittedPosition;
    else
      x = 0;
  }
  bottom = y + *windowHeight;
  if (bottom > DISPLAY_TILE_HEIGHT) {
    overflow = bottom - DISPLAY_TILE_HEIGHT;
    fittedPosition = y - overflow;
    if (fittedPosition >= 0)
      y = fittedPosition;
    else
      y = 0;
  }
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x > rightLimit - *windowWidth)
    x = rightLimit - *windowWidth;
  if (y > DISPLAY_TILE_HEIGHT - *windowHeight)
    y = DISPLAY_TILE_HEIGHT - *windowHeight;
  *windowX = x;
  *windowY = y;
}

/// Decodes message and returns its text's width and height in pixels.
void Text_MeasureMessage(s32 message, s32 *x, s32 *y) {
  Text_MeasureDecoded(Message_Decode(message, 0), (u32 *)x, (u32 *)y, 0);
}

/// Sizes and places a window for a message's first page.
DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSize, 0)
/// Sizes and places a window for all of a message's pages.
DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSizeAllPages,
                               WINDOW_FIT_ALL_PAGES)

/// Measures decoded text from entryIndex to its first page's end: returns the
/// width of the longest line and the height (TEXT_LINE_HEIGHT a line) in
/// pixels.
void Text_MeasureDecoded(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                         u16 *spacing) {
  struct WindowState *state;
  u32 lines;
  u32 width;
  u32 height;
  u32 lineWidth;
  u32 count;
  u32 code;
  u32 glyph;
  u32 lineIndex;
  u16 lineSpacingCounts[MESSAGE_LINE_COUNT];
  u16 lineWidths[MESSAGE_LINE_COUNT];
  s32 gap;
  u32 style;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 trailing;
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
  for (;;) {
    code = state->textBuffer[entryIndex];
    entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
    // A character adds its width.
    if (code >= TEXT_SPACE) {
#if LANGUAGE != LANGUAGE_JAPANESE
      if (code == TEXT_SPACE) {
        lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
        count++;
      } else {
        glyph = gFont_Glyphs[code - TEXT_SPACE].width;
        style = state->outline;
        if (style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
          glyph++;
        lineWidth += glyph;
      }
#else
      if (code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
        if (code == TEXT_SPACE) {
          lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
        } else {
          // Punctuation at the end of a line takes less room than its cell.
          if (code == TEXT_JAPANESE_MIDDLE_DOT)
            trailing = -1;
          else if (code == '!')
            trailing = -3;
          else if (code == TEXT_JAPANESE_FULL_STOP ||
                   code == TEXT_JAPANESE_COMMA)
            trailing = -6;
          else
            trailing = 0;
          glyph = KANJI_WIDTH;
          if (code <= TEXT_LAST_FONT_CODE)
            glyph = gFont_Glyphs[code - TEXT_SPACE].width;
          style = state->outline;
          if (style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
            glyph++;
          lineWidth += glyph;
        }
      }
      count++;
#endif
      continue;
    }
    switch (code) {
    case TEXT_END:
#if !TBS_PATCH_3
    case TEXT_PAGE:
#endif
      goto done;
    case TEXT_NEWLINE:
      // Outside Japanese, count is the line's spaces plus one, its words: the
      // gap is spread over the spaces.
#if LANGUAGE != LANGUAGE_JAPANESE
      count++;
#else
      lineWidth += trailing;
#endif
      lineSpacingCounts[lines] = count;
      lineWidths[lines] = lineWidth;
      if (width < lineWidth)
        width = lineWidth;
      if (lines < MESSAGE_LINE_COUNT - 1)
        lines++;
      count = 0;
      lineWidth = 0;
      height += TEXT_LINE_HEIGHT;
      break;
#if TBS_PATCH_3
    case TEXT_PAGE:
      // The French, Spanish and Italian releases end the style at a page
      // break, as their messages do (Message_Type).
      state->outline = TEXT_STYLE_PLAIN;
      goto done;
#endif
    case TEXT_UNUSED_14:
    case TEXT_ACTOR_NAME_START:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      // Skip the second operand too.
      // Fall through.
    case TEXT_COLOR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case TEXT_ACTOR_NAME:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    case TEXT_STYLE:
      state->outline = state->textBuffer[entryIndex];
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
  if (width < lineWidth)
    width = lineWidth;
#if !TBS_PATCH_2
  if (state->mindRead)
    width = width + 2;
#endif
  *outWidth = width;
  *outHeight = height;
  // Each line's extra gap between characters, in 1/256 pixel, to spread it
  // across the window's inner width: the window's width in tiles, as
  // Window_FitOnScreen makes it, less the frame, in pixels, and 4 pixels of
  // margin.
  width = (width + WINDOW_PAD_X) >> 3;
  width = (width << 3) - WINDOW_BORDER_WIDTH;
  if (spacing != NULL) {
    lineIndex = 0;
    while (lineIndex <= lines) {
      if (lineSpacingCounts[lineIndex] <= 1) {
        *spacing++ = 0;
      } else {
        gap = width - lineWidths[lineIndex];
        gap -= 4;
        if (gap < 0)
          gap = 0;
        // Outside Japanese a gap over LINE_GAP_MAX pixels falls back to 2; in
        // Japanese one over 8 is dropped and the rest held to at most 1.
        gap = ((gap << 8) / (lineSpacingCounts[lineIndex] - 1));
#if LANGUAGE != LANGUAGE_JAPANESE
        if (gap < 0 || gap > Q_8_8(LINE_GAP_MAX))
          gap = Q_8_8(2);
#else
        if (gap < 0 || gap > Q_8_8(8))
          gap = 0;
        if (gap < 0 || gap > Q_8_8(1))
          gap = Q_8_8(1);
#endif
        *spacing++ = gap;
      }
      lineIndex++;
    }
  }
}

/// Measures decoded text from start over all its pages: the width of the first
/// page's longest line and the height of its tallest page, in pixels, with each
/// line's extra character gap in spacing as Text_MeasureDecoded.
void Text_MeasureAllPages(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                          u16 *spacing) {
  u32 pageHeights[16];
  u16 lineSpacingCounts[MESSAGE_LINE_COUNT];
  u16 lineWidths[MESSAGE_LINE_COUNT];
  struct WindowState *state;
  u32 maxWidth;
  u32 line;
  u32 width;
  u32 count;
  u32 page;
  u32 code;
  u32 glyph;
  u32 outline;
  u32 pageIndex;
  u32 lineIndex;
  s32 gap;
#if LANGUAGE == LANGUAGE_JAPANESE
  s32 trailing;

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
  for (;;) {
    code = state->textBuffer[entryIndex];
    entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
    if (code >= TEXT_SPACE) {
#if LANGUAGE != LANGUAGE_JAPANESE
      if (code == TEXT_SPACE) {
        width += WINDOW_SPRITE_SPACE_WIDTH;
        count++;
      } else {
        glyph = gFont_Glyphs[code - TEXT_SPACE].width;
        outline = state->outline;
        if (outline == TEXT_STYLE_OUTLINED || outline == TEXT_STYLE_SPIN)
          glyph++;
        width += glyph;
      }
#else
      if (code != TEXT_MARK_VOICED && code != TEXT_MARK_SEMIVOICED) {
        if (code == TEXT_SPACE) {
          width += WINDOW_SPRITE_SPACE_WIDTH;
        } else {
          if (code == TEXT_JAPANESE_MIDDLE_DOT)
            trailing = -1;
          else if (code == '!')
            trailing = -3;
          else if (code == TEXT_JAPANESE_FULL_STOP ||
                   code == TEXT_JAPANESE_COMMA)
            trailing = -6;
          else
            trailing = 0;
          glyph = KANJI_WIDTH;
          if (code <= TEXT_LAST_FONT_CODE)
            glyph = gFont_Glyphs[code - TEXT_SPACE].width;
          outline = state->outline;
          if (outline == TEXT_STYLE_OUTLINED || outline == TEXT_STYLE_SPIN)
            glyph++;
          width += glyph;
        }
      }
      count++;
#endif
      continue;
    }
    switch (code) {
    case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
      lineSpacingCounts[line] = ++count;
#else
      width += trailing;
      lineSpacingCounts[line] = count;
#endif
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      if (line < MESSAGE_LINE_COUNT - 1)
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
      if (page == 0 && maxWidth < width)
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
      if (page == 0 && maxWidth < width)
        maxWidth = width;
#if TBS_PATCH_3
      state->outline = TEXT_STYLE_PLAIN;
#endif
      page++;
      break;
    case TEXT_UNUSED_14:
    case TEXT_ACTOR_NAME_START:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      // Skip the second operand too.
      // Fall through.
    case TEXT_COLOR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case TEXT_ACTOR_NAME:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    case TEXT_STYLE:
      state->outline = state->textBuffer[entryIndex];
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    }
  }
done:
  // Under Mind Read the pages measure 2 pixels wider. Patch 2 (German on)
  // dropped Mind Read's widening from the first-page measure, the fit and the
  // pen; patch 4 (Spanish and Italian) puts it back here alone.
#if !TBS_PATCH_2 || TBS_PATCH_4
  if (state->mindRead != 0)
    maxWidth += 2;
#endif
  for (pageIndex = 0; pageIndex < page; pageIndex++) {
    if (pageIndex == 0)
      *outHeight = pageHeights[0];
    else if (*outHeight < pageHeights[pageIndex])
      *outHeight = pageHeights[pageIndex];
  }
  *outWidth = maxWidth;
  maxWidth = (((maxWidth + WINDOW_PAD_X) >> 3) << 3) - WINDOW_BORDER_WIDTH;
  if (spacing != NULL) {
    for (lineIndex = 0; lineIndex <= line; lineIndex++) {
      if (lineSpacingCounts[lineIndex] <= 1) {
        *spacing++ = 0;
      } else {
        gap = maxWidth - lineWidths[lineIndex];
        gap -= 4;
        if (gap < 0)
          gap = 0;
        gap = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE == LANGUAGE_JAPANESE
        if (gap < 0 || gap > Q_8_8(8))
          gap = 0;
        if (gap < 0 || gap > Q_8_8(1))
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
