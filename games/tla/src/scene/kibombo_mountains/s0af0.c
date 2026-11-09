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
  Event_ResolvePendingActions(0);
  Actor_SetPosition(23, PIXELS(248), PIXELS(472));
  Actor_SetPosition(24, PIXELS(264), PIXELS(472));
  Actor_SetPosition(25, PIXELS(235), PIXELS(483));
  Actor_SetPosition(26, PIXELS(232), PIXELS(496));
  Actor_SetPosition(27, PIXELS(277), PIXELS(483));
  Actor_SetHorizontalPositionWithTerrain(28, PIXELS(280), PIXELS(496));
  Func_02000ad4(owner);
  Func_02000ad4(23);
  Func_02000ad4(24);
  Func_02000ad4(25);
  Func_02000ad4(26);
  Func_02000ad4(27);
  Func_02000ad4(28);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgHereTheIntruder);
  Func_080d47b4(27, 259, 30);
  Event_ShowMessageAndWait(27, 0);
  Actor_SetAngleToward(27, 25, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Actor_SetAngleToward(27, owner, 0);
  Event_Wait(10);
  Func_080d47b4(27, 263, 30);
  Event_ShowMessageAndWait(27, 0);
  Motion_SetModeAndWaitAnimation(27, 3);
  Event_ShowMessageAndWait(27, 0);
  Actor_SetAngleToward(25, 23, 0);
  Actor_ShowEmote(25, 256, 30);
  Event_ShowMessageAndWait(25, 0);
  Actor_WalkTo(23, 248, 536);
  Event_Wait(1);
  Actor_WalkTo(24, 264, 536);
  Actor_WatchAndSetScript(25, owner);
  Actor_WatchAndSetScript(26, owner);
  Actor_WatchAndSetScript(27, owner);
  Actor_WatchAndSetScript(28, owner);
  Event_Wait(2);
  Actor_GetById(owner)->actionFlags &= ~1;
  Actor_SetPositionAndReset(owner, 256, 552);
  Event_Wait(1);
  Actor_GetById(owner)->actionFlags |= 1;
  Actor_SetModeById(23, 1);
  Actor_SetModeById(24, 1);
  Event_Wait(20);
  Actor_SetModeById(23, 4);
  Event_Wait(3);
  Motion_SetModeAndWaitAnimation(24, 4);
  Event_Wait(20);
  Actor_WalkTo(23, 248, 488);
  Event_Wait(2);
  Actor_WalkToAndWait(24, 264, 488);
  Actor_WalkTo(23, 248, 440);
  Actor_WalkTo(24, 264, 440);
  Actor_WalkTo(25, 232, 456);
  Actor_WalkTo(26, 232, 472);
  Actor_WalkTo(27, 280, 456);
  Actor_WalkToAndWait(28, 280, 472);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
  Func_0200108c();
  GameFlag_SetBit(0x300);
}
