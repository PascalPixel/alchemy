#include "types.h"

#define SceneState_IsStateFour Func_02001554

s32 SceneState_IsStateFour(s32 *state)
{
    s32 ret = 0;

    if (*state == 4) {
        ret = 1;
    }
    return ret;
}
