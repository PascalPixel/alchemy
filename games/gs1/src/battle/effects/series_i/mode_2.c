#include "types.h"

#define BattleEffectI Func_080dbc30
#define BattleEffectI2 Func_080dbbe8

s32 BattleEffectI(s32, s32);

void BattleEffectI2(s32 effect) {
    BattleEffectI(effect, 2);
}
