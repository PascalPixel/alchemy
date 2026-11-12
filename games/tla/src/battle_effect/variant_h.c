#include "types.h"

struct BattleEffectArgument;

void Func_08173588(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Rime (kind 240).
void BattleEffect_RunRime(s32 a0) { Func_08173588(a0, 0); }

/// Abilities: Haze (kind 238).
void BattleEffect_RunHaze(s32 a0) { Func_08173588(a0, 1); }
