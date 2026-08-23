#include "inventory.h"

s32 PartyInventory_FindOwner(s32 item_id)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner_cursor;
    s16 candidate_owner;

    if (Inventory_Find(Data_02000240.current_owner, item_id) != -1)
        return Data_02000240.current_owner;
    count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    index = 0;
    if (index < count) {
        do {
            candidate_owner = *owner_cursor++;
            if (Inventory_Find(candidate_owner, item_id) != -1)
                return candidate_owner;
            index++;
        } while (index < count);
    }
    return -1;
}
