#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"
s32 Func_02001da0();
s32 Func_02001010();
s32 Func_02001d40();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_02000f00();
s32 Func_02001064();
s32 Func_02001f08();
s32 Func_02001da8();
extern u8 MsgIMightBe[];
extern u8 gData_02001f4c[];

struct GroupWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c4 - 0x1b0];
  u16 unk1c4;
};

void Func_02001710(s32 p0) {
  s32 *owner;
  s32 facing;
  s32 angleC;
  s32 zero;
  s32 turn;
  struct HeapSlots *heap;
  Func_02001da0(p0);
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  Func_02001010();
  Func_02001d40();
  System_Wait(1);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  {
    s32 angleA = 0xe000;

    Actor_SetPositionAndFacing(*owner, 0x1680000, 0xd80000, angleA);
    Actor_SetPositionAndFacing(7, 0x1780000, 0xd80000, angleA);
  }
  facing = 0xa000;
  zero = 0;
  Actor_SetPositionAndFacing(11, 0x1680000, 0xc80000, zero);
  Actor_SetPositionAndFacing(5, 0x1980000, 0xd80000, facing);
  angleC = 0x8000;
  Actor_SetPositionAndFacing(6, 0x1a80000, 0xc80000, angleC);
  {
    s32 angleB = 0xb000;

    Actor_SetPositionAndFacing(12, 0x1880000, 0xc80000, angleB);
  }
  heap = Ram_HeapSlots;
  System_Wait(1);
  {
    s32 *effect = &((struct GroupWork *)heap->eventWork)->screenEffect;

    turn = 0x100;
    *effect = turn;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Actor_FaceDirection(12, 0xd000, 20);
  Actor_FaceDirectionAndWait(12, 0x5000);
  Func_080d47b4(12, 0x106, 40);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(6, facing, 0);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkToAndWait(12, 0x188, 168);
  Event_PlaySound(210);
  Func_02000f00();
  FieldEvent_Wait(40);
  Event_PlaySound(185);
  {
    struct Actor *guard = Actor_GetById(8);

    guard->motionFlags = zero;
  }
  Func_02001064();
  Actor_SetSpeed(8, 0x6666, 0x3333);
  Actor_MoveToAndWait(8, 0x1a8, 152);
  Actor_FaceDirectionAndWait(12, 0x5000);
  FieldEvent_SetMessageId((s32)MsgIMightBe);
  Event_ShowMessage(0x400c, 0);
  Actor_SetAnimationAndWait(12, 3);
  Actor_WalkToAndWait(12, 0x188, 140);
  System_Wait(1);
  Actor_SetPosition(12, 0, 0);
  System_Wait(1);
  Func_080d47b4(5, turn, 20);
  Actor_FaceDirection(5, angleC, 0);
  FieldMessage_ShowAtActor(0x4005, 0);
  if (Event_AskYesNoWithActorAnimation(*owner, 0) == 0) {
    Actor_FaceDirectionAndWait(7, 0);
    Actor_SetAnimation(7, 3);
    Event_ShowMessage(0x4007, 0);
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
  } else {
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
    Actor_FaceDirectionAndWait(7, angleC);
    Event_ShowMessage(0x4007, 0);
    Func_02001f08();
  }
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_SetAttachedEffect(6, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x4006, 0);
  Actor_FaceDirectionAndWait(11, 0);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirectionAndWait(11, 0xd000);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirectionAndWait(11, 0x3000);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(0x400b, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 20);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(11, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  {
    s32 path = (s32)gData_02001f4c;

    Actor_SetScript(11, path);
    Actor_SetScript(7, path);
    Actor_SetScript(5, path);
    Actor_RunScriptAndWait(6, path);
  }
  Event_SetFlag(0x8fd);
  Func_02001da8();
}
