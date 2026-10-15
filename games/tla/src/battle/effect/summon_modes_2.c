#include "types.h"
#include "battle_effect_work.h"

void Func_081823a8(struct BattleEffectArgument *effect, s32 mode);

void Func_08182390(struct BattleEffectArgument *effect) {
  Func_081823a8(effect, 0);
}

void Func_0818239c(struct BattleEffectArgument *effect) {
  Func_081823a8(effect, 1);
}
