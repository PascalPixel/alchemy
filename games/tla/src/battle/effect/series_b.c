#include "types.h"

struct BattleEffectArgument;
void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

void Func_081445f8(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 0);
}

void Func_08144604(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 1);
}

void Func_08144610(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 2);
}

void Func_0814461c(struct BattleEffectArgument *effect) {
  Func_08144628(effect, 3);
}
