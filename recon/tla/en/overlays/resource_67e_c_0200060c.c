/* Draft, differ 0 of 73, but only through three goto loops, which new C may
 * not add (S2: the goto count may only fall). The native's loops are not
 * touched by loop.c: each pass rebuilds the palette base, nothing is
 * strength-reduced and the index counts down to 1 and is compared, as goto
 * loops without loop notes compile. As do-while, while or for loops (the
 * last over a PLTT index): differ 90 each (loop.c hoists the bases and
 * walks two pointers down). Adopted with the gotos at 12/12, it waits
 * here until a loop form without goto is found. */
#include "../../../../games/tla/src/field/overlay_67e/ov67e.h"

extern u32 gFrameCount;

// Every fourth frame, colours 1 to 14 of palettes 3, 4 and 5 rotate up by one.
void Func_0200060c(void)
{
    volatile u16 *palette;
    s16 saved;
    s32 index;

    if ((gFrameCount & 3) == 0) {
        saved = *(volatile u16 *)0x0500007c;
        index = 14;
    first:
        palette = (u16 *)0x05000060;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto first;
        *(volatile u16 *)0x05000062 = saved;

        saved = *(volatile u16 *)0x0500009c;
        index = 14;
    second:
        palette = (u16 *)0x05000080;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto second;
        *(volatile u16 *)0x05000082 = saved;

        saved = *(volatile u16 *)0x050000bc;
        index = 14;
    third:
        palette = (u16 *)0x050000a0;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 1)
            goto third;
        *(volatile u16 *)0x050000a2 = saved;
    }
}
