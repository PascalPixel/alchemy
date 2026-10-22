#include "types.h"
#include "battle_effect_work.h"

void Func_08181ed4(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Purgatory (kind 114).
DEFINE_EFFECT_KIND(BattleEffect_RunPurgatory, Func_08181ed4, 0)

/// Abilities: Djinn Blast (kind 461).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnBlast, Func_08181ed4, 2)

/// Abilities: Djinn Storm (kind 462).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnStorm, Func_08181ed4, 1)
