#include "types.h"

#define SoftFloatRecordAIsInfinity Func_02005ad0
s32 SoftFloatRecordAIsInfinity(s32 *state)
{
    return *state == 4;
}
