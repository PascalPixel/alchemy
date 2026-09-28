#include "INVENTORY.H"

void Event_ClearInvalidPackedValuesFar(s32);
void Owner_RecalculateStats(s32 owner);

/* Takes one item from an owner's slot: a stacked item loses one from its
   count, a single item is removed and the list is compacted so the empty
   slots follow the rest. The compaction reads the slots as signed
   halfwords. Returns 1 or 2 for those cases, -1 for an empty slot. */
s32 Inventory_Remove(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    u16 item = inv->inventory[slot];
    s32 result = -1;

    if (item != 0) {
        if (item & 0xf800) {
            inv->inventory[slot] = item - 0x800;
            result = 1;
        } else {
            s16 *list;
            s32 count;
            s32 i;

            inv->inventory[slot] = 0;
            list = (s16 *)inv->inventory;
            count = 0;
            for (i = 0; i < 15; i++) {
                if (list[i] != 0)
                    list[count++] = list[i];
            }
            for (; count < 15; count++)
                list[count] = 0;
            result = 2;
        }
    }
    Owner_RecalculateStats(owner);
    return result;
}

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)Owner_GetState(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Event_ClearInvalidPackedValuesFar(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    s32 item_id = inv->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirect(item_id);

    if (item_id == 0) {
        return -1;
    }
    if ((item->flags & 8) != 0) {
        return -4;
    }
    if ((inv->inventory[slot] & 0x200) != 0 &&
        (item->flags & 2) != 0) {
        return -3;
    }
    return 0;
}

s32 PartyInventory_Remove(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner(item_id);

    if (owner == -1)
        return 0;
    Inventory_Remove(owner, Inventory_Find(owner, item_id));
    return 0;
}

s32 PartyInventory_Discard(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner(item_id);

    if (owner == -1)
        return 0;
    Inventory_Discard(owner, Inventory_Find(owner, item_id));
    return 0;
}

s32 Inventory_Break(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] |= 0x400;
    return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}
