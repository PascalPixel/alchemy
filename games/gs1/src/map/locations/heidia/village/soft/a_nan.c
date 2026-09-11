#include "types.h"
#include "scene.h"

s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}
