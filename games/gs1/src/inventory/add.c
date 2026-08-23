#include "inventory.h"

/* 所持品追加。積み重ね可能な品は同一番号の枠を探して個数を増やし、
   そうでなければ空き枠へ入れる。戻り値は枠番号、失敗は -1。 */
s32 Inventory_Add(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *inv = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 slot;

    if ((item->flags & 0x10) != 0) {
        slot = 0;
        if (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0) {
            do {
                slot++;
                if (slot > 14)
                    break;
            } while (((inv->inventory[slot] ^ item_id) & 0x1ff) != 0);
        }
        if (slot != 15) {
            s32 entry = inv->inventory[slot];
            u32 count = ((u32)entry >> 11) + 1;

            if (count > 29)
                return -1;
            {
                s32 value = 0x7ff;

                value &= entry;
                value |= count << 11;
                inv->inventory[slot] = value;
            }
            return slot;
        }
    }

    slot = 0;
    do {
        if (inv->inventory[slot] == 0) {
            inv->inventory[slot] = item_id;
            return slot;
        }
        slot++;
    } while (slot <= 14);
    return -1;
}
