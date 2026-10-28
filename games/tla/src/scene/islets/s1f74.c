#include "islets.h"

void Func_02001f74(void) {
  struct Actor *object = Actor_GetById(10);

  Func_02003e64();
  Func_02003fa4(0);
  Actor_Stop(10);
  wait(1);
  object->motionFlags = 0;
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)object, 0x10c0000,
        0x400000, 0x1860000);
  wait(1);
  object->terrainHeight = 0x800000;
  wait(1);
  object->y = 0xa00000;
  wait(1);
  Task_Add(Func_02001f58, 0x480);
  Func_02003e0c(object, 0x840000, 0x800000, 0x1400000);
  Func_02003e14(object);
  Task_RemoveFar(Func_02001f58);
  Actor_SetModeById(10, 5);
  object->motionFlags = 3;
  Actor_SetSpritePriority(10, 2);
  Actor_SetAngleToward(10, gAdventure.currentOwner, 0);
  FieldEvent_Wait(10);
  Actor_Jump(10, 4, 0);
  Actor_SetPositionAndCommit(10, 120, 312);
  Actor_SetModeById(10, 5);
  object->motionFlags = 0;
  Func_02003f3c(10, 0);
  GameFlag_SetBit(0x215);
  wait(1);
  Func_02003e6c();
}
