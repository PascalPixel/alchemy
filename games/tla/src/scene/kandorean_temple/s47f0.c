#include "local.h"
#include "event_inline.h"

void Func_020044d4(void) {
  /* FAKEMATCH(temporary): a local holding the angle, set just before its call,
   * builds it ahead of the other arguments; as a plain argument, one row is
   * reordered. */
  s32 angle;

  Func_020051c0();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(328), PIXELS(360));
  angle = 0x5000;
  Actor_SetPositionAndFacing(12, PIXELS(472), PIXELS(408), angle);
  Actor_GetById(16)->angle = 0xc000;
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 328, 392);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(408), -1, PIXELS(400), 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 376, 408);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 1);
  Event_SetMessageId((s32)&MsgKandoreanTemple05);
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(12, FACING_WEST, 20);
  Func_080d47b4(12, 256, 0);
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 456, 408);
  Event_ShowMessageAndWait(12, 0);
  Func_02005240(16, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWait(16, 376, 392);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 20);
  Actor_SetAttachedEffect(12, 258);
  Event_ShowMessageAndWait(12, 0);
  Func_080d47b4(16, 265, 0);
  Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Event_ShowMessageAndWait(16, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(16, FACING_EAST, 0);
  Actor_WalkToAndWait(12, 468, 428);
  Actor_WalkToAndWait(12, 472, 454);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(13);
}

void Func_020046d8(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  wait(1);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetHorizontalPositionWithTerrain(12, PIXELS(472), PIXELS(648));
  wait(1);
  Actor_WalkToAndWait(12, 472, 664);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(456), PIXELS(40), PIXELS(664), 1);
  Func_020052e8();
  Event_Wait(40);
  Event_SetMessageId((s32)&MsgKandoreanTemple06);
  Event_ShowMessageAndWait(0x800c, 0);
  Actor_WalkToAndWait(12, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(14);
}

void Func_020047f0(void) {
  Actor_SetSpeed(12, 0x19999, 0xcccc);
  Actor_WalkToAndWait(12, 496, 392);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_ShowMessageAndWait(0x400c, 0);
}
