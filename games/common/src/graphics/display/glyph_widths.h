// The staff credits font's advance widths in pixels, characters 0x20 to
// 0x7f, beside the font (credits.png). Both games' slides.c include it
// after their line tables.
const u8 DisplayScroll_GlyphWidths[96] = {
    7, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 6, 4, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 6, 5, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 6, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 5, 6, 5, 7, 7,
    6, 7, 7, 7, 7, 7, 5, 7, 7, 4, 4, 6, 4, 8, 7, 7,
#if defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    // Their fonts draw a capital A with an accent at 0x7f.
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 8,
#else
    7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1,
#endif
};
