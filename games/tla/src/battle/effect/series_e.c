#include "types.h"
#include "battle_effect_argument.h"

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dragon Driver (kind 385).
DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1)

/// Abilities: Tiamat (scene kind 7).
DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0)
