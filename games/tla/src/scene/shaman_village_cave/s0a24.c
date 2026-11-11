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
    /* FAKEMATCH(pin): the emote pinned to r1, as the native */                \
    /* loads the emote and the frames before the actor at every */             \
    /* emote of this overlay; as an inline call the actor comes */             \
    /* one row early wherever the emote takes two instructions, */             \
    /* and an empty asm barrier at each call moves the literal */              \
    /* pools of a long scene. */                                               \
    register s32 emote_ __asm__("r1") = (emote);                               \
    /* FAKEMATCH(pin): the frames pinned to r2 with the emote above. */        \
    register s32 frames_ __asm__("r2") = (frames);                             \
    Actor_ShowEmote((actor), emote_, frames_);                                 \
  }

void Func_02000a24(void) {
  s16 *target;
  s32 angle = 0xe000;

  Event_ClearFlag(0xa71);
  Event_SetFlag(0xa72);
  Func_02001080();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(273), 0);
  FieldEvent_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(4, PIXELS(184), PIXELS(229), angle);
  Actor_SetPositionAndFacing(0, PIXELS(200), PIXELS(221), angle);
  Actor_SetPositionAndFacing(18, PIXELS(208), PIXELS(237), angle);
  Actor_SetAnimation(20, 8);
  Actor_SetAnimation(19, 8);
  Actor_SetAnimation(21, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 258, 30)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 263, 30)
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 265, 30) {
    /* FAKEMATCH(pin): the flags pinned to r1, so they are set before the
     * speaker as the native has; as Event_OpenMessage(19, 0), directly or
     * through the inline, one row is reordered. */
    register s32 flags __asm__("r1") = 0;

    Event_OpenMessage(19, flags);
  }
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
     * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the
     * actor comes one row early (see the macro). */
    SHAMAN_VILLAGE_CAVE_EMOTE(19, 258, 30)
    Event_OpenMessage(20, 0);
    EVENT_MESSAGE--;
  }
  EVENT_MESSAGE++;
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_SetAnimation(20, 1);
  Actor_SetAnimation(19, 1);
  Actor_SetAnimation(21, 1);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Func_02000404();
  FieldEvent_Wait(60);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  FieldEvent_Wait(10);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 264, 30)
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 265, 30)
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_SetAnimationAndWait(20, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Actor_SetScript(19, gData_020015e0);
  FieldEvent_Wait(20);
  Actor_SetScript(20, gData_0200161c);
  FieldEvent_Wait(20);
  Actor_SetScript(21, gData_0200161c);
  FieldEvent_Wait(100);
  Actor_FaceEachOther(0, 4, 0);
  Actor_FaceActor(18, 4, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(0, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_SetDestination(0, target[5], target[9]);
  FieldActor_WaitForMoveById(0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetAnimation(18, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_SetDestination(18, target[5], target[9]);
  FieldActor_WaitForMoveById(18);
  Actor_SetPosition(18, 0, 0);
  Func_02001088();
}
