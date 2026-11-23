#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

/// @file
/// The main font's glyphs, and the palette and buffers icons are built in.

#include "types.h"
#include "edition.h"
#include "static_assert.h"

#if LANGUAGE == LANGUAGE_JAPANESE
/// A glyph's height in rows of pixels: 12 in Japanese.
enum { FONT_GLYPH_ROWS = 12 };
#else
/// A glyph's height in rows of pixels: 15 outside Japanese.
enum { FONT_GLYPH_ROWS = 15 };
#endif

/// GlyphPalette.slot of an icon colour given no slot yet.
#define GLYPH_PALETTE_SLOT_FREE 255

/// A compact palette shared by a selection's command icons: the slot each
/// icon colour was given, or GLYPH_PALETTE_SLOT_FREE for none yet, and how
/// many slots are taken.
struct GlyphPalette {
  u8 slot[256];
  s32 count;
};

STATIC_ASSERT(sizeof(struct GlyphPalette) == 260);
STATIC_ASSERT(offsetof(struct GlyphPalette, count) == 256);

/// A character of the text font: its width and its rows of pixels.
struct FontGlyph {
  u16 width;                 ///< pixels
  u16 rows[FONT_GLYPH_ROWS]; ///< its pixels, a halfword a row, top first
} __attribute__((packed, aligned(2)));

STATIC_ASSERT(offsetof(struct FontGlyph, rows) == 2);
STATIC_ASSERT(sizeof(struct FontGlyph) == (2 + FONT_GLYPH_ROWS * 2));

/// The main font, from character code 32: its glyphs in code order.
extern const struct FontGlyph gFont_Glyphs[];

/// The buffers an icon or a portrait is decoded through, in
/// HEAP_SLOT_GLYPH_TRANSFER for one build: Icon_Decode unpacks encoded into
/// input, then expands input into tiles.
typedef struct {
  /// The icon unpacked a byte a pixel; a 256-colour command icon
  /// (ResourceId_CommandIcons) is unpacked here by Decompress_ByteLz and used
  /// as it is.
  u8 input[1024];
  u8 tiles[512]; ///< the icon as sprite tiles, ready for VRAM
  s16 width;     ///< tiles
  s16 height;    ///< tiles
  u8 *encoded;   ///< the compressed icon
} GlyphTransfer;

STATIC_ASSERT(sizeof(GlyphTransfer) == 1544);
STATIC_ASSERT(offsetof(GlyphTransfer, tiles) == 1024);
STATIC_ASSERT(offsetof(GlyphTransfer, width) == 1536);
STATIC_ASSERT(offsetof(GlyphTransfer, encoded) == 1540);

/// A 16-pixel square icon's tiles at 4 bits a pixel, what VRAM takes of
/// GlyphTransfer.tiles for an item, Psynergy or status icon.
#define ICON_TILE_BYTES (16 * 16 / 2)

#endif
