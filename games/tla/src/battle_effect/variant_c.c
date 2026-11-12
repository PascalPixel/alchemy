#include "types.h"

struct BattleEffectArgument;

void Func_0816b6ec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Waft (kind 234).
void BattleEffect_RunWaft(s32 a0) { Func_0816b6ec(a0, 0); }

/// Abilities: Aroma (kind 247).
void BattleEffect_RunAroma(s32 a0) { Func_0816b6ec(a0, 1); }
