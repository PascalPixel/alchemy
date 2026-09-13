#include "types.h"
#include "global_cells.h"

u32 Random16(void)
{
    u32 value = *(u32 *)ADDR_03001CB4 * 0x41c64e6d + 0x3039;

    *(u32 *)ADDR_03001CB4 = value;
    return (value << 8) >> 16;
}
