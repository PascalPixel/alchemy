#include "EDITION.H"
#include "GLYPH.H"

/* The European editions give every single-byte character the full ten
   pixels and count nothing for the rest. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || \
    defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || \
    defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define WIDE_WIDTH_FIXED 1
#endif

s32 UiText_GetWideStringWidth(u16 *text)
{
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
            } else if (c <= 0xFFU) {
#if defined(WIDE_WIDTH_FIXED)
                width += 0xA;
#else
                idx = c - 0xDE;
                if (idx > 1U) {
                    idx += 0xBE;
                    width += ((const struct FontGlyph *)
                        (idx * sizeof(struct FontGlyph) +
                        (const u8 *)UiText_Glyphs))->width;
                }
            } else {
                width += 0xA;
#endif
            }
            c = *p;
            p += 1;
        } while (c != 0);
    }
    return width;
}
