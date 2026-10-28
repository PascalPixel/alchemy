#include "local.h"

void Func_02000974(void) {
  Func_02001cc4();
  Func_02001dcc(0);
  Actor_GetById(17)->update = (void (*)(void))Func_02000770;
  Func_02001d44((s32)&MsgWhatWhatJust);
  Actor_SetAngleToward(gSequenceParentWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_ArmCallback(16, 0, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_ArmCallback_(16, 0x8000, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_ArmCallback_(16, 0xe000, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Func_02001d6c(16, 0x102, 20);
  Func_02001d54(16, 0);
  Actor_Jump(16, 2, 25);
  Func_02001d54(16, 0);
  Motion_SetVarCbAndRefresh(16, 1);
  Func_02001d54(16, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Func_02001d54(16, 0);
  Func_02001ccc();
  GameFlag_ClearBit(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

void Func_02000a68(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (GameFlag_Test(0xf9b) == 0) {
    Func_02001d04_(65, 0x1780000, 0x1780000);
    if (leader->x >> 20 == 23 && leader->z >> 20 == 23) {
      Func_02001cc4();
      Func_02001dcc(0);
      Func_02001d74(gSequenceParentWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gSequenceParentWork.currentOwner, 16,
                                                0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000af8(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (GameFlag_Test(0xf9a) == 0) {
    Func_02001d04_(64, 0x2580000, 0x1980000);
    if (leader->x >> 20 == 37 && leader->z >> 20 == 25) {
      Func_02001cc4();
      Func_02001dcc(0);
      Func_02001d74(gSequenceParentWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gSequenceParentWork.currentOwner, 16,
                                                0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

void Func_02000ba8(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_02001e0c_(67, 38, 11, 6, 67, 8, 1);
  if (GameFlag_Test(0xf9e) == 0) {
    Func_02001d04_(64, 0xc80000, 0x980000);
    if (leader->x >> 20 == 12 && leader->z >> 20 == 9) {
      Func_02001cc4();
      Func_02001dcc(0);
      Func_02001d74(gSequenceParentWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gSequenceParentWork.currentOwner, 16,
                                                0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000c38(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001cc4();
  Func_02001dcc(0);
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 2);
  Actor_GetById(gSequenceParentWork.currentOwner)->motionFlags = 0;
  Func_02001e2c(123);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 2);
  Actor_CenterOnColumn_(gSequenceParentWork.currentOwner, 2, -6);
  FieldEvent_Wait(10);
  Func_02001d94(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001ccc();
}

s32 Func_02000cb4(void) {
  s32 angle = Actor_GetById(gSequenceParentWork.currentOwner)->angle;
  u16 facing;

  /* FAKEMATCH: the native loads the angle before building the quarter turn. */
  __asm__ volatile("" : "+r"(angle));
  facing = (s16)((angle + 0x2000) & 0xc000);
  Func_02001cc4();
  Func_02001dcc(0);
  if (facing) {
    Actor_SetAngleToward(9, gSequenceParentWork.currentOwner, 0);
    Func_02001d44((s32)&MsgWhyDoesMy);
    Func_02001d54(9, 0);
    Actor_ArmCallback_(9, 0x2000, 0);
  }
  Func_02001ccc();
  return 0;
}

s32 Func_02000d20(void) {
  s32 angle = Actor_GetById(gSequenceParentWork.currentOwner)->angle;
  u16 facing;

  /* FAKEMATCH: the native loads the angle before building the quarter turn. */
  __asm__ volatile("" : "+r"(angle));
  facing = (s16)((angle + 0x2000) & 0xc000);
  Func_02001cc4();
  Func_02001dcc(0);
  if (facing) {
    Func_02001d44((s32)&MsgICanTEver);
    Func_02001d54(9, 0);
  }
  Func_02001ccc();
  return 0;
}

void Func_02000d74(void) { Func_02001d2c(gSequenceParentWork.currentOwner, 1, 5); }
