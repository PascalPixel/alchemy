#include "BATTLE_RUNTIME.H"
#include "ITEM.H"
#include "TYPES.H"
#include "SCENE.H"
s32 Item_CanOwnerEquip(s32, s32);

s32 Item_ClassifyUseAbility(s32 owner, s32 item)
{
    s32 ret;
    struct ItemDefinition *definition;

    if (item == 0) {
        return 1;
    }
    definition = Item_Get(item);
    ret = 1;
    if (definition->use_type == 3) {
        return ret;
    }
    if (definition->action_id == 0) {
        return ret;
    }
    if ((definition->type != 0) && (Item_CanOwnerEquip(owner, item) == 0)) {
        return ret;
    }
    if ((0x80 & BattleAction_Get(definition->action_id)->target_flags) == 0) {
        return 2;
    }
    return 0;
}
