#include "types.h"
#include "iwram_call.h"

extern const u16 gData_080f39c4[];

/// Copy a 16x16 image to the BG map, clipping cells outside the 30x20-tile
/// screen.
void Func_080e7fec(s32 x, s32 y) {
  const u16 *tiles = gData_080f39c4;
  s32 row;
  s32 column;
  s32 sx;
  s32 sy;
  s32 (*fill)(void *, s32, u32);

  /* FAKEMATCH(one-pass): the opening in a once-only block, which loads the
   * fill routine's entry before the size; as plain statements, 4 of 66
   * instructions differ in tla-en. */
  do {
    x /= 8;
    y /= 8;
    fill = Iwram_FillWordsEntry;
    /* FAKEMATCH(barrier): an empty asm taking the fill routine's entry; without
     * it, 4 of 66 instructions differ in tla-en. */
    asm volatile("" : : "l"(fill));
    fill((void *)0x06002000, 0x400, 0);
  } while (0);
  for (row = 0, sy = y; row < 16; row++, sy++) {
    if (sy < 0)
      continue;
    if (sy > 19)
      break;
    for (sx = x, column = 0; column < 16; column++, sx++) {
      if (sx < 0)
        continue;
      if (sx > 29)
        break;
      ((u16 *)0x06002000)[row * 32 + column] = tiles[row * 16 + column];
    }
  }
}
