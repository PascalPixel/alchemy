#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "scene_tables.h"
#include "event_inline.h"

void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_02000404();
void Func_02001080();
void Func_02001088();

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
    Func_080d47b4((actor), emote_, frames_);                                   \
  }

void Func_02000a24(void) {
  s16 *target;
  s32 angle = 0xe000;

  GameFlag_ClearBit(0xa71);
  GameFlag_SetBit(0xa72);
  Func_02001080();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds(PIXELS(199), -1, PIXELS(273), 0);
  Event_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(4, PIXELS(184), PIXELS(229), angle);
  Actor_SetPositionAndFacing(0, PIXELS(200), PIXELS(221), angle);
  Actor_SetPositionAndFacing(18, PIXELS(208), PIXELS(237), angle);
  Actor_SetModeById(20, 8);
  Actor_SetModeById(19, 8);
  Actor_SetModeById(21, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Motion_SetVarCbAndRefresh(20, 2);
  Event_Wait(5);
  Event_ShowMessageThenWait(21, 0, 5);
  Event_ShowMessageThenWait(19, 0, 5);
  Event_ShowMessageThenWait(20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 258, 30)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 263, 30)
  Event_ShowMessageThenWait(0, 0, 5);
  Motion_SetModeAndWaitAnimation(21, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(21, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 265, 30) {
    /* FAKEMATCH: the native sets the flags before the speaker here; pinned to
     * r1 they are set first (one reordered row through the inline or as a plain
     * call). */
    register s32 flags __asm__("r1") = 0;

    FieldMessage_ShowAtActor(19, flags);
  }
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_CAVE_EMOTE. */
    SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 30)
    FieldMessage_ShowAtActor(20, 0);
    EVENT_MESSAGE--;
  }
  EVENT_MESSAGE++;
  Event_ShowMessageThenWait(20, 0, 5);
  Actor_SetModeById(20, 1);
  Actor_SetModeById(19, 1);
  Actor_SetModeById(21, 1);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(21, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(21, 0, 5);
  Func_02000404();
  Event_Wait(60);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 5);
  Actor_ArmCallbackAndRefresh(0, FACING_SOUTH);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 264, 30)
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  Event_ShowMessageThenWait(20, 0, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_CAVE_EMOTE. */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 265, 30)
  Event_ShowMessageThenWait(21, 0, 5);
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 5);
  Motion_SetModeAndWaitAnimation(20, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(20, 0, 5);
  Actor_SetModeById(0, 3);
  Actor_SetModeById(4, 3);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Actor_SetMovementTypeById(19, gData_020015e0);
  Event_Wait(20);
  Actor_SetMovementTypeById(20, gData_0200161c);
  Event_Wait(20);
  Actor_SetMovementTypeById(21, gData_0200161c);
  Event_Wait(100);
  Actor_TurnToFaceEachOther(0, 4, 0);
  Actor_SetAngleToward(18, 4, 0);
  Event_Wait(40);
  Actor_SetModeById(0, 3);
  Actor_SetModeById(4, 3);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  Actor_SetModeById(0, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_ResetAndSetPosition(0, target[5], target[9]);
  Actor_WaitForMoveById(0);
  Actor_SetHorizontalPositionWithTerrain(0, 0, 0);
  Actor_SetModeById(18, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_ResetAndSetPosition(18, target[5], target[9]);
  Actor_WaitForMoveById(18);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Func_02001088();
}
