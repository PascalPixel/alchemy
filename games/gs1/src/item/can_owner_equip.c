#include "item.h"
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
