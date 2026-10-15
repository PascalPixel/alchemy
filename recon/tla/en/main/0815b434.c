/* Near miss of Func_0815b434, decompiled from the listing (106 instructions,
 * draft 106, differ 4, two reordered rows): the native copies map to r9
 * before the clear's size multiply, and reloads scale from the stack
 * (ldr r2, [sp, #4]) before it finishes cos * 63; here both come one row
 * later. The second is the load-latency pattern of this lane's other near
 * misses. Matched on the way: cosine and shade in one variable (the
 * multiply copies scale, not the cosine), the column in its own variable
 * (the sum keeps the product as its first operand), center declared before
 * last (register choice). Tried: the multiply's operands swapped, scale
 * through a forced temporary (differ 88), the map through a local. */
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

// Fills a size by size/2 byte map with circles of growing radius, each
// shaded by the cosine of its radius (0 to 63).
void Func_0815b434(u8 *map, s32 size, s32 scale)
{
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
