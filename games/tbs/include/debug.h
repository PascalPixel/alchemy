#ifndef DEBUG_H
#define DEBUG_H

/// @file
/// The main-image calls the debug menus import, and their palette swatch tiles.

#include "unit/overworld/overworld.h"
#include "unit/menu/menu.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct Window;

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 0xd0 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];

#endif
