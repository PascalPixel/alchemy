#include "local.h"

void Func_02000e18(s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;
  s32 i;
  u8 *flags;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  flags = &object->motionFlags;
  speed = 0;
  *flags = 0;
  object->priorityFlags |= 0x20;
  Func_02003cc4(object, 18);
  Event_PlaySound(215);
  System_Wait(13);
  Animation_ApplyChildValues(object, 9);
  Func_02003cc4(object, 28);
  Event_PlaySound(0x236);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += 0x4000;
    object->y += speed;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_Get(15));
    System_Wait(1);
  }
  Func_02003d84();
  FieldEvent_SetExitRequest(value);
}
