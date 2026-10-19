#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

/// @file
/// Font glyphs and their palettes.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

#if LANGUAGE == LANGUAGE_JAPANESE
enum { FONT_GLYPH_ROWS = 12 };
#else
enum { FONT_GLYPH_ROWS = 15 };
#endif

/// A compact palette shared by a selection's command icons: the slot each
/// icon colour was given, or 255 for none yet, and how many slots are taken.
struct GlyphPalette {
  u8 slot[256];
  s32 count;
};

STATIC_ASSERT(sizeof(struct GlyphPalette) == 0x104);
STATIC_ASSERT(offsetof(struct GlyphPalette, count) == 0x100);

/// A character of the text font: its width and its rows of pixels.
struct FontGlyph {
  u16 width; ///< pixels
  u16 rows[FONT_GLYPH_ROWS];
} __attribute__((packed, aligned(2)));

STATIC_ASSERT(offsetof(struct FontGlyph, rows) == 2);
STATIC_ASSERT(sizeof(struct FontGlyph) == (2 + FONT_GLYPH_ROWS * 2));

extern const struct FontGlyph gUiText_Glyphs[];

typedef struct {
  u8 input[0x400];
  u8 tiles[0x200];
  s16 width;
  s16 height;
  u8 *encoded;
} GlyphTransfer;

STATIC_ASSERT(sizeof(GlyphTransfer) == 0x608);
STATIC_ASSERT(offsetof(GlyphTransfer, tiles) == 0x400);
STATIC_ASSERT(offsetof(GlyphTransfer, width) == 0x600);
STATIC_ASSERT(offsetof(GlyphTransfer, encoded) == 0x604);

#endif
