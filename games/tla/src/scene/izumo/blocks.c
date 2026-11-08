#include "izumo.h"
#include "version.h"
#include "scene_tables.h"

void Func_02000b08(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 68 instructions
   * differ in tla-en. */
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_SetBit(SPLIT_CONSTANT(0x300, 1));
  if (GameFlag_Test(592)) {
    Func_02000ce0(36, 12, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Func_02000d40(gMainRoutineWork.currentOwner, PIXELS(376), PIXELS(136));
  } else {
    Func_02000ce0(36, 8, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
  }
}

void Func_02000ba4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 25 instructions
   * differ in tla-en. */
  GameFlag_ClearBit(SPLIT_CONSTANT(0x300, 1));
  Func_02000ce0(40, 8, 22, 8, 3, 3);
  Call6((void (*)())Func_02000ce8, 40, 8, 3, 3, 22, 8);
}

void Func_02000bdc(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 340);

  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
#if TLA_PATCH_1 /* From patch 1. */
  Func_02000e08(129);
#endif
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  FieldEvent_Wait(14);
  Func_02000db0(value);
}

void Func_02000c58(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Actor_CommitPositionAndActivate(4, 16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000c7c(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Actor_CommitPositionAndActivate(4, -16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000ca4(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}
