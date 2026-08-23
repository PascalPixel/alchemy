#include "inventory.h"

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
