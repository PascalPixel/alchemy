#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "event_inline.h"

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
void Func_02001904();
void Func_02001688(void);
void Func_02001730(void);
void Func_02001d14(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02001944(void) {
  struct Actor *actor = Actor_Get(8);
  struct Actor *leader;
  FieldEvent_SetMessageId(MsgHereLetMe);
  Event_ShowMessage(8, 0);
  FieldActor_Stop(9);
  FieldActor_Stop(10);
  (Actor_Get(9))->update = 0;
  (Actor_Get(10))->update = 0;
  System_Wait(1);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  Actor_SetScript(9, gData_0200277c);
  Actor_SetScript(10, gData_020027c0);
  (FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x1300000, -1, 0x36e0000, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 872);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
  Event_PlaySound(78);
  Actor_FaceDirection(8, 0xb000, 20);
  Actor_FaceDirectionAndWait(8, 0xd000);
  FieldActor_Jump(8, 2, 10);
  FieldActor_Jump(8, 4, 10);
  Actor_SetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  Actor_SetSpeed(11, 0x19999, 0xcccc);
  Actor_SetScript(8, gData_02002804);
  Actor_SetScript(9, gData_02002870);
  Actor_SetScript(10, gData_020028dc);
  Actor_RunScriptAndWait(11, gData_02002948);
  Actor_SetPosition(5, 0x1280000, 0x3680000);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xe000, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_WalkToAndWait(5, 312, 854);
  Actor_FaceDirectionAndWait(5, 0x6000);
  Actor_SetAnimationAndWait(5, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0) {
    Actor_SetDestination(5, ((s16 *)(&leader->x))[1], ((s16 *)(&leader->z))[1]);
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 316, 0x380);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 344, 0x390);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0xc000);
  Event_PlaySound(17);
  Actor_FaceDirectionAndWait(8, 0xc000);
  Event_ShowMessage(8, 0);
  FieldCamera_FollowActor(8, 1);
  Actor_SetScript(8, gData_02002998);
  Actor_SetScript(10, gData_020029c0);
  Actor_RunScriptAndWait(9, gData_02002a04);
  Actor_WalkToAndWait(11, 344, 890);
  Actor_FaceDirectionAndWait(11, 0xc000);
  Call1(Func_02001904, 8);
  Call1(Func_02001904, 10);
  Call1(Func_02001904, 9);
  Call1(Func_02001904, 11);
  Call1(Func_02001904, gMainRoutineWork.currentOwner);
  {
    s32 priority = 0x480;
    Task_Add(Func_02001688, priority);
  }
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait1:
  {
    s16 *count;
    System_Wait(1);
    if ((*(&actor->action)) != 34) {
      goto wait1;
    }
  }

  actor->unk66 = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait2:
  System_Wait(1);

  if (actor->action != 42) {
    goto wait2;
  }
  actor->unk66 = 2;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  actor->unk66 = 1;
wait3:
  System_Wait(1);

  if (actor->action != 108) {
    goto wait3;
  }
  actor->unk66 = 3;
  System_Wait(1);
  Event_ShowMessage(8, 0);
  Func_02001730();
  FieldActor_Jump(8, 4, 0);
  FieldActor_Jump(10, 4, 0);
  FieldActor_Jump(9, 4, 0);
  FieldActor_Jump(11, 4, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(8, 0x5000, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x8000, 0);
  Actor_FaceDirectionAndWait(11, 0x8000);
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Func_02001d14();
  } else {
    FieldEvent_ShowMessageThenWait(8, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(123);
    FieldEvent_Wait(40);
    FieldEvent_SetExitRequest(15);
  }
}
