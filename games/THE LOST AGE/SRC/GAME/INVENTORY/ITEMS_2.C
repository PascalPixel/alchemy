#include "INVENTORY.H"

/* 所持品追加。積み重ね可能な品は同一番号の枠を探して個数を増やし、
   そうでなければ空き枠へ入れる。戻り値は枠番号、失敗は -1。 */
s32 Inventory_AddItem(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner_id);
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
                /* FAKEMATCH: the mask gets its own temporary before the
                   and; the one-expression store allocates other registers. */
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
    s32 owner_count;
    s32 owner_index;
    s16 *owner_cursor;

    owner_count = Party_ListActiveOwners(owners);
    owner_cursor = owners;
    owner_index = 0;
    if (owner_index < owner_count) {
        do {
            s16 owner_id = *owner_cursor++;

            if (Inventory_AddItem(owner_id, item_id) >= 0)
                return owner_id;
            owner_index++;
        } while (owner_index < owner_count);
    }
    return -1;
}
