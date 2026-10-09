#ifndef GUARD_TEXT_FONT_H
#define GUARD_TEXT_FONT_H

#include "types.h"

extern const u8	 UiText_KanjiGlyphs[];
extern const u16 UiText_MarkGlyphCodes[];

enum TextMarkGlyph
{
	TEXT_MARK_NONE = 0,
	TEXT_MARK_DAKUTEN = 0xDE,
	TEXT_MARK_HANDAKUTEN = 0xDF
};

#endif
