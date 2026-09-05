#include "inventory.h"

void Func_0808a548(s32);

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Func_0808a548(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}
