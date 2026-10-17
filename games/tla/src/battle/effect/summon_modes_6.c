#include "types.h"
#include "battle_effect_work.h"

void Func_0818c7b8(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Human Hunt (kind 437).
void BattleFx_RunHumanHunt(struct BattleEffectArgument *effect) {
  Func_0818c7b8(effect, 0);
}

// Abilities: Echo Cut (kind 390).
void BattleFx_RunEchoCut(struct BattleEffectArgument *effect) {
  Func_0818c7b8(effect, 1);
}
