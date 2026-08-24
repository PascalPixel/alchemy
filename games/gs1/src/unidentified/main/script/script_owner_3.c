#include "event_runtime.h"

void Func_0808fefc(s32 first, s32 second);

void Func_08091dc8(void)
{
    struct EventRuntime *runtime = Data_03001ebc;

    Func_0808fefc(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}
