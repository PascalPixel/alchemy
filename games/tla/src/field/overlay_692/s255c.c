#include "local.h"

void Func_0200255c(void)
{
    s32 none = -1;
    s32 priority;

    Data_02006710 = none;
    priority = 0x480;
    /* FAKEMATCH: the native builds the priority before loading the callback; a direct call loads the callback first. */
    __asm__ volatile("" : "+r"(priority));
    Scheduler_AddOrUpdateCallback(Func_020024ac, priority);
}
