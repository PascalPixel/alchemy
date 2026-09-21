#include "TYPES.H"

s32 Func_0800439c(u32 value);
#define Scheduler_DisableCallbacks Func_0800439c
void DisplayBlend_RunScript(void);

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks((u32)DisplayBlend_RunScript);
}
