/// @file
/// The staff credits font's advance widths in pixels, characters 0x20 to
/// 0x7f, beside the font (credits.png). slides.c includes it after its line
/// tables.
// ☀️'s credits.c includes its own copy the same way.
// clang-format off
#include "edition.h"
const u8 gDisplayScroll_GlyphWidths[96] = {
    7, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 6, 4, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 6, 5, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 6, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 5, 6, 5, 7, 7,
    6, 7, 7, 7, 7, 7, 5, 7, 7, 4, 4, 6, 4, 8, 7, 7,
#if LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
    // Their fonts draw a capital A with an accent at 0x7f.
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 8,
#else
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1,
#endif
};
// clang-format on
