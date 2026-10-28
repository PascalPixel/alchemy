#include "islets.h"

void Func_02001f74(void) {
  struct ObjectRuntime *object = Object_GetById(10);

  Func_02003e64();
  Func_02003fa4(0);
  ObjectMotion_EnableActionAndResetMotion(10);
  WaitFrames(1);
  object->motionFlags = 0;
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call4((void (*)())Object_SetPositionAndResetMotion, (s32)object, 0x10c0000,
        0x400000, 0x1860000);
  WaitFrames(1);
  object->terrainHeight = 0x800000;
  WaitFrames(1);
  object->y = 0xa00000;
  WaitFrames(1);
  Scheduler_AddOrUpdateCallback(Func_02001f58, 0x480);
  Func_02003e0c(object, 0x840000, 0x800000, 0x1400000);
  Func_02003e14(object);
  Scheduler_RemoveCallbackFar(Func_02001f58);
  Object_SetModeById(10, 5);
  object->motionFlags = 3;
  ObjectMotion_SetActionVariant(10, 2);
  ObjectMotion_SetAngleToward(10, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(10);
  ObjectMotion_Launch(10, 4, 0);
  ObjectMotion_SetPositionAndCommit(10, 120, 312);
  Object_SetModeById(10, 5);
  object->motionFlags = 0;
  Func_02003f3c(10, 0);
  GameFlag_SetBit(0x215);
  WaitFrames(1);
  Func_02003e6c();
}
