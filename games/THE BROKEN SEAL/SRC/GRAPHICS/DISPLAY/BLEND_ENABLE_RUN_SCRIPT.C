#include "TYPES.H"

s32 Func_080042c8(u32 value);
#define Scheduler_EnableCallbacks Func_080042c8
void DisplayBlend_RunScript(void);

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks((u32)DisplayBlend_RunScript);
}
