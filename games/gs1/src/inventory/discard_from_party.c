#include "inventory.h"

s32 PartyInventory_Discard(s32 item_id)
{
    s32 owner = PartyInventory_FindOwner();

    if (owner == -1)
        return 0;
    Inventory_Discard(owner, Inventory_Find(owner, item_id));
    return 0;
}
