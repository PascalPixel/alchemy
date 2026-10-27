#include "types.h"
#include "input.h"
#include "field_event.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Camera_MoveWithinMapBounds();
void Motion_SetVarCbAndRefresh();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_WaitForAnimationChange();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_WaitTicks();
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
  Engine_EventStart();
  Func_02003310(0);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Task_Wait(1);
  Call4(Camera_MoveWithinMapBounds, 0x1800000, -1, 0x2d80000, 0);
  Task_Wait(1);
  Engine_ActorSetAnimation(gPartyState.currentOwner, 41);
  Task_Wait(1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Task_Wait(1);
  Call3(Func_02003228, gPartyState.currentOwner, 0x1800000, 0x2d80000);
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
  Motion_SetVarCbAndRefresh(gPartyState.currentOwner, 2);
  Task_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Engine_ActorSetAnimation(gPartyState.currentOwner, 42);
  Task_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Engine_ActorSetAnimation(gPartyState.currentOwner, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     1);
  Task_Wait(1);
  ObjectMotion_Launch(gPartyState.currentOwner, 4, 20);
  Engine_EventSetMessage((s32)MsgIdejimaCheckForInjuries);
  Value2(UiText_OpenMessageAtObject, 11, 0);
  Task_Wait(1);
  answer = Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0);
  leader = gPartyState.currentOwner;
  if (answer == 0) {
    ObjectMotion_WaitForAnimationChange(leader);
    Engine_ActorSetAnimation(gPartyState.currentOwner, 45);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(50);
    FelixWake_StartStep();
    Engine_ActorSetAnimation(gPartyState.currentOwner, 46);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(50);
    FelixWake_StartStep();
    Engine_EventShowMessage(11, 0);
    Task_Wait(1);
    Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(20);
    FelixWake_StartStep();
    Engine_ActorSetAnimation(gPartyState.currentOwner, 47);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(50);
    FelixWake_StartStep();
    Engine_ActorSetAnimation(gPartyState.currentOwner, 48);
    Task_Wait(1);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(50);
    FelixWake_StartStep();
    Engine_EventShowMessage(11, 0);
    Task_Wait(1);
    Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
  } else {
    ObjectMotion_WaitForAnimationChange(leader);
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x6666, 0x3333);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 384, 746);
    FelixWake_EndStep();
    FieldEvent_WaitTicks(20);
    FelixWake_StartStep();
    Engine_EventShowMessage(11, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
  }
  Func_02003228(11, 0, 0);
  GameFlag_ClearBit(0x12f);
  GameFlag_SetBit(0x844);
  Engine_EventEnd();
  FelixWake_EndStep();
}
