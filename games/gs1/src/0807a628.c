#include "inventory.h"

void Func_0807a628(s32 owner, s32 target)
{
    s32 state = (s32)OwnerState_Get(owner);
    u16 *entry;
    s32 index;

    Inventory_Add(owner, target);
    index = 0;
    entry = (u16 *)(state + 0xd8);
    do {
        if (*entry++ == target)
            Inventory_Equip(owner, index);
        index++;
    } while (index <= 14);
}
