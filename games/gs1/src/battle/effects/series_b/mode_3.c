#include "types.h"

#define BattleEffectB Func_080d91dc
#define BattleEffectB3 Func_080d91b8

s32 BattleEffectB(s32, s32);

void BattleEffectB3(s32 effect) {
    BattleEffectB(effect, 3);
}
