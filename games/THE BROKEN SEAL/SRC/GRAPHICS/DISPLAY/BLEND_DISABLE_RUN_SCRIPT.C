#include "TYPES.H"

s32 Scheduler_DisableCallbacks(u32 value);
void DisplayBlend_RunScript(void);

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks((u32)DisplayBlend_RunScript);
}
