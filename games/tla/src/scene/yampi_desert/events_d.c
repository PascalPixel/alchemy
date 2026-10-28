#include "scene.h"

void Func_020032c4(void) {
  s32 variant = Actor_GetById(gSequenceParentWork.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(13, variant);
  Actor_SetSpritePriority(14, variant);
}

void Func_020032f4(void) {
  Func_02003820(gData_0200478e);
  if (GameFlag_Test(0x20b)) {
    Func_020006ac();
    wait(10);
  }
  Scene_TaskAdd(Func_02002010, 0x480);
  Scene_TaskAdd(Func_020032c4, 0x480);
}

void Func_02003338(s32 id) {
  Actor_GetById(id)->priorityFlags |= 2;
  Actor_SetSpritePriority(id, 3);
  Func_020040f8(id);
  Actor_SetModeById(id, 2);
}
