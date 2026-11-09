#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Import_EventStart(void);
void Import_EventEnd();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000098();

extern const u8 gData_02003650[];
extern const u8 gData_02003734[];
extern const u8 gData_02003790[];

/// A scene of actor 6 and the leader that ends by setting flag 0x865.
void Func_0200049c(void) {
  struct Actor *leader;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_SetMessageId(MsgIdejimaSecondGroan);
  Event_ShowMessageThenWait(6, 0, 40);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetModeById(6, 18);
  Event_ShowMessageAndWait(6, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
  Actor_GetById(6)->motionFlags = 3;
  Actor_Jump(6, 4, 0);
  Actor_SetModeById(6, 1);
  Actor_SetSingleChildFlags(Actor_GetById(6), 1);
  Event_Wait(20);
  Actor_TurnToFaceEachOther(6, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Func_080d47b4(6, 0x105, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 20);
  Actor_FaceDirection(6, FACING_NORTHWEST, 40);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Func_080d47b4(6, 0x100, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(6, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_WalkToAndWait(6, 212, 680);
  Actor_FaceDirection(6, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetModeById(6, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(6, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Event_PrepareObjectAndApplyValue(6, 1);
  GameFlag_SetBit(0x865);
  Import_EventEnd();
}

void Func_02000640(void) {
  Import_EventStart();
  Event_ResolvePendingActions(0);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(3);
}

void Func_02000664(void) {
  if (!Event_GetFlag(0x200)) {
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 152, 756);
    Event_SetFlag(0x200);
  }
}

/// The three scenes of actors 8, 5 and 6 with the leader, each run once:
/// flags 0x863, 0x864 and 0x865 record them.
void Func_020006a8(void) {
  struct Actor *leader;

  if (Event_GetFlag(0x863) == 0) {
    Import_EventStart();
    FieldEvent_ResolvePendingActions(0);
    Func_02000664();
    FieldEvent_SetMessageId((s32)MsgIdejimaHeyFelix);
    Event_ShowMessage(8, 0);
    Actor_SetPosition(5, 0, 0);
    Actor_SetPosition(6, 0, 0);
    Camera_SetSpeedParameters(0x9999, 0x1333);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(721), 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
    Call1(Func_02000098, 8);
    Actor_SetHorizontalPositionWithTerrain(8, PIXELS(312), PIXELS(666));
    System_Wait(1);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_WalkToAndWait(8, 212, 680);
    Actor_WalkToAndWait(8, 192, 712);
    Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
    FieldActor_RefreshSelectorById(8);
    Actor_RunScriptAndWait(8, gData_02003650);
    FieldActor_Jump(8, 4, 20);
    Event_ShowMessage(8, 0);
    Actor_ShowEmote(8, 257, 0);
    Event_ShowMessage(8, 0);
    Actor_ShowEmote(8, 0x100, 20);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 0);
    Event_ShowMessage(8, 0);
    Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
    Event_ShowMessage(8, 0);
    Actor_SetAnimation(8, 4);
    Event_ShowMessage(8, 0);
    Actor_WalkToAndWait(8, 172, 756);
    Actor_FaceEachOther(8, gMainRoutineWork.currentOwner, 0);
    Actor_SetAnimation(8, 3);
    Event_ShowMessage(0x6008, 0);
    Actor_SetAnimation(8, 2);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != 0)
      Actor_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                                ((s16 *)&leader->z)[1]);
    FieldActor_WaitForMoveById(8);
    Actor_SetPosition(8, 0, 0);
    Message_ShowCentered((s32)MsgIdejimaHeyFelix + 8, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    Event_SetFlag(0x863);
    Import_EventEnd();
  }
  if (Event_GetFlag(0x864) == 0) {
    Import_EventStart();
    FieldEvent_ResolvePendingActions(0);
    Func_02000664();
    Actor_SetHorizontalPositionWithTerrain(5, PIXELS(312), PIXELS(666));
    System_Wait(1);
    FieldEvent_SetMessageId((s32)MsgIdejimaCantBelieveYouTwo);
    Event_ShowMessage(5, 0);
    Actor_SetPosition(6, 0, 0);
    Camera_SetSpeedParameters(0x9999, 0x1333);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(721), 1);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetScript(8, gData_02003734);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
    Call1(Func_02000098, 5);
    Actor_SetSpeed(5, 0x19999, 0xcccc);
    Actor_WalkToAndWait(5, 212, 680);
    Actor_WalkToAndWait(5, 192, 712);
    Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
    FieldActor_RefreshSelectorById(5);
    Actor_ShowEmote(5, 259, 0);
    FieldActor_Jump(5, 4, 20);
    Event_ShowMessage(5, 0);
    Event_ShowMessage(5, 0);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
    Actor_SetAttachedEffect(8, 258);
    Actor_ShowEmote(5, 263, 20);
    Event_ShowMessage(5, 0);
    Actor_ShowEmote(8, 261, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FieldEvent_ShowMessageThenWait(8, 0, 40);
    Actor_ShowEmote(5, 261, 40);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(5, FACING_NORTHEAST, 40);
    Event_ShowMessage(0x4005, 0);
    Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 20);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_WalkToAndWait(5, 178, 728);
    Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
    Event_ShowMessage(0x4005, 0);
    Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 20);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetScript(8, gData_02003790);
    Actor_RunScriptAndWait(5, gData_02003790);
    Event_PrepareActorAndApplyValue(5, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    Event_SetFlag(0x864);
    Import_EventEnd();
  }
  if (Event_GetFlag(0x865) == 0) {
    Import_EventStart();
    Event_ResolvePendingActions(0);
    Func_02000664();
    Actor_SetHorizontalPositionWithTerrain(6, PIXELS(312), PIXELS(666));
    wait(1);
    Event_SetMessageId((s32)MsgIdejimaWaitForMe);
    Event_ShowMessageAndWait(6, 0);
    Camera_SetSpeedParameters(0x9999, 0x1333);
    Camera_MoveWithinMapBounds(PIXELS(168), -1, PIXELS(721), 1);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetMovementTypeById(8, (s32)gData_02003734);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
    Actor_GetById(6)->motionFlags = 3;
    Actor_Jump(6, 4, 0);
    Actor_SetModeById(6, 1);
    Actor_SetSingleChildFlags(Actor_GetById(6), 1);
    Event_Wait(20);
    Actor_SetSpeed(6, 0x19999, 0xcccc);
    Actor_WalkToAndWait(6, 212, 680);
    Actor_WalkToAndWait(6, 192, 712);
    Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
    Actor_RefreshSelectorById(6);
    Event_ShowMessageAndWait(6, 0);
    Func_080d489c(gMainRoutineWork.currentOwner, 258);
    Func_080d47b4(6, 263, 20);
    Event_ShowMessageAndWait(6, 0);
    Event_ShowMessageThenWait(8, 0, 40);
    Event_ShowMessageAndWait(6, 0);
    Func_080d47b4(gMainRoutineWork.currentOwner, 265, 40);
    Event_ShowMessageAndWait(6, 0);
    Actor_SetModeById(6, 4);
    Event_ShowMessageAndWait(6, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_ShowMessageAndWait(6, 0);
    Actor_SetModeById(8, 3);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_FaceDirection(6, FACING_WEST, 40);
    Actor_FaceDirection(6, FACING_SOUTHEAST, 40);
    Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
    Event_ShowMessageAndWait(6, 0);
    Func_080d47b4(6, 257, 20);
    Event_ShowMessageAndWait(6, 0);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 4);
    Motion_SetModeAndWaitAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_ShowMessageAndWait(6, 0);
    Actor_SetModeById(8, 3);
    Motion_SetModeAndWaitAnimation(8, 3);
    Actor_SetMovementTypeById(8, (s32)gData_02003790);
    Actor_SetMovementTypeAndWaitById(6, (s32)gData_02003790);
    Event_PrepareObjectAndApplyValue(6, 1);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    GameFlag_SetBit(0x865);
    Import_EventEnd();
  }
}
