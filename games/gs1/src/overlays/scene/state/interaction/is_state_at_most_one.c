#include "types.h"
#include "scene.h"

s32 State_IsStateAtMostOne(u32 *state)
{
    return *state <= 1;
}
