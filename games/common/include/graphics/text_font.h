#ifndef GUARD_TEXT_FONT_H
#define GUARD_TEXT_FONT_H

#include "edition.h"
#include "types.h"

// Text windows lay text out on their 8x8-pixel tiles: the text writers place
// pixel column x in cell x / 8, at pixel x % 8 within it.
#define TEXT_CELL_WIDTH 8

extern const u8 gUiText_KanjiGlyphs[];
#if GAME_TLA
extern const u16 gUiText_MarkGlyphCodes[];
#endif

enum TextMarkGlyph {
  TEXT_MARK_NONE = 0,
  TEXT_MARK_VOICED = 0xDE,    // the voiced mark (dakuten)
  TEXT_MARK_SEMIVOICED = 0xDF // the semi-voiced mark (handakuten)
};

#endif
