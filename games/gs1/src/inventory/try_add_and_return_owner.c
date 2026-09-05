#include "types.h"

s32 Inventory_AddForOwner(s32, s32);

s32 Inventory_TryAddAndReturnOwner(
    s32 item_id, s32 unused, s32 owner_id)
{
    if (Inventory_AddForOwner(owner_id, item_id) >= 0)
        return owner_id;
    return -1;
}

void Inventory_NoOpCallback(void)
{
}
