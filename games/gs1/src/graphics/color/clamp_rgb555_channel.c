#include "types.h"

s32 Graphics_ClampRgb555Channel(s32 arg0)
{
    if (arg0 > 31)
        return 31;
    if (arg0 < 0)
        arg0 = 0;
    return arg0;
}
