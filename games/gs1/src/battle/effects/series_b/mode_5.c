#include "types.h"

#define BattleEffectB Func_080d91dc
#define BattleEffectB5 Func_080d91c4

s32 BattleEffectB(s32, s32);

void BattleEffectB5(s32 effect) {
    BattleEffectB(effect, 5);
}
