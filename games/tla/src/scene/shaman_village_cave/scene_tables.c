#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Func_0200004c(s32 arg0, s32 arg1) {
  if (arg1 != 8)
    goto L_02008072;
  Event_SetFlag(0x9a9);
  MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, arg1);
L_02008072:
  if (arg1 != 10)
    goto L_02008094;
  Event_SetFlag(0x9aa);
  MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
L_02008094:
  return;
}
