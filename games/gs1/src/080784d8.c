#include "types.h"
#include "owner_state.h"

s32 Func_080784d8(s32 owner)
{
    struct OwnerInventoryState *state = Func_08077394(owner);
    s32 count = 0;

    if (state->inventory[count] != 0) {
        do {
            count++;
            if (count > 14)
                break;
        } while (state->inventory[count] != 0);
    }
    return count;
}
