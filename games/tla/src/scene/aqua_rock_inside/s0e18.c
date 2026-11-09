#include "local.h"

void Func_02000e18(s32 value) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 speed;
  s32 i;
  u8 *flags;

  Func_02003d7c();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  flags = &object->motionFlags;
  speed = 0;
  *flags = 0;
  /* FAKEMATCH: the barrier stores the motion flags before the priority address
   * is started; without it, 2 instructions differ in tla-en. */
  __asm__("");
  object->priorityFlags |= 0x20;
  Func_02003cc4(object, 18);
  Sound_Play(215);
  wait(13);
  Animation_ApplyChildValues(object, 9);
  Func_02003cc4(object, 28);
  Sound_Play(0x236);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += 0x4000;
    object->y += speed;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_GetById(15));
    wait(1);
  }
  Func_02003d84();
  Import_EventSetExitRequest(value);
}
