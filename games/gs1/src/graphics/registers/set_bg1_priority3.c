#include "types.h"

#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Graphics_SetBg1Priority3(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    value |= 3;
    REG_BG1CNT = value;
}
