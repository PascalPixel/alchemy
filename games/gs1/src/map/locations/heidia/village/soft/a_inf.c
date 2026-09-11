#include "types.h"
#include "scene.h"

s32 SoftFloatRecordAIsInfinity(s32 *state)
{
    return *state == 4;
}
