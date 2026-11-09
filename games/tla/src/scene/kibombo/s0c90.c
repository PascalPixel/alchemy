#include "kibombo.h"
#include "scene_tables.h"

void Func_02000c90(void) {
  struct KibomboObject *leader;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(280), -1, PIXELS(88), 1);
  Func_02005bb4();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouVeCome);
  Event_ShowMessageAndWait(0x8007, 0);
  Kibombo_ObjTurn(7, FACING_WEST);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(7, *(s16 *)((u8 *)leader + 10),
                              *(s16 *)((u8 *)leader + 18));
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_ClearBit(0x92f);
  GameFlag_SetBit(0x92e);
  Event_PrepareObjectAndApplyValue(7, 0);
  Func_02005aa4();
}
