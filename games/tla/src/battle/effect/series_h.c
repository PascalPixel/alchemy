#include "types.h"

struct BattleEffectArgument;
void Func_08166b10(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Titan Blade (kind 61).
void BattleFx_RunTitanBlade(struct BattleEffectArgument *effect) {
  Func_08166b10(effect, 0);
}

// Abilities: Acheron's Grief (kind 104).
void BattleFx_RunAcheronsGrief(struct BattleEffectArgument *effect) {
  Func_08166b10(effect, 1);
}
