#include "inventory.h"

extern u16 gData_080b1f40[];

s32 Item_AdjustCounter(s32 itemId, s32 delta)
{
    s32 result;
    s32 index = ITEM_ID_MASK;
    u16 *table = gData_080b1f40;
    u16 counter;

    /* FAKEMATCH: the barrier keeps the table's load between the mask constant and its and, and the index is doubled apart so the table stays the base register; without them the and comes first (one reordered pair, differ 2). */
    asm volatile("");
    index &= itemId;
    index <<= 1;
    counter = *(u16 *)((u8 *)table + index);
    result = 0;
    if (counter)
        result = ItemCounter_Adjust(counter - 1, delta);
    return result;
}

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *base = UnitGet(owner);
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
