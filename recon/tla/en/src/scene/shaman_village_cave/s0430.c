#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "edition.h"
#include "scene_tables.h"

void FieldEvent_Wait();
void Func_02001080();
void Func_020010f0();
void Func_02001138();
void Func_02001148();
void Func_02001158();
void Func_02001160();
void Func_02001180();
void Func_020011a0();
void Func_020011e0();
void Func_020011f0();
void GameFlag_SetBit();
void GameFlag_Test();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_OffsetPositionAndReset();
void Actor_OffsetPositionAndResetMotion();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetPositionAndReset();
void Actor_TurnToFaceEachOther();
void Actor_SetModeById();
void Party_SetFields1eeAnd1f0();
void Party_SetFields1f2And1f4();

#define SHAMAN_VILLAGE_CAVE_EMOTE(actor, emote, frames)                        \
  {                                                                            \
    /* FAKEMATCH: the native loads the emote and the frames before the actor   \
     * at every */                                                             \
    /* emote of this overlay; pinned to their argument registers they are      \
     * loaded first, */                                                        \
    /* as there. Through an inline the actor comes one row early wherever the  \
     * emote takes */                                                          \
    /* two instructions, and an empty asm barrier at each call moves the       \
     * literal pools of */                                                     \
    /* a long scene. */                                                        \
    register s32 emote_ __asm__("r1") = (emote);                               \
    /* FAKEMATCH: the frames, pinned with the emote above. */                  \
    register s32 frames_ __asm__("r2") = (frames);                             \
    Func_02001160((actor), emote_, frames_);                                   \
  }

void Func_02000430(void) {
  s32 angle;

  if (Value1((s32 (*)())GameFlag_Test, SPLIT_CONSTANT(0x800, 33)) == 0)
    return;
  if (Value1((s32 (*)())GameFlag_Test, 0) == 0)
    return;
  Func_02001080();
  Func_020011e0(0);
  GameFlag_SetBit(0xa70);
  Func_02001138((s32)MsgIFound2);
  Actor_SetModeById(4, 1);
  Func_02001148(19, 0, 1);
  Actor_SetPositionAndReset(4, 200, 221);
  Motion_SetVarCbAndRefresh(4, 2);
  FieldEvent_Wait(5);
  Call3(Actor_OffsetPositionAndReset, 4, -16, 8);
  Call4(Func_020011f0, 0, 0, 0, 0xc000);
  Call4(Func_020011f0, 18, 8, 16, 0xc000);
  Actor_WaitForMoveById(0);
  Actor_WaitForMoveById(18);
  Call3(Actor_ArmCallback, 0, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Call3(Actor_ArmCallback, 0, FACING_NORTH, 0);
  Call3(Actor_ArmCallback, 4, FACING_NORTH, 0);
  Actor_ArmCallback(18, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 256, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 40)
  Call4(Camera_MoveWithinMapBounds, PIXELS(199), -1, PIXELS(177), 1);
  Func_02001180();
  angle = 0x4000;
  Call4(Func_020010f0, 19, PIXELS(199), PIXELS(130), angle);
  Actor_ResetAndSetPositionInMode2(19, 231, 199);
  FieldEvent_Wait(20);
  Call4(Func_020010f0, 20, PIXELS(199), PIXELS(130), angle);
  Actor_ResetAndSetPositionInMode2(20, 220, 191);
  FieldEvent_Wait(20);
  Call4(Func_020010f0, 21, PIXELS(199), PIXELS(130), angle);
  Actor_ResetAndSetPositionInMode2(21, 204, 191);
  Camera_MoveWithinMapBounds(PIXELS(199), -1, PIXELS(225), 1);
  Func_02001180();
  Actor_WaitForMoveById(19);
  Call3(Actor_ArmCallback, 19, 0x5000, 0);
  Actor_WaitForMoveById(20);
  Call3(Actor_ArmCallback, 20, 0x5000, 0);
  Actor_WaitForMoveById(21);
  Call2(Func_02001158, 21, 0x5000);
  Motion_SetVarCbAndRefresh(21, 2);
  Func_02001148(21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 258, 30)
  Func_02001148(18, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 264, 30)
  Func_02001148(19, 0, 5);
  Motion_SetVarCbAndRefresh(20, 2);
  FieldEvent_Wait(5);
  Func_02001148(20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
  Func_02001148(21, 0, 5);
  Call2(Func_02001158, 0, FACING_NORTHEAST);
  Motion_SetModeAndWaitAnimation(0, 3);
  FieldEvent_Wait(10);
  Func_02001148(0, 0, 5);
  Motion_SetVarCbAndRefresh(19, 2);
  FieldEvent_Wait(5);
  Func_02001148(19, 0, 5);
  Actor_TurnToFaceEachOther(18, 4, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  Func_02001148(0, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
  Func_02001148(20, 0, 5);
  Call3(Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Call2(Func_02001158, 18, FACING_NORTHEAST);
  Actor_SetModeById(18, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(10);
  Motion_SetModeAndWaitAnimation(21, 4);
  Func_02001148(21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 265, 30)
  Func_02001148(18, 0, 5);
  Actor_SetModeById(21, 4);
  Actor_SetModeById(19, 4);
  Motion_SetModeAndWaitAnimation(20, 4);
  Func_02001148(19, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 30)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 258, 30)
  Func_02001148(0, 0, 5);
  Actor_TurnToFaceEachOther(20, 21, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
  Func_02001148(20, 0, 5);
  Call3(Actor_ArmCallback, 20, 0x5000, 0);
  Call3(Actor_ArmCallback, 21, 0x5000, 0);
  Func_02001158(18, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Motion_SetModeAndWaitAnimation(18, 4);
  Func_02001148(18, 0, 5);
  Motion_SetModeAndWaitAnimation(21, 4);
  Func_02001148(21, 0, 5);
  Call3(Actor_OffsetPositionAndResetMotion, 20, -16, 24);
  Call3(Actor_OffsetPositionAndResetMotion, 21, -16, 24);
  Actor_OffsetPositionAndResetMotion(19, -16, 24);
  Actor_WaitForMoveById(19);
  /* FAKEMATCH: a one-pass block loads gMainRoutineWork before its offset is
   * built (one reordered row). */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov695, 20);
  Party_SetFields1f2And1f4((s32)&SceneId_Ov695, 21);
  Func_020011a0(102, 2);
}
