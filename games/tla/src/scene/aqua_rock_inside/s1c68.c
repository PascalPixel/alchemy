#include "scene_tables.h"
#include "local.h"

void Func_02001c68(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 41 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x900, 174))) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 152);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 184);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 184);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001cd8(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 38 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x900, 174))) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 184);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 184);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001d40(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 37 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x900, 174))) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 184);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 184);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}
