#include "owner_state.h"
#include "types.h"

s32 Inventory_CountStackedItem(s32 owner, s32 item_id)
{
    u16 *slot;
    s32 count;
    s32 i;

    slot = (u16 *)((u8 *)UnitGet(owner) + 0xd8);
    count = 0;
    for (i = 14; i >= 0; i--) {
        u32 value = *slot++;

        if (value != 0 && (value & 0x1ff) == item_id) {
            count += (value & 0xf800) >> 11;
            count++;
        }
    }
    return count;
}
