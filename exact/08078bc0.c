#include "types.h"
#include "owner_state.h"

s32 Func_08078bc0(s32 owner, s32 value)
{
    u8 *entry = (u8 *)Func_08077394(owner);
    s32 mask = 0x3fff;
    s32 index = 0;

    entry += 88;
    do {
        s32 current = *(u16 *)entry;

        current &= mask;
        entry += 4;
        if (current == value) {
            return 1;
        }
        index++;
    } while (index <= 31);
    return 0;
}
