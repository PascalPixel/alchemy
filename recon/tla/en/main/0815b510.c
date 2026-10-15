/* Draft of Func_0815b510, decompiled from the listing (142 instructions,
 * draft 148, differ 210): the logic matches Func_0815b434's mirrored in four
 * quarters, but register allocation differs throughout (here radius is
 * spilled where the native spills center, and size and map take r8 and sl
 * where the native gives them r6 and r7). alchemy permute 6 minutes reached
 * differ 34 (144 instructions) by routing the multiplies through an inline
 * helper, which points at the allocation of the products as the first
 * thing to fix. */
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"

// Func_0815b434 for a size by size map: each circle is drawn in all four
// quarters, mirrored about the centre row and column.
void Func_0815b510(u8 *map, s32 size, s32 scale)
{
    s32 last = size - 1;
    s32 center = size / 2 - 1;
    s32 radius;

    Iwram_ClearWords(map, size * size * 2);
    for (radius = 0; radius != last; radius++) {
        s32 x = radius;
        s32 d = radius;
        s32 y = 0;
        s32 shade;
        s32 col;
        s32 row;
        s32 top;
        s32 bottom;

        shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
        shade = shade * scale / 0x10000;
        if (shade > 63)
            shade = 63;
        while (x >= y) {
            col = center - x / 2;
            row = last - y;
            top = size * row;
            map[top + col] = shade;
            bottom = size * (size * 2 - row - 1);
            map[bottom + col] = shade;
            map[top + (size - col) - 1] = shade;
            map[bottom + (size - col) - 1] = shade;
            col = center - y / 2;
            row = last - x;
            top = size * row;
            map[top + col] = shade;
            bottom = size * (size * 2 - row - 1);
            map[bottom + col] = shade;
            map[top + (size - col) - 1] = shade;
            map[bottom + (size - col) - 1] = shade;
            d = d - y * 2 - 1;
            if (d < 0) {
                d = d + x * 2 - 2;
                x--;
            }
            y++;
        }
    }
}
