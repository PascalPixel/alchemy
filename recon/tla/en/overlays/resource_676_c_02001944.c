// Draft, differ 16 of 367: a scene in Yallam (message MsgHereLetMe on): actors
// 9 and 10 run off, actor 5 walks to the controlled character, the party is
// placed, Func_02001688 is scheduled and actor 8 is walked through three steps
// (its action count reaching 34, 42 and 108, its phase 1, 2 and 3), then actor
// 8 asks a yes-or-no question: yes runs Func_02001d14, no fades out and plays
// cue 123. The first wait reads the action count through a pointer taken
// before the leader is fetched (30 read from actor): it gives the action
// field's address r6 as the native has it. Left: the native computes that
// address inside the first wait loop (the GCSE hoist seen in overlay 698's
// landing burst) and orders a few loads around it differently.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 MsgHereLetMe[];
extern const u8 gData_0200277c[];
extern const u8 gData_020027c0[];
extern const u8 gData_02002804[];
extern const u8 gData_02002870[];
extern const u8 gData_020028dc[];
extern const u8 gData_02002948[];
extern const u8 gData_02002998[];
extern const u8 gData_020029c0[];
extern const u8 gData_02002a04[];
s32 Object_GetById();
void Func_02002520();
void Func_02002538();
void ObjectMotion_EnableActionAndResetMotion();
void WaitFrames();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_EnableActionAndSetCallback();
s32 Func_02002570();
void Func_02002560();
void Camera_MoveWithinMapBounds();
void ObjectMotion_SetPositionAndReset();
void Func_02002548();
void Func_020025a0();
void ObjectMotion_ArmCallback();
void ObjectMotion_Launch();
void Motion_SetModeAndWaitAnimation();
void Object_SetActionCallbackAndRefreshById();
void Func_020024f8();
void Object_SetModeById();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Camera_FollowActor();
void Func_02001904();
void Func_02001688(void);
void Task_Add();
void Func_02001730(void);
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
void Func_02001d14(void);
void Func_02002530();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitFrames();
void Func_02002578();

void Func_02001944(void) {
  struct ObjectRuntime *actor =
      (struct ObjectRuntime *)Value1(Object_GetById, 8);
  struct ObjectRuntime *leader;
  s16 *count;
  Call1(Func_02002520, MsgHereLetMe);
  Call2(Func_02002538, 8, 0);
  Call1(ObjectMotion_EnableActionAndResetMotion, 9);
  Call1(ObjectMotion_EnableActionAndResetMotion, 10);
  ((struct ObjectRuntime *)Value1(Object_GetById, 9))->update = 0;
  ((struct ObjectRuntime *)Value1(Object_GetById, 10))->update = 0;
  Call1(WaitFrames, 1);
  Call3(ObjectMotion_SetSpeedParameters, 9, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call2(ObjectMotion_EnableActionAndSetCallback, 9, gData_0200277c);
  Call2(ObjectMotion_EnableActionAndSetCallback, 10, gData_020027c0);
  ((struct ObjectRuntime *)Value0(Func_02002570))->motionFlags = 0;
  Call2(Func_02002560, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x36e0000, 1);
  Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
        0x6666);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 296, 872);
  Call2(Func_02002548, gAdventure.currentOwner, 0);
  Call1(Func_020025a0, 78);
  Call3(ObjectMotion_ArmCallback, 8, 0xb000, 20);
  Call2(Func_02002548, 8, 0xd000);
  Call3(ObjectMotion_Launch, 8, 2, 10);
  Call3(ObjectMotion_Launch, 8, 4, 10);
  Call2(Motion_SetModeAndWaitAnimation, 8, 3);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetSpeedParameters, 11, 0x19999, 0xcccc);
  Call2(ObjectMotion_EnableActionAndSetCallback, 8, gData_02002804);
  Call2(ObjectMotion_EnableActionAndSetCallback, 9, gData_02002870);
  Call2(ObjectMotion_EnableActionAndSetCallback, 10, gData_020028dc);
  Call2(Object_SetActionCallbackAndRefreshById, 11, gData_02002948);
  Call3(Func_020024f8, 5, 0x1280000, 0x3680000);
  Call1(WaitFrames, 1);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call3(ObjectMotion_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call3(ObjectMotion_SetPositionAndReset, 5, 312, 854);
  Call2(Func_02002548, 5, 0x6000);
  Call2(Motion_SetModeAndWaitAnimation, 5, 3);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call2(Object_SetModeById, 5, 2);
  count = &actor->action;
  leader =
      (struct ObjectRuntime *)Value1(Object_GetById, gAdventure.currentOwner);
  if (leader != 0)
    Call3(ObjectMotion_ResetAndSetPosition, 5, ((s16 *)&leader->x)[1],
          ((s16 *)&leader->z)[1]);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 5);
  Call3(Func_020024f8, 5, 0, 0);
  Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0x19999,
        0xcccc);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 316, 0x380);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 344, 0x390);
  Call2(Func_02002548, gAdventure.currentOwner, 0xc000);
  Call1(Func_020025a0, 17);
  Call2(Func_02002548, 8, 0xc000);
  Call2(Func_02002538, 8, 0);
  Call2(Camera_FollowActor, 8, 1);
  Call2(ObjectMotion_EnableActionAndSetCallback, 8, gData_02002998);
  Call2(ObjectMotion_EnableActionAndSetCallback, 10, gData_020029c0);
  Call2(Object_SetActionCallbackAndRefreshById, 9, gData_02002a04);
  Call3(ObjectMotion_SetPositionAndReset, 11, 344, 890);
  Call2(Func_02002548, 11, 0xc000);
  Call1(Func_02001904, 8);
  Call1(Func_02001904, 10);
  Call1(Func_02001904, 9);
  Call1(Func_02001904, 11);
  Call1(Func_02001904, gAdventure.currentOwner);
  Call2(Task_Add, (s32)Func_02001688, 0x480);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait1:
  Call1(WaitFrames, 1);
  if (*count != 34)
    goto wait1;
  actor->unk66 = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait2:
  Call1(WaitFrames, 1);
  if (actor->action != 42)
    goto wait2;
  actor->unk66 = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait3:
  Call1(WaitFrames, 1);
  if (actor->action != 108)
    goto wait3;
  actor->unk66 = 3;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  Func_02001730();
  Call3(ObjectMotion_Launch, 8, 4, 0);
  Call3(ObjectMotion_Launch, 10, 4, 0);
  Call3(ObjectMotion_Launch, 9, 4, 0);
  Call3(ObjectMotion_Launch, 11, 4, 40);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xc000, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x5000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
  Call2(Func_02002548, 11, 0x8000);
  Value2(UiText_OpenMessageAtObject, 8, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Func_02001d14();
  } else {
    Call3(Func_02002530, 8, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Call1(Func_020025a0, 123);
    Call1(FieldEvent_WaitFrames, 40);
    Call1(Func_02002578, 15);
  }
}
