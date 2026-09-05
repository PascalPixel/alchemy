#include "types.h"

#define Map_ClearLayerEntryFlag Func_080118a8

extern u8 * volatile Data_03001e70;

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = Data_03001e70;
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Func_080118c0(u32 no)
{
    u8 *base = Data_03001e70;
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}
