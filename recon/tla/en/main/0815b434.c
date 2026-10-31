// Near miss, differ 2 of 106 (tla-en, 2026-10-30): one row, scheduler
// order. The native copies map to r9 (mov r9, r0) before the clear's size
// multiply (mov r3, sl; muls r3, r1), here after it. In the sched2 dump the
// multiply has priority 7 (its cost of 4 before the size argument's move)
// and the map copy 6 (through the center's movs, negs, add, mov chain);
// both are ready together and the higher priority wins. Those dependencies
// follow from the instructions and registers both versions share, so no
// source form that keeps this code reorders them (untested reading: a
// multiply cost of 3 would tie them and the map copy's earlier position
// would pick the native order). Tried 2026-10-30 (differ): last assigned
// before center (16); the clear's size in a temporary before center and
// last (30); map as a void * parameter copied to a u8 * local, the size in
// a block-scoped temporary (2 each); alchemy permute 4 minutes, 2 jobs,
// seed 77, 4550 candidates (nothing below 2).
//
// Under -mthumb-slow-loads (2026-10-26): differ 2 of 106, the same row;
// the scale reload now matches. Tried (differ): half computed alone before
// the clear, center and last after it (72), an empty asm after the
// declarations (14); alchemy permute 5 minutes, 2 jobs (nothing below 2).

// Near miss of Func_0815b434, decompiled from the listing (106 instructions,
// draft 106, differ 4, two reordered rows): the native copies map to r9
// before the clear's size multiply, and reloads scale from the stack
// (ldr r2, [sp, #4]) before it finishes cos * 63; here both come one row
// later. The second is the load-latency pattern of this lane's other near
// misses. Matched on the way: cosine and shade in one variable (the
// multiply copies scale, not the cosine), the column in its own variable
// (the sum keeps the product as its first operand), center declared before
// last (register choice). Tried: the multiply's operands swapped, scale
// through a forced temporary (differ 88), the map through a local.
// 2026-10-18: alchemy-devtools permute (6 min, 2 jobs, seed 31): no candidate below
// the draft.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

// Fills a size by size/2 byte map with circles of growing radius, each
// shaded by the cosine of its radius (0 to 63).
void Func_0815b434(u8 *map, s32 size, s32 scale) {
  s32 half = size / 2;
  s32 center = half - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWords(map, half * size);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;

    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      map[half * (last - y) + col] = shade;
      col = center - y / 2;
      map[half * (last - x) + col] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}
