#include "types.h"

s32 Scheduler_EnableCallbacks();
void Func_080d1840();

void Func_080d1ac4(void)
{
    Scheduler_EnableCallbacks(Func_080d1840);
}

s32 Scheduler_DisableCallbacks();

void Func_080d1ad4(void)
{
    Scheduler_DisableCallbacks(Func_080d1840);
}
