#include "types.h"

struct BattleEffectArgument;

void Func_0816fca8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Earth Force (kind 459).
void BattleEffect_RunEarthForce(s32 a0) { Func_0816fca8(a0, 1); }

/// Abilities: Thunder Mine (kind 86).
void BattleEffect_RunThunderMine(s32 a0) { Func_0816fca8(a0, 0); }
