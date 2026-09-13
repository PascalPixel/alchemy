#include "item.h"

extern struct ItemDefinition Data_0807b6a8[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return Data_0807b6a8 + (item_id & 0x1ff);
}
#include "owner_state.h"

s32 Item_CanOwnerEquipDirect(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *owner = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    u32 class_id = owner->class_id;
    s32 result = item->equip_mask;

    if (class_id > 7) {
        result = 0;
    } else {
        result >>= owner->class_id;
        result &= 1;
    }
    return result;
}

s32 Item_IsCompatibleWithOwner(s32 owner_id, s32 item_id)
{
    if (Item_GetEquipmentGroup(item_id) == 0) {
        return 1;
    }
    return Item_CanOwnerEquipDirect(owner_id, item_id);
}
