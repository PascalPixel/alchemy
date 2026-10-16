#include "izumo.h"

void Func_02000b08(void) {
  struct FieldActor *object = Object_GetById(gPartyState.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_SetBit(769);
  if (GameFlag_Test(592)) {
    Call6((void (*)())Func_02000ce0, 36, 12, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Func_02000d40(gPartyState.currentOwner, 0x1780000, 0x880000);
  } else {
    Call6((void (*)())Func_02000ce0, 36, 8, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
  }
}

void Func_02000ba4(void) {
  GameFlag_ClearBit(769);
  Call6((void (*)())Func_02000ce0, 40, 8, 22, 8, 3, 3);
  Call6((void (*)())Func_02000ce8, 40, 8, 3, 3, 22, 8);
}

void Func_02000bdc(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 340);

  ObjectMotion_ArmCallback(4, 0xc000, 0);
  FieldEvent_WaitFrames(10);
  BYTE_AT(Object_GetById(4), 90) &= ~1;
  ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
#if !defined(TLA_EDITION_JA) /* Absent from the Japanese edition. */
  Func_02000e08(129);
#endif
  ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  Object_SetModeById(4, 13);
  FieldEvent_WaitFrames(14);
  Func_02000db0(value);
}

void Func_02000c58(void) {
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, 0, -14);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, 16, 0);
  FieldEvent_WaitFrames(10);
  Func_02000bdc();
}

void Func_02000c7c(void) {
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, 0, -14);
  Call3((void (*)())ObjectMotion_CommitPositionAndActivate, 4, -16, 0);
  FieldEvent_WaitFrames(10);
  Func_02000bdc();
}

void Func_02000ca4(void) {
  ObjectMotion_ArmCallback(4, 0xc000, 0);
  FieldEvent_WaitFrames(10);
  Func_02000bdc();
}
