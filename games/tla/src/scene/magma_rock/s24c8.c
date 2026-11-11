#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void Func_020024c8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(temporary): one local zero for the velocity store and, through a
   * 64-bit copy, the call's argument, so r1 is cleared after the owner's load;
   * as plain zeros, one pair is reordered (not measured as N of M). */
  u32 zero = 0;

  if (Event_GetFlag(0x109)) {
    s32 height;

    System_Wait(1);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->velocityY = zero;
    object->terrainHeight = height;
    object->y = height;
    object->targetY = PIXELS(-32768);
    {
      s64 flags = zero;

      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, flags);
    }
  }
}
