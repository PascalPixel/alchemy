#include "local.h"

void Func_020004a8(void) {
  struct Actor *object;

  if (GameFlag_Test(0x842) == 0) {
    GameFlag_SetBit(0x230);
    Func_020051c0();
    Func_02005370(0);
    Actor_SetPositionAndReset(4, 104, 252);
    Actor_ArmCallback_(4, FACING_NORTHWEST, 0);
    Func_02005378_(14, -16, -16, FACING_NORTH);
    Actor_WaitForMoveById(14);
    FieldEvent_Wait(20);
    Func_02005280((s32)&MsgKandoreanTemple11);
    Func_02005298(14, 0);
    Func_020052b0(14, FACING_SOUTHEAST);
    FieldEvent_Wait(30);
    Actor_SetModeById(14, 3);
    Motion_SetModeAndWaitAnimation(4, 3);
    FieldEvent_Wait(20);
    Actor_SetSpeedParameters_(14, 0x13333, 0x9999);
    Actor_SetModeById(14, 2);
    object = Actor_GetById(4);
    if (object)
      Actor_ResetAndSetPosition(14, ((s16 *)&object->x)[1],
                                ((s16 *)&object->z)[1]);
    Actor_WaitForMoveById(14);
    Func_02005230(14, 0, 0);
    Func_020051c8();
  }
}
