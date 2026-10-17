#include "types.h"

struct BattleEffectArgument;
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

// Abilities: Dragon Driver (kind 385).
void BattleFx_RunDragonDriver(struct BattleEffectArgument *effect) {
  Func_08147aec(effect, 1);
}

// Abilities: Tiamat (scene kind 7).
void BattleFx_RunTiamat(struct BattleEffectArgument *effect) {
  Func_08147aec(effect, 0);
}
