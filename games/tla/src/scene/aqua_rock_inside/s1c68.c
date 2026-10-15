#include "local.h"

void Func_02001c68(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 152);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 184);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 184);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02001a1c();
  }
}

void Func_02001cd8(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 280, 184);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 184);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02001a1c();
  }
}

void Func_02001d40(void) {
  if (GameFlag_Test(0x9ae)) {
    Func_02003d7c();
    Func_02003eac(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 184);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 184);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02001a1c();
  }
}
