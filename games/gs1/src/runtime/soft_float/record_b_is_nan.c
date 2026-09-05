#include "types.h"

#define SoftFloatRecordBIsNaN Func_02005bd8
s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}
