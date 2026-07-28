#include "types.h"
#include "item.h"
#include "owner_state.h"

void *Func_0807882c(struct OwnerInventoryState *owner, s32 type)
{
    s32 i;
    struct ItemDefinition *item;

    for (i = 0; i <= 14; i++) {
        if (owner->inventory[i] & 0x200) {
            item = Func_08078414(owner->inventory[i]);
            if (item->type == type) {
                return item;
            }
        }
    }
    return 0;
}
