#include "INVENTORY.H"

void Inventory_AddAndEquip(s32 owner, s32 target)
{
    s32 state = (s32)Owner_GetState(owner);
    u16 *entry;
    s32 index;

    Inventory_AddItem(owner, target);
    index = 0;
    entry = (u16 *)(state + 0xd8);
    do {
        if (*entry++ == target)
            Inventory_Equip(owner, index);
        index++;
    } while (index <= 14);
}
