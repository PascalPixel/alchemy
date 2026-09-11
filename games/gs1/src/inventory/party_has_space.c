#include "scene.h"
#include "abi/inventory/party_has_space.h"
#include "inventory.h"

s32 PartyInventory_HasSpace(void)
{
    s16 owners[10];
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    if (Inventory_Count(gCell.current_owner) != 15)
        return 1;
    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            if (Inventory_Count(*owner_cursor++) != 15)
                return 1;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return 0;
}
