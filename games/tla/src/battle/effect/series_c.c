#include "types.h"
#include "battle_effect_argument.h"

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
DEFINE_EFFECT_KIND(BattleFx_RunRevive, Func_08144b14, 0)

// Abilities: Dew (kind 209).
DEFINE_EFFECT_KIND(BattleFx_RunDew, Func_08144b14, 1)

// Abilities: Spark (kind 255).
DEFINE_EFFECT_KIND(BattleFx_RunSpark, Func_08144b14, 2)
