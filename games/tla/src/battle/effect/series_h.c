#include "types.h"

struct BattleEffectArgument;
void Func_08166b10(struct BattleEffectArgument *effect, s32 mode);

void Func_08166af8(struct BattleEffectArgument *effect) {
  Func_08166b10(effect, 0);
}

void Func_08166b04(struct BattleEffectArgument *effect) {
  Func_08166b10(effect, 1);
}
