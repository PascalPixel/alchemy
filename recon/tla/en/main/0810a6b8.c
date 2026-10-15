/* Draft, differ 4 of 28 (2026-10-15), ported from The Broken Seal's EventTable_CopyRowHeader over
 * this game's gData_0810c3f4: the native copies output to r5 and loads the table between the two
 * halves of row * 33 (lsls r3, r0, #5; adds r5, r1, #0; ldr r1, =table; adds r3, r3, r0); here the
 * add comes right after the shift. rowtype.c and 0810a670 show the same order. Three minutes of
 * alchemy permute found nothing better. */
#include "types.h"

extern const s16 gData_0810c3f4[][33];

s32 Func_0810a6b8(s32 rowNo, s16 *output)
{
    const s16 *src;
    s16 *dst;
    s32 count;

    count = 0;
    if (gData_0810c3f4[rowNo][0] != 0) {
        dst = output;
        src = gData_0810c3f4[rowNo];
        do {
            *dst = *(const volatile s16 *)src;
            count++;
            src++;
            dst++;
            if (count > 23)
                break;
        } while (*src != 0);
    }
    output[count] = 0;
    return count;
}
