#include "inventory.h"

s32 PartyInventory_Add(s32 item_id)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner;

    count = Party_ListActiveOwners(owners);
    owner = owners;
    index = 0;
    if (index < count) {
        do {
            s16 value = *owner++;

            if (Inventory_Add(value, item_id) >= 0)
                return value;
            index++;
        } while (index < count);
    }
    return -1;
}
