#include "INVENTORY.H"

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & 0x1ff) == item_id) {
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}

s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;
    s16 owner;

    if (Inventory_Find(gPartyState.current_owner, item_id) != -1)
        return gPartyState.current_owner;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            owner = *owner_cursor++;
            if (Inventory_Find(owner, item_id) != -1)
                return owner;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}
