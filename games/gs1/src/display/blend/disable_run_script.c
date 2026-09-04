#include "types.h"

#define DisplayBlend_DisableRunScript Func_08011af0

s32 Func_0800439c(u32 value);
void DisplayBlend_RunScript(void);

void DisplayBlend_DisableRunScript(void)
{
    Func_0800439c((u32)DisplayBlend_RunScript);
}
