/* Near miss: differ 2, the split-constant scheduling gap: the listing loads
   the slot (ldrh r3, [r7, r6]) before building ITEM_ID_MASK; this build
   starts the mask (movs r5, #128) first, as in Item_AdjustCounter. ☀️'s
   Inventory_CheckDiscard returns codes; ⚓️'s returns bits. Goes after
   Inventory_CheckUnequip in MUTATION.C. */
#include "INVENTORY.H"

/* 8 for an empty slot, else bit 1 when the item is indispensable and bit 0
   when it is equipped and cannot be taken off. */
s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
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
