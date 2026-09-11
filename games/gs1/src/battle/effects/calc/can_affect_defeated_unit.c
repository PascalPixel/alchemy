#include "types.h"

#define BattleEffect_CanAffectDefeatedUnit Func_080bbae8

s32 BattleEffect_CanAffectDefeatedUnit(s32 effect)
{
    switch (effect) {
    case 31:
    case 32:
    case 60:
    case 69:
        return 1;
    default:
        return 0;
    }
}
