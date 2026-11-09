#include "local.h"


// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_0200124c(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
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

    MapGrid_CopyCellsLayered(70, 8, 7, 6, column, row);
    System_Wait(1);
    MapGrid_CopyCellsLayered(50, 8, 7, 6, column, row);
    System_Wait(1);
  }
  i--;
  if (i >= 0)
    goto shake;
  System_Wait(10);
  for (i = 127; i >= 0; i--) {
    layer[3] += 0x4000;
    System_Wait(1);
  }
  Func_02003d44_(-1, -1, 0xe666);
  Func_02003d84();
  Func_02003d1c_(38, 9, 7, 6, 13, 9);
  Event_SetFlag(0x349);
}
