#include "types.h"

#define SceneState_IsStateFourDirect Func_02001750

s32 SceneState_IsStateFourDirect(s32 *state)
{
    return *state == 4;
}
