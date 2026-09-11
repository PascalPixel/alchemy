#include "types.h"
#include "scene.h"

s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}
