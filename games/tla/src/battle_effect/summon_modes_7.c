#include "types.h"
#include "battle_effect_work.h"

void Func_0818d5a8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Planet Diver (kind 60).
DEFINE_EFFECT_KIND(BattleEffect_RunPlanetDiver, Func_0818d5a8, 0)

/// Abilities: Planetary (kind 128).
DEFINE_EFFECT_KIND(BattleEffect_RunPlanetary, Func_0818d5a8, 1)
