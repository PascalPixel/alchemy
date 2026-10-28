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
#include "actor.h"
#include "main_routine_work.h"
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
s32 Actor_GetById();
void Func_02002520();
void Func_02002538();
void Actor_Stop();
void wait();
void Actor_SetSpeedParameters();
void Actor_SetMovementTypeById();
s32 Func_02002570();
void Func_02002560();
void Camera_MoveWithinMapBounds();
void Actor_SetPositionAndReset();
void Func_02002548();
void Func_020025a0();
void Actor_ArmCallback();
void Actor_Jump();
void Motion_SetModeAndWaitAnimation();
void Actor_SetMovementTypeAndWaitById();
void Func_020024f8();
void Actor_SetModeById();
void Actor_ResetAndSetPosition();
void Actor_WaitForMoveById();
void Camera_FollowActor();
void Func_02001904();
void Func_02001688(void);
void Task_Add();
void Func_02001730(void);
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Func_02001d14(void);
void Func_02002530();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait();
void Func_02002578();

void Func_02001944(void) {
  struct Actor *actor =
      (struct Actor *)Value1(Actor_GetById, 8);
  struct Actor *leader;
  s16 *count;
  Call1(Func_02002520, MsgHereLetMe);
  Call2(Func_02002538, 8, 0);
  Call1(Actor_Stop, 9);
  Call1(Actor_Stop, 10);
  ((struct Actor *)Value1(Actor_GetById, 9))->update = 0;
  ((struct Actor *)Value1(Actor_GetById, 10))->update = 0;
  Call1(wait, 1);
  Call3(Actor_SetSpeedParameters, 9, 0x19999, 0xcccc);
  Call3(Actor_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call2(Actor_SetMovementTypeById, 9, gData_0200277c);
  Call2(Actor_SetMovementTypeById, 10, gData_020027c0);
  ((struct Actor *)Value0(Func_02002570))->motionFlags = 0;
  Call2(Func_02002560, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x36e0000, 1);
  Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xcccc,
        0x6666);
  Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 296, 872);
  Call2(Func_02002548, gMainRoutineWork.currentOwner, 0);
  Call1(Func_020025a0, 78);
  Call3(Actor_ArmCallback, 8, 0xb000, 20);
  Call2(Func_02002548, 8, 0xd000);
  Call3(Actor_Jump, 8, 2, 10);
  Call3(Actor_Jump, 8, 4, 10);
  Call2(Motion_SetModeAndWaitAnimation, 8, 3);
  Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3(Actor_SetSpeedParameters, 11, 0x19999, 0xcccc);
  Call2(Actor_SetMovementTypeById, 8, gData_02002804);
  Call2(Actor_SetMovementTypeById, 9, gData_02002870);
  Call2(Actor_SetMovementTypeById, 10, gData_020028dc);
  Call2(Actor_SetMovementTypeAndWaitById, 11, gData_02002948);
  Call3(Func_020024f8, 5, 0x1280000, 0x3680000);
  Call1(wait, 1);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xe000, 0);
  Call3(Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call3(Actor_SetPositionAndReset, 5, 312, 854);
  Call2(Func_02002548, 5, 0x6000);
  Call2(Motion_SetModeAndWaitAnimation, 5, 3);
  Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
  Call2(Actor_SetModeById, 5, 2);
  count = &actor->action;
  leader =
      (struct Actor *)Value1(Actor_GetById, gMainRoutineWork.currentOwner);
  if (leader != 0)
    Call3(Actor_ResetAndSetPosition, 5, ((s16 *)&leader->x)[1],
          ((s16 *)&leader->z)[1]);
  Call1(Actor_WaitForMoveById, 5);
  Call3(Func_020024f8, 5, 0, 0);
  Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0x19999,
        0xcccc);
  Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 316, 0x380);
  Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 344, 0x390);
  Call2(Func_02002548, gMainRoutineWork.currentOwner, 0xc000);
  Call1(Func_020025a0, 17);
  Call2(Func_02002548, 8, 0xc000);
  Call2(Func_02002538, 8, 0);
  Call2(Camera_FollowActor, 8, 1);
  Call2(Actor_SetMovementTypeById, 8, gData_02002998);
  Call2(Actor_SetMovementTypeById, 10, gData_020029c0);
  Call2(Actor_SetMovementTypeAndWaitById, 9, gData_02002a04);
  Call3(Actor_SetPositionAndReset, 11, 344, 890);
  Call2(Func_02002548, 11, 0xc000);
  Call1(Func_02001904, 8);
  Call1(Func_02001904, 10);
  Call1(Func_02001904, 9);
  Call1(Func_02001904, 11);
  Call1(Func_02001904, gMainRoutineWork.currentOwner);
  Call2(Task_Add, (s32)Func_02001688, 0x480);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait1:
  Call1(wait, 1);
  if (*count != 34)
    goto wait1;
  actor->unk66 = 2;
  Call1(wait, 1);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait2:
  Call1(wait, 1);
  if (actor->action != 42)
    goto wait2;
  actor->unk66 = 2;
  Call1(wait, 1);
  Call2(Func_02002538, 8, 0);
  actor->unk66 = 1;
wait3:
  Call1(wait, 1);
  if (actor->action != 108)
    goto wait3;
  actor->unk66 = 3;
  Call1(wait, 1);
  Call2(Func_02002538, 8, 0);
  Func_02001730();
  Call3(Actor_Jump, 8, 4, 0);
  Call3(Actor_Jump, 10, 4, 0);
  Call3(Actor_Jump, 9, 4, 0);
  Call3(Actor_Jump, 11, 4, 40);
  Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xc000, 0);
  Call3(Actor_ArmCallback, 8, 0x5000, 0);
  Call3(Actor_ArmCallback, 10, 0x5000, 0);
  Call3(Actor_ArmCallback, 9, 0x8000, 0);
  Call2(Func_02002548, 11, 0x8000);
  Value2(FieldMessage_ShowAtActor, 8, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gMainRoutineWork.currentOwner, 0) ==
      0) {
    Func_02001d14();
  } else {
    Call3(Func_02002530, 8, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Call1(Func_020025a0, 123);
    Call1(FieldEvent_Wait, 40);
    Call1(Func_02002578, 15);
  }
}
