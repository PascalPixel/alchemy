#include "types.h"
#include "owner_state.h"

s32 Func_08078664(s32 owner, s32 value)
{
    struct OwnerInventoryState *state = Func_08077394(owner);
    s32 index = 0;
    u16 *entry = state->inventory;

    do {
        if (((*entry++) & 0x1ff) == value) {
            return index;
        }
        index++;
    } while (index <= 14);
    return -1;
}
