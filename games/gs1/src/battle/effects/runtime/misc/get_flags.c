#include "battle_effect_runtime.h"

struct BattleEffectEntry *BattleEffect_FindDefinition(u32 id);
s32 BattleEffect_GetAnimationValue(void);

u8 BattleEffect_GetFlags(void)
{
    return BattleEffect_FindDefinition(BattleEffect_GetAnimationValue())->flags;
}
