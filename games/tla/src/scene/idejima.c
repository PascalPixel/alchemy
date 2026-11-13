/// @file
/// The scene code of overlay 0x650, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "call.h"
#include "io_reg.h"
#include "dma.h"
#include "map.h"
#include "module/scene/idejima.h"
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gIdejimaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gIdejimaExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gIdejimaPlacements;
}

const struct Trigger *Scene_GetEvents(void) {
  switch (gMainRoutineWork.entrance) {
  case 1:
  case 3:
    return gIdejimaEventsEntrance1;
  case ENTRANCE_WAKE:
  case ENTRANCE_WAKE_AGAIN:
    return gIdejimaEventsWake;
  default:
    return gIdejimaEvents;
  }
}

void Func_02000098(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetAnimation(id, 1);
  Actor_SetSingleChildFlags(Actor_Get(id), 1);
  object->x -= PIXELS(12);
  ((struct ActorSprite *)object->sprite)->rotation = 0;
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(id), 1);
  object->velocityY = PIXELS(4);
  object->motionFlags = 3;
  System_Wait(40);
}

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
    Actor_SetDestination(8, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
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
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
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
    Actor_SetDestination(5, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Event_PrepareActorAndApplyValue(5, 1);
  Event_SetFlag(0x864);
  Import_EventEnd();
}

extern u32 gData_02003790[];

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
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
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
    FieldCamera_SetSpeed(0x9999, 0x1333);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(721), 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
    Func_02000098(8);
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
    FieldCamera_SetSpeed(0x9999, 0x1333);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(721), 1);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetScript(8, gData_02003734);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
    Func_02000098(5);
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
    System_Wait(1);
    Event_SetMessageId((s32)MsgIdejimaWaitForMe);
    Event_ShowMessageAndWait(6, 0);
    FieldCamera_SetSpeed(0x9999, 0x1333);
    FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(721), 1);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetScript(8, (s32)gData_02003734);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
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
    FieldActor_RefreshSelectorById(6);
    Event_ShowMessageAndWait(6, 0);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
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
    Actor_SetScript(8, (s32)gData_02003790);
    Actor_RunScriptAndWait(6, (s32)gData_02003790);
    Event_PrepareObjectAndApplyValue(6, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    GameFlag_SetBit(0x865);
    Import_EventEnd();
  }
}

void Idejima_RunAlexDeparture(void) {
  const u8 *script;

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(PIXELS(108), -1, PIXELS(720), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 86, 712);
  Import_ActorPlaceAtActor(8, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 104, 712);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaWhereHasAlexGone);
  Event_ShowMessage(0x2008, 0);
  Actor_FaceDirection(8, FACING_EAST, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 104, 728);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Actor_SetAnimation(8, 3);
  Event_AskYesNo(8, 0);
  Actor_ShowEmote(8, 0x106, 40);
  Event_ShowMessage(8, 0);
  Import_ActorPlaceAtActor(5, gMainRoutineWork.currentOwner);
  Import_ActorPlaceAtActor(6, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_WalkTo(5, 86, 712);
  Actor_WalkToAndWait(6, 86, 728);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  FieldActor_WaitForMoveById(5);
  Actor_SetAnimation(5, 1);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 40);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetAnimation(8, 4);
  Event_OpenMessage(0x2008, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessage(0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(0x2008, 0);
  }
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 40);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetAnimation(8, 4);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0x2008, 0);
  Actor_FaceDirection(6, FACING_EAST, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Event_OpenMessage(5, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(8, 0) == 0) {
    Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessage(0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_ShowEmote(8, 0x103, 0);
    Actor_FaceDirection(8, FACING_SOUTH, 20);
    Event_ShowMessage(0x2008, 0);
    Func_02003320();
  }
  Actor_ShowEmote(5, 0x100, 0);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(8, 0x100, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Event_OpenMessage(0x2008, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_ShowEmote(6, 0x103, 0);
    Actor_FaceDirection(6, FACING_EAST, 20);
    Event_ShowMessage(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_ShowEmote(6, 0x108, 0);
    Actor_FaceDirection(6, FACING_EAST, 20);
    Event_ShowMessage(6, 0);
  }
  Actor_ShowEmote(8, 0x100, 0);
  Event_ShowMessage(0x2008, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(6, 3);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(5, 0x101, 40);
  Event_ShowMessage(5, 0);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, 6);
  FieldActor_WatchAndSetScript(8, 6);
  FieldActor_WatchAndSetScript(5, 6);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 86, 748);
  FieldEvent_Wait(40);
  Actor_ShowEmote(6, 0x109, 80);
  Event_ShowMessage(0x2008, 0);
  FieldActor_Stop(gMainRoutineWork.currentOwner);
  FieldActor_Stop(8);
  FieldActor_Stop(5);
  Actor_SetAnimationAndWait(6, 3);
  FieldEvent_Wait(40);
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(6);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(0x2008, 0);
  Actor_SetAnimationAndWait(6, 3);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(8, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_AskYesNo(0x2008, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 40);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(8, 0x105, 0);
  Actor_ShowEmote(5, 0x105, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessage(6, 0);
  Actor_SetAttachedEffect(8, 0x102);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(6, 0x108, 20);
  Actor_FaceDirection(6, FACING_NORTHEAST, 20);
  Event_ShowMessage(6, 0);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Event_OpenMessage(5, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimation(5, 3);
    Event_ShowMessage(5, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_SetAnimation(5, 4);
    Event_ShowMessage(5, 0);
  }
  Actor_ShowEmote(8, 0x109, 40);
  Actor_SetAnimationAndWait(6, 4);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2008, 0);
  Actor_SetAnimationAndWait(6, 3);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Event_ShowMessage(0x2008, 0);
  FieldActor_Jump(8, 4, 20);
  Event_ShowMessage(0x2008, 0);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  script = gIdejima_ActorDepartureScript;
  Actor_SetScript(8, script);
  Actor_SetScript(5, script);
  Actor_RunScriptAndWait(6, script);
  Event_SetFlag(0x867);
  Import_EventEnd();
}

extern s32 gData_02003c10;

void Func_0200153c(void) {
  gData_02003c14 = 0;
  gData_02003c10 = 0;
}

/// The water's wave scroll: each scanline's vertical offset for BG3 from the
/// back one of two line tables, streamed by DMA 0 at every HBlank.
void Func_02001558(void) {
  s16 *line = gData_02003990[(s16)(gData_02003c14 ^ 1)];

  s32 offset = line[0];

  /* FAKEMATCH(dead): an empty once-only block, so BG3VOFS's address is built
   * after the first line is loaded; as no block, 2 of 26 instructions differ in
   * tla-en. */
  do {
  } while (0);
  REG_BG3VOFS = offset;
  Dma_Set(line + 1, (void *)&REG_BG3VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          REG_DMA0);
}

void Func_020015a0(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  s16 *line = gData_02003990[gData_02003c14];
  s32 i;

  for (i = 0; i != 160; i++) {
    s32 wave =
        Math_Sin(((u8)gData_02003c10 - i - (layer->y >> 16) + 160) << 9) +
        0x10000;

    line[i] = (layer->y >> 16) + (wave >> 15);
  }
  gData_02003c14 ^= 1;
  gData_02003c10++;
}

/// Copies colours 128 to 191 of heap block 32 over colours 0 to 63, clears
/// colour 0 and calls PaletteFade_ApplyTarget(0x10000, 0).
void Func_0200161c(void) {
  struct FadePalette *palette = Ram_HeapSlots->block32;
  u16 *to = palette->colors;
  u16 *from = palette->colors + 128;
  u32 i;

  for (i = 0; i < 64; i++)
    *to++ = *from++;
  palette->colors[0] = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
}

void DriftScene_Prepare(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];

  layer->unk18 = 0x4000;
  layer->unk1c = 0x2000;
  MapGrid_CopyCellsTo(72, 10, 75, 8, 1, 1);
  Actor_SetAnimation(9, 2);
}

#include "shared/drift_spawn.c"
