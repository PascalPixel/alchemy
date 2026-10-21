/// @file
/// Sizing windows to their text and fitting them on the screen.
// Part of window.c.

#include "../window_internal.h"

#if TBS_PATCH_2

#define FIT_FIXED_LIMIT 1
#endif

/// Sizes a window for decoded text no and moves (*px, *py) so the window fits
/// on the screen. flags: bit 0 measures every page (UiText_MeasureAllPages)
/// rather than the first; bit 1 takes the size in *pw and *ph as given, in
/// tiles, instead of measuring. Measuring returns the size in tiles in *pw and
/// *ph and fills spacing with each line's extra character gap. An empty text
/// (size 0 by 0) leaves the position as it was. Under Mind Read (Japanese and
/// English) the window is 2 tiles wider and kept off the screen's last column.
void UiWindow_FitOnScreen(s32 entryIndex, s32 *windowX, s32 *windowY,
                          u32 *windowWidth, u32 *windowHeight, u16 *spacing,
                          u32 flags) {
#if !defined(FIT_FIXED_LIMIT)
  u8 *base;
#endif
  s32 x;
  s32 y;
  s32 rightLimit;
  s32 right;
  s32 bottom;
  s32 overflow;
  s32 fittedPosition;

#if !defined(FIT_FIXED_LIMIT)
  base = gWindowWork[0];
#endif
  x = *windowX;
  y = *windowY;
  rightLimit = 30;

  if (!(flags & 2)) {
    if (flags & 1)
      UiText_MeasureAllPages(entryIndex, windowWidth, windowHeight, spacing);
    else
      UiText_MeasureEntryDimensions(entryIndex, windowWidth, windowHeight,
                                    spacing);
  }

  if (*windowWidth == 0 && *windowHeight == 0)
    return;

  // Pixels to tiles, with room for the frame and a margin: + 19 is the two
  // frame tiles (16) and 3, so a width rounds up to the next tile only past
  // 4 pixels into it. Lines are 15 pixels, so 1 to 4 lines are 3, 5, 7 and 9
  // tiles tall.
  if (!(flags & 2)) {
    *windowWidth = (*windowWidth + 19) >> 3;
    *windowHeight = (*windowHeight + 15) >> 3;
#if !defined(FIT_FIXED_LIMIT)
    if (((struct UiRenderWork *)base)->mode != 0) {
      *windowWidth += 2;
      rightLimit = 29;
    }
#endif
  }

  // Slide the window back onto the screen, rightLimit columns by 20 rows.
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
  if (bottom > 20) {
    overflow = bottom - 20;
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
  if (y > 20 - *windowHeight)
    y = 20 - *windowHeight;
  *windowX = x;
  *windowY = y;
}

/// Decodes message and returns its text's width and height in pixels. The
/// argument table is left as it is (clear 0), so the message can be decoded
/// again to be shown.
void UiText_MeasureResourceEntries(s32 message, s32 *x, s32 *y) {
  UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(message, 0), (u32 *)x,
                                (u32 *)y, 0);
}

DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensions, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensionsAlt, 1)

/// Measures decoded text from entryIndex to its first page's end: returns the
/// width of the longest line and the height (15 pixels a line) in pixels. With
/// spacing set, fills one entry per line with the extra gap between characters,
/// in 1/256 pixel, that spreads the line to fill the window. spacing, when
/// non-NULL, has room for four line entries. A page of more than four lines
/// keeps measuring into the fourth. It sets UiRenderWork.outline to each
/// TEXT_OUTLINE operand it passes and leaves it at the last, the style the
/// next glyphs are drawn in unless the style is reset first (UiWindow_Create
/// resets it).
void UiText_MeasureEntryDimensions(s32 entryIndex, u32 *outWidth,
                                   u32 *outHeight, u16 *spacing) {
  struct UiRenderWork *work;
  u32 lines;
  u32 width;
  u32 height;
  u32 lineWidth;
  u32 count;
  u32 code;
  u32 glyph;
  u32 lineIndex;
  u16 lineSpacingCounts[4];
  u16 lineWidths[4];
  s32 gap;
  u32 style;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 trailing;
#endif

  work = UI_WORK;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  trailing = 0;
#endif
  width = 0;
  height = 15;
  lines = 0;
  lineWidth = 0;
  count = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
    // A character adds its width. Spaces (in Japanese every character) are
    // counted as the points the line can be stretched at. A space is
    // UI_SPRITE_SPACE_WIDTH pixels. Style 1 (outlined) draws a glyph a pixel
    // wider. XXX: style 5 (spinning) is counted a pixel wider too, though
    // UiText_RenderGlyphPair widens only style 1. No message in the six
    // catalogs uses style 5 ({text_effect:6}), so none shows whether the pixel
    // is meant; a use of the style would settle it.
    if (code > 31) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
      if (code == 32) {
        lineWidth += 5;
        count++;
      } else {
        glyph = gUiText_Glyphs[code - 32].width;
        style = work->outline;
        if (style == 1 || style == 5)
          glyph++;
        lineWidth += glyph;
      }
#else
      if (code != TEXT_VOICED_MARK && code != TEXT_SEMIVOICED_MARK) {
        if (code == 32) {
          lineWidth += 7;
        } else {
          // Punctuation at the end of a line takes less room than its cell:
          // 1 pixel less for code 0xa5, 3 for code 0x21 and 6 for codes 0xa1
          // and 0xa4.
          if (code == 0xa5)
            trailing = -1;
          else if (code == 0x21)
            trailing = -3;
          else if (code == 0xa1 || code == 0xa4)
            trailing = -6;
          else
            trailing = 0;
          glyph = 10;
          if (code <= 0xff)
            glyph = gUiText_Glyphs[code - 32].width;
          style = work->outline;
          if (style == 1 || style == 5)
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
      count++;
#else
      lineWidth += trailing;
#endif
      lineSpacingCounts[lines] = count;
      lineWidths[lines] = lineWidth;
      if (width < lineWidth)
        width = lineWidth;
      if (lines < 3)
        lines++;
      count = 0;
      lineWidth = 0;
      height += 15;
      break;
#if TBS_PATCH_3
    case TEXT_PAGE:
      // The French, Spanish and Italian releases end the style at a page
      // break, as their channels do (UiWork_StepChannelScript).
      work->outline = 0;
      goto done;
#endif
    case 14:
    case 28:
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      // Skip the second operand too.
      // Fall through.
    case TEXT_COLOUR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case 17:
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      break;
    case TEXT_OUTLINE:
      work->outline = work->entries[entryIndex];
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      break;
    }
  }
done:
#if (LANGUAGE != LANGUAGE_JAPANESE)
  count++;
#else
  lineWidth += trailing;
#endif
  lineSpacingCounts[lines] = count;
  lineWidths[lines] = lineWidth;
  if (width < lineWidth)
    width = lineWidth;
#if !TBS_PATCH_2
  if (work->mode)
    width = width + 2;
#endif
  *outWidth = width;
  *outHeight = height;
  // Each line's extra gap between characters, in 1/256 pixel, to spread it
  // across the window's inner width: the window's width in tiles, as
  // UiWindow_FitOnScreen makes it, less the frame, in pixels, and 4 pixels of
  // margin. Outside Japanese a gap over 12 pixels (0xc00) falls back to 2; in
  // Japanese one over 8 is dropped and the rest held to at most 1.
  width = (width + 19) >> 3;
  width = (width << 3) - 16;
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
        gap = ((gap << 8) / (lineSpacingCounts[lineIndex] - 1));
#if (LANGUAGE != LANGUAGE_JAPANESE)
        if (gap < 0 || gap > 0xc00)
          gap = 0x200;
#else
        if (gap < 0 || gap > 0x800)
          gap = 0;
        if (gap < 0 || gap > 0x100)
          gap = 0x100;
#endif
        *spacing++ = gap;
      }
      lineIndex++;
    }
  }
}

/// Measures decoded text from start over all its pages: the width of the first
/// page's longest line and the height of its tallest page, in pixels, with each
/// line's extra character gap in spacing as UiText_MeasureEntryDimensions.
/// The text must fit 16 page-height slots; this routine does not clamp the page
/// count. Unlike UiText_MeasureEntryDimensions it puts no ceiling on the gaps
/// outside Japanese, and a page break restarts neither the line count nor the
/// line's width, so the gaps past the first page do not fit their lines; its
/// one caller, UiWindow_FitOnScreen with flags bit 0, is reached only from
/// UiText_GetResourceDimensionsAlt, which passes no spacing. It changes
/// UiRenderWork.outline as UiText_MeasureEntryDimensions does.
void UiText_MeasureAllPages(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                            u16 *spacing) {
  u32 pageHeights[16];
  u16 lineSpacingCounts[4];
  u16 lineWidths[4];
  struct UiRenderWork *work;
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 trailing;

  trailing = 0;
#endif
  work = UI_WORK;
  pageHeights[0] = 15;
  pageHeights[1] = 15;
  pageHeights[2] = 15;
  pageHeights[3] = 15;
  pageHeights[4] = 15;
  pageHeights[5] = 15;
  pageHeights[6] = 15;
  pageHeights[7] = 15;
  pageHeights[8] = 15;
  pageHeights[9] = 15;
  pageHeights[10] = 15;
  pageHeights[11] = 15;
  pageHeights[12] = 15;
  maxWidth = 0;
  pageHeights[13] = 15;
  pageHeights[14] = 15;
  pageHeights[15] = 15;
  line = 0;
  width = 0;
  count = 0;
  page = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
    if (code >= 32) {
#if (LANGUAGE != LANGUAGE_JAPANESE)
      if (code == 32) {
        width += 5;
        count++;
      } else {
        glyph = gUiText_Glyphs[code - 32].width;
        outline = work->outline;
        if (outline == 1 || outline == 5)
          glyph++;
        width += glyph;
      }
#else
      if (code != TEXT_VOICED_MARK && code != TEXT_SEMIVOICED_MARK) {
        if (code == 32) {
          width += 7;
        } else {
          if (code == 0xa5)
            trailing = -1;
          else if (code == 0x21)
            trailing = -3;
          else if (code == 0xa1 || code == 0xa4)
            trailing = -6;
          else
            trailing = 0;
          glyph = 10;
          if (code <= 0xff)
            glyph = gUiText_Glyphs[code - 32].width;
          outline = work->outline;
          if (outline == 1 || outline == 5)
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
      lineSpacingCounts[line] = ++count;
#else
      width += trailing;
      lineSpacingCounts[line] = count;
#endif
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      if (line < 3)
        line++;
      count = 0;
      width = 0;
      pageHeights[page] += 15;
      break;
    case 0:
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
      lineSpacingCounts[line] = ++count;
#else
      width += trailing;
      lineSpacingCounts[line] = count;
#endif
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
#if TBS_PATCH_3
      work->outline = 0;
#endif
      page++;
      break;
    case 14:
    case 28:
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      // Skip the second operand too.
      // Fall through.
    case TEXT_COLOUR:
    case TEXT_LINE_SPACING:
    case TEXT_WINDOW_STATE:
    case 17:
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      break;
    case TEXT_OUTLINE:
      work->outline = work->entries[entryIndex];
      entryIndex = (entryIndex + 1) & RENDER_ENTRY_MASK;
      break;
    }
  }
done:
#if !(LANGUAGE == LANGUAGE_GERMAN) && !(LANGUAGE == LANGUAGE_FRENCH)
  if (work->mode != 0)
    maxWidth += 2;
#endif
  for (pageIndex = 0; pageIndex < page; pageIndex++) {
    if (pageIndex == 0)
      *outHeight = pageHeights[0];
    else if (*outHeight < pageHeights[pageIndex])
      *outHeight = pageHeights[pageIndex];
  }
  *outWidth = maxWidth;
  maxWidth = (((maxWidth + 19) >> 3) << 3) - 16;
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
        if (gap < 0 || gap > 0x800)
          gap = 0;
        if (gap < 0 || gap > 0x100)
          gap = 0x100;
#endif
        *spacing++ = gap;
      }
    }
  }
}
