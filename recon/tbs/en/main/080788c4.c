/* Draft, not exact (2026-09-24): 16 differing halfwords, 132 of 132 bytes.
   The slot base is taken before the cleared slot is stored and the
   compaction loop tests *src directly, which gives the reference registers
   through the first loop. Residual: the reference tests the item with
   lsls #16 (here a register copy), and the fill loop takes r0/r2/r5 for
   pointer, zero and counter (here r2/r3/r0). */
#include "INVENTORY.H"

void Func_08077428(s32 owner);

s32 Inventory_Remove(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv;
    u16 entry;
    s32 result = -1;

    inv = OwnerState_Get(owner);
    entry = inv->inventory[slot];
    if (entry != 0) {
        s32 quantity = entry & 0xf800;

        if (quantity != 0) {
            inv->inventory[slot] = entry - 0x800;
            result = 1;
        } else {
            u16 *base;
            u16 *src;
            u16 *dst;
            u16 *fill;
            s32 count;
            s32 i;

            base = inv->inventory;
            inv->inventory[slot] = quantity;
            src = base;
            count = 0;
            dst = base;
            for (i = 14; i >= 0; i--) {
                if (*src != 0) {
                    *dst++ = *src;
                    count++;
                }
                src++;
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
