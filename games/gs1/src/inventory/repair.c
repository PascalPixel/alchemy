#include "inventory.h"

s32 Inventory_Repair(s32 owner, s32 slot) {
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    if (inv->inventory[slot] == 0) {
        return -1;
    }
    inv->inventory[slot] &= ~0x400;
    return 0;
}
