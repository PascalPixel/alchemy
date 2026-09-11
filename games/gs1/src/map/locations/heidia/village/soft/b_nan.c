#include "types.h"

s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}
