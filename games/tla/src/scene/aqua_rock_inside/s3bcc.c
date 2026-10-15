#include "local.h"

s32 Func_02003bcc(void)
{
    return VENT_WORK->counter;
}

s32 Func_02003bd8(s32 limit)
{
    struct VentWork *work = VENT_WORK;

    if (GameFlag_Test(0x104) == 0)
    {
        if (++work->counter >= limit)
            work->counter = 0;
    }
    return work->counter;
}
