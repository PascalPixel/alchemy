#include "types.h"

/* resource_39c owner at 0x02000d58, 164 bytes: every fifth frame, rotate six
 * palette entries while composing their upper colour channels from three
 * in-image state cells. The branch at 0x02000d82 skips the inline three-word
 * pool; the sole return is at 0x02000de2-0x02000de8, followed by alignment and
 * four tail words through 0x02000dfb. Both arithmetic calls resolve and match. */
s32 Func_030003e0();
s32 Func_03000380();

void Func_02000d58(void)
{
    volatile s32 *const phase = (volatile s32 *)0x0200e004;
    volatile s32 *const green = (volatile s32 *)0x0200e008;
    volatile s32 *const blue = (volatile s32 *)0x0200e00c;
    volatile u16 *const palette = (volatile u16 *)0x05000000;
    s32 index;
    s32 upper = 0;

    if (Func_030003e0(*(volatile s32 *)0x03001e40, 5) != 0)
        return;

    *phase = (*phase + 4) & 31;

    for (index = 0; index <= 5; index++) {
        s32 low = palette[110 - index] & 31;
        if (index <= 2)
            low -= Func_03000380(low << 2, 10);

        upper = (*blue << 10) | (*green << 5);
        palette[111 - index] = (u16)(low | upper);
    }

    palette[105] = (u16)(*phase | upper);
}
