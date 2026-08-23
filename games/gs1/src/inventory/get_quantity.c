#include "inventory.h"

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
    s32 item_id;

    owner = ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    item_id = 0x1ff;
    item_id &= owner;
    owner = (u32)owner >> 11;
    owner++;
    if (item_id == 0) {
        owner = 0;
    }
    return owner;
}
