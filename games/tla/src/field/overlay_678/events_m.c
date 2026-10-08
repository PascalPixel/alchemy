#include "local.h"

void Func_02002778(s32 i, s32 id)
{
    struct VentWork *work = VENT_WORK;

    if ((u32)i <= 3)
        work->ids[i] = id;
}
