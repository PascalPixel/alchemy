#include "types.h"

#define BattleEffectI Func_080dbc30
#define BattleEffectI4 Func_080dbc24

s32 BattleEffectI(s32, s32);

void BattleEffectI4(s32 effect) {
    BattleEffectI(effect, 4);
}
