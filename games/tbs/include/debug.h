#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

/// @file
/// The main-image calls the debug menus import, and their palette swatch tiles.

#include "field/field_sequence.h"
#include "menu/menu.h"
#include "game/game.h"
#include "window/window.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 208 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];

#endif
