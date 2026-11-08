#include "local.h"

extern char SceneId_Ov692Room3;

s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020034f0(void) {
  GameFlag_SetBit(0x301);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    Func_02000694(0, 24, 15, 0);
    Func_02000694(0, 9, 16, 0);
    Actor_SetModeById(11, 4);
  } else {
    Func_02000694(0, 26, 14, 0);
  }
}
