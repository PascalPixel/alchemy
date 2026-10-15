#include "local.h"

extern const u8 MsgBlazeTabletInscription[];

void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 mode);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Func_020030cc(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_020032d8(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    UiText_ShowPositionedMessageAndWait(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 107);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_020030cc();
  }
}

void Func_02003348(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    UiText_ShowPositionedMessageAndWait(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 280, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_020030cc();
  }
}

void Func_020033b0(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    UiText_ShowPositionedMessageAndWait(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 248, 136);
    ObjectMotion_SetPositionAndReset(gPartyState.currentOwner, 264, 136);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_020030cc();
  }
}
