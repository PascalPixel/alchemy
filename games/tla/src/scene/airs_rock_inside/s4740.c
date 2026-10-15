#include "airs_rock_inside.h"
#include "text/msg_ids.h"

void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004534(void);

void Func_02004740(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 107);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02004534();
  }
}

void Func_020047b0(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 280, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02004534();
  }
}

void Func_02004818(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    UiText_ShowPositionedMessageAndWait(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02004534();
  }
}

void Func_02004880(void) {}
