// Near miss, differ 2 of 66 (2026-10-19; 10 on 2026-10-15). The opening sits
// in a one-pass block with the fill routine's entry read into a local
// behind a volatile asm (10 -> 6 for the asm, 6 -> 2 for the block, which
// the permuter found): the entry loads before the size and the row counter
// clears before y is copied. One pair remains: the ROM loads the map base
// before the row's shift (ldr r5; lsls r3, r6, #5). Reload rematerializes
// the base just before its add, and sched2 has it as a constant move, not
// a load, so it stays there. Measured: the store as *(map + i), as
// *(u16 *)(base + i * 2), as map[column + row * 32], through a map local
// shared with the fill, inside a one-pass block (all 2); the base through
// a "+l" asm (57) or a volatile asm input (35). The permuter found nothing
// better in four more minutes.
#include "types.h"
#include "iwram_call.h"

extern const u16 gData_080f39c4[];

// Copy a 16x16 image to the BG map, clipping cells outside the 30x20-tile
// screen.
void Func_080e7fec(s32 x, s32 y) {
  const u16 *tiles = gData_080f39c4;
  s32 row;
  s32 column;
  s32 sx;
  s32 sy;
  s32 (*fill)(void *, s32, u32);

  do {
    x /= 8;
    y /= 8;
    fill = Iwram_FillWordsEntry;
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
