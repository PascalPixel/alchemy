#include "types.h"

struct BattleEffectArgument;

void Func_0817e698(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Spasm (kind 348).
void BattleEffect_RunSpasm(s32 a0) { Func_0817e698(a0, 0); }

/// Abilities: Berserk (kind 353).
void BattleEffect_RunBerserk(s32 a0) { Func_0817e698(a0, 1); }

/// Abilities: Recovery (kind 389).
void BattleEffect_RunRecovery(s32 a0) { Func_0817e698(a0, 2); }

/// Abilities: Sack (kind 431).
void BattleEffect_RunSack(s32 a0) { Func_0817e698(a0, 3); }
