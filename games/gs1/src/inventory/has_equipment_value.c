#include "inventory.h"

s32 Equipment_HasValue(s32 owner, s32 value)
{
    u8 *entry = OwnerState_Get(owner);
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
