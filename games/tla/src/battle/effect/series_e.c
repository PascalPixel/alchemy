#include "types.h"

struct BattleEffectArgument;
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

void Func_08147ad4(struct BattleEffectArgument *effect) {
  Func_08147aec(effect, 1);
}

void Func_08147ae0(struct BattleEffectArgument *effect) {
  Func_08147aec(effect, 0);
}
