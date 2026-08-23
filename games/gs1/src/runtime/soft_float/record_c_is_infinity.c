#include "types.h"

#define SoftFloatRecordCIsInfinity Func_02005de4

s32 SoftFloatRecordCIsInfinity(s32 *state)
{
    return *state == 4;
}
