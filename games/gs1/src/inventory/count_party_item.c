#include "inventory.h"

s32 PartyInventory_CountItem(s32 item_id)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Party_ListActiveOwners(owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            result += Inventory_CountItem(*owner++, item_id);
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
