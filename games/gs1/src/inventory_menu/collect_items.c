#include "types.h"
#include "owner_state.h"

#define InventoryMenu_CollectItems Func_080a3ddc

s32 InventoryMenu_CollectItems(struct OwnerInventoryState *owner, u16 *items)
{
    s32 count;
    s32 i;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    for (i = 0; i < 15; i++) {
        items[i] = 0;
        if (owner->inventory[i] != 0) {
            items[count] = owner->inventory[i];
            count++;
        }
    }
    return count;
}
