#include "types.h"

struct BattleEffectArgument;

void Func_0816aeec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dreamtide (kind 106).
void BattleEffect_RunDreamtide(s32 a0) { Func_0816aeec(a0, 0); }

/// Abilities: Watery Grave (kind 391).
void BattleEffect_RunWateryGrave(s32 a0) { Func_0816aeec(a0, 1); }
