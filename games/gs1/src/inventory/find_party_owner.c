#include "inventory.h"

s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;
    s16 candidate_owner;

    if (Inventory_Find(Data_02000240.current_owner, item_id) != -1)
        return Data_02000240.current_owner;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            candidate_owner = *owner_cursor++;
            if (Inventory_Find(candidate_owner, item_id) != -1)
                return candidate_owner;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}
