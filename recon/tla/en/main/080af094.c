#include "inventory.h"

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
