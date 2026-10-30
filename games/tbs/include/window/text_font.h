#ifndef ALCHEMY_TEXT_FONT_H
#define ALCHEMY_TEXT_FONT_H

/// @file
/// The kanji glyphs and the text marks drawn with characters.

#include "edition.h"
#include "types.h"

/// In Japanese, the glyphs of character codes from 256: 24 bytes each, drawn
/// 10 pixels wide.
extern const u8 gFont_KanjiGlyphs[];

/// The marks a kana is drawn with: their character codes.
enum TextMarkGlyph {
  TEXT_MARK_NONE = 0,
  TEXT_MARK_VOICED = 222,    ///< the voiced mark (dakuten)
  TEXT_MARK_SEMIVOICED = 223 ///< the semi-voiced mark (handakuten)
};

#endif
