#include "types.h"
#include "battle_effect_work.h"

void Func_0818a220(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Bramble Card (kind 88).
void BattleFx_RunBrambleCard(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 0);
}

// Abilities: Frost Card (kind 87).
void BattleFx_RunFrostCard(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 1);
}

// Abilities: Flame Card (kind 79).
void BattleFx_RunFlameCard(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 2);
}

// Abilities: Thunder Card (kind 89).
void BattleFx_RunThunderCard(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 3);
}
