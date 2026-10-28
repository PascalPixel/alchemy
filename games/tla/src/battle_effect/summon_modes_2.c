#include "types.h"
#include "battle_effect_work.h"

void Func_081823a8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Djinn Stun (kind 465).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnStun, Func_081823a8, 0)

/// Abilities: Mystic Call (kind 466).
DEFINE_EFFECT_KIND(BattleEffect_RunMysticCall, Func_081823a8, 1)
