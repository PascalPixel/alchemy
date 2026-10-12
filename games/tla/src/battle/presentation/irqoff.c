#include "types.h"
#include "irq.h"

void Func_08126218(s32 a, s32 b);

void Func_0812756c(void)
{
    Func_08126218(0, 0);
}

void Func_08127578(void)
{
    Runtime_SetIrqHandler(2, 0, 0);
}
