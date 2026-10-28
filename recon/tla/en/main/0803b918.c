// Draft, differ 8 of 217 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search).
// Near miss, differ 10 of 217: The Broken Seal's international
// Text_MeasureEntry with ⚓️'s changes: code 176 is a control
// code, a page break (1) resets the counters before it ends the text, a text
// mode window taller than 44 pixels keeps two lines and flags spacing[3], and
// the counters are reset at the end. With case 1 before case 3 the call
// block moves (differ 30). The barrier keeps the prologue's parameter
// spills before the work pointer is started (differ 14 without it, 12 with a
// one-pass block). Left: the lineWidths address
// is built and spilled late, and in case 9 the jump table reloads after the
// mask is started: scheduling only.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"

void Text_ResetStyle(void);

void Text_MeasureEntry(s32 entryIndex, u32 *outWidth,
                                   u32 *outHeight, u16 *spacing) {
  struct WindowWork *work;
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

  /* FAKEMATCH: the parameter spills are stored before the work load. */
  asm("");
  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  width = 0;
  height = 15;
  lines = 0;
  lineWidth = 0;
  count = 0;
  for (;;) {
    code = work->entries[entryIndex];
    entryIndex = (entryIndex + 1) & (UI_TEXT_ENTRY_COUNT - 1);
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
    switch (code) {
    case 3:
      count++;
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
  count++;
  lineSpacingCounts[lines] = count;
  lineWidths[lines] = lineWidth;
  if (width < lineWidth)
    width = lineWidth;
  if (work->mode)
    width = width + 2;
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
        gap = ((gap << 8) / (lineSpacingCounts[lineIndex] - 1));
        if (gap < 0 || gap > 0xc00)
          gap = 0x200;
        *spacing++ = gap;
      }
      lineIndex++;
    }
  }
  Text_ResetStyle();
}
