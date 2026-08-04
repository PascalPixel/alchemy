#include "types.h"

extern s32 Func_030003e0(s32 frame, s32 period);
extern s32 Func_03000380(s32 value, s32 divisor);

/*
 * Advance the six-colour highlight ramp every fifth frame.  The low colour
 * channel is copied from the preceding palette entry and eased on the first
 * three entries; the two high channels come from the scene's authored colour
 * controls.  Complete owner and pools: 0x02000d98-0x02000e3b.
 */
void Func_02000d98(void)
{
    volatile s32 *frame = (volatile s32 *)0x03001e40;
    volatile s32 *phase = (volatile s32 *)0x0200a91c;
    volatile s32 *blue = (volatile s32 *)0x0200a924;
    volatile s32 *green = (volatile s32 *)0x0200a920;
    volatile u16 *palette = (volatile u16 *)0x05000000;
    s32 highChannels = 0;
    s32 i;

    if (Func_030003e0(*frame, 5) != 0)
        return;
    *phase = (*phase + 4) & 31;

    for (i = 0; i < 6; i++) {
        s32 red = palette[110 - i] & 31;
        if (i <= 2)
            red -= Func_03000380(red << 2, 10);
        highChannels = (*blue << 10) | (*green << 5);
        palette[111 - i] = (u16)(red | highChannels);
    }
    palette[105] = (u16)(*phase | highChannels);
}
