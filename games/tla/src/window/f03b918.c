#include "types.h"
#include "edition.h"
#include "version.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"

void Text_ResetStyle(void);
s32 GameFlag_Test(s32 flag);

void Text_MeasureEntry(s32 entryIndex, u32 *outWidth, u32 *outHeight,
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
