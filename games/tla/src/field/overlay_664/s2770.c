#include "ov664.h"

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_020026c0(void);

void Func_02002770(void)
{
    s32 value = -1;

    Data_02006c0c = value;
    {
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load. */
        s32 priority = 0x480;
        Scheduler_AddOrUpdateCallback(Func_020026c0, priority);
    }
}
