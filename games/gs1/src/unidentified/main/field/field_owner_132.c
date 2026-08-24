#include "types.h"

#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Func_08021e14(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}
