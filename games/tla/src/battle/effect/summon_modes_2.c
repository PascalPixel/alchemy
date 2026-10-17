#include "types.h"
#include "battle_effect_work.h"

void Func_081823a8(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Djinn Stun (kind 465).
void BattleFx_RunDjinnStun(struct BattleEffectArgument *effect) {
  Func_081823a8(effect, 0);
}

// Abilities: Mystic Call (kind 466).
void BattleFx_RunMysticCall(struct BattleEffectArgument *effect) {
  Func_081823a8(effect, 1);
}
