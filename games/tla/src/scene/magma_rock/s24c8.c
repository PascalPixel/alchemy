#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void Func_020024c8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (Event_GetFlag(0x109)) {
    s32 height;

    System_Wait(1);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->velocityY = 0;
    object->terrainHeight = height;
    object->y = height;
    object->targetY = PIXELS(-32768);
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}
