#include "types.h"
#include "battle_effect_work.h"

void Func_0818bc98(struct BattleEffectArgument *effect, s32 mode);

void Func_0818bc60(struct BattleEffectArgument *effect) {
  Func_0818bc98(effect, 0);
}

void Func_0818bc6c(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 1);
}

void Func_0818bc7c(struct BattleEffectArgument *effect) {
  effect->variant = 2;
  Func_0818bc98(effect, 2);
}

void Func_0818bc8c(struct BattleEffectArgument *effect) {
  Func_0818bc98(effect, 3);
}
