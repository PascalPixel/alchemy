#include "types.h"
#include "iwram_call.h"

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}

void Func_08143354(void) {
  u32 *map = (u32 *)0x0600f800;
  u32 tiles;
  s32 row;
  s32 column;

  /* FAKEMATCH(wrapper): both fills forwarded through FillWords_, which keeps
   * each call's own constants, as the scene layer does; as direct
   * Iwram_FillWords calls, 33 of 40 instructions differ in tla-en. */
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
