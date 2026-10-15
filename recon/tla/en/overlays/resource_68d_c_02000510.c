/* Draft of the two blend fades Func_02000510 and Func_02000550 of overlay 68D (differ 39 of
 * 29 and 27). The native rebuilds 0x04000052 and 16 inside the loop on every pass and uses
 * those registers again after it; the draft's loop pass moves both out in front of the loop.
 * Measured: a plain and a volatile u16 store of the constants load them from the literal pool
 * (the native builds 0x1000 with a shift), so the stores take an s32 through the inline; with
 * it the constants are right and only the loop invariants differ.
 * Trials: a do-while loop and a while (1) loop with a break differ 39 too. Written as goto
 * loops (no loop notes, so the loop pass leaves the constants and CSE carries both registers
 * past the exit), with every write through a do/while (0) statement macro, both functions
 * match all twelve editions (commit 3e896829de; differ 4 and 2 with the plain inline), but
 * S2's goto count may only fall, so that form is not adopted. */
#include "types.h"

extern u8 Data_0200308c;

void WaitFrames(s32 frames);

static __inline__ void BlendSet(s32 value)
{
    *(volatile u16 *)0x04000052 = value;
}

void Func_02000510(void)
{
    s32 i;

    BlendSet(0x1000);
    Data_0200308c = 0;
    for (i = 0; i < 16; i++) {
        BlendSet(((16 - i) << 8) | i);
        WaitFrames(4);
    }
    BlendSet(16);
    Data_0200308c = 1;
}

void Func_02000550(void)
{
    s32 i;

    BlendSet(16);
    Data_0200308c = 0;
    for (i = 0; i < 16; i++) {
        BlendSet((i << 8) | (16 - i));
        WaitFrames(3);
    }
    BlendSet(0x1000);
}
