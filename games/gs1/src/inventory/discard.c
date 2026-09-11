#include "scene.h"
#include "abi/inventory/discard.h"
#include "inventory.h"

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Sys_Do(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}
