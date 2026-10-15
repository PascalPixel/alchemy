#include "inventory.h"

s32 Inventory_AddItem(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = UnitGet(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 slot;

    if ((item->flags & ITEM_STACKABLE) != 0) {
        slot = 0;
        if (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0) {
            do {
                slot++;
                if (slot >= INVENTORY_SLOTS)
                    break;
            } while (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0);
        }
        if (slot != INVENTORY_SLOTS) {
            s32 entry = inv->inventory[slot];
            u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

            if (count > 29)
                return -1;
            {
                /* FAKEMATCH: a separate mask temporary preserves the and/store registers. */
                s32 value = INVENTORY_ENTRY_MASK;

                value &= entry;
                value |= count << INVENTORY_QUANTITY_SHIFT;
                inv->inventory[slot] = value;
            }
            return slot;
        }
    }

    slot = 0;
    do {
        if (inv->inventory[slot] == 0) {
            inv->inventory[slot] = item_id;
            return slot;
        }
        slot++;
    } while (slot < INVENTORY_SLOTS);
    return -1;
}

s32 PartyInventory_Add(s32 item_id)
{
    s16 owners[10];
    s32 ownerCount;
    s32 ownerIndex;
    s16 *owner_cursor;

    ownerCount = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    ownerIndex = 0;
    if (ownerIndex < ownerCount) {
        do {
            s16 owner_id = *owner_cursor++;

            if (Inventory_AddItem(owner_id, item_id) >= 0)
                return owner_id;
            ownerIndex++;
        } while (ownerIndex < ownerCount);
    }
    return -1;
}

s32 PartyInventory_FindRoom(s32 item_id)
{
    s16 owners[10];
    s32 ownerCount;
    s32 ownerIndex;

    ownerCount = Party_ListActiveOwners(owners);
    for (ownerIndex = 0; ownerIndex < ownerCount; ownerIndex++) {
        s16 owner_id = owners[ownerIndex];
        s32 slot = Inventory_AddItem(owner_id, item_id);

        if (slot >= 0) {
            Inventory_Remove(owner_id, slot);
            return owner_id;
        }
    }
    return -1;
}
