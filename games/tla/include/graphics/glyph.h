#ifndef ALCHEMY_GLYPH_H
#define ALCHEMY_GLYPH_H

/* The main font's glyphs, and the palette and buffers icons are built in. */

#include "types.h"
#include "edition.h"
#include "static_assert.h"

#if LANGUAGE == LANGUAGE_JAPANESE
/* A glyph's height in rows of pixels: 12 in Japanese. */
enum { FONT_GLYPH_ROWS = 12 };
#else
/* A glyph's height in rows of pixels: 15 outside Japanese. */
enum { FONT_GLYPH_ROWS = 15 };
#endif

/* ICON_PALETTE.slot of an icon colour given no slot yet. */
#define GLYPH_PALETTE_SLOT_FREE 255

/* A compact palette shared by a selection's command icons: the slot each icon
   colour was given, or GLYPH_PALETTE_SLOT_FREE for none yet, and how many slots
   are taken. */
typedef struct ICON_PALETTE {
	Uint8  slot[256];
	Sint32 cnt;
} ICON_PALETTE;

STATIC_ASSERT(sizeof(ICON_PALETTE) == 260);
STATIC_ASSERT(offsetof(ICON_PALETTE, cnt) == 256);

/* A character of the text font: its width and its rows of pixels. */
typedef struct GLYPH {
	Uint16 width;				  /* pixels */
	Uint16 rows[FONT_GLYPH_ROWS]; /* its pixels, a halfword a row, top first */
} __attribute__((packed, aligned(2))) GLYPH;

STATIC_ASSERT(offsetof(GLYPH, rows) == 2);
STATIC_ASSERT(sizeof(GLYPH) == (2 + FONT_GLYPH_ROWS * 2));

/* The main font, from character code 32: its glyphs in code order. */
extern const GLYPH FontGlyphs[];

/* The buffers an icon or a portrait is decoded through, in
   HEAP_SLOT_ICON_TRANSFER for one build: mnIconDecode unpacks encoded into
   input, then expands input into tiles. */
typedef struct {
	/* The icon unpacked a byte a pixel; a 256-colour command icon
	   (ResourceIdCommandIcons) is unpacked here by syDecompressByteLZ and used
	   as it is. */
	Uint8  input[1024];
	Uint8  tiles[512]; /* the icon as sprite tiles, ready for VRAM */
	Sint16 width;	   /* tiles */
	Sint16 height;	   /* tiles */
	Uint8 *encoded;	   /* the compressed icon */
} ICON_TRANSFER;

STATIC_ASSERT(sizeof(ICON_TRANSFER) == 1544);
STATIC_ASSERT(offsetof(ICON_TRANSFER, tiles) == 1024);
STATIC_ASSERT(offsetof(ICON_TRANSFER, width) == 1536);
STATIC_ASSERT(offsetof(ICON_TRANSFER, encoded) == 1540);

/* A 16-pixel square icon's tiles at 4 bits a pixel, what VRAM takes of
   ICON_TRANSFER.tiles for an item, Psynergy or status icon. */
#define ICON_TILE_BYTES (16 * 16 / 2)

#endif
