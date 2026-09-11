#include "scene.h"
#include "battle_effect_runtime.h"

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);
s32 BattleFx_GetAnimationValue(void);

u8 BattleFx_GetFlags(void)
{
    return BattleFx_FindDefinition(BattleFx_GetAnimationValue())->flags;
}
