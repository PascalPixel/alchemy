#include "ride.h"

void Func_02002860(void);

s32 Func_020029a0(void)
{
    if (RIDE_WORK->active) {
        Func_02002860();
        return 1;
    }
    return 0;
}

s32 Func_020029bc(void)
{
    return RIDE_WORK->active;
}
