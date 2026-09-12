#include "types.h"

#define SoftFloatRecordAIsNaN Func_02005ac0
#define SoftFloatRecordAIsInfinity Func_02005ad0
#define SoftFloatRecordAIsZero Func_02005ae0
#define SoftFloatRecordBIsNaN Func_02005bd8
#define SoftFloatRecordBIsInfinity Func_02005be8
#define SoftFloatRecordBIsZero Func_02005bf8
#define SoftFloatRecordCIsNaN Func_02005dd4
#define SoftFloatRecordCIsInfinity Func_02005de4
#define SoftFloatRecordCIsZero Func_02005df4
s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}

s32 SoftFloatRecordAIsInfinity(s32 *state)
{
    return *state == 4;
}

s32 SoftFloatRecordAIsZero(s32 *state)
{
    s32 match = 0;

    if (*state == 2) {
        match = 1;
    }
    return match;
}

s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}

s32 SoftFloatRecordBIsInfinity(s32 *state)
{
    return *state == 4;
}

s32 SoftFloatRecordBIsZero(s32 *state)
{
    return *state == 2;
}

s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}

s32 SoftFloatRecordCIsInfinity(s32 *state)
{
    return *state == 4;
}

s32 SoftFloatRecordCIsZero(s32 *state)
{
    return *state == 2;
}
