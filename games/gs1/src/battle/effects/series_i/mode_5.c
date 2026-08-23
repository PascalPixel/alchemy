#include "types.h"

#define BattleEffectI Func_080dbc30
#define BattleEffectI5 Func_080dbc0c

s32 BattleEffectI(s32, s32);

void BattleEffectI5(s32 effect) {
    BattleEffectI(effect, 5);
}
