#include "inventory.h"

void Func_0808a548(s32);

s32 Inventory_Discard(s32 owner, s32 slot)
{
    s32 item =
        ((struct OwnerInventoryState *)OwnerState_Get(owner))->inventory[slot];
    s32 removed_slot = Inventory_Remove(owner, slot);

    if (removed_slot != -1) {
        Func_0808a548(Item_AdjustCounter(item, 1));
    }
    return removed_slot;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
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
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] |= 0x400;
    return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}
