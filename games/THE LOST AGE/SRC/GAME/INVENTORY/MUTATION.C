#include "INVENTORY.H"

void Event_ClearInvalidPackedValuesFar(s32);

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)Owner_GetState(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Event_ClearInvalidPackedValuesFar(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}
