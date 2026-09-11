#include "types.h"
#include "scene.h"

s32 State_IsStateFour(s32 *state)
{
    s32 ret = 0;

    if (*state == 4) {
        ret = 1;
    }
    return ret;
}
