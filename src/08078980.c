#include "types.h"
#include "item.h"
#include "owner_state.h"

s32 Func_08078980(s32 owner, s32 index) {
    struct OwnerInventoryState *state = Func_08077394(owner);
    s32 id = state->inventory[index] & 0x1FF;
    struct ItemDefinition *description = Func_08078414(id);

    if (id == 0) {
        return -1;
    }
    if ((description->flags & 8) != 0) {
        return -4;
    }
    if ((state->inventory[index] & 0x200) != 0 &&
        (description->flags & 2) != 0) {
        return -3;
    }
    return 0;
}
