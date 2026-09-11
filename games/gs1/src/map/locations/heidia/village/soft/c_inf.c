#include "types.h"
#include "scene.h"

s32 SoftFloatRecordCIsInfinity(s32 *state)
{
    return *state == 4;
}
