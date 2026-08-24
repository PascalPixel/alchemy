#include "types.h"

s32 BattleEffect_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}
