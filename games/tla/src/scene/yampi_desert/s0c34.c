#include "scene.h"

void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02001c5c(void);

void Func_02000c34(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Func_02004118(0x26666, 0x4ccc);
  Scene_CamBounds(0x12c0000, -1, 0xf80000, 1);
  Scene_SetPositionAndCommit(16, 254, 248);
  Scene_SetPositionAndCommit(16, 352, 240);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Func_02004090(17, 0, 0);
  FieldEvent_Wait(40);
  Func_02000b84(0x1600000, 0, 0xf80000);
  Func_020041f8(176);
  Actor_GetById(16)->velocityY = 0x60000;
  Actor_SetPartAttribute(Actor_GetById(16), 0);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Scene_SetPositionAndCommit(16, 380, 248);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Func_020041f8(241);
  Scene_SetSpeed(16, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(16, 428, 248);
  Func_020041f8(390);
  Func_02004090(16, 0, 0);
  GameFlag_SetBit(0x889);
  Func_02004048();
}

void Func_02000d50(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  Func_02004118(0x26666, 0x4ccc);
  Scene_CamBounds(0xf60000, -1, 0x2680000, 1);
  Scene_SetPositionAndCommit(21, 248, 648);
  Scene_SetPositionAndCommit(21, 248, 576);
  Task_RemoveFar(Func_02002180);
  FieldEvent_Wait(10);
  Task_RemoveFar(Func_02001c5c);
  wait(1);
  Func_02004090(22, 0, 0);
  FieldEvent_Wait(20);
  Func_02000b84(0xf80000, 0, 0x2400000);
  Func_020041f8(176);
  Actor_GetById(21)->velocityY = 0x60000;
  Actor_SetPartAttribute(Actor_GetById(21), 0);
  Actor_SetSingleChildFlags(Actor_GetById(21), 1);
  Scene_SetPositionAndCommit(21, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Func_020041f8(241);
  Scene_SetSpeed(21, 0xcccc, 0x6666);
  Scene_SetPositionAndReset(21, 248, 496);
  Func_020041f8(390);
  Func_02004090(21, 0, 0);
  GameFlag_SetBit(0x88a);
  Func_02004048();
}
