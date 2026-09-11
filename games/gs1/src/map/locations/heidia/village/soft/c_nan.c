#include "types.h"
#include "scene.h"

s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}
