// Near miss, differ 2 of 40: the native loads the first tile pair between the
// two halves of the pair's step (movs r4, #128; ldr r1, [pc]; lsls r4, r4,
// #10); here the load follows the shift. The step is loop.c's hoisted constant,
// so a barrier on the tile pair cannot sit inside it (an operand barrier
// changes nothing).
#include "types.h"
#include "iwram_call.h"

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as the scene layer does: called directly, CSE shares the 0x100 size with
   * the map's step. */
  Iwram_FillWords(destination, size, value);
}

void Func_08143354(void) {
  u32 *map = (u32 *)0x0600f800;
  u32 tiles;
  s32 row;
  s32 column;

  FillWords_((void *)map, 0x100, -1);
  map += 0x40;
  FillWords_((void *)map, 0x80, 0x03ff03ff);
  map += 0x20;
  tiles = 0x02010200;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += 0x00020002;
    }
    for (column = 0; column != 8; column++)
      *map++ = 0x03ff03ff;
  }
}
