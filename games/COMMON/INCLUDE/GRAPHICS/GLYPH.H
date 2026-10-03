#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

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
