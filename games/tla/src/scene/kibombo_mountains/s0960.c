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
  /* FAKEMATCH(wrapper): the Call3 and Value3 calls of call.h keep each call's
   * own constants and argument order; as direct SetPositionAndReset calls, 42
   * instructions differ in tla-en. */
  Func_02001084();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgIsTheWay);
  Actor_SetPosition(27, PIXELS(312), PIXELS(304));
  Event_ShowMessage(27, 0);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(392), 1);
  Func_02001134();
  Actor_ResetTargetsAndVelocity(11);
  Actor_FaceActor(11, 27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkToAndWait(27, 256, 400);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Actor_RunRepeatedMotion(11, 1);
  Event_ShowMessage(11, 0);
  Actor_SetAnimationAndWait(27, 4);
  Event_ShowMessage(27, 0);
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(11, 258, 30);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, 256, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(27, 258, 30);
  Event_ShowMessage(27, 0);
  Actor_WalkToAndWait(27, 264, 312);
  Actor_WalkTo(27, 360, 312);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
  System_Wait(30);
  Event_ShowMessage(11, 0);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(60);
  Actor_SetPosition(27, 0, 0);
  Actor_SetScript(11, gData_020017f8);
  Func_0200108c();
  Event_SetFlag(0x880);
}
