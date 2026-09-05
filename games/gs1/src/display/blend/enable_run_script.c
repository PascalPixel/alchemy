#include "types.h"

#define DisplayBlend_EnableRunScript Func_08011ae0

s32 Func_080042c8(u32 value);
void DisplayBlend_RunScript(void);

void DisplayBlend_EnableRunScript(void)
{
    Func_080042c8((u32)DisplayBlend_RunScript);
}
