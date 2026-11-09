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

void Func_02000cec(void) {
  GameFlag_SetBit(0x271);
  Func_080d2b4c(17, 17);
  return;
}

void Func_02001870();
void Func_02001878();

void Func_02000d04(void) {
  Func_02001870();
  Event_ResolvePendingActions(0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x101, 32);
  Func_02001878();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }
