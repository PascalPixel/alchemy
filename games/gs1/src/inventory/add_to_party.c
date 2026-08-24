#include "inventory.h"

s32 PartyInventory_Add(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            s16 owner_id = *owner_cursor++;

            if (Inventory_Add(owner_id, item_id) >= 0)
                return owner_id;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}
