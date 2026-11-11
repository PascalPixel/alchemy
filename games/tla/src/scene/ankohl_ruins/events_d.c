#include "ankohl_ruins.h"

void Func_020011c0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02002e44();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  object->motionFlags = 0;
  object->actionFlags = 0;
  {
    s32 zero;

    zero = 0;
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, zero,
                                              8);
  }
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_080d47b4(gMainRoutineWork.currentOwner, 256, 0);
  FieldEvent_Wait(10);
  AnkohlRuins_CamBounds(-1, -1, -1, 0);
  Event_PlaySound(204);
  Func_02000b30(gMainRoutineWork.currentOwner, 188);
  object->actionFlags = 1;
  Func_02002e4c();
}

void Func_02000850(s32 a, s32 id);
void Func_02000c24(s32 a, s32 id);

void Func_0200124c(s32 a, s32 id) {
  if ((u32)(id - 9) <= 1)
    Func_02000c24(a, id);
  else
    Func_02000850(a, id);
}
