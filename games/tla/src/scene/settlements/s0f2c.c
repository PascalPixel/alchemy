#include "local.h"

void Func_0200156c(s32 id);

static __inline__ void Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020020c0(id, x, z);
}

void Func_02000f2c(void) {
  struct ObjectRuntime *object = Object_GetById(9);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 i;

  if (!GameFlag_Test(0x204) && column == 11 && row == 12) {
    object->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      object->y -= 0x20000;
      Battle_WaitMode0(1);
    }
    Func_0200156c(9);
    Func_020020c0(9, 0, 0);
    Place(21, 0xb80000, 0xb80000);
    Object_GetById(21)->motionFlags = 0;
    Object_GetById(21)->y -= 0x400000;
    CopyCells2(9, 11, 1, 1, 11, 12);
    GameFlag_SetBit(0x204);
  }
}
