#include "types.h"

#define SoftFloatRecordAIsNaN Func_02005ac0

s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}
