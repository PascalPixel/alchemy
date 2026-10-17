#include "types.h"

struct BattleEffectArgument;
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
void BattleFx_RunRevive(struct BattleEffectArgument *effect) {
  Func_08144b14(effect, 0);
}

// Abilities: Dew (kind 209).
void BattleFx_RunDew(struct BattleEffectArgument *effect) {
  Func_08144b14(effect, 1);
}

// Abilities: Spark (kind 255).
void BattleFx_RunSpark(struct BattleEffectArgument *effect) {
  Func_08144b14(effect, 2);
}
