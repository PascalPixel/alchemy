#ifndef DEBUG_H
#define DEBUG_H

/* DEBUG */

#include "unit/battle/effect/object2.h"
#include "types.h"
#include "edition.h"
#include "farcall.h"

struct ItemDefinition;
struct RenderInput;

extern const u8		   Debug_PaletteSwatchTiles[];
void				   Engine_DebugClearWindow(struct RenderInput *window);
void				   Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);
u8					  *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void				   Engine_DebugRedrawWindow(struct RenderInput *window);

#endif
