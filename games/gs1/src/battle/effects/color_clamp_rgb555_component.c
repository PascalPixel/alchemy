#include "types.h"

s32 BattleFx_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}
