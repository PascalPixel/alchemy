#ifndef ALCHEMY_TITLE_H
#define ALCHEMY_TITLE_H

/// @file
/// The Golden Sun logo the title and the Clear scene show.

#include "system/io_reg.h"

/// The logo: an 8-bit background on BG2, its map in screen block 6 and its
/// tiles from tile 416 of character block 0.
#define LOGO_BGCNT (BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6))
#define LOGO_MAP ((u16 *)BG_SCREEN_BLOCK(6))
#define LOGO_FIRST_TILE 416
#define LOGO_TILES (BG_CHAR_BLOCK(0) + LOGO_FIRST_TILE * TILE_SIZE_8BPP)
/// The map entry after each row's 30 visible tiles.
#define LOGO_BLANK_TILE 511
/// The logo's palette, at the start of its resource before its tiles.
#define LOGO_PALETTE_COLORS 224

#endif
