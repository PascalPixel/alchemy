/* Draft of the two blend fades Func_02000510 and Func_02000550 of overlay 68D (differ 39 of
 * 29 and 27). The native rebuilds 0x04000052 and 16 inside the loop on every pass and uses
 * those registers again after it; the draft's loop pass moves both out in front of the loop.
 * Measured: a plain and a volatile u16 store of the constants load them from the literal pool
 * (the native builds 0x1000 with a shift), so the stores take an s32 through the inline; with
 * it the constants are right and only the loop invariants differ. A do-while form is untried. */
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
