#include "types.h"
#include "battle_effect_argument.h"

void Func_08151ff0(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Smoke Bomb, Sleep Bomb (kind 380).
DEFINE_EFFECT_KIND(BattleEffect_RunSmokeBomb, Func_08151ff0, 1)

/// Abilities: Blast, Mad Blast, Fiery Blast (kind 14).
DEFINE_EFFECT_KIND(BattleEffect_RunBlast, Func_08151ff0, 0)
