#include "types.h"

struct BattleEffectArgument;
void Func_08151ff0(struct BattleEffectArgument *effect, s32 mode);

void Func_08151fd8(struct BattleEffectArgument *effect) {
  Func_08151ff0(effect, 1);
}

void Func_08151fe4(struct BattleEffectArgument *effect) {
  Func_08151ff0(effect, 0);
}
