#include "TYPES.H"
#include "SCENE.H"

s32 *Runtime_ClearWordIfSet(s32 *value)
{
    if (*value != 0) {
        *value = 0;
    }
    return value;
}
