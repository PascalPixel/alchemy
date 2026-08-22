#include "event_runtime.h"

u32 Func_080901c0(s32 first, s32 second);

void Func_08091df4(void)
{
    struct EventRuntime *runtime = Data_03001ebc;

    Func_080901c0(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}
