#include "types.h"
#include "input.h"
#include "field_event.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Camera_MoveWithinMapBounds();
void Motion_SetVarCbAndRefresh();
void Motion_SetModeAndWaitAnimation();
void Actor_Jump();
void Actor_SetSpeedParameters();
void Actor_SetPositionAndReset();
void Actor_WaitForAnimationChange();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_Wait();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void GameFlag_SetBit();
void GameFlag_ClearBit();
void Func_02003180();
void Func_02003228();
void Func_02003310();

void FelixWake_StartStep(void) {}

void FelixWake_EndStep(void) {}

void FelixWake_WaitForButton(void) {
  s32 frames = 180;

  FelixWake_StartStep();
wait:
  if (--frames != -1) {
    Task_Wait(1);
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
  Func_02003310(0);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Task_Wait(1);
  Call4(Camera_MoveWithinMapBounds, 0x1800000, -1, 0x2d80000, 0);
  Task_Wait(1);
  Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 41);
  Task_Wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     0);
  Task_Wait(1);
  Call3(Func_02003228, gSequenceParentWork.currentOwner, 0x1800000, 0x2d80000);
  Call3(Func_02003228, 11, 0x1800000, 0x2be0000);
  Task_Wait(1);
  Func_02003180();
  Task_Wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Motion_SetVarCbAndRefresh(gSequenceParentWork.currentOwner, 2);
  Task_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 42);
  Task_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                     1);
  Task_Wait(1);
  Actor_Jump(gSequenceParentWork.currentOwner, 4, 20);
  Import_EventSetMessageId((s32)MsgIdejimaCheckForInjuries);
  Value2(FieldMessage_ShowAtActor, 11, 0);
  Task_Wait(1);
  answer = Event_AskYesNoWithActorAnimation(gSequenceParentWork.currentOwner, 0);
  leader = gSequenceParentWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 45);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 46);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Import_EventShowMessageAndWait(11, 0);
    Task_Wait(1);
    Motion_SetModeAndWaitAnimation(gSequenceParentWork.currentOwner, 3);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 47);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Import_ActorSetAnimation(gSequenceParentWork.currentOwner, 48);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Import_EventShowMessageAndWait(11, 0);
    Task_Wait(1);
    Motion_SetModeAndWaitAnimation(gSequenceParentWork.currentOwner, 3);
  } else {
    Actor_WaitForAnimationChange(leader);
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x6666, 0x3333);
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 384, 746);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Import_EventShowMessageAndWait(11, 0);
    Motion_SetModeAndWaitAnimation(gSequenceParentWork.currentOwner, 3);
  }
  Func_02003228(11, 0, 0);
  GameFlag_ClearBit(0x12f);
  GameFlag_SetBit(0x844);
  Import_EventEnd();
  FelixWake_EndStep();
}
