#include "local.h"

void Func_020004b4(void) { Func_02003e6c(); }

void Func_0200349c(u32 index, s32 height);

void Func_020004bc(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;
  s32 column;
  s32 objectId = id;

  layer = (s32 *)(map + 0x140);
  Actor_SetSpriteColor(Actor_Get(objectId), 0);
  Func_02001e78(objectId);
  Actor_SetAnimation(objectId, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  column = (Actor_Get(12)->x >> 20) - 42;
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    {
      /* FAKEMATCH(pin): the zero pinned to r1, set before the column is copied
       * to r0; as a plain local, 2 of 72 instructions differ in tla-en (2 as a
       * literal 0). */
      register s32 height asm("r1") = 0;

      Func_0200349c(column, height);
    }
    layer[3] += 0x10000;
    System_Wait(4);
  }
  if (Event_GetFlag(0x250))
    Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Func_02003d84();
  Event_SetFlag(0x230);
}
