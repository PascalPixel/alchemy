#include "types.h"

#define Graphics_ClearBg0Vofs Func_080c0298

void Graphics_ClearBg0Vofs(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}
