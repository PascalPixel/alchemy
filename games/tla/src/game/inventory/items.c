#include "inventory.h"

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
    s32 item_id;

    owner = ((struct OwnerInventoryState *)UnitGet(owner))->inventory[slot];
    /* FAKEMATCH: keeps the mask after the slot load. */
    __asm__("");
    item_id = ITEM_ID_MASK;
    item_id &= owner;
    owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
    owner++;
    if (item_id == 0)
    {
        owner = 0;
    }
    return owner;
}

s32 Inventory_Count(s32 owner)
{
    struct OwnerInventoryState *inv = UnitGet(owner);
    s32                         count = 0;

    if (inv->inventory[count] != 0)
    {
        do
        {
            count++;
            if (count > 14)
                break;
        } while (inv->inventory[count] != 0);
    }
    return count;
}

s32 PartyInventory_HasSpace(void)
{
    s16  owners[10];
    s32  ownerCount;
    s32  ownerIndex;
    s16 *ownerCursor;

    if (Inventory_Count(gPartyState.currentOwner) != 15)
        return 1;
    ownerCount = Party_ListActiveOwners(owners);
    ownerCursor = owners;
    ownerIndex = 0;
    if (ownerIndex < ownerCount)
    {
        do
        {
            if (Inventory_Count(*ownerCursor++) != 15)
                return 1;
            ownerIndex++;
        } while (ownerIndex < ownerCount);
    }
    return 0;
}

s32 PartyInventory_CountFreeSlots(void)
{
    s16  owners[10];
    s32  ownerCount = Party_ListActiveOwners(owners);
    s32  cnt = 0;
    s16 *ownerCursor = owners;

    if (cnt < ownerCount)
    {
        s32 n = ownerCount;

        do
        {
            cnt = cnt - Inventory_Count(*ownerCursor++) + 15;
            n--;
        } while (n != 0);
    }
    return cnt;
}
