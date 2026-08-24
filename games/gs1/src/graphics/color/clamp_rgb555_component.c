#include "types.h"

s32 Graphics_ClampRgb555Component(s32 arg0)
{
    if (arg0 > 31744)
        arg0 = 31744;
    return arg0;
}
