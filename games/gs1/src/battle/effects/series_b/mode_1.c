#include "types.h"

#define BattleEffectB1 Func_080d91a0
#define BattleEffectB Func_080d91dc
s32 BattleEffectB(s32, s32);

void BattleEffectB1(s32 effect) {
    BattleEffectB(effect, 1);
}
