#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u8 MsgIsTheWay[];
extern const u8 gData_020017f8[];

void Actor_ResetTargetsAndVelocity(s32 actor);
void Func_02001084(void);
void Func_0200108c(void);
void Func_02001134(void);

void Func_02000960(void) {
  /* FAKEMATCH: the Call3 and Value3 calls of call.h keep each call's own
   * constants and argument order; as direct SetPositionAndReset calls, 42
   * instructions differ in tla-en. */
  Func_02001084();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgIsTheWay);
  Actor_SetPosition(27, PIXELS(312), PIXELS(304));
  Event_ShowMessageAndWait(27, 0);
  Camera_MoveWithinMapBounds(PIXELS(256), -1, PIXELS(392), 1);
  Func_02001134();
  Actor_ResetTargetsAndVelocity(11);
  Actor_SetAngleToward(11, 27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkToAndWait(27, 256, 400);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_ShowMessageAndWait(11, 0);
  Motion_SetVarCbAndRefresh(11, 1);
  Event_ShowMessageAndWait(11, 0);
  Motion_SetModeAndWaitAnimation(27, 4);
  Event_ShowMessageAndWait(27, 0);
  Event_ShowMessageAndWait(27, 0);
  Actor_ShowEmote(11, 258, 30);
  Event_ShowMessageAndWait(11, 0);
  Func_080d47b4(27, 256, 30);
  Event_ShowMessageAndWait(27, 0);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_ShowMessageAndWait(11, 0);
  Func_080d47b4(27, 258, 30);
  Event_ShowMessageAndWait(27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkTo(27, 360, 312);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_ShowMessageAndWait(11, 0);
  Actor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
  wait(30);
  Event_ShowMessageAndWait(11, 0);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(60);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Actor_SetMovementTypeById(11, gData_020017f8);
  Func_0200108c();
  GameFlag_SetBit(0x880);
}
