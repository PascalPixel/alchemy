#include "types.h"

#define BattleEffectH Func_080db6e0
#define BattleEffectH1 Func_080db6c8

s32 BattleEffectH(s32, s32);

void BattleEffectH1(s32 effect) {
    BattleEffectH(effect, 1);
}
