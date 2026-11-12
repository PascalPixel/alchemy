#include "types.h"

struct BattleEffectArgument;

void Func_0816e65c(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Heavy Divide (kind 101).
void BattleEffect_RunHeavyDivide(s32 a0) { Func_0816e65c(a0, 0); }

/// Abilities: Strong Hit (kind 469).
void BattleEffect_RunStrongHit(s32 a0) { Func_0816e65c(a0, 1); }
