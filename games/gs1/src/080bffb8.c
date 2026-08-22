#include "types.h"

void Func_08003b70(s32 arg0);
u32 Func_08004458(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Func_08004458
void Func_080030f8(s32);

s32 Func_080bffb8(void)
{
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    s32 i;
    volatile u16 *reg;

    bg0cnt = *(volatile u16 *)0x04000008;
    *(volatile u16 *)0x04000008 = bg0cnt | 0x40;
    reg = (volatile u16 *)0x04000008;
    bg1cnt = *++reg;
    *reg = bg1cnt | 0x40;
    bg2cnt = *++reg;
    *reg = bg2cnt | 0x40;
    bg3cnt = *++reg;
    *reg = bg3cnt | 0x40;
    reg += 0x21;
    *reg = 0x3eee;

    Func_08003b70(16);
    for (i = 0; i <= 15; i++) {
        Rand();
        Rand();
        Rand();
        Rand();
        *(volatile u16 *)0x0400004c = (i << 8) | i;
        Func_080030f8(1);
    }

    *(volatile u16 *)0x04000000 = 1;
    Func_080030f8(4);
    reg = (volatile u16 *)0x04000008;
    *reg++ = bg0cnt;
    *reg++ = bg1cnt;
    *reg++ = bg2cnt;
    *reg = bg3cnt;
    return 0;
}
