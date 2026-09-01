#include "types.h"

void Func_080030f8(s32 frames);

extern u16 Data_02002090[];

#define BattlePresentation_ConfigureEffectDisplay Func_080c9048

void BattlePresentation_ConfigureEffectDisplay(void)
{
    s32 saved_interrupt_master;
    s32 index;

    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;

    {
        volatile u16 *interrupt_master = (volatile u16 *)0x04000208;

        saved_interrupt_master = *interrupt_master;
        *interrupt_master = (u16)interrupt_master;
    }
    index = *Data_02002090;
    if (index <= 31) {
        u32 *entry = (u32 *)((u8 *)Data_02002090 + index * 12);

        *Data_02002090 = index + 1;
        entry++;
        *entry++ = 0x7741;
        *entry++ = 0x04000000;
        *entry = 0x00020000;
    }
    *(volatile u16 *)0x04000208 = saved_interrupt_master;
    Func_080030f8(1);
}
