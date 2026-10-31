// Near miss, differ 4 of 142 (tla-en, 2026-10-30): one place in the
// prologue, scheduler order, no device left. The native schedules the
// size's sign shift (lsrs r3, r6, #31) before the copy of last to r9 and
// finishes the halving (adds r3, r6, r3) before the clear's size multiply;
// here the copy comes first and the add after the multiply. In the sched2
// dump the multiply has priority 7 (its cost of 4, then the shift and the
// call) and the halving's add 6 (through the center's asrs, subs and stack
// store), so the multiply wins; the copy to r9 inherits the multiply's 7
// through its anti-dependence and beats the sign shift (also 7) on its
// three dependents against two. Those dependencies follow from the
// instructions and registers both versions share, so no source form that
// keeps this code reorders them. Worked by hand from the dump's own
// priorities, a multiply cost of 3 gives exactly the native order (the
// same reading as Func_0815b434's two-row residue). The first quarter's
// one-pass block is gone: without it the draft also differs by these 4.
// Tried 2026-10-30 (differ): center through a half variable as in
// Func_0815b434 (4), the clear's size as size * size * sizeof(u16) or
// (u32)(size * size) * 2 (4), center and last assigned after the clear
// (51); alchemy permute 4 minutes, 2 jobs, seed 113 (2,750 candidates, all
// but a few failing to compile; nothing below 4).
//
// Under -mthumb-slow-loads (2026-10-26): differ 4 of 142, the same place.
// The scale and center reloads now match. Tried (differ): the size written
// size * 2 * size, (size * size) << 1 or size * (size * 2) (4), last
// declared before center (4), center as (size - 2) / 2 (50).

// Draft of Func_0815b510, decompiled from the listing: differ 10 of 142
// (tla-en, 2026-10-18; was 210). The logic is Func_0815b434's mirrored in
// four quarters. What moved it: center declared before last, as in
// Func_0815b434 (the two allocate as the native's only together with the
// rest), the map indices written out in each store instead of through top
// and bottom temporaries (210 to 32: the temporaries spilled radius and
// moved size and map to r8 and r10), each row index multiplied by the
// size rather than the reverse (32 to 16: the native copies size and
// multiplies by the row), and the first quarter's four stores in a tagged
// one-pass block (16 to 10, found by alchemy permute; no longer needed
// under -mthumb-slow-loads). Earlier: alchemy permute 6 minutes reached 34
// through an inline multiply helper (not kept); 5 more minutes on this
// draft found nothing below 10.

#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

// Func_0815b434 for a size by size map: each circle is drawn in all four
// quarters, mirrored about the centre row and column.
void Func_0815b510(u8 *map, s32 size, s32 scale) {
  s32 center = size / 2 - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWords(map, size * size * 2);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;
    s32 row;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;
    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      row = last - y;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      col = center - y / 2;
      row = last - x;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}
