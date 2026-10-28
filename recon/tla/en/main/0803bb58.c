// Draft, differ 60 of 258 (2026-10-18): every instruction is the native's
// but the high registers rotate: the native keeps spacing in r8, the widest
// line in r9 and the line-count array in r11, this the widest line in r8, the
// array in r9 and spacing in r11; a scheduling pair at the start. Six minutes
// of permuter and a loop-local copy of spacing (96) found nothing better.
// ☀️'s Text_MeasureAllPages on this game's WindowWork: code 176
// is skipped as a control code, a page break resets the text style, and the
// line gaps are not clamped.
#include "types.h"
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

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
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
  maxWidth = 0;
  line = 0;
  width = 0;
  count = 0;
  page = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
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
    switch (code) {
    case 3:
      lineSpacingCounts[line] = ++count;
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      if (line < 3)
        line++;
      pageHeights[page] += 15;
      count = 0;
      width = 0;
      break;
    case 0:
      lineSpacingCounts[line] = ++count;
      lineWidths[line] = width;
      if (page == 0 && maxWidth < width)
        maxWidth = width;
      page++;
      goto done;
    case 1:
      lineSpacingCounts[line] = ++count;
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
  if (work->mode != 0)
    maxWidth += 2;
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
        *spacing = 0;
      } else {
        gap = maxWidth - lineWidths[lineIndex];
        gap -= 4;
        if (gap < 0)
          gap = 0;
        *spacing = (gap << 8) / (lineSpacingCounts[lineIndex] - 1);
      }
      spacing++;
    }
  }
  Text_ResetStyle();
}
