/* Near miss, differ 2: the native loads the inventory entry before
   building the item id mask; this draft builds the mask first (the
   load-first gap). */
#include "TYPES.H"
#include "OWNER_STATE.H"
#include "ITEM.H"

/* Whether the owner's item in slot is equipped and cannot be taken off:
   1 if so, 0 if not, 8 for an empty slot. */
s32 Inventory_IsLockedEquipped(s32 owner, s32 slot)
{
    struct OwnerInventoryState *state = Owner_GetState(owner);
    u32 item = state->inventory[slot] & ITEM_ID_MASK;
    struct ItemDefinition *definition = Item_GetDirect(item);
    s32 locked = 0;

    if (item == 0)
        return 8;
    if ((state->inventory[slot] & INVENTORY_EQUIPPED) && (definition->flags & ITEM_CANNOT_UNEQUIP))
        locked = 1;
    return locked;
}
