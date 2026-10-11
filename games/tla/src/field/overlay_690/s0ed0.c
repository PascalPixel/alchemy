#include "local.h"

void Func_02000ed0(void)
{
    s32 priority;

    Func_02001fa8(1, 0, Func_02000e30);
    priority = 0xc80;
    /* FAKEMATCH: the native builds the priority before loading the callback. */
    __asm__ volatile("" : "+r"(priority));
    Scheduler_AddOrUpdateCallback(Func_02000e90, priority);
}
