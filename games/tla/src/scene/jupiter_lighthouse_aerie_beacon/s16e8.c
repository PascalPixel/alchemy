#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02005814();
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200593c();
extern u8 MsgBlank6[];

void Func_020016e8(void) {
  s32 northwest;
  s32 southeast;
  s32 north;

  /* FAKEMATCH(wrapper): 20 calls forwarded through call.h's Call1, Call3,
   * Call4, Value2 and Value3 (Func_080d47b4's through a cast of its type); as
   * direct calls, 171 of 400 instructions differ in tla-en (any one alone, 2
   * to 98; the five Value3 calls, 27). */
  Func_02005814();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the three angles held in locals; as FACING_
   * constants at each call, 57 of 400 instructions differ in tla-en. */
  northwest = FACING_NORTHWEST;
  Actor_SetPositionAndFacing(4, PIXELS(512), PIXELS(312), northwest);
  Actor_SetPositionAndFacing(7, PIXELS(528), PIXELS(304), northwest);
  southeast = FACING_SOUTHEAST;
  Actor_SetPositionAndFacing(27, PIXELS(504), PIXELS(304), southeast);
  Actor_SetPositionAndFacing(28, PIXELS(520), PIXELS(288), southeast);
  Actor_SetPositionAndFacing(5, PIXELS(528), PIXELS(320), northwest);
  north = FACING_NORTH;
  Actor_SetPositionAndFacing(6, PIXELS(544), PIXELS(384), north);
  Camera_MoveWithinMapBounds(PIXELS(520), PIXELS(104), PIXELS(312), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId((s32)MsgBlank6);
  Event_ShowMessageThenWait(6, 0, 0);
  Actor_SetModeById(27, 9);
  Actor_SetModeById(28, 8);
  Actor_SetModeById(4, 50);
  Actor_SetModeById(7, 12);
  Actor_SetModeById(5, 27);
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(28, 0x100, 0);
  ((void *)Func_080d47b4(27, 0x100, 30));
  Actor_GetById(4)->actionFlags &= ~1;
  Actor_GetById(7)->actionFlags &= ~1;
  Actor_GetById(5)->actionFlags &= ~1;
  Actor_GetById(27)->actionFlags &= ~1;
  Actor_GetById(28)->actionFlags &= ~1;
  Actor_GetById(4)->velocityY = PIXELS(3);
  Actor_GetById(7)->velocityY = PIXELS(3);
  Actor_GetById(5)->velocityY = PIXELS(3);
  Actor_GetById(27)->velocityY = PIXELS(3);
  Actor_GetById(28)->velocityY = PIXELS(3);
  Actor_OffsetPositionAndResetMotion(4, 8, 4);
  Actor_OffsetPositionAndResetMotion(7, 8, 4);
  Actor_OffsetPositionAndResetMotion(5, 8, 4);
  Actor_SetDestinationOffset(27, -16, -8);
  Actor_SetDestinationOffset(28, -16, -8);
  Actor_WaitForMoveById(4);
  Actor_GetById(4)->actionFlags |= 1;
  Actor_GetById(7)->actionFlags |= 1;
  Actor_GetById(5)->actionFlags |= 1;
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(28)->actionFlags |= 1;
  Camera_FollowActor(4, 1);
  Actor_WalkToAndWait(6, 552, 344);
  Actor_ShowEmote(6, 0x102, 30);
  Event_ShowMessageThenWait(6, 0, 0);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 0);
  ((void *)Func_080d47b4(28, 0x102, 30));
  Event_ShowMessageThenWait(28, 0, 0);
  Motion_SetVarCbAndRefresh(27, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(27, 0, 0);
  Event_Wait(10);
  Actor_ArmCallbackAndRefresh(6, north);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 0);
  Actor_OffsetPositionAndReset(6, 0, -24);
  Actor_WaitForMoveById(6);
  Actor_ArmCallbackAndRefresh(4, southeast);
  Actor_ArmCallback(6, northwest, 0);
  /* FAKEMATCH(one-pass): the three calls kept in a once-only block; as the
   * plain calls, 18 of 400 instructions differ in tla-en (the angles' copies
   * into r3 in five of the six placements, and the argument order of the
   * calls around the block). */
  do {
    Actor_SetModeById(4, 3);
    Event_Wait(20);
    Actor_ArmCallback(7, northwest, 0);
  } while (0);
  Actor_ArmCallbackAndRefresh(4, northwest);
  Actor_ArmCallbackAndRefresh(5, northwest);
  Event_Wait(10);
  Actor_SetDestinationOffset(4, -16, -8);
  Actor_SetDestinationOffset(7, -16, -8);
  Actor_SetDestinationOffset(5, -16, -8);
  Actor_SetDestinationOffset(6, -16, -8);
  Actor_OffsetPositionAndResetMotion(27, 16, 8);
  Actor_OffsetPositionAndResetMotion(28, 16, 8);
  Actor_GetById(4)->velocityY = PIXELS(4);
  Actor_GetById(7)->velocityY = PIXELS(4);
  Actor_GetById(5)->velocityY = PIXELS(4);
  Actor_GetById(6)->velocityY = PIXELS(4);
  Actor_GetById(27)->velocityY = PIXELS(4);
  Actor_GetById(28)->velocityY = PIXELS(4);
  Event_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1)
    FieldUnit_AddHp(4, -10);
  /* FAKEMATCH(one-pass): the byte store kept in a once-only block, whose end
   * holds the scene id's load behind it, as shaman_village_cave/s0430.c
   * does with the same store; as the plain store, 4 of 400 instructions
   * differ in tla-en. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6a5, 12);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov6a5, 13);
  Event_ClearFlag(0x11b);
  Func_0200593c(13, 7);
}
