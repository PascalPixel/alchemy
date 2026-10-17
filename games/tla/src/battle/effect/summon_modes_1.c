#include "types.h"
#include "battle_effect_work.h"

void Func_08181ed4(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Purgatory (kind 114).
void BattleFx_RunPurgatory(struct BattleEffectArgument *effect) {
  Func_08181ed4(effect, 0);
}

// Abilities: Djinn Blast (kind 461).
void BattleFx_RunDjinnBlast(struct BattleEffectArgument *effect) {
  Func_08181ed4(effect, 2);
}

// Abilities: Djinn Storm (kind 462).
void BattleFx_RunDjinnStorm(struct BattleEffectArgument *effect) {
  Func_08181ed4(effect, 1);
}
