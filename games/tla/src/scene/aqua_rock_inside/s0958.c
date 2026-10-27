#include "local.h"

void Func_02003cf4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);

void Func_02000958(s32 value) {
  s32 width = 1;
  s32 height = 2;

  Func_02003d7c();
  Func_02003eac(0);
  Func_02003ef4(188);
  Func_02003cf4(107, 96, 19, 29, width, height);
  FieldEvent_WaitTicks(8);
  Func_02003cf4(108, 96, 19, 29, width, height);
  FieldEvent_WaitTicks(8);
  Func_02003cf4(107, 98, 19, 29, width, height);
  FieldEvent_WaitTicks(8);
  Func_02003cf4(108, 98, 19, 29, width, height);
  FieldEvent_WaitTicks(15);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
  Object_GetById(gPartyState.currentOwner)->speedLimit =
      Object_GetById(gPartyState.currentOwner)->acceleration = 0x8000;
  Func_02003ef4(123);
  ObjectMotion_ResetAndSetPositionInMode2(gPartyState.currentOwner, 312, 496);
  FieldEvent_WaitTicks(5);
  Func_02003e5c(value);
}
