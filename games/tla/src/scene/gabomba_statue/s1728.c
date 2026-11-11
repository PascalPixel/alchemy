#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_020056b4(struct Actor *object);
void Func_020056bc(void);

void Func_02001728(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 424);
  Func_020056bc();
}

void Func_0200176c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;

  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  actor->motionFlags = 0;
  speed = 0x19999;
  /* FAKEMATCH(barrier): an empty asm on speed, so the speed is loaded before
   * the owner, whose base is already in a register; as the plain call, the
   * owner is loaded first and 2 instructions differ. */
  asm volatile("" : "+l"(speed));
  Actor_SetSpeed(gMainRoutineWork.currentOwner, speed, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 488);
  actor->motionFlags = 3;
  FieldEvent_Wait(8);
  Event_PlaySound(161);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  FieldEvent_Wait(20);
  Func_020056bc();
}
