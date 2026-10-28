#include "local.h"

void Func_02000110(void) {
  if (gMainRoutineWork.entrance == 99)
    GameFlag_SetBit(0x894);
  if (GameFlag_Test(0x894)) {
    Func_020009d0_(8, 0x1c80000, 0x1380000);
    Func_02000a98(8, 0x4000, 0);
    Actor_SetActionCallback(Actor_GetById(8), 8);
  }
}

void Func_02000168(void) {
  Func_02000978_(108, 38, 1, 1, 102, 56);
  if (gMainRoutineWork.entrance == 99)
    GameFlag_SetBit(0x895);
  if (GameFlag_Test(0x895)) {
    Func_020009d0_(13, 0x24e0000, 0x3980000);
    Func_020009d0_(14, 0x2700000, 0x3ab0000);
    Func_020009d0_(15, 0x2780000, 0x3980000);
    Actor_GetById(13)->angle = 0x4000;
    Func_02000a90(13, 0x6000);
    Actor_GetById(14)->angle = 0;
    Func_02000a90(14, 0);
    Actor_GetById(15)->angle = 0xc000;
    Func_02000a90(15, 0);
    Actor_GetById(13)->collisionFlags = 1;
    Actor_GetById(14)->collisionFlags = 1;
    Actor_GetById(15)->collisionFlags = 1;
  }
  Actor_SetModeById(8, 4);
  Actor_SetModeById(9, 4);
  Actor_SetModeById(10, 3);
  Actor_SetModeById(11, 4);
  Actor_SetModeById(12, 3);
}

void Func_02000264(void) {
  if (GameFlag_Test(0x109) == 0 && gMainRoutineWork.entrance == 99)
    Func_02000800();
}
