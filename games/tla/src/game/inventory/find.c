#include "inventory.h"

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = UnitGet(owner);
    s32 slot;

    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if ((inv->inventory[slot] & ITEM_ID_MASK) == item_id) {
            return slot;
        }
    }
    return -1;
}

s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 ownerCount;
    s32 ownerIndex;
    s16 *owner_cursor;
    s16 owner;

    if (Inventory_Find(gPartyState.currentOwner, item_id) != -1)
        return gPartyState.currentOwner;
    ownerCount = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    ownerIndex = 0;
    if (ownerIndex < ownerCount) {
        do {
            owner = *owner_cursor++;
            if (Inventory_Find(owner, item_id) != -1)
                return owner;
            ownerIndex++;
        } while (ownerIndex < ownerCount);
    }
    return -1;
}
