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
  struct Actor *object = Actor_GetById(id);

  Actor_SetModeById(id, 1);
  Actor_SetSingleChildFlags(Actor_GetById(id), 1);
  object->x -= PIXELS(12);
  ((struct ActorSprite *)object->animation)->rotation = 0;
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(id), 1);
  object->velocityY = PIXELS(4);
  object->motionFlags = 3;
  wait(40);
}

void Import_EventStart();
void Import_EventEnd();

/// A scene of actor 8 and the leader that ends by setting flag 0x863.
void Func_020000f0(void) {
  s32 message;
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Motion_SetVarCbAndRefresh(8, 1);
  message = (s32)MsgIdejimaGroan;
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_ShowMessageAndWait(8, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
  actor = Actor_GetById(8);
  Actor_SetModeById(8, 1);
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  actor->velocityY = PIXELS(4);
  actor->motionFlags = 3;
  wait(40);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(8, 0x102);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 20);
  Func_080d47b4(8, 0x101, 20);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_WEST, 40);
  Func_080d47b4(8, 0x100, 20);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 416, 680);
  Event_ShowMessageAndWait(8, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
  Event_Wait(20);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetModeById(8, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(8);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  FieldMessage_Show(message + 8, 1);
  GameFlag_SetBit(0x863);
  Import_EventEnd();
}

/// A scene of actor 5 and the leader that ends by setting flag 0x864.
void Func_020002b4(void) {
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Motion_SetVarCbAndRefresh(5, 1);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgIdejimaAwakeningExclamation);
  Event_ShowMessageThenWait(5, 0, 40);
  Actor_ShowEmote(5, 0x101, 20);
  Event_ShowMessageAndWait(5, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
  actor = Actor_GetById(5);
  Actor_SetModeById(5, 1);
  Actor_SetSingleChildFlags(Actor_GetById(5), 1);
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(5), 1);
  actor->velocityY = PIXELS(4);
  actor->motionFlags = 3;
  wait(40);
  Func_080d47b4(5, 0x100, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(5, FACING_NORTHWEST, 20);
  Actor_FaceDirection(5, FACING_WEST, 40);
  Actor_Jump(5, 4, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetAngleToward(5, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 5, 0);
  Func_080d47b4(5, 0x108, 0);
  Event_ShowMessageAndWait(5, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_Wait(40);
  Func_080d47b4(5, 0x101, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(5, 3);
    Event_ShowMessageAndWait(5, 0);
  } else {
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Event_ShowMessageAndWait(5, 0);
  }
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetModeById(5, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(5, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Event_PrepareObjectAndApplyValue(5, 1);
  GameFlag_SetBit(0x864);
  Import_EventEnd();
}
