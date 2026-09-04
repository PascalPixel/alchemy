#include "types.h"

#define SceneState_IsStateAtMostOneB Func_02001544

s32 SceneState_IsStateAtMostOneB(u32 *state)
{
    return *state <= 1;
}
