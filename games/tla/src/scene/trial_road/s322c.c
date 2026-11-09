#include "trial_road.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02005880(void);
void Func_020058f0(s32 actor, s32 value);
s32 Func_02002d20(s32 value);
s32 Func_02002d50(s32 value);
s32 Func_02002d38(s32 value);
void Func_02005980(void);

void Func_0200322c(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  Camera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(264), 1);
  Func_020058f0(5, 4);
  Func_020058f0(6, 4);
  Func_020058f0(7, 4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(4, Func_02002d20(28), 272);
  Actor_WalkTo(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2(5, Func_02002d20(27), 248);
  Actor_ResetAndSetPositionInMode2(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  Func_080d47b4(8, 256, 10);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(15);
  Actor_SetSpeed(4, 0x20000, 0x10000);
  Actor_SetSpeed(5, 0x1e666, 0xf333);
  Actor_SetSpeed(6, 0x1b333, 0xd999);
  Actor_SetSpeed(7, 0x1cccc, 0xe666);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_SetSpeed(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  wait(1);
  Event_Wait(6);
}
