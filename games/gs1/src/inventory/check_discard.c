#include "inventory.h"

s32 Inventory_CheckDiscard(s32 owner, s32 slot) {
    struct OwnerInventoryState *inv = OwnerState_Get(owner);
    s32 item_id = inv->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirect(item_id);

    if (item_id == 0) {
        return -1;
    }
    if ((item->flags & 8) != 0) {
        return -4;
    }
    if ((inv->inventory[slot] & 0x200) != 0 &&
        (item->flags & 2) != 0) {
        return -3;
    }
    return 0;
}
