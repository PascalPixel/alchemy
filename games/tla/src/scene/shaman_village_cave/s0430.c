#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "edition.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001080();
void Func_02001180();
void Func_020011a0();

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

void Func_02000430(void) {
  s32 angle;

  if (Event_GetFlag(0x821) == 0)
    return;
  if (Event_GetFlag(0) == 0)
    return;
  Func_02001080();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa70);
  FieldEvent_SetMessageId((s32)MsgIFound2);
  Actor_SetAnimation(4, 1);
  FieldEvent_ShowMessageThenWait(19, 0, 1);
  Actor_WalkToAndWait(4, 200, 221);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(5);
  Actor_WalkBy(4, -16, 8);
  FieldActor_LaunchFromLeader(0, 0, 0, 0xc000);
  FieldActor_LaunchFromLeader(18, 8, 16, 0xc000);
  FieldActor_WaitForMoveById(0);
  FieldActor_WaitForMoveById(18);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(18, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 256, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 40)
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(177), 1);
  Func_02001180();
  angle = 0x4000;
  Actor_SetPositionAndFacing(19, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(19, 231, 199);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(20, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(20, 220, 191);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(21, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(21, 204, 191);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(225), 1);
  Func_02001180();
  FieldActor_WaitForMoveById(19);
  Actor_FaceDirection(19, 0x5000, 0);
  FieldActor_WaitForMoveById(20);
  Actor_FaceDirection(20, 0x5000, 0);
  FieldActor_WaitForMoveById(21);
  Actor_FaceDirectionAndWait(21, 0x5000);
  Actor_RunRepeatedMotion(21, 2);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 258, 30)
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(19, 264, 30)
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(21, 263, 30)
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_RunRepeatedMotion(19, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_FaceEachOther(18, 4, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 257, 30)
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHEAST);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 265, 30)
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  Actor_SetAnimation(21, 4);
  Actor_SetAnimation(19, 4);
  Actor_SetAnimationAndWait(20, 4);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(4, 256, 0)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(18, 256, 30)
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(0, 258, 30)
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceEachOther(20, 21, 0);
  FieldEvent_Wait(10);
  /* FAKEMATCH(pin): the emote and the frames, pinned to r1 and r2 by
   * SHAMAN_VILLAGE_CAVE_EMOTE, load before the actor; as a plain call the actor
   * comes one row early (see the macro). */
  SHAMAN_VILLAGE_CAVE_EMOTE(20, 263, 30)
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(21, 0x5000, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_SetDestinationOffset(20, -16, 24);
  Actor_SetDestinationOffset(21, -16, 24);
  Actor_SetDestinationOffset(19, -16, 24);
  FieldActor_WaitForMoveById(19);
  /* FAKEMATCH(dead): the once-only block loads gMainRoutineWork before its
   * offset is built; as the plain store, one row is reordered. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov695, 20);
  Event_SetDefeatDestination((s32)&SceneId_Ov695, 21);
  Func_020011a0(102, 2);
}
