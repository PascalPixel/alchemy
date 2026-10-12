#ifndef DEBUG_H
#define DEBUG_H

/* DEBUG */

#include "unit/graphics/window/sideobjt.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
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
