#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002bd4(void);
void Func_020009f8(s32 id, s32 x, s32 z, s32 dz, s32 dx);
void Func_020002f0(void);

/// Drops objects 18 and 19 once any of flags 2576 to 2581 is set: the camera
/// moves to (592, 248), each object drops (Func_020009f8) at the spot whose
/// flag is set ((35, 15), (31, 16) or (33, 10)), the six flags are cleared,
/// Func_020002f0 resets the room, both objects are waited on until they land
/// (at most 30 frames each) and stop, and the camera follows the leader again.
void Func_02000a74(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(2576) + Event_GetFlag(2578) + Event_GetFlag(2580) +
          Event_GetFlag(2577) + Event_GetFlag(2579) + Event_GetFlag(2581) ==
      0)
    return;
  FieldCamera_MoveTo(148 << 18, -1, PIXELS(248), 1);
  Func_02002bd4();
  FieldEvent_Wait(10);
  if (Event_GetFlag(2576))
    Func_020009f8(18, 35, 15, -16, 0);
  if (Event_GetFlag(2577))
    Func_020009f8(19, 35, 15, -16, 0);
  if (Event_GetFlag(2578))
    Func_020009f8(18, 31, 16, 16, 0);
  if (Event_GetFlag(2579))
    Func_020009f8(19, 31, 16, 16, 0);
  if (Event_GetFlag(2580))
    Func_020009f8(18, 33, 10, -16, 0);
  if (Event_GetFlag(2581))
    Func_020009f8(19, 33, 10, -16, 0);
  Event_ClearFlag(2576);
  Event_ClearFlag(2577);
  Event_ClearFlag(2578);
  Event_ClearFlag(2579);
  Event_ClearFlag(2580);
  Event_ClearFlag(2581);
  Func_020002f0();
  System_Wait(2);
  object = Actor_Get(18);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  object = Actor_Get(19);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  FieldEvent_Wait(30);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02002bd4();
}
