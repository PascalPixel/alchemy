#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

#include "TYPES.H"
#include "EDITION.H"
#include "LAYOUT_GUARD.H"

#if EDITION_INTERNATIONAL
enum { FONT_GLYPH_ROWS = 15 };
#else
enum { FONT_GLYPH_ROWS = 12 };
#endif

/* Text fonts store their advance width followed by the monochrome rows.
   Records are halfword aligned; the Japanese 26-byte stride has no tail pad. */
struct FontGlyph {
    u16 width;
    u16 rows[FONT_GLYPH_ROWS];
} __attribute__((packed, aligned(2)));

LAYOUT_OFFSET_GUARD(FontGlyph_Rows, struct FontGlyph, rows, 2);
LAYOUT_SIZE_GUARD(FontGlyph_Size, struct FontGlyph, 2 + FONT_GLYPH_ROWS * 2);

extern const struct FontGlyph UiText_Glyphs[];

/* The glyph loader decodes the input, then expands it into 4bpp tiles. */
typedef struct {
    u8 input[0x400];
    u8 tiles[0x200];
    s16 width;
    s16 height;
    u8 *encoded;
} GlyphTransfer;

LAYOUT_SIZE_GUARD(GlyphTransfer_Size, GlyphTransfer, 0x608);
LAYOUT_OFFSET_GUARD(GlyphTransfer_Tiles, GlyphTransfer, tiles, 0x400);
LAYOUT_OFFSET_GUARD(GlyphTransfer_Width, GlyphTransfer, width, 0x600);
LAYOUT_OFFSET_GUARD(GlyphTransfer_Encoded, GlyphTransfer, encoded, 0x604);

#endif
