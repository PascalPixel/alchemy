#include "types.h"
#include "item.h"
#include "owner_state.h"

s32 Func_08078870(struct OwnerInventoryState *state, s32 type)
{
    s32 i;

    for (i = 0; i <= 14; i++) {
        if (state->inventory[i] & 0x200) {
            struct ItemDefinition *item =
                Func_08078414(state->inventory[i]);

            if (item->type == type) {
                return state->inventory[i] & 0x1FF;
            }
        }
    }
    return 0;
}
