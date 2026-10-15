#include "local.h"

void Func_0200255c(void)
{
    s32 none = -1;
    s32 priority;

    gData_02006710 = none;
    priority = 0x480;
    Scheduler_AddOrUpdateCallback(Func_020024ac, priority);
}
