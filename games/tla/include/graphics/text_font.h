#ifndef GUARD_TEXT_FONT_H
#define GUARD_TEXT_FONT_H

#include "edition.h"
#include "types.h"

/// In Japanese, the glyphs of character codes from 256: KANJI_GLYPH_SIZE bytes
/// each, drawn KANJI_WIDTH pixels wide.
#define KANJI_GLYPH_SIZE 24
#define KANJI_WIDTH 10
/// The Japanese font's kanji, from KANJI_FIRST_CODE on.
extern const u8 gFont_KanjiGlyphs[];
extern const u16 gFont_MarkGlyphCodes[];

/// The marks a kana is drawn with: their character codes.
enum TextMarkGlyph {
  TEXT_MARK_NONE = 0,
  TEXT_MARK_VOICED = 222,    ///< the voiced mark (dakuten)
  TEXT_MARK_SEMIVOICED = 223 ///< the semi-voiced mark (handakuten)
};

/// Japanese text carries a kana's mark in the top bits of its code, as its
/// index in gFont_MarkGlyphCodes.
#define TEXT_MARK_SHIFT 14

#endif
