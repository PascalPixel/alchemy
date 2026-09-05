#include "types.h"

#define SoftFloatRecordCIsNaN Func_02005dd4
s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}
