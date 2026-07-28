#include "types.h"

s32 Func_08091294(s32 value)
{
    if (value > 31)
        return 31;
    if (value < 0)
        value = 0;
    return value;
}
