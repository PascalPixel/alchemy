// Matches (tla-en, 2026-11-07).
// Taken from its matched twin in games/tla/src/scene/aqua_rock_inside/s04b4.c.
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"


void Func_0200349c(u32 index, s32 height);

void Func_020004bc(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;
  s32 column;
  s32 objectId = id;

  layer = (s32 *)(map + 0x140);
  Actor_SetPartAttribute(Actor_GetById(objectId), 0);
  Func_02001e78(objectId);
  Actor_SetModeById(objectId, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  column = (Actor_GetById(12)->x >> 20) - 42;
  Func_02003d7c();
  Func_02003eac(0);
  Func_02003ef4(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    {
      /* FAKEMATCH: the zero pinned to r1 is set before the column is copied to
       * r0 (differ 2 plain). */
      register s32 height asm("r1") = 0;

      Func_0200349c(column, height);
    }
    layer[3] += 0x10000;
    wait(4);
  }
  if (GameFlag_Test(0x250))
    Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Func_02003d84();
  GameFlag_SetBit(0x230);
}
