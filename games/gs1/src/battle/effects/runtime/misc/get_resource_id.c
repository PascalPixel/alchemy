#include "scene.h"
#include "abi/battle/effects/runtime/misc/get_resource_id.h"
#include "battle_effect_runtime.h"

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);

s32 BattleFx_GetResourceId(u32 id)
{
    u8 value;

    if (gCell.enabled_20a == 0 ||
        (value = BattleFx_FindDefinition(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}
