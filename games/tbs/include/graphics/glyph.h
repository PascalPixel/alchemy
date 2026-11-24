#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

/// @file
/// The text font's characters, and the buffers and shared palette through
/// which the icons beside text (commands, items, Psynergy, status) are drawn.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/// A main-font character's height in rows of pixels: the Japanese font is 12
/// tall, the other languages' 15.
#if LANGUAGE == LANGUAGE_JAPANESE
enum { FONT_GLYPH_ROWS = 12 };
#else
enum { FONT_GLYPH_ROWS = 15 };
#endif

/// IconPalette.slot of an icon colour given no slot yet.
#define ICON_PALETTE_SLOT_UNSET 255
/// The colours an icon's 256-colour pixels hold, each with a IconPalette.slot
/// entry.
#define ICON_PALETTE_COLORS 256

/// A compact palette shared by a selection's command icons: the slot each
/// icon colour was given, or ICON_PALETTE_SLOT_UNSET for none yet, and how
/// many slots are taken.
struct IconPalette {
  u8 slot[ICON_PALETTE_COLORS];
  s32 count;
};

STATIC_ASSERT(sizeof(struct IconPalette) == 260);
STATIC_ASSERT(offsetof(struct IconPalette, count) == 256);

/// A character of the text font: its width and its rows of pixels.
struct FontGlyph {
  u16 width;                 ///< pixels
  u16 rows[FONT_GLYPH_ROWS]; ///< its pixels, a halfword a row, top first
} __attribute__((packed, aligned(2)));

STATIC_ASSERT(offsetof(struct FontGlyph, rows) == 2);
STATIC_ASSERT(sizeof(struct FontGlyph) == (2 + FONT_GLYPH_ROWS * 2));

/// The main font, from character code 32: its glyphs in code order.
extern const struct FontGlyph gFont_Glyphs[];

/// The buffers an icon (command, item, Psynergy or status) or a portrait is
/// decoded through, held in HEAP_SLOT_ICON_TRANSFER while one icon is drawn:
/// Icon_Decode unpacks encoded into input, then expands input into tiles.
typedef struct {
  /// The icon unpacked by Tile_Decompress4bpp; a 256-colour command icon
  /// (ResourceId_CommandIcons) is unpacked here by Decompress_ByteLz and
  /// used as it is.
  u8 input[1024];
  u8 tiles[512]; ///< the icon as sprite tiles, ready for VRAM
  s16 width;     ///< tiles
  s16 height;    ///< tiles
  u8 *encoded;   ///< the compressed icon
} IconTransfer;

STATIC_ASSERT(sizeof(IconTransfer) == 1544);
STATIC_ASSERT(offsetof(IconTransfer, tiles) == 1024);
STATIC_ASSERT(offsetof(IconTransfer, width) == 1536);
STATIC_ASSERT(offsetof(IconTransfer, encoded) == 1540);

/// A 16-pixel square icon's tiles at 4 bits a pixel, what VRAM takes of
/// IconTransfer.tiles for an item, Psynergy or status icon.
#define ICON_TILE_BYTES (16 * 16 / 2)

#endif
