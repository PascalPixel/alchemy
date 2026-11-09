#include "trial_road.h"
#include "text/msg_ids.h"
#include "event_inline.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005880(void);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);
s32 Func_02002d20(s32 value);
s32 Func_02002d50(s32 value);
void Func_02005860(s32 cue);

void Func_020034b4(void) {
  Func_02005880();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02002d78(4, 248, 272, 0);
  Func_02002d78(7, 224, 248, 0);
  Func_02002d78(5, 232, 264, 0);
  Func_02002d78(6, 224, 280, 0);
  Func_02002d78(9, 248, 256, 0);
  Func_02002d78(8, 272, 264, 0x4000);
  Func_02002d78(13, 288, 248, 0x4000);
  Func_02002d78(14, 288, 280, 0x4000);
  Actor_SetModeById(8, 6);
  Actor_SetModeById(13, 6);
  Actor_SetModeById(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(13, 2);
  wait(4);
  Actor_StartRepeatedMotion(14, 2);
  wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(8, 7);
  Event_Wait(15);
  Actor_SetModeById(13, 7);
  Event_Wait(10);
  Actor_SetModeById(14, 7);
  Event_Wait(5);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_SetModeById(14, 1);
  Event_Wait(5);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetSpeedParameters(8, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2(8, Func_02002d20(32), 264);
  Actor_TurnToFaceEachOther(9, 7, 0);
  Actor_TurnToFaceEachOther(5, 6, 0);
  Actor_WaitForMoveById(8);
  Actor_SetAngleToward(9, 4, 0);
  Actor_SetAngleToward(7, 4, 0);
  Actor_SetAngleToward(6, 4, 0);
  Actor_SetAngleToward(5, 4, 0);
  Event_Wait(20);
  Func_02005860(65);
  PartyInventory_GiveItem(215, 0);
  Actor_SetAngleToward(4, 8, 0);
  OBJECT_BYTE(Actor_GetById(8), 0x5a) &= ~1;
  Actor_SetPositionAndReset(8, Func_02002d20(35), 264);
  Event_Wait(1);
  /* FAKEMATCH: the bit is set through the 8-bit field, as in overlay 685: read
   * as a plain byte the store takes the order of a store that aliases what
   * follows. */
  ((struct ActorSolid *)Actor_GetById(8))->solid |= 1;
  Event_Wait(20);
  Func_080d47b4(8, 261, 10);
  Event_ShowMessageAndWait(8, 0);
  Actor_ArmCallbackAndRefresh(8, Func_02002d50(0));
  Actor_SetAngleToward(13, 8, 0);
  Actor_SetAngleToward(14, 8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(13, 3);
  wait(3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetAngleToward(4, 9, 0);
  Actor_SetAngleToward(7, 9, 0);
  Actor_SetAngleToward(5, 9, 0);
  Actor_SetAngleToward(6, 9, 0);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(4, 3);
  wait(2);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  wait(1);
  Actor_SetModeById(5, 3);
  wait(30);
}
