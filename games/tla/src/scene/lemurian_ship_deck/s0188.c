#include "types.h"

extern u32 gFrameCount;

#define OBJ_PALETTE ((volatile u16 *)0x05000160)

void Func_02000188(void) {
  if ((gFrameCount & 7) == 0) {
    s16 last = OBJ_PALETTE[15];
    s32 i;

    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: the bytes rebuild the palette base on every pass, and a for or do
     * loop hoists it and steps two pointers (differ 30). */
    i = 15;
  loop:
    OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];
    i--;
    if (i != 12)
      goto loop;
    OBJ_PALETTE[12] = last;
  }
}
