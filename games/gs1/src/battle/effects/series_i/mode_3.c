#include "types.h"

#define BattleEffectI3 Func_080dbc00
#define BattleEffectI Func_080dbc30
s32 BattleEffectI(s32, s32);

void BattleEffectI3(s32 effect) {
    BattleEffectI(effect, 3);
}
