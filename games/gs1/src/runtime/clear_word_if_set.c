#include "types.h"

#define Runtime_ClearWordIfSet Func_080197b4

s32 *Runtime_ClearWordIfSet(s32 *value)
{
    if (*value != 0) {
        *value = 0;
    }
    return value;
}
