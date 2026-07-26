#include "types.h"

extern u8 * volatile Data_03001e70;

void Func_080118a8(u32 index)
{
    u8 *base = Data_03001e70;
    u8 *object = base + index * 12;
    u32 value = 0;
    *(u16 *)(object + 0x22) = value;
}

void Func_080118c0(u32 index)
{
    u8 *base = Data_03001e70;
    u8 *object = base + index * 12;
    u32 value = 1;
    *(u16 *)(object + 0x22) = value;
}
