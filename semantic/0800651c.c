#include "types.h"

void Func_0800651c(void)
{
    u8 *control = (u8 *)0x02002220;
    volatile u16 *ime = (volatile u16 *)0x04000208;
    u32 saved = *ime;
    int zero = 0;

    *ime = (u32)ime;
    *(volatile u8 *)(control + 1) = 0x80;
    *(volatile u32 *)0x02002080 = zero;
    *(volatile u16 *)0x02002008 = zero;
    *(volatile u32 *)0x020023ac = zero;
    *(volatile u8 *)(control + 3) = zero;
    *(volatile u8 *)(control + 2) = zero;
    *(volatile u16 *)0x02002238 = zero;
    *ime = saved;
}
