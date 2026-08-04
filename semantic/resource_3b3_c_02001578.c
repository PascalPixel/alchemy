#include "types.h"

extern u32 Func_080000f8(void);

/*
 * Complete 84-byte affine-register refresh, including alignment and its six
 * literal words through 0x020015cb.  Around the top/bottom of the display it
 * occasionally selects the prepared alternate transform; otherwise it copies
 * the live three-word transform into the GBA BG2 affine reference registers.
 */
void Func_02001578(void)
{
    volatile u16 *vcount = (volatile u16 *)0x04000006;
    volatile u32 *destinationX = (volatile u32 *)0x04000014;
    volatile u32 *destinationY = (volatile u32 *)0x04000018;
    const u32 *source = (const u32 *)0x03001ad4;

    if (*vcount == 227 || *vcount <= 46) {
        u32 chance = (Func_080000f8() * 100u) >> 16;
        if (chance < *(u32 *)0x0200b738)
            source = (const u32 *)0x0200b72c;
    }

    *destinationX = source[0];
    destinationY[0] = source[1];
    destinationY[1] = source[2];
}
