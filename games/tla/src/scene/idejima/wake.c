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
    System_Wait(1);
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
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(384), -1, PIXELS(728), 0);
  System_Wait(1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(384), PIXELS(728));
  Actor_SetPosition(11, PIXELS(384), PIXELS(702));
  System_Wait(1);
  Func_02003180();
  System_Wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  System_Wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  System_Wait(1);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaCheckForInjuries);
  Event_OpenMessage(11, 0);
  System_Wait(1);
  answer = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  leader = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 45);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 46);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 47);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 48);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  } else {
    Actor_WaitForAnimationChange(leader);
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 384, 746);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  }
  Actor_SetPosition(11, 0, 0);
  Event_ClearFlag(0x12f);
  Event_SetFlag(0x844);
  Import_EventEnd();
  FelixWake_EndStep();
}
