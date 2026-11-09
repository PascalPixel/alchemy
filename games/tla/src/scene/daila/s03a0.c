#include "types.h"
#include "scene_tables.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"


/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct FieldActor *leader;

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

void Func_020003a0(void) {
  s32 msg;

  if (GameFlag_Test(0x84b))
    return;
  if (GameFlag_Test(0x910))
    return;
  if (GameFlag_Test(7))
    return;
  Import_EventStart();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaFelixCall;
  Event_SetMessageId(msg);
  Actor_SetHorizontalPositionWithTerrain(27, 472 << 16, 264 << 16);
  Event_Wait(30);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 504, 296);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(27, 472, 280);
  Actor_FaceDirection(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Func_080d47b4(27, 256, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  FieldActor_LaunchFromLeader(5, 8, 16, 0xa000);
  FieldActor_LaunchFromLeader(6, 16, 0, 0xa000);
  Func_080d3070(28, 32, -8, 0x8000);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_ShowEmote(5, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(5, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_EAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_SetAngleToward(6, 28, 0);
  Event_Wait(30);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(30);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 28, 0);
  Event_Wait(30);
  Actor_SetAngleToward(28, 27, 0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(30);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetModeById(27, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_ArmCallback(27, FACING_NORTH, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Actor_SetAngleToward(27, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(28, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(30);
  Actor_SetModeById(27, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(30);
  Actor_SetAngleToward(5, gMainRoutineWork.currentOwner, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(30);
    Event_SetMessageId(msg + 11);
    Event_ShowMessageAndWait(27, 0);
    Event_Wait(60);
    Func_080d47b4(5, 259, 0);
    Event_Wait(60);
    Actor_SetAngleToward(5, 27, 0);
    Event_ShowMessageAndWait(5, 0);
    Event_SetMessageId(msg + 15);
  } else {
    Event_Wait(30);
    Event_SetMessageId(msg + 13);
    Event_ShowMessageAndWait(27, 0);
    Event_Wait(60);
    Motion_SetVarCbAndRefresh(5, 1);
    Actor_SetAngleToward(5, 27, 0);
    Event_ShowMessageAndWait(5, 0);
  }
  Actor_SetModeById(27, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Motion_SetVarCbAndRefresh(6, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(60);
  Actor_SetAngleToward(27, 6, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Event_Wait(60);
  Func_080d47b4(28, 258, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(60);
  Actor_SetModeById(27, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(27, 0);
  Actor_WalkToAndWait(27, 504, 328);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Actor_WalkToAndWait(27, 456, 328);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Actor_WalkToAndWait(27, 424, 472);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 27, 0);
  Actor_SetAngleToward(5, 27, 0);
  Actor_SetAngleToward(6, 27, 0);
  Actor_SetAngleToward(28, 27, 0);
  Event_Wait(60);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Event_Wait(120);
  Actor_Jump(5, 4, 20);
  Actor_Jump(5, 4, 20);
  Event_Wait(30);
  Func_080d47b4(5, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(5, 0);
  Event_Wait(60);
  Actor_SetModeById(6, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(6, 0);
  Event_Wait(60);
  Actor_SetAngleToward(28, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_ArmCallback(28, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(28, 0);
  Event_Wait(60);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(120);
  Actor_JoinLeader(5);
  Actor_JoinLeader(6);
  Actor_JoinLeader(28);
  Import_EventEnd();
  GameFlag_SetBit(0x84b);
}
