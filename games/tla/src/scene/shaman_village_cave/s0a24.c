#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "scene_tables.h"

void FieldEvent_Wait();
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
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_SetMovementTypeById();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_SetSpeedParameters();
void Actor_GetById();
void Actor_TurnToFaceEachOther();
void Actor_SetModeById();
void FieldMessage_ShowAtActor();

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
#if (LANGUAGE != LANGUAGE_JAPANESE)
void Func_02000a24(void) {
  s16 *target;
  s32 angle = 0xe000;

  Call1(GameFlag_ClearBit, 0xa71);
  Call1(GameFlag_SetBit, 0xa72);
  Func_02001080();
  Call1(Func_020011e0, 0);
  Call4(Camera_MoveWithinMapBounds, PIXELS(199), -1, PIXELS(273), 0);
  Call1(Func_02001138, (s32)MsgTheseGuysWere);
  Call4(Func_020010f0, 4, PIXELS(184), PIXELS(229), angle);
  Call4(Func_020010f0, 0, PIXELS(200), PIXELS(221), angle);
  Call4(Func_020010f0, 18, PIXELS(208), PIXELS(237), angle);
  Call2(Actor_SetModeById, 20, 8);
  Call2(Actor_SetModeById, 19, 8);
  Call2(Actor_SetModeById, 21, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call2(Motion_SetVarCbAndRefresh, 20, 2);
  Call1(FieldEvent_Wait, 5);
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
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 265, 30) {
    /* FAKEMATCH: the native sets the flags before the speaker here; pinned to
     * r1 they are set first (one reordered row through the inline or as a plain
     * call). */
    register s32 flags __asm__("r1") = 0;

    FieldMessage_ShowAtActor(19, flags);
  }
  while (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 0, 0) != 0) {
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 30)
    FieldMessage_ShowAtActor(20, 0);
    EVENT_MESSAGE--;
  }
  EVENT_MESSAGE++;
  Call3(Func_02001148, 20, 0, 5);
  Call2(Actor_SetModeById, 20, 1);
  Call2(Actor_SetModeById, 19, 1);
  Call2(Actor_SetModeById, 21, 1);
  Call1(FieldEvent_Wait, 30);
  Call2(Motion_SetModeAndWaitAnimation, 21, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 21, 0, 5);
  Func_02000404();
  Call1(FieldEvent_Wait, 60);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Func_02001158, 0, FACING_SOUTH);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 264, 30)
  Call3(Actor_ArmCallback, 0, FACING_NORTHEAST, 0);
  Call3(Func_02001148, 20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 265, 30)
  Call3(Func_02001148, 21, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 19, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 19, 0, 5);
  Call2(Motion_SetModeAndWaitAnimation, 20, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Func_02001148, 20, 0, 5);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 4, 3);
  Call2(Motion_SetModeAndWaitAnimation, 18, 3);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetSpeedParameters, 19, 0x10000, 0x8000);
  Call3(Actor_SetSpeedParameters, 20, 0x10000, 0x8000);
  Call3(Actor_SetSpeedParameters, 21, 0x10000, 0x8000);
  Call2(Actor_SetMovementTypeById, 19, gData_020015e0);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetMovementTypeById, 20, gData_0200161c);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetMovementTypeById, 21, gData_0200161c);
  Call1(FieldEvent_Wait, 100);
  Call3(Actor_TurnToFaceEachOther, 0, 4, 0);
  Call3(Actor_SetAngleToward, 18, 4, 0);
  Call1(FieldEvent_Wait, 40);
  Call2(Actor_SetModeById, 0, 3);
  Call2(Actor_SetModeById, 4, 3);
  Call2(Motion_SetModeAndWaitAnimation, 18, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_SetModeById, 0, 2);
  target = (s16 *)Value1((s32 (*)())Actor_GetById, 4);
  if (target != 0)
    Call3(Actor_ResetAndSetPosition, 0, target[5], target[9]);
  Call1(Actor_WaitForMoveById, 0);
  Call3(Func_020010e8, 0, 0, 0);
  Call2(Actor_SetModeById, 18, 2);
  target = (s16 *)Value1((s32 (*)())Actor_GetById, 4);
  if (target != 0)
    Call3(Actor_ResetAndSetPosition, 18, target[5], target[9]);
  Call1(Actor_WaitForMoveById, 18);
  Call3(Func_020010e8, 18, 0, 0);
  Func_02001088();
}
#endif
