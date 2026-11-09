#include "scene_tables.h"
#include "local.h"

void Func_02000974(void) {
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(17)->update = (void (*)(void))Func_02000770;
  FieldEvent_SetMessageId((s32)&MsgWhatWhatJust);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 16, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(16, FACING_EAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_WEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Actor_ArmCallback_(16, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(16, 3);
  FieldActor_ShowEmote(16, 0x102, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(16, 2, 25);
  Event_ShowMessage(16, 0);
  Actor_RunRepeatedMotion(16, 1);
  Event_ShowMessage(16, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Func_02001ccc();
  Event_ClearFlag(0x95e);
}

void Func_02000a48(void) { Func_02001e0c_(68, 38, 39, 24, 68, 4, 1); }

void Func_02000a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (Event_GetFlag(0xf9b) == 0) {
    Func_02001d04_(65, PIXELS(376), PIXELS(376));
    if (leader->x >> 20 == 23 && leader->z >> 20 == 23) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000af8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(68, 38, 38, 21, 68, 4, 1);
  if (Event_GetFlag(0xf9a) == 0) {
    Func_02001d04_(64, PIXELS(600), PIXELS(408));
    if (leader->x >> 20 == 37 && leader->z >> 20 == 25) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000b88(void) { Func_02001e0c_(67, 38, 11, 6, 67, 8, 1); }

void Func_02000ba8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02001e0c_(67, 38, 11, 6, 67, 8, 1);
  if (Event_GetFlag(0xf9e) == 0) {
    Func_02001d04_(64, PIXELS(200), PIXELS(152));
    if (leader->x >> 20 == 12 && leader->z >> 20 == 9) {
      Func_02001cc4();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Func_02001ccc();
    }
  }
}

void Func_02000c38(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Event_PlaySound(123);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterOnColumn_(gMainRoutineWork.currentOwner, 2, -6);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001ccc();
}

s32 Func_02000cb4(void) {
  u16 facing;

  facing =
      (s16)((Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000);
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)&MsgWhyDoesMy);
    Event_ShowMessage(9, 0);
    Actor_ArmCallback_(9, FACING_SOUTHEAST, 0);
  }
  Func_02001ccc();
  return 0;
}

s32 Func_02000d20(void) {
  u16 facing;

  facing =
      (s16)((Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000);
  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  if (facing) {
    FieldEvent_SetMessageId((s32)&MsgICanTEver);
    Event_ShowMessage(9, 0);
  }
  Func_02001ccc();
  return 0;
}

void Func_02000d74(void) { Func_02001d2c(gMainRoutineWork.currentOwner, 1, 5); }
