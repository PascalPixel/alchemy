#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 Func_020005dc(s32 terrain, s32 x, s32 z, s32 height);

void Func_02000880(u16 *list) {
  for (; *list != 0xffff; list += 3) {
    struct Actor *object = Actor_Get(*list);
    s32 height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_020005dc(object->terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
  }
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }
