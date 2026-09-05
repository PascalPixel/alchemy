#include "inventory.h"

s32 PartyInventory_CountFreeSlots(void)
{
    s16 owners[10];
    s32 owner_count = Party_ListActiveOwners(owners);
    s32 cnt = 0;
    s16 *owner_cursor = owners;

    if (cnt < owner_count) {
        s32 n = owner_count;

        do {
            cnt = cnt - Inventory_Count(*owner_cursor++) + 15;
            n--;
        } while (n != 0);
    }
    return cnt;
}
