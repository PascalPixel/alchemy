#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Import_ActorPlaceAtActor();
void Func_02003320();

void Idejima_RunAlexDeparture(void) {
  const u8 *script;

  Import_EventStart();
  Event_ResolvePendingActions(0);
  Camera_SetSpeedParameters(0x9999, 0x1333);
  Camera_MoveWithinMapBounds(PIXELS(108), -1, PIXELS(720), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 86, 712);
  Import_ActorPlaceAtActor(8, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 104, 712);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgIdejimaWhereHasAlexGone);
  Event_ShowMessage(0x2008, 0);
  Actor_ArmCallback(8, FACING_EAST, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 104, 728);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 20);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Actor_SetModeById(8, 3);
  Event_AskYesNoAndShowResponse(8, 0);
  Func_080d47b4(8, 0x106, 40);
  Event_ShowMessageAndWait(8, 0);
  Import_ActorPlaceAtActor(5, gMainRoutineWork.currentOwner);
  Import_ActorPlaceAtActor(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_WalkTo(5, 86, 712);
  Actor_WalkToAndWait(6, 86, 728);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_WaitForMoveById(5);
  Actor_SetModeById(5, 1);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(6, FACING_EAST, 40);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetModeById(8, 4);
  Event_OpenMessage(0x2008, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(8, 4);
    Event_ShowMessageAndWait(0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessageAndWait(0x2008, 0);
  }
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(0x2008, 0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x101, 40);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessage(0x2008, 0);
  Func_080d47b4(5, 0x100, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessage(0x2008, 0);
  Actor_ArmCallback(6, FACING_EAST, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(8, 0) == 0) {
    Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessageAndWait(0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Func_080d47b4(8, 0x103, 0);
    Actor_FaceDirection(8, FACING_SOUTH, 20);
    Event_ShowMessageAndWait(0x2008, 0);
    Func_02003320();
  }
  Actor_ShowEmote(5, 0x100, 0);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(8, 0x100, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(0x2008, 0);
  Func_080d47b4(6, 0x101, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 20);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  FieldMessage_ShowAtActor(0x2008, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_080d47b4(6, 0x103, 0);
    Actor_ArmCallback(6, FACING_EAST, 20);
    Event_ShowMessageAndWait(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Func_080d47b4(6, 0x108, 0);
    Actor_ArmCallback(6, FACING_EAST, 20);
    Event_ShowMessageAndWait(6, 0);
  }
  Func_080d47b4(8, 0x100, 0);
  Event_ShowMessage(0x2008, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Func_080d47b4(5, 0x101, 40);
  Event_ShowMessageAndWait(5, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 6);
  Actor_WatchAndSetScript(8, 6);
  Actor_WatchAndSetScript(5, 6);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 86, 748);
  Event_Wait(40);
  Func_080d47b4(6, 0x109, 80);
  Event_ShowMessage(0x2008, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(8);
  Actor_Stop(5);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(40);
  Actor_SetModeById(5, 4);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_RefreshSelectorById(6);
  Actor_SetModeById(6, 4);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(0x2008, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetModeById(8, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_AskYesNo(0x2008, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 40);
  Event_ShowMessageAndWait(6, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Actor_SetModeById(6, 4);
  Event_ShowMessageAndWait(6, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(8, 0x105, 0);
  Func_080d47b4(5, 0x105, 40);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAttachedEffect(8, 0x102);
  Event_ShowMessageAndWait(0x2008, 0);
  Func_080d47b4(6, 0x108, 20);
  Actor_FaceDirection(6, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(5, 3);
    Event_ShowMessageAndWait(5, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_SetModeById(5, 4);
    Event_ShowMessageAndWait(5, 0);
  }
  Func_080d47b4(8, 0x109, 40);
  Motion_SetModeAndWaitAnimation(6, 4);
  Event_Wait(20);
  Event_ShowMessage(0x2008, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Event_ShowMessage(0x2008, 0);
  Actor_Jump(8, 4, 20);
  Event_ShowMessageAndWait(0x2008, 0);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  script = gIdejima_ActorDepartureScript;
  Actor_SetMovementTypeById(8, script);
  Actor_SetMovementTypeById(5, script);
  Actor_SetMovementTypeAndWaitById(6, script);
  GameFlag_SetBit(0x867);
  Import_EventEnd();
}

extern s16 gData_02003c14;
extern s32 gData_02003c10;

void Func_0200153c(void) {
  gData_02003c14 = 0;
  gData_02003c10 = 0;
}
