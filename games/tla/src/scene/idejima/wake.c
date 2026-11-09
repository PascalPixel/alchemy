#include "types.h"
#include "input.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Actor_WaitForAnimationChange();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_02003180();

void FelixWake_StartStep(void) {}

void FelixWake_EndStep(void) {}

void FelixWake_WaitForButton(void) {
  s32 frames = 180;

  FelixWake_StartStep();
wait:
  if (--frames != -1) {
    wait(1);
    if (gInput.pressed == 0) {
      goto wait;
    }
  }
  FelixWake_EndStep();
}

/// The leader wakes, step by step at each button press, and is asked about
/// injuries; the scene ends by clearing flag 0x12f and setting flag 0x844.
void Func_02002c64(void) {
  s32 answer;
  s32 leader;

  FelixWake_StartStep();
  Import_EventStart();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  FieldCamera_MoveTo(PIXELS(384), -1, PIXELS(728), 0);
  wait(1);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 41);
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(384), PIXELS(728));
  Actor_SetHorizontalPositionWithTerrain(11, PIXELS(384), PIXELS(702));
  wait(1);
  Func_02003180();
  wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Motion_SetVarCbAndRefresh(gMainRoutineWork.currentOwner, 2);
  wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetModeById(gMainRoutineWork.currentOwner, 42);
  wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  wait(1);
  Actor_Jump(gMainRoutineWork.currentOwner, 4, 20);
  Event_SetMessageId((s32)MsgIdejimaCheckForInjuries);
  FieldMessage_ShowAtActor(11, 0);
  wait(1);
  answer = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  leader = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 45);
    wait(1);
    FelixWake_EndStep();
    Event_Wait(50);
    FelixWake_StartStep();
    Actor_SetModeById(gMainRoutineWork.currentOwner, 46);
    wait(1);
    FelixWake_EndStep();
    Event_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessageAndWait(11, 0);
    wait(1);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
    FelixWake_EndStep();
    Event_Wait(20);
    FelixWake_StartStep();
    Actor_SetModeById(gMainRoutineWork.currentOwner, 47);
    wait(1);
    FelixWake_EndStep();
    Event_Wait(50);
    FelixWake_StartStep();
    Actor_SetModeById(gMainRoutineWork.currentOwner, 48);
    wait(1);
    FelixWake_EndStep();
    Event_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessageAndWait(11, 0);
    wait(1);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  } else {
    Actor_WaitForAnimationChange(leader);
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 384, 746);
    FelixWake_EndStep();
    Event_Wait(20);
    FelixWake_StartStep();
    Event_ShowMessageAndWait(11, 0);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  }
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  GameFlag_ClearBit(0x12f);
  GameFlag_SetBit(0x844);
  Import_EventEnd();
  FelixWake_EndStep();
}
