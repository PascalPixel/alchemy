#include "types.h"

#define BattleEffectB Func_080d91dc
#define BattleEffectB4 Func_080d91d0

s32 BattleEffectB(s32, s32);

void BattleEffectB4(s32 effect) {
    BattleEffectB(effect, 4);
}
