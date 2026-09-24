#include "TYPES.H"

s32 Scheduler_EnableCallbacks(u32 value);
void DisplayBlend_RunScript(void);

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks((u32)DisplayBlend_RunScript);
}
