#include "types.h"

#define SceneState_IsStateAtMostOne Func_0200142c

s32 SceneState_IsStateAtMostOne(u32 *state)
{
    return *state <= 1;
}
