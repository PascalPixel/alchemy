#include "types.h"

u32 Func_08004938(s32 size)
{
    u32 *state = (u32 *)0x03001E50;
    u32 next_address;
    u32 next;
    u32 result;
    u32 units = ((u32)size + 3) >> 2;

    result = state[1];
    size = (s32)(units << 2);
    next = result + (u32)size;
    if (next > 0x030077FFU) {
        result = state[0];
        next_address = result + (u32)size;
        if (next_address >= 0x02040000U) {
            return 0U;
        }
        state[0] = next_address;
        goto block_5;
    }
    state[1] = next;
block_5:
    return result;
}
