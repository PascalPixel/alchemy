#include "inventory.h"

s32 PartyInventory_CountFreeSlots(void)
{
    s16 owners[10];
    s32 count = Party_ListActiveOwners(owners);
    s32 result = 0;
    s16 *owner = owners;

    if (result < count) {
        s32 remaining = count;

        do {
            result = result - Inventory_Count(*owner++) + 15;
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
