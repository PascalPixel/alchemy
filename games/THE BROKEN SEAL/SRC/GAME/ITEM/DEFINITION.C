#include "ITEM.H"

extern struct ItemDefinition Item_DefinitionTable[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return Item_DefinitionTable + (item_id & 0x1ff);
}
#include "OWNER_STATE.H"

s32 Item_CanOwnerEquipDirect(s32 owner_id, s32 item_id)
{
    struct OwnerInventoryState *owner = OwnerState_Get(owner_id);
    struct ItemDefinition *item = Item_GetDirect(item_id);
    s32 mask = item->equip_mask;

    if (owner->class_id >= 8) {
        return 0;
    }
    return (mask >> owner->class_id) & 1;
}

s32 Item_IsCompatibleWithOwner(s32 owner_id, s32 item_id)
{
    if (Item_GetEquipmentGroup(item_id) == 0) {
        return 1;
    }
    return Item_CanOwnerEquipDirect(owner_id, item_id);
}
