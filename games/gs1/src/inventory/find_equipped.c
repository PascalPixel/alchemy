#include "inventory.h"

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
    u8 *base = OwnerState_Get(owner);
    s32 index;
    s32 offset;
    struct ItemDefinition *item;

    for (index = 0, offset = 216; index <= 14; index++) {
        if (*(u16 *)((u8 *)offset + (s32)base) & 0x200) {
            item = Item_GetDirect(
                *(u16 *)((u8 *)offset + (s32)base));
            if (item->type == type) break;
        }
        offset += 2;
    }
    if (index == 15) index = -1;
    return index;
}
