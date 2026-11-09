#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u8 MsgHereTheIntruder[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000ad4(s32 actor);
void Func_02001084(void);
void Func_0200108c(void);

void Func_02000af0(void) {
  s32 owner = gMainRoutineWork.currentOwner;

  /* FAKEMATCH: Call3 and Value3, as in Func_02000960, build each call's
   * constants again in the native's argument order; as direct calls, 126
   * instructions differ in tla-en. */
  Func_02001084();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(23, PIXELS(248), PIXELS(472));
  Actor_SetPosition(24, PIXELS(264), PIXELS(472));
  Actor_SetPosition(25, PIXELS(235), PIXELS(483));
  Actor_SetPosition(26, PIXELS(232), PIXELS(496));
  Actor_SetPosition(27, PIXELS(277), PIXELS(483));
  Actor_SetPosition(28, PIXELS(280), PIXELS(496));
  Func_02000ad4(owner);
  Func_02000ad4(23);
  Func_02000ad4(24);
  Func_02000ad4(25);
  Func_02000ad4(26);
  Func_02000ad4(27);
  Func_02000ad4(28);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgHereTheIntruder);
  Actor_ShowEmote(27, 259, 30);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(27, 25, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceActor(27, owner, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(27, 263, 30);
  Event_ShowMessage(27, 0);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_FaceActor(25, 23, 0);
  Actor_ShowEmote(25, 256, 30);
  Event_ShowMessage(25, 0);
  Actor_WalkTo(23, 248, 536);
  FieldEvent_Wait(1);
  Actor_WalkTo(24, 264, 536);
  FieldActor_WatchAndSetScript(25, owner);
  FieldActor_WatchAndSetScript(26, owner);
  FieldActor_WatchAndSetScript(27, owner);
  FieldActor_WatchAndSetScript(28, owner);
  FieldEvent_Wait(2);
  Actor_Get(owner)->actionFlags &= ~1;
  Actor_WalkToAndWait(owner, 256, 552);
  FieldEvent_Wait(1);
  Actor_Get(owner)->actionFlags |= 1;
  Actor_SetAnimation(23, 1);
  Actor_SetAnimation(24, 1);
  FieldEvent_Wait(20);
  Actor_SetAnimation(23, 4);
  FieldEvent_Wait(3);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_Wait(20);
  Actor_WalkTo(23, 248, 488);
  FieldEvent_Wait(2);
  Actor_WalkToAndWait(24, 264, 488);
  Actor_WalkTo(23, 248, 440);
  Actor_WalkTo(24, 264, 440);
  Actor_WalkTo(25, 232, 456);
  Actor_WalkTo(26, 232, 472);
  Actor_WalkTo(27, 280, 456);
  Actor_WalkToAndWait(28, 280, 472);
  Actor_SetPosition(23, 0, 0);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(25, 0, 0);
  Actor_SetPosition(26, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetPosition(28, 0, 0);
  Func_0200108c();
  Event_SetFlag(0x300);
}
