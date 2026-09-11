#include "types.h"

s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}
