#include "types.h"

void Graphics_ClearBg0Vofs(void)
{
    u32 zero = 0;

    *(volatile u16 *)0x04000012 = zero;
}
