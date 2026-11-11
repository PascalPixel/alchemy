#include "local.h"


void Func_02001f04(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Animation_ApplyChildValues(object, 9);
  *flags = 0;
  object->y = 0x500000;
  speed = 0x30000;
  // A goto loop: the ROM rebuilds the turn step on every pass, which a for loop
  // hoists (differ 43).
  i = 31;
fall:
  object->y -= speed;
  object->angle += 0x2000;
  speed += -0xccc;
  Actor_SetSingleChildFlags(object, 0);
  System_Wait(1);
  i--;
  if (i >= 0)
    goto fall;
  Func_02002bd4(object);
  Animation_ApplyChildValues(object, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  *flags = saved;
  object->velocityY = 0x80000;
  Event_PlaySound(215);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 168, 200);
  Func_02003d84();
}
