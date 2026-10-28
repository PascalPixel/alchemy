#ifndef GUARD_TEXT_FONT_H
#define GUARD_TEXT_FONT_H

#include "edition.h"
#include "types.h"

extern const u8 gFont_KanjiGlyphs[];
extern const u16 gFont_MarkGlyphCodes[];

enum TextMarkGlyph {
  TEXT_MARK_NONE = 0,
  TEXT_MARK_VOICED = 0xDE,    ///< the voiced mark (dakuten)
  TEXT_MARK_SEMIVOICED = 0xDF ///< the semi-voiced mark (handakuten)
};

/// Japanese text carries a kana's mark in the top bits of its code, as its
/// index in gFont_MarkGlyphCodes.
#define TEXT_MARK_SHIFT 14

#endif
