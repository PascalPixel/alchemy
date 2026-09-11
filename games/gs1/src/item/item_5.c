#include "scene.h"
#include "item.h"
#include "owner_state.h"

/* item/get_definition.c */
extern struct ItemDefinition gRom[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return gRom + (item_id & 0x1ff);
}

/* item/can_owner_equip.c */
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

/* item/is_compatible_with_owner.c */
s32 Item_IsCompatibleWithOwner(s32 owner_id, s32 item_id)
{
    if (Item_GetEquipmentGroup(item_id) == 0) {
        return 1;
    }
    return Item_CanOwnerEquipDirect(owner_id, item_id);
}

/* item/get_equipment_group.c */
s32 Item_GetEquipmentGroup(s32 item_id)
{
    s32 group;
    struct ItemDefinition *item;

    item = Item_GetDirect(item_id);
    group = 0;
    if (item->type == 1) {
        group = 1;
        goto end;
    }
    if (item->type == 2) {
        goto two;
    }
    if (item->type == 3) {
        goto two;
    }
    if (item->type == 4) {
        goto two;
    }
    if (item->type == 5) {
        goto two;
    }
    if (item->type == 9) {
        goto two;
    }
    goto end;
two:
    group = 2;
end:
    return group;
}
