#include "local.h"
#include "event_inline.h"

extern char SceneId_SuhallaGate1;
extern char SceneId_VenusLighthouseEntrance;
extern u8 MsgWaahhhItS[];

void Func_02000a40(s32 a, s32 b);

/// Places the leader and the camera after a message, runs actor 8's motions
/// and sends the party to Suhalla Gate 1, entrance 99. The message is
/// "Waahhh".
void Func_020003ac(void) {
  Func_02000988();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWaahhhItS);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 472, 320);
  Camera_MoveWithinMapBounds(PIXELS(456), -1, PIXELS(280), 1);
  Func_02000a30();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(8, 6, 20);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  {
    s32 x = 256;
    s32 z = 50;

    Func_080d47b4(8, x, z);
  }
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_WalkToAndWait(8, 456, 312);
  Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate1, 99);
  Party_SetFields1f2And1f4((s32)&SceneId_VenusLighthouseEntrance, 98);
  gMainRoutineWork.battleRule24b = 2;
  Func_02000a40(9, 1);
  Func_02000990();
}
