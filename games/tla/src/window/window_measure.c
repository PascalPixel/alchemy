/// @file
/// Sizing windows to their text and fitting them on the screen.
// Part of window.c.

#include "window_internal.h"

void Text_MeasureAllPages(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                          u16 *spacing);
void Text_MeasureDecoded(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                         u16 *spacing);

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

/// Sizes a window for decoded text entryIndex and moves (*windowX, *windowY) so
/// the window fits on the screen, as flags (enum WindowFitFlag) say. In the
/// Japanese release, while a mind-read window is open, a measured window is 2
/// tiles wider and kept off the screen's last column.
void Window_FitOnScreen(s32 entryIndex, s32 *windowX, s32 *windowY,
                        u32 *windowWidth, u32 *windowHeight, u16 *spacing,
                        u32 flags) {
#if !TLA_PATCH_1
  struct WindowState *state;
#endif
  s32 x;
  s32 y;
  s32 rightLimit;
  s32 right;
  s32 bottom;
  s32 overflow;
  s32 fittedPosition;

#if !TLA_PATCH_1
  state = WINDOW_STATE;
#endif
  x = *windowX;
  y = *windowY;
  rightLimit = DISPLAY_TILE_WIDTH;

  if (!(flags & WINDOW_FIT_SIZE_GIVEN)) {
    if (flags & WINDOW_FIT_ALL_PAGES)
      Text_MeasureAllPages(entryIndex, windowWidth, windowHeight, spacing);
    else
      Text_MeasureDecoded(entryIndex, windowWidth, windowHeight, spacing);
  }

  if (*windowWidth == 0 && *windowHeight == 0)
    return;

  // Pixels to tiles, with room for the border and a margin.
  if (!(flags & WINDOW_FIT_SIZE_GIVEN)) {
    *windowWidth = (*windowWidth + WINDOW_PAD_X) >> 3;
    *windowHeight = (*windowHeight + WINDOW_PAD_Y) >> 3;
#if !TLA_PATCH_1
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

/// Defines a routine that decodes message and sizes its window to fit on the
/// screen near tile (*x, *y), as Window_FitOnScreen does with flags; it
/// returns 0 when the message is empty.
#define DEFINE_MESSAGE_SIZE(name, flags)                                       \
  s32 name(s32 message, s32 *x, s32 *y, u32 *width, u32 *height) {             \
    struct WindowState *state;                                                 \
    s32 entry;                                                                 \
                                                                               \
    state = WINDOW_STATE;                                                      \
    entry = Message_Decode(message, 0);                                        \
    if (state->textBuffer[entry] == 0) {                                       \
      return 0;                                                                \
    }                                                                          \
    Window_FitOnScreen(entry, x, y, width, height, 0, flags);                  \
    return 1;                                                                  \
  }

/// Sizes a message's window by its first page.
DEFINE_MESSAGE_SIZE(Text_GetMessageSize, 0)
/// Sizes a message's window to hold every page.
DEFINE_MESSAGE_SIZE(Text_GetMessageSizeAllPages, WINDOW_FIT_ALL_PAGES)

/// Measures decoded text from entryIndex to its first page's end: returns the
/// width of the longest line and the height (TEXT_LINE_HEIGHT a line) in
/// pixels. With spacing set, fills one entry per line, MESSAGE_LINE_COUNT at
/// most, with the
/// extra gap between characters, in 1/256 pixel, that spreads the line across
/// the window. It leaves WindowState.outline at the last style it passed.
void Text_MeasureDecoded(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                         u16 *spacing) {
  struct WindowState *state;
  u32 height;
  u32 lines;
  u32 width;
  u32 lineWidth;
  u32 count;
  u32 code;
  u32 glyph;
  u32 lineIndex;
  u16 lineSpacingCounts[MESSAGE_LINE_COUNT];
  u16 lineWidths[MESSAGE_LINE_COUNT];
  s32 gap;
  u32 step;
  u32 style;

  state = WINDOW_STATE;
  width = 0;
  height = TEXT_LINE_HEIGHT;
  lines = 0;
  lineWidth = 0;
  count = 0;
  for (;;) {
    code = state->textBuffer[entryIndex];
    entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
    if (code >= TEXT_SPACE && code != TEXT_EM_DASH) {
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
      continue;
    }
#else
    // Japanese counts every character as a point the line can be stretched
    // at. The kana marks (TEXT_MARK_VOICED and TEXT_MARK_SEMIVOICED) sit over
    // the character before them; codes above TEXT_LAST_FONT_CODE are kanji,
    // outside gFont_Glyphs.
    if (code >= TEXT_SPACE) {
      if (code - TEXT_MARK_VOICED <= 1) {
        lineWidth += 1;
      } else if (code == TEXT_JAPANESE_MIDDLE_DOT) {
        lineWidth += 6;
      } else if (code == TEXT_SPACE) {
        lineWidth += WINDOW_SPRITE_SPACE_WIDTH;
      } else {
        glyph = KANJI_WIDTH;
        if (code <= TEXT_LAST_FONT_CODE)
          glyph = gFont_Glyphs[code - TEXT_SPACE].width;
        style = state->outline;
        if (style == TEXT_STYLE_OUTLINED || style == TEXT_STYLE_SPIN)
          glyph++;
        lineWidth += glyph;
      }
      count++;
      continue;
    }
#endif
    switch (code) {
    case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
      // Outside Japanese, count is the line's spaces plus one, its words.
      count++;
#endif
#if TLA_PATCH_1
      lineSpacingCounts[lines] = count;
#else
      // UB: with FLAG_SPREAD_LINES clear the line's count is never stored, and
      // the spacing below reads what the stack held there.
      if (GameFlag_Get(FLAG_SPREAD_LINES))
        lineSpacingCounts[lines] = count;
#ifdef UBFIX
      else
        lineSpacingCounts[lines] = 0;
#endif
#endif
      lineWidths[lines] = lineWidth;
      if (width < lineWidth)
        width = lineWidth;
      if (lines < MESSAGE_LINE_COUNT - 1)
        lines++;
      count = 0;
      lineWidth = 0;
      height += TEXT_LINE_HEIGHT;
      break;
    case TEXT_PAGE:
      Text_ResetStyle();
      // Fall through.
    case TEXT_END:
      goto done;
    case 14:
    case TEXT_ACTOR_NAME_START:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
    case TEXT_COLOR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case TEXT_ACTOR_NAME:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    case TEXT_OUTLINE:
      state->outline = state->textBuffer[entryIndex];
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    }
  }
done:
#if LANGUAGE != LANGUAGE_JAPANESE
  count++;
#endif
  lineSpacingCounts[lines] = count;
  lineWidths[lines] = lineWidth;
  if (width < lineWidth)
    width = lineWidth;
  // While a mind-read window is open the text is 2 pixels wider, except in the
  // German release and the French and Italian after it (Spanish keeps them).
#if !TLA_PATCH_2 || LANGUAGE == LANGUAGE_SPANISH
  if (state->mindRead)
    width = width + 2;
#endif
  // In textMode a message of three lines or more is squeezed into 32 pixels,
  // and spacing's last entry tells Message_Start so (Message.compressLines).
  if (state->textMode != 0) {
    if (height > 44) {
      spacing[3] = 1;
      height = 32;
    } else {
      spacing[3] = 0;
    }
  }
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
        step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE != LANGUAGE_JAPANESE
        if (step > Q_8_8(LINE_GAP_MAX))
          step = Q_8_8(2);
#else
        if (step > Q_8_8(8))
          step = 0;
        if (step > Q_8_8(1))
          step = Q_8_8(1);
#endif
        *spacing++ = step;
      }
      lineIndex++;
    }
  }
  Text_ResetStyle();
}

/// Measures decoded text from entryIndex over all its pages: the width of the
/// first page's longest line and the height of its tallest page, in pixels,
/// with each line's extra character gap in spacing as Text_MeasureDecoded.
/// The text must fit 16 pages. Outside Japanese the gaps have no ceiling, and
/// past the first page they do not fit their lines; its one caller,
/// Text_GetMessageSizeAllPages, passes no spacing.
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
  u32 step;

  state = WINDOW_STATE;
#if TLA_PATCH_1 && !TLA_PATCH_2
  maxWidth = 0;
#endif
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
  pageHeights[13] = TEXT_LINE_HEIGHT;
  pageHeights[14] = TEXT_LINE_HEIGHT;
  pageHeights[15] = TEXT_LINE_HEIGHT;
#if !TLA_PATCH_1 || TLA_PATCH_2
  maxWidth = 0;
#endif
  line = 0;
  width = 0;
  count = 0;
  page = 0;
  for (;;) {
    code = state->textBuffer[entryIndex];
    entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
#if LANGUAGE != LANGUAGE_JAPANESE
    if (code >= TEXT_SPACE && code != TEXT_EM_DASH) {
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
      continue;
    }
#else
    // Japanese counts every character as a point the line can be stretched
    // at. The kana marks (TEXT_MARK_VOICED and TEXT_MARK_SEMIVOICED) sit over
    // the character before them; codes above TEXT_LAST_FONT_CODE are kanji,
    // outside gFont_Glyphs.
    if (code >= TEXT_SPACE) {
      if (code - TEXT_MARK_VOICED <= 1) {
        width += 1;
      } else if (code == TEXT_JAPANESE_MIDDLE_DOT) {
        width += 6;
      } else if (code == TEXT_SPACE) {
        width += WINDOW_SPRITE_SPACE_WIDTH;
      } else {
        glyph = KANJI_WIDTH;
        if (code <= TEXT_LAST_FONT_CODE)
          glyph = gFont_Glyphs[code - TEXT_SPACE].width;
        outline = state->outline;
        if (outline == TEXT_STYLE_OUTLINED || outline == TEXT_STYLE_SPIN)
          glyph++;
        width += glyph;
      }
      count++;
      continue;
    }
#endif
    switch (code) {
    case TEXT_NEWLINE:
#if LANGUAGE != LANGUAGE_JAPANESE
      // Outside Japanese, count is the line's spaces plus one, its words.
      count++;
#endif
      lineSpacingCounts[line] = count;
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
      count++;
#endif
      lineSpacingCounts[line] = count;
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      page++;
      goto done;
    case TEXT_PAGE:
#if LANGUAGE != LANGUAGE_JAPANESE
      count++;
#endif
      lineSpacingCounts[line] = count;
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      page++;
      Text_ResetStyle();
      break;
    case 14:
    case TEXT_ACTOR_NAME_START:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
    case TEXT_COLOR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case TEXT_ACTOR_NAME:
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    case TEXT_OUTLINE:
      state->outline = state->textBuffer[entryIndex];
      entryIndex = (entryIndex + 1) & TEXT_BUFFER_MASK;
      break;
    }
  }
done:
  // From the German release on, a mind-read window is no longer 2 pixels
  // wider.
#if !TLA_PATCH_2
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
        // The gap between characters, in 1/256 pixel: in Japanese one over 8
        // pixels is dropped and the rest held to at most 1.
        step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE == LANGUAGE_JAPANESE
        if (step > Q_8_8(8))
          step = 0;
        if (step > Q_8_8(1))
          step = Q_8_8(1);
#endif
        *spacing++ = step;
      }
    }
  }
  Text_ResetStyle();
}

#undef DEFINE_MESSAGE_SIZE
