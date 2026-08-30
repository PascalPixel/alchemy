#include "types.h"

extern s32 Func_08002322(s32 angle);

void Func_0801fd34(void)
{
    s32 *phase;
    volatile u16 *palette;
    s32 packed;
    s32 value;
    s32 index;

    phase = (s32 *)0x03001800;
    palette = (volatile u16 *)0x050001D0;
    index = 0;
    do {
        value = Func_08002322((*phase + index * 8) * 0x300);
        if (value < 0) {
            value += 0x3FFF;
        }
        packed = value >> 14;
        {
            u32 low = packed << 1;
            u32 middle = packed;

            low += 22;
            middle += 16;
            packed += 20;
            packed <<= 10;
            middle <<= 5;
            packed |= middle;
            packed |= low;
        }
        *palette++ = packed;
    } while (++index <= 3);
}
