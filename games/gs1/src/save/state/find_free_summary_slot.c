#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 SaveState_FindFreeSummarySlot(void) {
    u32 i;
    u8 *p;

    p = (u8 *)(*(s32 *)ADDR_03001F1C + 0x1040);
    for (i = 0; i < 3; i++, p += 0x40) {
        if (FIELD_AT_OFFSET(p, u8 *, 0x1C) == 0)
            return i;
    }
    return 0x3E7;
}
