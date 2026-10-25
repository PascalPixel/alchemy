#include "types.h"

extern u16 gUiText_Glyphs[];

void UiText_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

s32 Func_0803cfd0(u8 *canvas, s32 ch, s32 x, s32 row) {
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
