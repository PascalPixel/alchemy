#include "item.h"

s32 Item_IsCompatibleWithOwner(s32 owner_id, s32 item_id) {
    if (Item_GetEquipmentGroup(item_id) == 0) {
        return 1;
    }
    return Item_CanOwnerEquipDirect(owner_id, item_id);
}
