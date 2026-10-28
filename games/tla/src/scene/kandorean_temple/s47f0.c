#include "local.h"

void Func_020044d4(void) {
  /* FAKEMATCH: a temporary set just before its call builds the angle ahead of
   * the other arguments (one reordered row as a plain argument). */
  s32 angle;

  Func_020051c0();
  Func_02005370(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  wait_(1);
  Func_02005230_(gMainRoutineWork.currentOwner, PIXELS(328), PIXELS(360));
  angle = 0x5000;
  Func_02005238_(12, PIXELS(472), PIXELS(408), angle);
  Actor_GetById_(16)->angle = 0xc000;
  wait_(1);
  Event_StartScreenTransition();
  Actor_SetSpeedParameters_(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset_(gMainRoutineWork.currentOwner, 328, 392);
  Func_020052d8_(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds_(PIXELS(408), -1, PIXELS(400), 1);
  Actor_SetPositionAndReset_(gMainRoutineWork.currentOwner, 376, 408);
  FieldEvent_Wait_(20);
  Motion_SetVarCbAndRefresh(12, 1);
  Func_02005280((s32)&MsgKandoreanTemple05);
  Func_02005298(12, 0);
  Actor_ArmCallback_(12, FACING_WEST, 20);
  Func_020052c8(12, 256, 0);
  Actor_SetSpeedParameters_(12, 0x19999, 0xcccc);
  Actor_SetPositionAndReset_(12, 456, 408);
  Func_02005298(12, 0);
  Func_02005240(16, gMainRoutineWork.currentOwner);
  wait_(1);
  Actor_SetSpeedParameters_(16, 0xcccc, 0x6666);
  Actor_SetPositionAndReset_(16, 376, 392);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ArmCallback_(16, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback_(16, FACING_EAST, 20);
  Func_020052d0_(12, 258);
  Func_02005298(12, 0);
  Func_020052c8(16, 265, 0);
  Actor_ArmCallback_(16, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Func_02005298(16, 0);
  Motion_SetModeAndWaitAnimation_(gMainRoutineWork.currentOwner, 3);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback_(16, FACING_EAST, 0);
  Actor_SetPositionAndReset_(12, 468, 428);
  Actor_SetPositionAndReset_(12, 472, 454);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(13);
}

void Func_020046d8(void) {
  Func_020051c0();
  Func_02005370(0);
  Func_02005230_(gMainRoutineWork.currentOwner, 0, 0);
  Func_02005230_(12, 0, 0);
  Func_02005230_(15, 0, 0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  wait_(1);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005230_(12, PIXELS(472), PIXELS(648));
  wait_(1);
  Actor_SetPositionAndReset_(12, 472, 664);
  Actor_ArmCallback_(12, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_ArmCallback_(12, FACING_SOUTH + FACING_STEP, 20);
  Actor_ArmCallback_(12, FACING_SOUTHEAST + FACING_STEP, 0);
  Func_020052d8(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds_(PIXELS(456), PIXELS(40), PIXELS(664), 1);
  Func_020052e8();
  FieldEvent_Wait_(40);
  Func_02005280((s32)&MsgKandoreanTemple06);
  Func_02005298(0x800c, 0);
  Actor_SetPositionAndReset_(12, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(14);
}

void Func_020047f0(void) {
  Actor_SetSpeedParameters_(12, 0x19999, 0xcccc);
  Actor_SetPositionAndReset_(12, 496, 392);
  Actor_ArmCallback_(12, FACING_WEST, 0);
  Func_02005298(0x400c, 0);
}
