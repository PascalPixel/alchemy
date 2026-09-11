#include "types.h"
#include "scene.h"

s32 SoftFloatRecordBIsInfinity(s32 *state)
{
    return *state == 4;
}
