#include "types.h"
#include "global_cells.h"

s32 Runtime_GetRemainingEwram(void) {
    return 0x02040000 - *(s32 *)ADDR_03001E50;
}
