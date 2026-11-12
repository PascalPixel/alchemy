#include "types.h"

struct BattleEffectArgument;

void Func_08172b00(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Core (kind 236).
void BattleEffect_RunCore(s32 a0) { Func_08172b00(a0, 0); }

/// Abilities: Cannon (kind 246).
void BattleEffect_RunCannon(s32 a0) { Func_08172b00(a0, 1); }
