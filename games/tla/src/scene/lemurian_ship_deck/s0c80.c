#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001f5c(void);
void Func_02001f64(void);
void Func_0200206c(void);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so the arguments are set up in their own order and each call
   * keeps its constants; as a direct call, the costly constant is built first
   * or shared, 15 of 40 instructions of Func_02000c80 differ in tla-en. */
  Map_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

s32 Func_02000c80(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov657Room1) {
    Func_02001f2c_(2, 2, 10, 24, 18, 10);
    Func_02001f2c_(0, 62, 26, 62, 3, 8);
    Func_02001f2c_(50, 65, 25, 61, 6, 1);
  }
  return 0;
}

/// Unless flag 0x8ff is set: the crew sights the ship from the deck and the
/// leader is asked about it. Either way flag 0x980 is set.
void Func_02000ce0(void) {
  s32 answer;
  s32 owner;

  if (Event_GetFlag(0x8ff) != 0) {
    Event_SetFlag(0x980);
    return;
  }
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_WalkToAndWait(4, 264, 136);
  FieldEvent_SetMessageId(MsgLookItS);
  FieldActor_LaunchFromLeader(5, 16, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(9, 32, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, -16, -16, FACING_SOUTH);
  FieldEvent_Wait(80);
  Actor_ShowEmote(9, 0x102, 20);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_WalkToAndWait(9, 300, 152);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(PIXELS(200), -1, PIXELS(344), 1);
  Func_0200206c();
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(600), -1, PIXELS(344), 1);
  Func_0200206c();
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0x66666, 0xcccc);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(128), 1);
  Func_0200206c();
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Actor_ShowEmote(9, 0x101, 0);
  Event_OpenMessage(9, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  answer = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(owner);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_WaitForAnimationChange(owner);
    FieldEvent_SetMessageId(MsgLookItS + 5); // the fifth message after it
    Event_ShowMessage(9, 0);
  }
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(9, 4);
  Event_ShowMessage(9, 0);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_Get(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 288, 144);
  FieldEvent_Wait(1);
  Actor_Get(9)->actionFlags |= 1;
  Event_ShowMessage(0x2009, 0);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  {
    struct Actor *target;

    Actor_SetAnimation(9, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(9, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
    Actor_SetAnimation(5, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(5, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
    Actor_SetAnimation(6, 2);
    target = Actor_Get(4);
    if (target != NULL)
      Actor_SetDestination(6, ((s16 *)&target->x)[1],
                                ((s16 *)&target->z)[1]);
  }
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Func_02001f64();
  Event_SetFlag(0x980);
}
