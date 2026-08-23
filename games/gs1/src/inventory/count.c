#include "inventory.h"

s32 Inventory_Count(s32 owner)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 count = 0;

    if (inv->inventory[count] != 0) {
        do {
            count++;
            if (count > 14)
                break;
        } while (inv->inventory[count] != 0);
    }
    return count;
}
