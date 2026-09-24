/* Draft, not exact (2026-09-24): 22 differing halfwords, 132 of 132 bytes.
   The quantity test, the -0x800 halfword pool constant and the zero fill
   now match (a literal 0 fill pools its halfword zero as the ROM does).
   Residual: the reference computes the slot base into r6 before storing
   the cleared slot and loads items with ldrh; here the base reuses r0 and
   the items load with ldrsh, which renumbers the compaction loop. */
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

            inv->inventory[slot] = quantity;
            base = inv->inventory;
            src = base;
            count = 0;
            dst = base;
            for (i = 14; i >= 0; i--) {
                u16 item = *src++;

                if (item != 0) {
                    *dst++ = item;
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
