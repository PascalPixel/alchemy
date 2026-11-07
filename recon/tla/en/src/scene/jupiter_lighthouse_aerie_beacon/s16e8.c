#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"
#include "scene_ids.h"
#include "scene_tables.h"

void Func_02005814();
void Func_02005984();
void Func_0200588c();
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020058c4();
void Func_020058d4();
void *Func_020058fc(s32 actor, s32 flags, s32 state);
struct Actor *Actor_GetById(s32 id);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Camera_FollowActor(s32 id, s32 flag);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void FieldEvent_Wait(s32 frames);
void Func_020058ec();
void Actor_OffsetPositionAndReset(u32 objectId, s32 xOffset, s32 zOffset);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
s32 Unit_AddHp(s32 owner, s32 delta);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void GameFlag_ClearBit(s32 flag);
void Func_0200593c();
extern u8 MsgBlank6[];

void Func_020016e8(void) {
  s32 northwest;
  s32 southeast;
  s32 north;

  /* FAKEMATCH(wrapper): 20 calls forwarded through call.h's Call1, Call3,
   * Call4, Value2 and Value3 (Func_020058fc's through a cast of its type); as
   * direct calls, 171 of 400 instructions differ in tla-en (any one alone, 2
   * to 98; the five Value3 calls, 27). */
  Func_02005814();
  Func_02005984(0);
  /* FAKEMATCH(temporary): the three angles held in locals; as FACING_
   * constants at each call, 57 of 400 instructions differ in tla-en. */
  northwest = FACING_NORTHWEST;
  Call4(Func_0200588c, 4, PIXELS(512), PIXELS(312), northwest);
  Call4(Func_0200588c, 7, PIXELS(528), PIXELS(304), northwest);
  southeast = FACING_SOUTHEAST;
  Call4(Func_0200588c, 27, PIXELS(504), PIXELS(304), southeast);
  Call4(Func_0200588c, 28, PIXELS(520), PIXELS(288), southeast);
  Call4(Func_0200588c, 5, PIXELS(528), PIXELS(320), northwest);
  north = FACING_NORTH;
  Call4(Func_0200588c, 6, PIXELS(544), PIXELS(384), north);
  Camera_MoveWithinMapBounds(PIXELS(520), PIXELS(104), PIXELS(312), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020058c4((s32)MsgBlank6);
  Func_020058d4(6, 0, 0);
  Actor_SetModeById(27, 9);
  Actor_SetModeById(28, 8);
  Actor_SetModeById(4, 50);
  Actor_SetModeById(7, 12);
  Actor_SetModeById(5, 27);
  Value3((s32 (*)())Func_020058fc, 4, 0x100, 0);
  Value3((s32 (*)())Func_020058fc, 7, 0x100, 0);
  Value3((s32 (*)())Func_020058fc, 5, 0x100, 0);
  Value3((s32 (*)())Func_020058fc, 28, 0x100, 0);
  Func_020058fc(27, 0x100, 30);
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
  Call3(Actor_OffsetPositionAndResetMotion, 27, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 28, -16, -8);
  Actor_WaitForMoveById(4);
  Actor_GetById(4)->actionFlags |= 1;
  Actor_GetById(7)->actionFlags |= 1;
  Actor_GetById(5)->actionFlags |= 1;
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(28)->actionFlags |= 1;
  Camera_FollowActor(4, 1);
  Call3(Actor_SetPositionAndReset, 6, 552, 344);
  Value3((s32 (*)())Func_020058fc, 6, 0x102, 30);
  Func_020058d4(6, 0, 0);
  Motion_SetVarCbAndRefresh(5, 2);
  FieldEvent_Wait(5);
  Func_020058d4(5, 0, 0);
  Func_020058fc(28, 0x102, 30);
  Func_020058d4(28, 0, 0);
  Motion_SetVarCbAndRefresh(27, 2);
  FieldEvent_Wait(5);
  Func_020058d4(27, 0, 0);
  FieldEvent_Wait(10);
  Func_020058ec(6, north);
  FieldEvent_Wait(10);
  Func_020058d4(6, 0, 0);
  Actor_OffsetPositionAndReset(6, 0, -24);
  Actor_WaitForMoveById(6);
  Func_020058ec(4, southeast);
  Actor_ArmCallback(6, northwest, 0);
  /* FAKEMATCH(one-pass): the three calls kept in a once-only block; as the
   * plain calls, 18 of 400 instructions differ in tla-en (the angles' copies
   * into r3 in five of the six placements, and the argument order of the
   * calls around the block). */
  do {
    Actor_SetModeById(4, 3);
    FieldEvent_Wait(20);
    Actor_ArmCallback(7, northwest, 0);
  } while (0);
  Func_020058ec(4, northwest);
  Func_020058ec(5, northwest);
  FieldEvent_Wait(10);
  Call3(Actor_OffsetPositionAndResetMotion, 4, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 7, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 5, -16, -8);
  Call3(Actor_OffsetPositionAndResetMotion, 6, -16, -8);
  Actor_OffsetPositionAndResetMotion(27, 16, 8);
  Actor_OffsetPositionAndResetMotion(28, 16, 8);
  Actor_GetById(4)->velocityY = PIXELS(4);
  Actor_GetById(7)->velocityY = PIXELS(4);
  Actor_GetById(5)->velocityY = PIXELS(4);
  Actor_GetById(6)->velocityY = PIXELS(4);
  Actor_GetById(27)->velocityY = PIXELS(4);
  Actor_GetById(28)->velocityY = PIXELS(4);
  FieldEvent_Wait(4);
  if (gMainRoutineWork.sequenceResult == -1)
    Value2(Unit_AddHp, 4, -10);
  /* FAKEMATCH(one-pass): the byte store kept in a once-only block, whose end
   * holds the scene id's load behind it, as shaman_village_cave/s0430.c
   * does with the same store; as the plain store, 4 of 400 instructions
   * differ in tla-en. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6a5, 12);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov6a5, 13);
  Call1(GameFlag_ClearBit, 0x11b);
  Func_0200593c(13, 7);
}
