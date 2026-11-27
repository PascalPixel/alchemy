#ifndef ALCHEMY_TEXT_FONT_H
#define ALCHEMY_TEXT_FONT_H

/* The Japanese font's kanji, and the voiced marks drawn over its kana. */

#include "edition.h"
#include "types.h"

/* In Japanese, the glyphs of character codes from KANJI_FIRST_CODE:
   KANJI_GLYPH_SIZE bytes each, drawn KANJI_WIDTH pixels wide. */
#define KANJI_GLYPH_SIZE 24
#define KANJI_WIDTH		 10
/* The Japanese font's kanji, from KANJI_FIRST_CODE on. */
extern const Uint8 FontKanjiGlyphs[];

/* The mark each of a kana's top-bit values stands for (TEXT_MARK_SHIFT): none,
   voiced, semi-voiced, none. */
extern const Uint16 FontMarkGlyphCodes[];

/* The marks a kana is drawn with: their character codes. */
typedef enum TEXT_MARK_GLYPH {
	TEXT_MARK_NONE = 0,
	TEXT_MARK_VOICED = 222,	   /* the voiced mark (dakuten) */
	TEXT_MARK_SEMIVOICED = 223 /* the semi-voiced mark (handakuten) */
} TEXT_MARK_GLYPH;

/* Japanese text carries a kana's mark in the top bits of its code, as its index
   in FontMarkGlyphCodes. */
#define TEXT_MARK_SHIFT 14

#endif
