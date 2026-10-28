#include "local.h"

void Func_02001c68(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 248, 152);
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 248, 184);
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 264, 184);
    Actor_ArmCallback(gSequenceParentWork.currentOwner, 0xc000, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001cd8(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 280, 184);
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 264, 184);
    Actor_ArmCallback(gSequenceParentWork.currentOwner, 0xc000, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001d40(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    FieldMessage_Show(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 248, 184);
    Actor_SetPositionAndReset(gSequenceParentWork.currentOwner, 264, 184);
    Actor_ArmCallback(gSequenceParentWork.currentOwner, 0xc000, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}
