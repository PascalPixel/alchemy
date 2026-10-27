#include "local.h"

void Func_02003d44(s32 a, s32 b, s32 c);

static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003d44(a, b, c);
}

void Func_0200124c(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Func_02003d7c();
  Func_02003eac(0);
  layer[3] -= 0x200000;
  Func_02003cdc();
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes set the column and the row on every pass, and a for loop hoists
   * both (differ 44). */
  i = 14;
shake:
  {
    s32 column = 77;
    s32 row = 8;

    Func_02003d24(70, 8, 7, 6, column, row);
    WaitFrames(1);
    Func_02003d24(50, 8, 7, 6, column, row);
    WaitFrames(1);
  }
  i--;
  if (i >= 0)
    goto shake;
  WaitFrames(10);
  for (i = 127; i >= 0; i--) {
    layer[3] += 0x4000;
    WaitFrames(1);
  }
  Func_02003d44_(-1, -1, 0xe666);
  Func_02003d84();
  Func_02003d1c_(38, 9, 7, 6, 13, 9);
  GameFlag_SetBit(0x349);
}
