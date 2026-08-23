#include "types.h"

#define SoftFloatRecordAIsZero Func_02005ae0

s32 SoftFloatRecordAIsZero(s32 *state)
{
    s32 match = 0;

    if (*state == 2) {
        match = 1;
    }
    return match;
}
