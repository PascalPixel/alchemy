#include "EDITION.H"
#include "TEXT_FONT.H"

/* The top two bits of a Japanese glyph code select its voicing mark. */
#if !EDITION_INTERNATIONAL
const u16 UiText_MarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_DAKUTEN,
    TEXT_MARK_HANDAKUTEN,
    TEXT_MARK_NONE
};
#endif
