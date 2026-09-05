#include "types.h"

#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Graphics_ClearBg1ControlBit2(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}
