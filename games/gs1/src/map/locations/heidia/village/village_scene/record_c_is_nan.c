#include "types.h"

s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}
