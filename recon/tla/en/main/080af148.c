#include "INVENTORY.H"

s32 Inventory_Remove(s32 owner, s32 slot)
{
    struct OwnerInventoryState *inv = Owner_GetState(owner);
    u16 item = inv->inventory[slot];
    s32 result = -1;

    if (item != 0) {
        if (item & 0xf800) {
            inv->inventory[slot] = item - 0x800;
            result = 1;
        } else {
            s16 *list;
            s32 count;
            s32 i;

            inv->inventory[slot] = 0;
            list = (s16 *)inv->inventory;
            count = 0;
            for (i = 0; i < 15; i++) {
                if (list[i] != 0)
                    list[count++] = list[i];
            }
            for (; count < 15; count++)
                list[count] = 0;
            result = 2;
        }
    }
    Owner_RecalculateStats(owner);
    return result;
}
