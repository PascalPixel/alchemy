/// @file
/// The window tilemap's text tiles that code names: window.h and the menus
/// that draw a label's tiles without the rest of the window system read it.
#ifndef ALCHEMY_WINDOW_TILE_H
#define ALCHEMY_WINDOW_TILE_H

#include "edition.h"

/// A window tilemap entry: the tile in palette 15, the text tiles' palette.
#define WINDOW_TILE(tile) (0xf000 | (tile))

/// The HP and PP labels before a number (Text_DrawPrefixedNumberAtOffset)
/// or a Psynergy's cost: each label's first tile, and the second tile the
/// HP label always takes, as ☀️'s.
enum {
  WINDOW_TILE_HP_LABEL = WINDOW_TILE(29),
  WINDOW_TILE_HP_LABEL_END = WINDOW_TILE(30),
  WINDOW_TILE_PP_LABEL = WINDOW_TILE(31),
};

/// The PP label's second tile: the HP label's in English, German and
/// Japanese, a tile of its own in Spanish, French and Italian.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE(5)
#else
#define WINDOW_TILE_PP_LABEL_END WINDOW_TILE_HP_LABEL_END
#endif

#endif // ALCHEMY_WINDOW_TILE_H
