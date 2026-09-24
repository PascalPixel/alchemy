#include "TYPES.H"

extern u8 *volatile gMapWork;

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = gMapWork;
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Func_080118c0(u32 no)
{
    u8 *base = gMapWork;
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}
