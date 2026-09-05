#include "types.h"

s32 Graphics_ClampRgb555Channel(s32 val)
{
    if (val > 31)
        return 31;
    if (val < 0)
        val = 0;
    return val;
}
