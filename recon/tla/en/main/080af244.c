#include "inventory.h"

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = UnitGet(owner);
    s32 item_id = inv->inventory[slot] & ITEM_ID_MASK;
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 result = 0;

    if (item_id == 0)
        return 8;
    if ((item->flags & ITEM_INDISPENSABLE) != 0)
        result = 2;
    if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0 &&
        (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
        result |= 1;
    return result;
}
