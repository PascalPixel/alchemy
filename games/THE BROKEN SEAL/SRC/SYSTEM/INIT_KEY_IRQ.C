#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
extern u8 Data_03001cb8[];

void Input_InitKeyIrq(void)
{
    u32 keyInterruptMask;
    volatile u16 *keyControl;
    s32 enabled;

    if (gSleepActive == 0) {
        keyInterruptMask = 0xC3FF;
        *(keyControl = (volatile u16 *)0x04000132) = keyInterruptMask;
        *(volatile s8 *)((u32)&Data_03001cb8) = (enabled = 1);
    }
}
