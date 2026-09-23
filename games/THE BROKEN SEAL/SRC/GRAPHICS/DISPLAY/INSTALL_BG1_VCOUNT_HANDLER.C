#include "TYPES.H"

typedef void (*InterruptHandler)(void);

void Func_0800307c(s32 index, s32 priority, InterruptHandler handler);
void Graphics_ClearBg1ControlBit2(void);

/* Clears BG0's vertical offset and installs Graphics_ClearBg1ControlBit2 as
   the V-count interrupt at line 136. */
void Graphics_InstallBg1VCountHandler(void)
{
    u32 address = 0x04000012;
    s32 zero = 0;

    *(volatile u16 *)address = zero;
    /* FAKEMATCH: the do-while keeps the handler load ahead of the index. */
    do { address = (u32)Graphics_ClearBg1ControlBit2; } while (0);
    Func_0800307c(2, 136, (InterruptHandler)address);
}
