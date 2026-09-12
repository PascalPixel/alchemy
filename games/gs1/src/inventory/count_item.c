#include "inventory.h"

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
    u8 *base = OwnerState_Get(owner);
    s32 count = 0;
    s32 target = item_id & 0x1ff;
    s32 index = 0;
    s32 offset = 216;

    do {
        if ((*(u16 *)((u8 *)offset + (s32)base) & 0x1FF) == target) {
            struct ItemDefinition *item = Item_GetDirect(target);

            if (item->flags & 0x10) {
                count = (*(u16 *)((u8 *)offset + (s32)base) >> 11) + 1;
                break;
            }
            count++;
        }
        index++;
        offset += 2;
    } while (index <= 14);
    return count;
}
