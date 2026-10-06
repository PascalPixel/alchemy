#include "INVENTORY.H"

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *base = Owner_GetState(owner);
    s32 count = 0;
    s32 target = item_id & ITEM_ID_MASK;
    s32 index = 0;

    do {
        if ((base->inventory[index] & ITEM_ID_MASK) == target) {
            struct ItemDefinition *item = Item_GetDirect(target);

            if (item->flags & ITEM_STACKABLE) {
                count = (base->inventory[index] >> INVENTORY_QUANTITY_SHIFT) + 1;
                break;
            }
            count++;
        }
        index++;
    } while (index < INVENTORY_SLOTS);
    return count;
}
