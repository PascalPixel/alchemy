#include "types.h"
#include "irq.h"

void Func_080d0954(void);

s32 Func_080d072c(void)
{
    s32 other = (s32)gIrqHandlers[1] ^ (s32)Func_080d0954;

    return 1 - ((u32)(-other | other) >> 31);
}
