#include "scene_tables.h"
#include "local.h"

void Func_02000974(void) {
  Func_02001cc4();
  Event_ResolvePendingActions(0);
  Actor_GetById(17)->update = (void (*)(void))Func_02000770;
  Event_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_ArmCallback(16, FACING_EAST, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_ArmCallback_(16, FACING_WEST, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Actor_ArmCallback_(16, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Func_080d47b4(16, 0x102, 20);
  Event_ShowMessageAndWait(16, 0);
  Actor_Jump(16, 2, 25);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetVarCbAndRefresh(16, 1);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_ShowMessageAndWait(16, 0);
  Func_02001ccc();
  GameFlag_ClearBit(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

void Func_02000a68(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (GameFlag_Test(0xf9b) == 0) {
    Func_02001d04_(65, PIXELS(376), PIXELS(376));
    if (leader->x >> 20 == 23 && leader->z >> 20 == 23) {
      Func_02001cc4();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000af8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (GameFlag_Test(0xf9a) == 0) {
    Func_02001d04_(64, PIXELS(600), PIXELS(408));
    if (leader->x >> 20 == 37 && leader->z >> 20 == 25) {
      Func_02001cc4();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

void Func_02000ba8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Func_02001e0c_(67, 38, 11, 6, 67, 8, 1);
  if (GameFlag_Test(0xf9e) == 0) {
    Func_02001d04_(64, PIXELS(200), PIXELS(152));
    if (leader->x >> 20 == 12 && leader->z >> 20 == 9) {
      Func_02001cc4();
      Event_ResolvePendingActions(0);
      Func_080d489c(gMainRoutineWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000c38(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001cc4();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn_(gMainRoutineWork.currentOwner, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001ccc();
}

s32 Func_02000cb4(void) {
  u16 facing;

  facing =
      (s16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000);
  Func_02001cc4();
  Event_ResolvePendingActions(0);
  if (facing) {
    Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)&MsgWhyDoesMy);
    Event_ShowMessageAndWait(9, 0);
    Actor_ArmCallback_(9, FACING_SOUTHEAST, 0);
  }
  Func_02001ccc();
  return 0;
}

s32 Func_02000d20(void) {
  u16 facing;

  facing =
      (s16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000);
  Func_02001cc4();
  Event_ResolvePendingActions(0);
  if (facing) {
    Event_SetMessageId((s32)&MsgICanTEver);
    Event_ShowMessageAndWait(9, 0);
  }
  Func_02001ccc();
  return 0;
}

void Func_02000d74(void) { Func_02001d2c(gMainRoutineWork.currentOwner, 1, 5); }
