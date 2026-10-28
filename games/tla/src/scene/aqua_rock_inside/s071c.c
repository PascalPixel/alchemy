#include "local.h"

void Func_0200071c(void) {
  s32 priority;

  Func_02003d7c();
  Func_02003eac(0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, 0x4000, 0);
  Func_02003ef4(0x22d);
  priority = 0x480;
  Task_Add(Func_020006d0, priority);
  Func_02003e44_(0x28000, 0x4000);
  Camera_MoveWithinMapBounds_(0x3080000, -1, 0x2880000, 1);
  Func_02003e54();
  FieldEvent_Wait(30);
  Func_02003d84();
  GameFlag_SetBit(0x240);
  Func_02003d1c_(59, 16, 5, 32, 46, 16);
}
