#include "inventory.h"

s32 PartyInventory_CountItem(s32 item_id)
{
    u16 owners[16];
    s32 item_count = 0;
    s32 owner_count = Party_ListActiveOwners(owners);

    if (item_count < owner_count) {
        u16 *owner_cursor = owners;
        s32 remaining_owners = owner_count;

        do {
            item_count += Inventory_CountItem(*owner_cursor++, item_id);
            remaining_owners--;
        } while (remaining_owners != 0);
    }
    return item_count;
}
