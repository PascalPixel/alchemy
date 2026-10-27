#include "local.h"

void Func_020044d4(void) {
  /* FAKEMATCH: a temporary set just before its call builds the angle ahead of
   * the other arguments (one reordered row as a plain argument). */
  s32 angle;

  Func_020051c0();
  Func_02005370(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  WaitTicks_(1);
  Func_02005230_(gPartyState.currentOwner, 0x1480000, 0x1680000);
  angle = 0x5000;
  Func_02005238_(12, 0x1d80000, 0x1980000, angle);
  Object_GetById_(16)->angle = 0xc000;
  WaitTicks_(1);
  Event_StartScreenTransition();
  ObjectMotion_SetSpeedParameters_(gPartyState.currentOwner, 0xcccc, 0x6666);
  ObjectMotion_SetPositionAndReset_(gPartyState.currentOwner, 328, 392);
  Func_020052d8_(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds_(0x1980000, -1, 0x1900000, 1);
  ObjectMotion_SetPositionAndReset_(gPartyState.currentOwner, 376, 408);
  FieldEvent_WaitTicks_(20);
  Motion_SetVarCbAndRefresh(12, 1);
  Func_02005280((s32)&MsgKandoreanTemple05);
  Func_02005298(12, 0);
  ObjectMotion_ArmCallback_(12, 0x8000, 20);
  Func_020052c8(12, 256, 0);
  ObjectMotion_SetSpeedParameters_(12, 0x19999, 0xcccc);
  ObjectMotion_SetPositionAndReset_(12, 456, 408);
  Func_02005298(12, 0);
  Func_02005240(16, gPartyState.currentOwner);
  WaitTicks_(1);
  ObjectMotion_SetSpeedParameters_(16, 0xcccc, 0x6666);
  ObjectMotion_SetPositionAndReset_(16, 376, 392);
  ObjectMotion_ArmCallback_(gPartyState.currentOwner, 0xc000, 0);
  ObjectMotion_ArmCallback_(16, 0x3000, 40);
  ObjectMotion_ArmCallback_(gPartyState.currentOwner, 0, 0);
  ObjectMotion_ArmCallback_(16, 0, 20);
  Func_020052d0_(12, 258);
  Func_02005298(12, 0);
  Func_020052c8(16, 265, 0);
  ObjectMotion_ArmCallback_(16, 0x3000, 0);
  ObjectMotion_ArmCallback_(gPartyState.currentOwner, 0xc000, 20);
  Func_02005298(16, 0);
  Motion_SetModeAndWaitAnimation_(gPartyState.currentOwner, 3);
  ObjectMotion_ArmCallback_(gPartyState.currentOwner, 0, 0);
  ObjectMotion_ArmCallback_(16, 0, 0);
  ObjectMotion_SetPositionAndReset_(12, 468, 428);
  ObjectMotion_SetPositionAndReset_(12, 472, 454);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(13);
}

void Func_020046d8(void) {
  Func_020051c0();
  Func_02005370(0);
  Func_02005230_(gPartyState.currentOwner, 0, 0);
  Func_02005230_(12, 0, 0);
  Func_02005230_(15, 0, 0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  WaitTicks_(1);
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 514;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005230_(12, 0x1d80000, 0x2880000);
  WaitTicks_(1);
  ObjectMotion_SetPositionAndReset_(12, 472, 664);
  ObjectMotion_ArmCallback_(12, 0x3000, 20);
  ObjectMotion_ArmCallback_(12, 0x5000, 20);
  ObjectMotion_ArmCallback_(12, 0x3000, 0);
  Func_020052d8(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds_(0x1c80000, 0x280000, 0x2980000, 1);
  Func_020052e8();
  FieldEvent_WaitTicks_(40);
  Func_02005280((s32)&MsgKandoreanTemple06);
  Func_02005298(0x800c, 0);
  ObjectMotion_SetPositionAndReset_(12, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020052f8(14);
}

void Func_020047f0(void) {
  ObjectMotion_SetSpeedParameters_(12, 0x19999, 0xcccc);
  ObjectMotion_SetPositionAndReset_(12, 496, 392);
  ObjectMotion_ArmCallback_(12, 0x8000, 0);
  Func_02005298(0x400c, 0);
}
