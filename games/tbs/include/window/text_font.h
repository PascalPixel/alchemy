#ifndef ALCHEMY_TEXT_FONT_H
#define ALCHEMY_TEXT_FONT_H

/* The kanji glyphs and the text marks drawn with characters. */

#include "edition.h"
#include "types.h"

/* In Japanese, the glyphs of character codes from 256: KANJI_GLYPH_SIZE bytes
   each, drawn KANJI_WIDTH pixels wide. */
extern const Uint8 FontKanjiGlyphs[];
#define KANJI_GLYPH_SIZE 24 /* bytes */
#define KANJI_WIDTH		 10 /* pixels */

/* The marks a kana is drawn with: their character codes. */
typedef enum TEXT_MARK_GLYPH {
	TEXT_MARK_NONE = 0,
	TEXT_MARK_VOICED = 222,	   /* the voiced mark (dakuten) */
	TEXT_MARK_SEMIVOICED = 223 /* the semi-voiced mark (handakuten) */
} TEXT_MARK_GLYPH;

#if LANGUAGE == LANGUAGE_JAPANESE
/* The character codes of kana that start runs in the font's order
   (charmap_ja.tsv), as the File Screen's password spells from them. */
#define KANA_WO 134 /* を */
#define KANA_A	145 /* あ, then い to そ */
#define KANA_KA 150 /* か, then き to そ */
#define KANA_TA 224 /* た, then ち to ん */
#define KANA_TE 227 /* て */
#define KANA_TO 228 /* と */
#define KANA_HA 234 /* は, then ひ to ほ */
#endif

#endif
