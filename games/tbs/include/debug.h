#ifndef DEBUG_H
#define DEBUG_H

/// @file
/// The engine calls the debug menus import, and their palette swatch tiles.

#include "unit/object/object.h"
#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct Window;

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 0xd0 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];
void Engine_DebugClearWindow(struct Window *window);
void Engine_DebugDrawItemDetails(struct Window *window, s32 item);
u8 *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_DebugRedrawWindow(struct Window *window);

#endif
