#include "types.h"
#include "edition.h"

void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color);

#if (LANGUAGE == LANGUAGE_JAPANESE)
#include "glyph.h"
#include "text_font.h"

/// The first kanji's code; the codes below it are the font's glyphs.
#define KANJI_FIRST_CODE 256
/// Every kanji is drawn 10 pixels wide.
#define KANJI_WIDTH 10

/// Draws a character on the canvas at (x, row) with its shadow and returns its
/// width: a font glyph, or a kanji from KANJI_FIRST_CODE on, with the mark its
/// code carries above TEXT_MARK_SHIFT drawn over it one pixel wider.
s32 Func_0803cfd0(u8 *canvas, u32 ch, s32 x, s32 row) {
  const u16 *mark = NULL;
  const u16 *rows;
  s32 width;
  s32 markCode = gFont_MarkGlyphCodes[ch >> TEXT_MARK_SHIFT];

  if (markCode != 0) {
    markCode -= 32;
    mark = gFont_Glyphs[markCode].rows;
  }
  ch &= (1 << TEXT_MARK_SHIFT) - 1;
  if (ch < KANJI_FIRST_CODE) {
    s32 index = ch - 32;

    // The width, then the rows after it.
    rows = &gFont_Glyphs[index].width;
    width = *rows++;
  } else {
    s32 index = ch - KANJI_FIRST_CODE;

    width = KANJI_WIDTH;
    rows = (const u16 *)(gFont_KanjiGlyphs + index * (FONT_GLYPH_ROWS * 2));
  }
  canvas += x + row * 192;
  Text_PlotGlyphRows((u16 *)rows, canvas + 3 * 192 + 1, 1);
  if (mark != NULL)
    Text_PlotGlyphRows((u16 *)mark, canvas + 192 + 8, 1);
  Text_PlotGlyphRows((u16 *)rows, canvas + 2 * 192, 15);
  if (mark != NULL) {
    Text_PlotGlyphRows((u16 *)mark, canvas + 7, 15);
    width++;
  }
  return width;
}
#else
extern u16 gFont_Glyphs[];

s32 Func_0803cfd0(u8 *canvas, s32 ch, s32 x, s32 row) {
  u16 *glyph = (u16 *)((u8 *)gFont_Glyphs + (((ch & 0xff) - 32) << 5));
  s32 width = *glyph;
  u8 *dst = canvas + (x + row * 192);
  u8 *rows;

  glyph++;
  rows = dst + 3 * 192 + 1;
  Text_PlotGlyphRows(glyph, rows, 1);
  Text_PlotGlyphRows(glyph, rows = dst + 2 * 192, 15);
  return width;
}
#endif
