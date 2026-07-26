#include "types.h"

s32 Func_080f3898(s32 arg0)
{
    if (arg0 > 31)
        return 31;
    if (arg0 < 0)
        arg0 = 0;
    return arg0;
}
