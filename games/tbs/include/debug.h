#ifndef DEBUG_H
#define DEBUG_H

/// @file
/// The engine calls the debug menus import, and their palette swatch tiles.

#include "unit/object/object.h"
#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct UiWindow;

extern const u8 gDebug_PaletteSwatchTiles[];
void Engine_DebugClearWindow(struct UiWindow *window);
void Engine_DebugDrawItemDetails(struct UiWindow *window, s32 item);
u8 *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_DebugRedrawWindow(struct UiWindow *window);

#endif
