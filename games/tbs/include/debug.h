#ifndef DEBUG_H
#define DEBUG_H

/// @file
/// The main-image calls the debug menus import, and their palette swatch tiles.

#include "module/field/field_sequence.h"
#include "module/menu/menu.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct Window;

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 0xd0 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];

#endif
