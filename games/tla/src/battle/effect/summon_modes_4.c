#include "types.h"
#include "battle_effect_work.h"

void Func_0818ad40(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Sleep Card (kind 92).
void BattleFx_RunSleepCard(struct BattleEffectArgument *effect) {
  Func_0818ad40(effect, 0);
}

// Abilities: Baffle Card (kind 90).
void BattleFx_RunBaffleCard(struct BattleEffectArgument *effect) {
  Func_0818ad40(effect, 1);
}

// Abilities: Death Card (kind 93).
void BattleFx_RunDeathCard(struct BattleEffectArgument *effect) {
  Func_0818ad40(effect, 2);
}

// Abilities: Sword Card (kind 91).
void BattleFx_RunSwordCard(struct BattleEffectArgument *effect) {
  Func_0818ad40(effect, 3);
}
