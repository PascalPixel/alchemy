#include "INVENTORY.H"

void Inventory_AddAndEquip(s32 owner, s32 target)
{
    struct BattleUnit *state = Owner_GetState(owner);
    u16 *entry;
    s32 index;

    Inventory_AddItem(owner, target);
    index = 0;
    entry = state->inventory;
    do {
        if (*entry++ == target)
            Inventory_Equip(owner, index);
        index++;
    } while (index < INVENTORY_SLOTS);
}
