/* main:080a3d9c, complete 64-byte owner through 080a3ddc.
 * H1: typed owner inventory and a bounded post-increment scan; quantity is
 * decoded from the first matching nonempty slot, not a sum across slots.
 * Own-ROM caller 080a4f08 passes an owner and a masked item ID. The exact
 * Inventory_GetQuantity family establishes the packed count plus one.
 * H1 result: 60/64 bytes, 22 aligned halfword edits. The for-loop rotates
 * the scan and strength reduction removes the explicit packed-count mask.
 * H2 tests the reference's bottom-tested scan, with the increment confined
 * to the unmatched path. Prediction: no peeled first iteration, and the
 * H2 result: 56/64 bytes, 14 aligned edits. Bottom-test topology agrees,
 * but unsigned-load range folding removes the count mask and halfword
 * conversion copy; the item mask is loaded inside the loop.
 */
#include "TYPES.H"
#include "OWNER_STATE.H"

struct OwnerInventoryState *Owner_GetStateFar(s32 owner);

s32 Func_080a3d9c(s32 owner, s32 item)
{
    s32 i;
    s32 quantity;
    u16 *slots;
    s32 entry;

    quantity = 0;
    slots = Owner_GetStateFar(owner)->inventory;
    i = 0;
next_slot:
    entry = *slots++;
    if ((u16)entry != 0 && (entry & 0x1ff) == item) {
        quantity = (u32)(entry & 0xf800) >> 11;
        quantity++;
    } else {
        i++;
        if (i < 15)
            goto next_slot;
    }
    return quantity;
}
