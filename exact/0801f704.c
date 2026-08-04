#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Func_0801f704(void) {
    u32 index;
    u8 *entry;

    entry = (u8 *)(*(s32 *)0x03001F1C + 0x1040);
    for (index = 0; index < 3; index++, entry += 0x40) {
        if (M2C_FIELD(entry, u8 *, 0x1C) == 0)
            return index;
    }
    return 0x3E7;
}
