#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02000098(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetAnimation(id, 1);
  Actor_SetSingleChildFlags(Actor_Get(id), 1);
  object->x -= PIXELS(12);
  ((struct ActorSprite *)object->animation)->rotation = 0;
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(id), 1);
  object->velocityY = PIXELS(4);
  object->motionFlags = 3;
  System_Wait(40);
}

void Import_EventStart();
void Import_EventEnd();

/// A scene of actor 8 and the leader that ends by setting flag 0x863.
void Func_020000f0(void) {
  s32 message;
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_RunRepeatedMotion(8, 1);
  message = (s32)MsgIdejimaGroan;
  FieldEvent_SetMessageId(message);
  Event_ShowMessage(8, 0);
  Actor_RunRepeatedMotion(8, 2);
  Event_ShowMessage(8, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
  actor = Actor_Get(8);
  Actor_SetAnimation(8, 1);
  Actor_SetSingleChildFlags(Actor_Get(8), 1);
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(8), 1);
  actor->velocityY = PIXELS(4);
  actor->motionFlags = 3;
  System_Wait(40);
  Event_ShowMessage(8, 0);
  Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(8, 0x102);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  Actor_ShowEmote(8, 0x101, 20);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_WEST, 40);
  Actor_ShowEmote(8, 0x100, 20);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 416, 680);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(8, 0);
  Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  Event_ShowMessage(8, 0);
  Actor_SetAnimation(8, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(8, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(8);
  Actor_SetPosition(8, 0, 0);
  Message_ShowCentered(message + 8, 1);
  Event_SetFlag(0x863);
  Import_EventEnd();
}

/// A scene of actor 5 and the leader that ends by setting flag 0x864.
void Func_020002b4(void) {
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_RunRepeatedMotion(5, 1);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaAwakeningExclamation);
  FieldEvent_ShowMessageThenWait(5, 0, 40);
  Actor_ShowEmote(5, 0x101, 20);
  Event_ShowMessage(5, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
  actor = Actor_Get(5);
  Actor_SetAnimation(5, 1);
  Actor_SetSingleChildFlags(Actor_Get(5), 1);
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(5), 1);
  actor->velocityY = PIXELS(4);
  actor->motionFlags = 3;
  System_Wait(40);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(5, FACING_NORTHWEST, 20);
  Actor_FaceDirection(5, FACING_WEST, 40);
  FieldActor_Jump(5, 4, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceActor(5, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 5, 0);
  Actor_ShowEmote(5, 0x108, 0);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(40);
  Actor_ShowEmote(5, 0x101, 0);
  Event_OpenMessage(5, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetAnimation(5, 3);
    Event_ShowMessage(5, 0);
  } else {
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Event_ShowMessage(5, 0);
  }
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(5, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Event_PrepareActorAndApplyValue(5, 1);
  Event_SetFlag(0x864);
  Import_EventEnd();
}
