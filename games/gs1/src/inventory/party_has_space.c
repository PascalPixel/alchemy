#include "inventory.h"

s32 PartyInventory_HasSpace(void)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner_cursor;

    if (Inventory_Count(Data_02000240.current_owner) != 15)
        return 1;
    count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    index = 0;
    if (index < count) {
        do {
            if (Inventory_Count(*owner_cursor++) != 15)
                return 1;
            index++;
        } while (index < count);
    }
    return 0;
}
