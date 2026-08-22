#include "types.h"
#include "global_cells.h"

s32 Func_0800488c(void)
{
    s32 state = ADDR_03001E50;

    return 0x03007800 - *(s32 *)(state + 4);
}
