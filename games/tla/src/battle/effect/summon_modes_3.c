#include "types.h"
#include "battle_effect_work.h"

void Func_0818a220(struct BattleEffectArgument *effect, s32 mode);

void Func_0818a1f0(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 0);
}

void Func_0818a1fc(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 1);
}

void Func_0818a208(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 2);
}

void Func_0818a214(struct BattleEffectArgument *effect) {
  Func_0818a220(effect, 3);
}
