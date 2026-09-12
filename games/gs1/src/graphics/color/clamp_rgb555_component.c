#include "types.h"

s32 Graphics_ClampRgb555Component(s32 val)
{
    if (val > 31744)
        val = 31744;
    return val;
}
