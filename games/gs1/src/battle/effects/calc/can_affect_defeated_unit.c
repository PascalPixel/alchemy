#include "types.h"
#include "scene.h"
#include "abi/battle/effects/calc/can_affect_defeated_unit.h"

s32 BattleFx_CanAffectDefeatedUnit(s32 effect)
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
