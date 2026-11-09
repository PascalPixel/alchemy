#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001f74(void) {
  struct Actor *object = Actor_Get(10);

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(10);
  System_Wait(1);
  object->motionFlags = 0;
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  FieldActor_SetPositionAndResetMotion(object, PIXELS(268), PIXELS(64), PIXELS(390));
  System_Wait(1);
  object->terrainHeight = PIXELS(128);
  System_Wait(1);
  object->y = PIXELS(160);
  System_Wait(1);
  FieldTask_AddFrame(Func_02001f58, 0x480);
  FieldActor_SetMoveTarget(object, PIXELS(132), PIXELS(128), PIXELS(320));
  Actor_WaitForMove(object);
  FieldTask_Remove(Func_02001f58);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 3;
  FieldActor_SetSpritePriority(10, 2);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(10, 4, 0);
  Actor_MoveToAndWait(10, 120, 312);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 0;
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  Event_SetFlag(0x215);
  System_Wait(1);
  Func_02003e6c();
}
