#include "types.h"
#include "edition.h"

// The rows of a glyph drawn: all twelve of the Japanese font's, and fourteen of
// the European fonts' fifteen.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PLOT_GLYPH_ROWS 12
#else
#define PLOT_GLYPH_ROWS 14
#endif

void Text_PlotGlyphRows(u16 *rows, u8 *dst, s32 color) {
  s32 y;
  u8 *p;
  s32 bits;

  for (y = 0; y != PLOT_GLYPH_ROWS; y++) {
    bits = *rows++ & ~3;
    p = dst;
    bits <<= 16;
    while (bits != 0) {
      if (bits < 0)
        *p = color;
      bits <<= 1;
      p++;
    }
    dst += 192;
  }
}
