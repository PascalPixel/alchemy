#include "local.h"
#include "event_inline.h"

void Func_020004a8(void) {
  struct Actor *object;

  if (GameFlag_Test(0x842) == 0) {
    GameFlag_SetBit(0x230);
    Func_020051c0();
    Event_ResolvePendingActions(0);
    Actor_SetPositionAndReset(4, 104, 252);
    Actor_FaceDirection(4, FACING_NORTHWEST, 0);
    FieldActor_LaunchFromLeader(14, -16, -16, FACING_NORTH);
    Actor_WaitForMoveById(14);
    Event_Wait(20);
    Event_SetMessageId((s32)&MsgKandoreanTemple11);
    Event_ShowMessageAndWait(14, 0);
    Actor_ArmCallbackAndRefresh(14, FACING_SOUTHEAST);
    Event_Wait(30);
    Actor_SetModeById(14, 3);
    Motion_SetModeAndWaitAnimation(4, 3);
    Event_Wait(20);
    Actor_SetSpeed(14, 0x13333, 0x9999);
    Actor_SetModeById(14, 2);
    object = Actor_GetById(4);
    if (object)
      Actor_ResetAndSetPosition(14, ((s16 *)&object->x)[1],
                                ((s16 *)&object->z)[1]);
    Actor_WaitForMoveById(14);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
    Func_020051c8();
  }
}
