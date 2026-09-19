#include "INVENTORY.H"

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    u8 *base = OwnerState_Get(owner);
    s32 index;
    s32 offset;
    struct ItemDefinition *item;

    for (index = 0, offset = 216; index <= 14; index++) {
        if (*(u16 *)((u8 *)offset + (s32)base) & 0x200) {
            item = Item_GetDirect(
                *(u16 *)((u8 *)offset + (s32)base));
            if (item->type == type) break;
        }
        offset += 2;
    }
    if (index == 15) index = -1;
    return index;
}

struct ItemDefinition *Inventory_GetEquippedDefinition(
    struct OwnerInventoryState *inv,
    s32 type)
{
    s32 slot;
    struct ItemDefinition *item;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            item = Item_GetDirect(inv->inventory[slot]);
            if (item->type == type) {
                return item;
            }
        }
    }
    return 0;
}

s32 Inventory_GetEquippedItem(struct OwnerInventoryState *inv, s32 type)
{
    s32 slot;

    for (slot = 0; slot <= 14; slot++) {
        if (inv->inventory[slot] & 0x200) {
            struct ItemDefinition *item =
                Item_GetDirect(inv->inventory[slot]);

            if (item->type == type) {
                return inv->inventory[slot] & 0x1ff;
            }
        }
    }
    return 0;
}
