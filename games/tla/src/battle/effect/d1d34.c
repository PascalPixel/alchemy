#include "types.h"

s32 Scheduler_EnableCallbacks();
void Func_080d1ae4();

void Func_080d1d34(void)
{
    Scheduler_EnableCallbacks(Func_080d1ae4);
}

s32 Scheduler_DisableCallbacks();

void Func_080d1d44(void)
{
    Scheduler_DisableCallbacks(Func_080d1ae4);
}
