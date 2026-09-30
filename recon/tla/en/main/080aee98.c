/*
 * Draft: Inventory_Find does not yet match; 3 halfwords differ from ☀️'s C, first at +0xe (adds r4, #255).
 * Links as recon/tla/raw/080aee98.s.
 */
#include "INVENTORY.H"

s32 Inventory_Find(s32 owner, s32 item_id)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    s32 slot = 0;
    u16 *entry = inv->inventory;

    do {
        if (((*entry++) & 0x1ff) == item_id) {
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}
