#include "types.h"
#include "global_cells.h"

#define Runtime_GetRemainingIwram Func_0800488c

s32 Runtime_GetRemainingIwram(void)
{
    s32 state = ADDR_03001E50;

    return 0x03007800 - *(s32 *)(state + 4);
}
