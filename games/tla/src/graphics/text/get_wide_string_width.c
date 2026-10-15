//--------------------------------------------------------------------------
// get_wide_string_width.c
// 文字：16 ビット文字列の表示幅（ドット）を求める
//--------------------------------------------------------------------------

#include "edition.h"
#include "glyph.h"

// 独・西・仏・伊版は 0xff 以下の文字をすべて 10 ドット、それより上を 0 で数える
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) ||                      \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) ||                      \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define WIDE_WIDTH_FIXED 1
#endif

// フォントの n 番目の文字
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gUiText_Glyphs))

//--------------------------------------------------------------------------
// 空白は 4 ドット、0xff 以下の文字はフォントの幅、それより上は 10 ドット
// 0xde・0xdf（濁点・半濁点）は幅に数えない
//--------------------------------------------------------------------------
s32 UiText_GetWideStringWidth(u16 *text) {
  s32 width;
  u16 *p;
  u32 c;
#if !defined(WIDE_WIDTH_FIXED)
  u32 idx;
#endif

  c = *text;
  width = 0;
  p = text + 1;
  if (c != 0) {
    do {
      if (c == 0x20) {
        width += 4;
      } else if (c <= 0xffU) {
#if defined(WIDE_WIDTH_FIXED)
        width += 10;
#else
        idx = c - 0xde;
        if (idx > 1U) {
          idx += 0xbe; // フォントは 0x20 から
          width += FONT_GLYPH(idx)->width;
        }
      } else {
        width += 10;
#endif
      }
      c = *p;
      p += 1;
    } while (c != 0);
  }
  return width;
}
