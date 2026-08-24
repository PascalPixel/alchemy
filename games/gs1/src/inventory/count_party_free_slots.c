#include "inventory.h"

s32 PartyInventory_CountFreeSlots(void)
{
    s16 owners[10];
    s32 owner_count = Party_ListActiveOwners(owners);
    s32 free_slot_count = 0;
    s16 *owner_cursor = owners;

    if (free_slot_count < owner_count) {
        s32 remaining_owners = owner_count;

        do {
            free_slot_count = free_slot_count - Inventory_Count(*owner_cursor++) + 15;
            remaining_owners--;
        } while (remaining_owners != 0);
    }
    return free_slot_count;
}
