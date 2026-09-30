#include "TYPES.H"

s32 Graphics_ClampRgb555Channel(s32 val)
{
    if (val > 31)
        return 31;
    if (val < 0)
        val = 0;
    return val;
}

s32 Graphics_ClampRgb555Component(s32 val)
{
    if (val > 31744)
        val = 31744;
    return val;
}
