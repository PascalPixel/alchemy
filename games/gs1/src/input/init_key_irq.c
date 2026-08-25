#include "types.h"
#include "global_cells.h"

#define Input_InitKeyIrq Func_08003810

void Input_InitKeyIrq(void)
{
    u32 keyInterruptMask;
    volatile u16 *keyControl;
    s32 enabled;

    if (*(volatile u16 *)0x02002000 == 0) {
        keyInterruptMask = 0xC3FF;
        *(keyControl = (volatile u16 *)0x04000132) = keyInterruptMask;
        *(volatile s8 *)ADDR_03001CB8 = (enabled = 1);
    }
}
