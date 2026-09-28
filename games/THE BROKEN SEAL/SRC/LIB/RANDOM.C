#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001cb4[];

u32 Random16(void)
{
    u32 value = *(u32 *)((u32)&Data_03001cb4) * 0x41c64e6d + 0x3039;

    *(u32 *)((u32)&Data_03001cb4) = value;
    return (value << 8) >> 16;
}
