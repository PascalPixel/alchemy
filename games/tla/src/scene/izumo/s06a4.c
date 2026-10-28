#include "izumo.h"

void Func_020006a4(void) {
  /* FAKEMATCH: the Call3 and Value calls of call.h set each call's arguments up
   * in the native's order (differ 2 for each one called directly, 12 with all
   * six direct). */
  GameFlag_SetBit(0x929);
  Func_02000d00();
  Func_02000dd0(0);
  Func_02000d60(MsgWait2);
  Actor_ResetAndSetPositionInMode2(gAdventure.currentOwner, 200, 264);
  Actor_WaitForMoveById(gAdventure.currentOwner);
  Func_02000d70(32, 0);
  FieldEvent_WaitFrames(15);
  Value3((s32 (*)())Func_02000d88, gAdventure.currentOwner, 256, 30);
  Call3((void (*)())Func_02000d40, 32, 200 << 16, 216 << 16);
  Actor_ResetAndSetPositionInMode2(32, 200, 248);
  Actor_WaitForMoveById(32);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(gAdventure.currentOwner, 32, 0);
  FieldEvent_WaitFrames(30);
  Actor_SetModeById(32, 4);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Value3((s32 (*)())Func_02000d88, gAdventure.currentOwner, 261, 60);
  Func_02000d88(32, 262, 30);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_WaitFrames(15);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 32, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 1) {
    FieldEvent_WaitFrames(90);
    Call3((void (*)())Actor_SetSpeedParameters, 32, 0x6666, 0x3333);
    Actor_ResetAndSetPositionInMode2(32, 200, 216);
    Actor_WaitForMoveById(32);
    Func_02000da8(32, 1);
    FieldEvent_WaitFrames(60);
    Call3((void (*)())Actor_ArmCallback, 32, 0x4000, 0);
    Func_02000d88(32, 265, 30);
    Func_02000d70(32, 0);
    Actor_SetSpritePriority(32, 3);
    Actor_GetById(32)->motionFlags |= 2;
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
  Actor_ArmCallback(32, 0x2000, 0);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(32, gAdventure.currentOwner, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Actor_StartRepeatedMotion(32, 1);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 4);
  FieldEvent_WaitFrames(30);
  Func_02000d70(32, 0);
  Actor_SetModeById(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(15);
  Func_02000d88(32, 265, 30);
  Func_02000d70(32, 0);
  Actor_SetAngleToward(32, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(15);
  Func_02000d70(32, 0);
  Actor_SetModeById(32, 3);
  FieldEvent_WaitFrames(15);
  Actor_SetModeById(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(30);
  Actor_SetSpeedParameters(32, 0x3333, 0x1999);
  Actor_GetById(32)->motionFlags = 0;
  Actor_SetSpritePriority(32, 3);
  Actor_ResetAndSetPositionInMode2(32, 200, 208);
  Actor_WaitForMoveById(32);
  Func_02000d40(32, 0, 0);
  GameFlag_SetBit(0x92a);
  GameFlag_ClearBit(0x302);
}
