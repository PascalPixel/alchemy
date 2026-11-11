#include "local.h"

extern char SceneId_Ov690Room4;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void CenterOn(s32 id, s32 column, s32 offset) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, which sets the
   * arguments up in their own order; as a direct Actor_CenterOnColumn call,
   * 2 of 74 instructions differ in tla-en (the offset is negated after the
   * column). */
  Actor_CenterOnColumn(id, column, offset);
}

void Func_02000c44(void) {
  u32 i;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop carries no loop notes: the bytes rebuild the zero on every
  // pass, and a for loop hoists it.
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4)
    CopyCells(33, 30, 23, 14, 1, 2);
  else
    CopyCells(27, 29, 40, 16, 1, 2);
  Event_PlaySound(158);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  CenterOn(gMainRoutineWork.currentOwner, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002070();
}
