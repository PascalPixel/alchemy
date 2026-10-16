#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_02000404();
void Func_02001080();
void Func_02001088();
void Func_020010e8();
void Func_020010f0();
void Func_02001138();
void Func_02001148();
void Func_02001158();
void Func_02001160();
void Func_020011e0();
void GameFlag_ClearBit();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetSpeedParameters();
void Object_GetById();
void Object_LinkPair();
void Object_SetModeById();
void UiText_OpenMessageAtObject();

#define EVENT_MESSAGE                                                          \
  (((struct {                                                                  \
      u8 unk000[0x1c4];                                                        \
      s16 message;                                                             \
    } *)Ram_HeapSlots->eventWork)                                              \
       ->message)

extern const u8 gData_020015e0[];
extern const u8 gData_0200161c[];

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

// The JA edition has its own form of this function; it stays on its listing in
// recon/tla/raw/overlays (S4).
#if !defined(TLA_EDITION_JA)
void Func_02000a24(void) {
  s16 *target;
  s32 angle = 0xe000;

  Call1(GameFlag_ClearBit, 0xa71);
  Call1(GameFlag_SetBit, 0xa72);
  Func_02001080();
  Call1(Func_020011e0, 0);
  Call4(Camera_MoveWithinMapBounds, 0xc70000, -1, 0x1110000, 0);
  Call1(Func_02001138, (s32)MsgTheseGuysWere);
  Call4(Func_020010f0, 4, 0xb80000, 0xe50000, angle);
  Call4(Func_020010f0, 0, 0xc80000, 0xdd0000, angle);
  Call4(Func_020010f0, 18, 0xd00000, 0xed0000, angle);
  Call2(Object_SetModeById, 20, 8);
  Call2(Object_SetModeById, 19, 8);
  Call2(Object_SetModeById, 21, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Motion_SetVarCbAndRefresh, 20, 2);
  Call1(FieldEvent_WaitFrames, 5);
  Call3(Func_02001148, 21, 0, 5);
  Call3(Func_02001148, 19, 0, 5);
  Call3(Func_02001148, 20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 258, 30)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 263, 30)
  Call3(Func_02001148, 0, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 21, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 265, 30) {
    /* FAKEMATCH: the native sets the flags before the speaker here; pinned to
     * r1 they are set first (one reordered row through the inline or as a plain
     * call). */
    register s32 flags __asm__("r1") = 0;

    UiText_OpenMessageAtObject(19, flags);
  }
  while (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 0, 0) != 0) {
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 30)
    UiText_OpenMessageAtObject(20, 0);
    EVENT_MESSAGE--;
  }
  EVENT_MESSAGE++;
  Call3(Func_02001148, 20, 0, 5);
  Call2(Object_SetModeById, 20, 1);
  Call2(Object_SetModeById, 19, 1);
  Call2(Object_SetModeById, 21, 1);
  Call1(FieldEvent_WaitFrames, 30);
  Call2(Motion_SetModeAndWaitAnimation, 21, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 21, 0, 5);
  Func_02000404();
  Call1(FieldEvent_WaitFrames, 60);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Func_02001158, 0, 0x4000);
  Call1(FieldEvent_WaitFrames, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 264, 30)
  Call3(ObjectMotion_ArmCallback, 0, 0xe000, 0);
  Call3(Func_02001148, 20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 265, 30)
  Call3(Func_02001148, 21, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 20, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(Func_02001148, 20, 0, 5);
  Call2(Object_SetModeById, 0, 3);
  Call2(Object_SetModeById, 4, 3);
  Call2(Motion_SetModeAndWaitAnimation, 18, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call3(ObjectMotion_SetSpeedParameters, 19, 0x10000, 0x8000);
  Call3(ObjectMotion_SetSpeedParameters, 20, 0x10000, 0x8000);
  Call3(ObjectMotion_SetSpeedParameters, 21, 0x10000, 0x8000);
  Call2(ObjectMotion_EnableActionAndSetCallback, 19, gData_020015e0);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(ObjectMotion_EnableActionAndSetCallback, 20, gData_0200161c);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(ObjectMotion_EnableActionAndSetCallback, 21, gData_0200161c);
  Call1(FieldEvent_WaitFrames, 100);
  Call3(Object_LinkPair, 0, 4, 0);
  Call3(ObjectMotion_SetAngleToward, 18, 4, 0);
  Call1(FieldEvent_WaitFrames, 40);
  Call2(Object_SetModeById, 0, 3);
  Call2(Object_SetModeById, 4, 3);
  Call2(Motion_SetModeAndWaitAnimation, 18, 3);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Object_SetModeById, 0, 2);
  target = (s16 *)Value1((s32 (*)())Object_GetById, 4);
  if (target != 0)
    Call3(ObjectMotion_ResetAndSetPosition, 0, target[5], target[9]);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0);
  Call3(Func_020010e8, 0, 0, 0);
  Call2(Object_SetModeById, 18, 2);
  target = (s16 *)Value1((s32 (*)())Object_GetById, 4);
  if (target != 0)
    Call3(ObjectMotion_ResetAndSetPosition, 18, target[5], target[9]);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 18);
  Call3(Func_020010e8, 18, 0, 0);
  Func_02001088();
}
#endif
