#include "types.h"

#define BattleEffectB Func_080d91dc
#define BattleEffectB2 Func_080d91ac

s32 BattleEffectB(s32, s32);

void BattleEffectB2(s32 effect) {
    BattleEffectB(effect, 2);
}
