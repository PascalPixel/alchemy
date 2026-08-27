#include "inventory.h"

void Func_08077428(s32 owner);

s32 Inventory_Remove(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv;
    s32 entry;
    s32 result = -1;

    inv = OwnerState_Get(owner);
    entry = inv->inventory[slot];

    if (entry != 0) {
        s32 masked = entry & 0xf800;

        if (masked != 0) {
            entry -= 0x800;
            inv->inventory[slot] = entry;
            result = 1;
        } else {
            u16 *base;
            u16 *src;
            u16 *dst;
            u16 *fill;
            s32 count;
            s32 i;

            inv->inventory[slot] = masked;
            base = inv->inventory;
            src = base;
            dst = base;
            count = 0;

            for (i = 14; i >= 0; i--) {
                entry = *src++;
                if (entry != 0) {
                    *dst++ = entry;
                    count++;
                }
            }

            if (count <= 14) {
                fill = base + count;
                i = 15 - count;
                do {
                    i--;
                    *fill++ = 0;
                } while (i != 0);
            }

            result = 2;
        }
    }

    Func_08077428(owner);
    return result;
}
