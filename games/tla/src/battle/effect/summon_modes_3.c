#include "types.h"
#include "battle_effect_work.h"

void Func_0818a220(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Bramble Card (kind 88).
DEFINE_EFFECT_KIND(BattleFx_RunBrambleCard, Func_0818a220, 0)

/// Abilities: Frost Card (kind 87).
DEFINE_EFFECT_KIND(BattleFx_RunFrostCard, Func_0818a220, 1)

/// Abilities: Flame Card (kind 79).
DEFINE_EFFECT_KIND(BattleFx_RunFlameCard, Func_0818a220, 2)

/// Abilities: Thunder Card (kind 89).
DEFINE_EFFECT_KIND(BattleFx_RunThunderCard, Func_0818a220, 3)
