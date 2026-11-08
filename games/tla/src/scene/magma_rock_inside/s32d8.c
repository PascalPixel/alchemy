#include "local.h"

extern const u8 MsgBlazeTabletInscription[];

void FieldMessage_Show(const u8 *message, s32 mode);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Func_020030cc(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_020032d8(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 41 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x9fa))) {
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
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 38 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x9fa))) {
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
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 37 instructions
   * differ in tla-en. */
  if (GameFlag_Test(SPLIT_CONSTANT(0x9fa))) {
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
