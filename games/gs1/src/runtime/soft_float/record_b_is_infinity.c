#include "types.h"

#define SoftFloatRecordBIsInfinity Func_02005be8

s32 SoftFloatRecordBIsInfinity(s32 *state)
{
    return *state == 4;
}
