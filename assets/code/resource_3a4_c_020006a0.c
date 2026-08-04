#include "types.h"

extern s32 Func_02004142(void);
extern u16 Data_0200d238;

void Func_020006a0(void)
{
    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Func_02004142() * 100) >> 16 >= Data_0200d238) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}
