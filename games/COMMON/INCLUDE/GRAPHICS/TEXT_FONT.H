#ifndef GUARD_TEXT_FONT_H
#define GUARD_TEXT_FONT_H

#include "TYPES.H"

extern const u8 UiText_KanjiGlyphs[];
extern const u16 UiText_MarkGlyphCodes[];

/* These glyph codes are the two Japanese voicing-mark records in TEXT_JA. */
enum TextMarkGlyph {
    TEXT_MARK_NONE = 0,
    TEXT_MARK_DAKUTEN = 0xDE,
    TEXT_MARK_HANDAKUTEN = 0xDF
};

#endif
