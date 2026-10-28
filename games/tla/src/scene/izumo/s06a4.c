#include "izumo.h"

void Func_020006a4(void) {
  /* FAKEMATCH: the Call3 and Value calls of call.h set each call's arguments up
   * in the native's order (differ 2 for each one called directly, 12 with all
   * six direct). */
  GameFlag_SetBit(0x929);
  Func_02000d00();
  Func_02000dd0(0);
  Func_02000d60(MsgWait2);
  ObjectMotion_ResetAndSetPositionInMode2(gAdventure.currentOwner, 200, 264);
  ObjectMotion_CommitCurrentPositionAndActivate(gAdventure.currentOwner);
  Func_02000d70(32, 0);
  FieldEvent_WaitFrames(15);
  Value3((s32 (*)())Func_02000d88, gAdventure.currentOwner, 256, 30);
  Call3((void (*)())Func_02000d40, 32, 200 << 16, 216 << 16);
  ObjectMotion_ResetAndSetPositionInMode2(32, 200, 248);
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 32, 0);
  FieldEvent_WaitFrames(30);
  Object_SetModeById(32, 4);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Value3((s32 (*)())Func_02000d88, gAdventure.currentOwner, 261, 60);
  Func_02000d88(32, 262, 30);
  Func_02000d70(32, 0);
  Object_SetModeById(32, 3);
  FieldEvent_WaitFrames(15);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 32, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 1) {
    FieldEvent_WaitFrames(90);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 32, 0x6666, 0x3333);
    ObjectMotion_ResetAndSetPositionInMode2(32, 200, 216);
    ObjectMotion_CommitCurrentPositionAndActivate(32);
    Func_02000da8(32, 1);
    FieldEvent_WaitFrames(60);
    Call3((void (*)())ObjectMotion_ArmCallback, 32, 0x4000, 0);
    Func_02000d88(32, 265, 30);
    Func_02000d70(32, 0);
    ObjectMotion_SetActionVariant(32, 3);
    Object_GetById(32)->motionFlags |= 2;
    GameFlag_SetBit(0x302);
  } else {
    Func_02000814();
  }
  Func_02000d08();
}

void Func_02000814(void) {
  Func_02000d60(MsgThisTokenIsn);
  Func_02000d88(32, 264, 30);
  Func_02000d70(32, 0);
  ObjectMotion_ArmCallback(32, 0x2000, 0);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  ObjectMotion_SetAngleToward(32, gAdventure.currentOwner, 0);
  Object_SetModeById(32, 3);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  ObjectMotion_SetVariantCallback(32, 1);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Object_SetModeById(32, 4);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Object_SetModeById(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(15);
  Func_02000d88(32, 265, 30);
  Func_02000d70(32, 0);
  ObjectMotion_SetAngleToward(32, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(15);
  Func_02000d70(32, 0);
  Object_SetModeById(32, 3);
  FieldEvent_WaitFrames(15);
  Object_SetModeById(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(30);
  ObjectMotion_SetSpeedParameters(32, 0x3333, 0x1999);
  Object_GetById(32)->motionFlags = 0;
  ObjectMotion_SetActionVariant(32, 3);
  ObjectMotion_ResetAndSetPositionInMode2(32, 200, 208);
  ObjectMotion_CommitCurrentPositionAndActivate(32);
  Func_02000d40(32, 0, 0);
  GameFlag_SetBit(0x92a);
  GameFlag_ClearBit(0x302);
}
