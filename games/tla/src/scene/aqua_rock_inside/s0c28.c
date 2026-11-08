#include "local.h"

void Func_02000c28(s32 unused, s32 id) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Actor_SetPartAttribute(Actor_GetById(9), 5);
  Func_02001e78(id);
  Actor_SetModeById(id, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  Func_02003d7c();
  Func_02003eac(0);
  Func_02003ef4(0x22d);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    wait(4);
  }
  Func_02003d84();
  Func_02000b5c();
  GameFlag_SetBit(0x249);
}

void Func_02000ca8(void) {
  Func_02003df4(gMainRoutineWork.currentOwner, 6, 0);
  Func_02003d84();
}
