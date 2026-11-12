#include "types.h"

struct BattleEffectArgument;

void Func_0815c970(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Poison Beat (kind 382).
void BattleEffect_RunPoisonBeat(void *object) { Func_0815c970(object, 0); }

/// Abilities: Spinning Beat (kind 383).
void BattleEffect_RunSpinningBeat(void *object) { Func_0815c970(object, 1); }
