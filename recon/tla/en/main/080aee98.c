/* Near miss: differ 2, the split-constant scheduling gap: ☀️'s
   Inventory_Find as is; the listing builds ITEM_ID_MASK's add (adds r4,
   #255) before stepping to the inventory (adds r0, #216), this build after.
   Goes after PartyInventory_FindRoom in ITEMS_2.C. */
#include "INVENTORY.H"

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & ITEM_ID_MASK) == item_id) {
            return slot;
        }
        slot++;
    } while (slot < INVENTORY_SLOTS);
    return -1;
}
