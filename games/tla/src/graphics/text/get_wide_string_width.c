//--------------------------------------------------------------------------
// get_wide_string_width.c
// Text: measure a 16-bit string's display width in pixels.
//--------------------------------------------------------------------------

#include "edition.h"
#include "glyph.h"

// German, Spanish, French, and Italian use ten pixels for nonspace characters
// up to 0xff and zero for characters above it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN) ||                      \
    (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define WIDE_WIDTH_FIXED 1
#endif

// Glyph n in the font.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gUiText_Glyphs))

//--------------------------------------------------------------------------
// Spaces take four pixels; characters up to 0xff use their glyph width;
// higher characters take ten. Voicing marks 0xde and 0xdf add no width.
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
          idx += 0xbe; // The font starts at character 0x20.
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
