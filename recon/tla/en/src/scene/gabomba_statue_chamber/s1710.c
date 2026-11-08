#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
s32 Func_02001da0();
s32 Func_02001ee8();
s32 Camera_MoveWithinMapBounds();
s32 wait();
s32 Func_02001010();
s32 Func_02001d40();
s32 Func_02001e18();
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 FieldEvent_Wait();
s32 Actor_ArmCallback();
s32 Func_02001e70();
s32 Func_02001e88();
s32 Actor_SetSpeedParameters();
s32 Actor_SetPositionAndReset();
s32 Func_02001f10();
s32 Func_02000f00();
s32 Func_02001064();
s32 Actor_SetPositionAndCommit();
s32 Func_02001e48();
s32 Func_02001e60();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_02001e10();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
s32 Func_02001f08();
s32 Func_02001e90();
s32 Actor_SetMovementTypeById();
s32 Actor_SetMovementTypeAndWaitById();
s32 GameFlag_SetBit();
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
  Call1((void (*)())Func_02001da0, p0);
  Call1((void (*)())Func_02001ee8, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1((void (*)())wait, 1);
  Func_02001010();
  Func_02001d40();
  Call1((void (*)())wait, 1);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  {
    s32 angleA = 0xe000;

    Call4((void (*)())Func_02001e18, *owner, 0x1680000, 0xd80000, angleA);
    Call4((void (*)())Func_02001e18, 7, 0x1780000, 0xd80000, angleA);
  }
  facing = 0xa000;
  zero = 0;
  Call4((void (*)())Func_02001e18, 11, 0x1680000, 0xc80000, zero);
  Call4((void (*)())Func_02001e18, 5, 0x1980000, 0xd80000, facing);
  angleC = 0x8000;
  Call4((void (*)())Func_02001e18, 6, 0x1a80000, 0xc80000, angleC);
  {
    s32 angleB = 0xb000;

    Call4((void (*)())Func_02001e18, 12, 0x1880000, 0xc80000, angleB);
  }
  heap = Ram_HeapSlots;
  Call1((void (*)())wait, 1);
  {
    s32 *effect = &((struct GroupWork *)heap->eventWork)->screenEffect;

    turn = 0x100;
    *effect = turn;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_ArmCallback, 12, 0xd000, 20);
  Call2((void (*)())Func_02001e70, 12, 0x5000);
  Func_02001e88(12, 0x106, 40);
  Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 6, facing, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 12, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
  Call3((void (*)())Actor_SetPositionAndReset, 12, 0x188, 168);
  Call1((void (*)())Func_02001f10, 210);
  Func_02000f00();
  Call1((void (*)())FieldEvent_Wait, 40);
  Call1((void (*)())Func_02001f10, 185);
  {
    struct Actor *guard = Actor_GetById(8);

    guard->motionFlags = zero;
  }
  Func_02001064();
  Call3((void (*)())Actor_SetSpeedParameters, 8, SPLIT_CONSTANT(0x6600, 102), SPLIT_CONSTANT(0x3300, 51));
  Call3((void (*)())Actor_SetPositionAndCommit, 8, 0x1a8, 152);
  Call2((void (*)())Func_02001e70, 12, 0x5000);
  Call1((void (*)())Func_02001e48, (s32)MsgIMightBe);
  Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 12), 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 12, 3);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 0x188, 140);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_02001e10, 12, 0, 0);
  Call1((void (*)())wait, 1);
  Func_02001e88(5, turn, 20);
  Call3((void (*)())Actor_ArmCallback, 5, angleC, 0);
  FieldMessage_ShowAtActor(SPLIT_CONSTANT(0x4000, 5), 0);
  if (Event_AskYesNoWithActorAnimation(*owner, 0) == 0) {
    Call2((void (*)())Func_02001e70, 7, 0);
    Call2((void (*)())Actor_SetModeById, 7, 3);
    Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 7), 0);
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
  } else {
    ((struct GroupWork *)heap->eventWork)->unk1c4++;
    Call2((void (*)())Func_02001e70, 7, angleC);
    Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 7), 0);
    Func_02001f08();
  }
  Call3((void (*)())Actor_ArmCallback, 6, 0x8000, 0);
  Call2((void (*)())Func_02001e90, 6, 0x102);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 6), 0);
  Call2((void (*)())Func_02001e70, 11, 0);
  Call2((void (*)())Actor_SetModeById, 11, 4);
  Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 11), 0);
  Call2((void (*)())Func_02001e70, 11, 0xd000);
  Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 11), 0);
  Call2((void (*)())Func_02001e70, 11, 0x3000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
  Call2((void (*)())Func_02001e60, SPLIT_CONSTANT(0x4000, 11), 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 11, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
  Call3((void (*)())Actor_SetSpeedParameters, 7, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, SPLIT_CONSTANT(0x9900, 153));
  {
    s32 path = (s32)gData_02001f4c;

    Call2((void (*)())Actor_SetMovementTypeById, 11, path);
    Call2((void (*)())Actor_SetMovementTypeById, 7, path);
    Call2((void (*)())Actor_SetMovementTypeById, 5, path);
    Call2((void (*)())Actor_SetMovementTypeAndWaitById, 6, path);
  }
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x800, 253));
  Func_02001da8();
}
