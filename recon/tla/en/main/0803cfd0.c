// Draft, differ 2 of 35 (2026-10-18; was 11): the second destination is a
// temporary of its own. Left: the native loads the font address before it
// subtracts 32 from the code (the lane's load-earlier near misses); a
// barrier on the code and the font gets that order but moves the registers
// (11 to 14).
#include "types.h"

extern u16 gUiText_Glyphs[];

void UiText_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

s32 UiText_DrawShadowedGlyph(u8 *canvas, s32 ch, s32 x, s32 row) {
  u16 *glyph = (u16 *)((u8 *)gUiText_Glyphs + (((ch & 0xff) - 32) << 5));
  s32 width = *glyph;
  u8 *dst = canvas + (x + row * 192);
  u8 *rows;

  glyph++;
  rows = dst + 3 * 192 + 1;
  UiText_PlotGlyphRows(glyph, rows, 1);
  UiText_PlotGlyphRows(glyph, rows = dst + 2 * 192, 15);
  return width;
}
