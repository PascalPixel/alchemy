#include "local.h"

extern const u8 MsgBlazeTabletInscription[];

void FieldMessage_Show(const u8 *message, s32 mode);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Func_020030cc(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_020032d8(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    FieldMessage_Show(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 107);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_02003348(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    FieldMessage_Show(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_020033b0(void) {
  if (GameFlag_Test(0x9fa)) {
    Func_02004d40();
    Func_02004e90(0);
    FieldMessage_Show(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}
