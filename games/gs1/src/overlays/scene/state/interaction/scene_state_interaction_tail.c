#include "types.h"

#define SceneState_IsStateAtMostOneB Func_02001544
#define SceneState_IsStateAtMostOneBB Func_02001740
s32 SceneState_IsStateAtMostOneB(u32 *state)
{
    return *state <= 1;
}

s32 SceneState_IsStateAtMostOneBB(u32 *state)
{
    s32 ret = 0;

    if (*state <= 1) {
        ret = 1;
    }
    return ret;
}
