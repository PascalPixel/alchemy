#include "types.h"
#include "battle_effect_work.h"

void Func_0818c7b8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Human Hunt (kind 437).
DEFINE_EFFECT_KIND(BattleEffect_RunHumanHunt, Func_0818c7b8, 0)

/// Abilities: Echo Cut (kind 390).
DEFINE_EFFECT_KIND(BattleEffect_RunEchoCut, Func_0818c7b8, 1)
