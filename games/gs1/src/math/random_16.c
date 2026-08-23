#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

u32 Random16(void)
{
    s32 value;
    value = (*(s32 *)ADDR_03001CB4 * 0x41c64e6d) + 0x3039;
    *(s32 *)ADDR_03001CB4 = value;
    return (u32)(value << 8) >> 16;
}
