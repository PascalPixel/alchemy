/// @file
/// The scene code of overlay 0x684, in ROM order.

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
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020000b8(void);

void Func_02000054(void) { Func_020000b8(); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_02000064(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 513;
  Actor_SetSpritePriority(8, 2);
  Actor_GetById(8)->collisionFlags |= 4;
  Actor_GetById(8)->collisionFlags |= 16;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
