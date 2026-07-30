#include "types.h"

#define FIELD(base, type, offset) (*(volatile type *)((u8 *)(base) + (offset)))

extern u16 Data_02002090[];

void Func_080030f8(u32);

void Func_080c9048(void)
{
    u32 saved_interrupt_master;
    s32 index;
    void *cursor;
    volatile s16 *middle;
    volatile u16 *interrupt_master;
    u16 *queue;

    FIELD((void *)0x04000050, s16, 0) = 0;
    FIELD((void *)0x04000050, s16, 2) = 0x100e;
    FIELD((void *)0x04000040, s16, 0) = 0x00f0;
    FIELD((void *)0x04000040, s16, 4) = 0x1088;
    cursor = (u8 *)0x04000040 + 2;
    middle = cursor;
    *middle = 0x00f0;
    middle += 2;
    *middle = 0x1088;

    queue = Data_02002090;
    FIELD((void *)0x04000048, s16, 0) = 0x3537;
    FIELD((void *)0x04000048, s16, 2) = 0x3f21;

    interrupt_master = (volatile u16 *)0x04000208;
    saved_interrupt_master = *interrupt_master;
    *interrupt_master = (u16)(u32)interrupt_master;
    index = *queue;
    if (index <= 31) {
        volatile u32 *entry =
            (volatile u32 *)((u8 *)queue + 4 + index * 12);

        *queue = index + 1;
        *entry++ = 0x7741;
        *entry++ = 0x04000000;
        *entry = 0x00020000;
    }
    *interrupt_master = saved_interrupt_master;

    Func_080030f8(1);
}
