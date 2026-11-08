#include "types.h"
#include "edition.h"
#include "version.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"

void Text_ResetStyle(void);

void Func_0803bb58(s32 entryIndex, u32 *outWidth, u32 *outHeight,
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
