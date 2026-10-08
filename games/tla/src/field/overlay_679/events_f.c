#include "local.h"

void Func_02000f98(void)
{
    s32 priority;

    Func_02001c0c(1, 0, Func_02000ef8);
    priority = 0xc80;
    /* FAKEMATCH: the native builds the priority before loading the callback. */
    __asm__ volatile("" : "+r"(priority));
    Scheduler_AddOrUpdateCallback(Func_02000f58, priority);
}
