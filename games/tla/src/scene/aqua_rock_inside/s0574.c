#include "local.h"

void Func_02000574(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  /* FAKEMATCH: the loop note ends the scheduling block: the native loads the
   * map work before it copies the id (differ 2 without it). */
  do {
  } while (0);
  layer = (s32 *)(map + 0x140);
  Actor_SetPartAttribute(Actor_GetById(id), 0);
  Func_02001e78(id);
  Actor_SetModeById(id, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  Func_02003d7c();
  Func_02003eac(0);
  Func_02003ef4(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    layer[3] += 0x10000;
    wait(4);
  }
  Func_02003d84();
  GameFlag_SetBit(0x231);
}

void Data_020005f4(void) {}
