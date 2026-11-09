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
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s16 gData_02002c5c[];
extern s16 gData_02002efc[];

const s16 *Func_0200004c(void) {
  if (Event_GetFlag(0x97f))
    return gData_02002efc;
  return gData_02002c5c;
}
