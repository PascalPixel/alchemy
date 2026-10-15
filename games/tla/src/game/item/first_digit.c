#include "types.h"

s32 Func_08128228(u8 *s)
{
    s32 i;

    for (i = 0; i <= 13; i++)
    {
        if ((u8)(s[i] - 49) <= 8)
            return s[i] - 49;
    }
    return 9;
}
