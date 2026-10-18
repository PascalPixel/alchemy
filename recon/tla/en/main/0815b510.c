// Draft of Func_0815b510, decompiled from the listing: differ 10 of 142
// (tla-en, 2026-10-18; was 210). The logic is Func_0815b434's mirrored in
// four quarters. What moved it: center declared before last, as in
// Func_0815b434 (the two allocate as the native's only together with the
// rest), the map indices written out in each store instead of through top
// and bottom temporaries (210 to 32: the temporaries spilled radius and
// moved size and map to r8 and r10), each row index multiplied by the
// size rather than the reverse (32 to 16: the native copies size and
// multiplies by the row), and the first quarter's four stores in a tagged
// one-pass block (16 to 10, found by alchemy permute).

// Left: order only, four places: the native copies last to r9 after the
// size's sign bit, finishes cos * 63 after reloading scale, and loads
// center from the stack before the column's sign shift (twice). The
// load-latency residue of this lane's other near misses. Earlier: alchemy
// permute 6 minutes reached 34 through an inline multiply helper (not
// kept); 5 more minutes on this draft found nothing below 10.

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
      /* FAKEMATCH: the one-pass block ends the first quarter's stores before
       * the second column is started, as the native orders them (differ 16
       * without it). */
      do {
        map[row * size + col] = shade;
        map[(size * 2 - row - 1) * size + col] = shade;
        map[row * size + (size - col) - 1] = shade;
        map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      } while (0);
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
