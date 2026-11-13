/// @file
/// Sizing windows to their text and fitting them on the screen.
// Part of window.c.

#include "../window_internal.h"

void Text_MeasureAllPages(s32 no, u32 *pw, u32 *ph, u16 *spacing);
void Text_MeasureDecoded(s32 no, u32 *pw, u32 *ph, u16 *spacing);

// In the Japanese release, while the window work's mode is set, a measured
// window is 2 tiles wider and kept off the screen's last column.
void Window_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                        u16 *spacing, u32 flags) {
#if !TLA_PATCH_1
  struct WindowWork *work;
#endif
  s32 x;
  s32 y;
  s32 limit;
  s32 right;
  s32 bottom;
  s32 over;
  s32 pos;

#if !TLA_PATCH_1
  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
#endif
  x = *px;
  y = *py;
  limit = 30;

  if (!(flags & 2)) {
    if (flags & 1)
      Text_MeasureAllPages(no, pw, ph, spacing);
    else
      Text_MeasureDecoded(no, pw, ph, spacing);
  }

  if (*pw == 0 && *ph == 0)
    return;

  if (!(flags & 2)) {
    *pw = (*pw + 19) >> 3;
    *ph = (*ph + 15) >> 3;
#if !TLA_PATCH_1
    if (work->mode != 0) {
      *pw += 2;
      limit = 29;
    }
#endif
  }

  right = x + *pw;
  if (right > limit) {
    over = right - limit;
    pos = x - over;
    if (pos >= 0)
      x = pos;
    else
      x = 0;
  }
  bottom = y + *ph;
  if (bottom > 20) {
    over = bottom - 20;
    pos = y - over;
    if (pos >= 0)
      y = pos;
    else
      y = 0;
  }
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x > limit - *pw)
    x = limit - *pw;
  if (y > 20 - *ph)
    y = 20 - *ph;
  *px = x;
  *py = y;
}

s32 Message_Decode(s32, s32);

void Text_MeasureMessage(s32 no, s32 *x, s32 *y) {
  Text_MeasureDecoded(Message_Decode(no, 0), x, y, 0);
}

/// Builds the render entries of message no and fits its window on screen,
/// passing flags to Window_FitOnScreen; returns 0 when the message is empty.
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                            \
  s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height) {                  \
    struct WindowWork *work;                                                 \
    s32 idx;                                                                   \
                                                                               \
    work = (struct WindowWork *)Ram_HeapSlots->windowTiles;                  \
    idx = Message_Decode(no, 0);                                    \
    if (work->entries[idx] == 0) {                                             \
      return 0;                                                                \
    }                                                                          \
    Window_FitOnScreen(idx, x, y, width, height, 0, flags);                  \
    return 1;                                                                  \
  }

DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSize, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSizeAlt, 1)

void Text_ResetStyle(void);

/// Measures decoded text from entryIndex to its first page's end: returns the
/// width of the longest line and the height (15 pixels a line) in pixels. With
/// spacing set, fills one entry per line with the extra gap between characters,
/// in 1/256 pixel, that spreads the line to fill the window. spacing, when
/// non-NULL, has room for four line entries. A page of more than four lines
/// keeps measuring into the fourth. It sets WindowWork.outline to each
/// TEXT_OUTLINE operand it passes and leaves it at the last, the style the
/// next glyphs are drawn in unless the style is reset first (Window_Create
/// resets it).
void Text_MeasureDecoded(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                       u16 *spacing) {
  struct WindowWork *work;
  u32 height;
  u32 lines;
  u32 width;
  u32 lineWidth;
  u32 count;
  u32 code;
  u32 glyph;
  u32 lineIndex;
  u16 lineSpacingCounts[4];
  u16 lineWidths[4];
  s32 gap;
  u32 step;
  u32 style;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  width = 0;
  height = 15;
  lines = 0;
  lineWidth = 0;
  count = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
#if LANGUAGE != LANGUAGE_JAPANESE
    if (code > 31 && code != 176) {
      if (code == 32) {
        lineWidth += 5;
        count++;
      } else {
        glyph = gFont_Glyphs[code - 32].width;
        style = work->outline;
        if (style == 1 || style == 5)
          glyph++;
        lineWidth += glyph;
      }
      continue;
    }
#else
    // Japanese counts every character as a point the line can be stretched
    // at. The kana marks (codes 222 and 223) sit over the character before
    // them; codes above TEXT_LAST_FONT_CODE are kanji, outside gFont_Glyphs.
    if (code > 31) {
      if (code - 222 <= 1) {
        lineWidth += 1;
      } else if (code == 165) {
        lineWidth += 6;
      } else if (code == 32) {
        lineWidth += 7;
      } else {
        glyph = 10;
        if (code <= TEXT_LAST_FONT_CODE)
          glyph = gFont_Glyphs[code - 32].width;
        style = work->outline;
        if (style == 1 || style == 5)
          glyph++;
        lineWidth += glyph;
      }
      count++;
      continue;
    }
#endif
    switch (code) {
    case 3:
#if LANGUAGE != LANGUAGE_JAPANESE
      // Outside Japanese, count is the line's spaces plus one, its words.
      count++;
#endif
#if TLA_PATCH_1
      lineSpacingCounts[lines] = count;
#else
      // UB: with flag 0xc00 clear the line's count is never stored, and the
      // spacing below reads what the stack held there.
      if (GameFlag_Test(0xc00))
        lineSpacingCounts[lines] = count;
#ifdef UBFIX
      else
        lineSpacingCounts[lines] = 0;
#endif
#endif
      lineWidths[lines] = lineWidth;
      if (width < lineWidth)
        width = lineWidth;
      if (lines < 3)
        lines++;
      count = 0;
      lineWidth = 0;
      height += 15;
      break;
    case 1:
      Text_ResetStyle();
    case 0:
      goto done;
    case 14:
    case 28:
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
    case 8:
    case 10:
    case 15:
    case 17:
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
      break;
    case 9:
      work->outline = work->entries[entryIndex];
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
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
  // The German release drops the 2 pixels a window's mode adds, and French and
  // Italian follow it; Spanish, released with Italian, keeps them.
#if !TLA_PATCH_2 || LANGUAGE == LANGUAGE_SPANISH
  if (work->mode)
    width = width + 2;
#endif
  if (work->textMode != 0) {
    if (height > 44) {
      spacing[3] = 1;
      height = 32;
    } else {
      spacing[3] = 0;
    }
  }
  *outWidth = width;
  *outHeight = height;
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
        // The gap between characters, in 1/256 pixel: outside Japanese one
        // over 12 pixels falls back to 2; in Japanese one over 8 is dropped
        // and the rest held to at most 1.
        step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE != LANGUAGE_JAPANESE
        if (step > (12 << 8))
          step = 2 << 8;
#else
        if (step > (8 << 8))
          step = 0;
        if (step > (1 << 8))
          step = 1 << 8;
#endif
        *spacing++ = step;
      }
      lineIndex++;
    }
  }
  Text_ResetStyle();
}

void Text_ResetStyle(void);

/// Measures decoded text from start over all its pages: the width of the first
/// page's longest line and the height of its tallest page, in pixels, with each
/// line's extra character gap in spacing as Text_MeasureDecoded.
/// The text must fit 16 page-height slots; this routine does not clamp the page
/// count. Unlike Text_MeasureDecoded it puts no ceiling on the gaps
/// outside Japanese, and a page break restarts neither the line count nor the
/// line's width, so the gaps past the first page do not fit their lines; its
/// one caller, Window_FitOnScreen with flags bit 0, is reached only from
/// Text_GetMessageSizeAlt, which passes no spacing. It changes
/// WindowWork.outline as Text_MeasureDecoded does.
void Text_MeasureAllPages(s32 entryIndex, u32 *outWidth, u32 *outHeight,
                   u16 *spacing) {
  u32 pageHeights[16];
  u16 lineSpacingCounts[4];
  u16 lineWidths[4];
  struct WindowWork *work;
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

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
#if TLA_PATCH_1 && !TLA_PATCH_2
  // English alone clears maxWidth before the page heights rather than after
  // them; only the registers the compiler gives it show the difference.
  maxWidth = 0;
#endif
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
  pageHeights[13] = 15;
  pageHeights[14] = 15;
  pageHeights[15] = 15;
#if !TLA_PATCH_1 || TLA_PATCH_2
  maxWidth = 0;
#endif
  line = 0;
  width = 0;
  count = 0;
  page = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
#if LANGUAGE != LANGUAGE_JAPANESE
    if (code >= 32 && code != 176) {
      if (code == 32) {
        width += 5;
        count++;
      } else {
        glyph = gFont_Glyphs[code - 32].width;
        outline = work->outline;
        if (outline == 1 || outline == 5)
          glyph++;
        width += glyph;
      }
      continue;
    }
#else
    // Japanese counts every character as a point the line can be stretched
    // at. The kana marks (codes 222 and 223) sit over the character before
    // them; codes above TEXT_LAST_FONT_CODE are kanji, outside gFont_Glyphs.
    if (code >= 32) {
      if (code - 222 <= 1) {
        width += 1;
      } else if (code == 165) {
        width += 6;
      } else if (code == 32) {
        width += 7;
      } else {
        glyph = 10;
        if (code <= TEXT_LAST_FONT_CODE)
          glyph = gFont_Glyphs[code - 32].width;
        outline = work->outline;
        if (outline == 1 || outline == 5)
          glyph++;
        width += glyph;
      }
      count++;
      continue;
    }
#endif
    switch (code) {
    case 3:
#if LANGUAGE != LANGUAGE_JAPANESE
      // Outside Japanese, count is the line's spaces plus one, its words.
      count++;
#endif
      lineSpacingCounts[line] = count;
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
#if LANGUAGE != LANGUAGE_JAPANESE
      count++;
#endif
      lineSpacingCounts[line] = count;
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      page++;
      goto done;
    case 1:
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
    case 28:
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
    case 8:
    case 10:
    case 15:
    case 17:
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
      break;
    case 9:
      work->outline = work->entries[entryIndex];
      entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
      break;
    }
  }
done:
  // From the German release on, a window's mode no longer adds its 2 pixels.
#if !TLA_PATCH_2
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
        // The gap between characters, in 1/256 pixel: in Japanese one over 8
        // pixels is dropped and the rest held to at most 1.
        step = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
#if LANGUAGE == LANGUAGE_JAPANESE
        if (step > (8 << 8))
          step = 0;
        if (step > (1 << 8))
          step = 1 << 8;
#endif
        *spacing++ = step;
      }
    }
  }
  Text_ResetStyle();
}

#undef DEFINE_GET_RESOURCE_DIMENSIONS
