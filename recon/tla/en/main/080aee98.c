#include "INVENTORY.H"

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & ITEM_ID_MASK) == item_id) {
            return slot;
        }
        slot++;
    } while (slot < INVENTORY_SLOTS);
    return -1;
}
