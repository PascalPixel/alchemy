#include "INVENTORY.H"

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    struct OwnerInventoryState *base = Owner_GetState(owner);
    s32 index;
    struct ItemDefinition *item;

    for (index = 0; index < INVENTORY_SLOTS; index++) {
        if (base->inventory[index] & INVENTORY_EQUIPPED) {
            item = Item_GetDirect(
                base->inventory[index]);
            if (item->type == type) break;
        }
    }
    if (index == INVENTORY_SLOTS) index = -1;
    return index;
}

struct ItemDefinition *Inventory_GetEquippedDefinition(
    struct OwnerInventoryState *inv,
    s32 type)
{
    s32 slot;
    struct ItemDefinition *item;

    for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (inv->inventory[slot] & INVENTORY_EQUIPPED) {
            item = Item_GetDirect(inv->inventory[slot]);
            if (item->type == type) {
                return item;
            }
        }
    }
    return 0;
}
