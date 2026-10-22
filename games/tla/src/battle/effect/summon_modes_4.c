#include "types.h"
#include "battle_effect_work.h"

void Func_0818ad40(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sleep Card (kind 92).
DEFINE_EFFECT_KIND(BattleEffect_RunSleepCard, Func_0818ad40, 0)

/// Abilities: Baffle Card (kind 90).
DEFINE_EFFECT_KIND(BattleEffect_RunBaffleCard, Func_0818ad40, 1)

/// Abilities: Death Card (kind 93).
DEFINE_EFFECT_KIND(BattleEffect_RunDeathCard, Func_0818ad40, 2)

/// Abilities: Sword Card (kind 91).
DEFINE_EFFECT_KIND(BattleEffect_RunSwordCard, Func_0818ad40, 3)
