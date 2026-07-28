#include "types.h"

void Func_08003810(void)
{
    u32 keyInterruptMask;
    volatile u16 *keyControl;
    s32 enabled;

    if (*(volatile u16 *)0x02002000 == 0) {
        keyInterruptMask = 0xC3FF;
        *(keyControl = (volatile u16 *)0x04000132) = keyInterruptMask;
        *(volatile s8 *)0x03001CB8 = (enabled = 1);
    }
}
