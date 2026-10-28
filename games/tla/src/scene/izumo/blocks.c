#include "izumo.h"
#include "edition.h"

void Func_02000b08(void) {
  struct FieldActor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  GameFlag_SetBit(769);
  if (GameFlag_Test(592)) {
    Call6((void (*)())Func_02000ce0, 36, 12, 22, 8, 3, 3);
    Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    if (x == 23 && z == 9)
      Func_02000d40(gMainRoutineWork.currentOwner, 0x1780000, 0x880000);
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

  Actor_ArmCallback(4, 0xc000, 0);
  FieldEvent_Wait(10);
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
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
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000c7c(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, -16, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}

void Func_02000ca4(void) {
  Actor_ArmCallback(4, 0xc000, 0);
  FieldEvent_Wait(10);
  Func_02000bdc();
}
