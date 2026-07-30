#include "types.h"

struct DmaChannel_08005d10 {
    const void *source;
    void *destination;
    u32 control;
};

void Func_0800307c(s32 interrupt, s32 priority, void *handler);
void Func_0800651c(void);

void Func_08005d10(void)
{
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    volatile u16 *interrupt_enable = (volatile u16 *)0x04000200;
    volatile u16 *interrupt_flags = (volatile u16 *)0x04000202;
    volatile u16 *serial_control = (volatile u16 *)0x04000128;
    volatile u16 *rcnt = (volatile u16 *)0x04000134;
    volatile struct DmaChannel_08005d10 *dma =
        (volatile struct DmaChannel_08005d10 *)0x040000d4;
    u8 *work = (u8 *)0x02002240;
    u32 zero = 0;
    u16 saved_master = *interrupt_master;
    s32 index;

    *interrupt_master = (u16)(u32)interrupt_master;
    Func_0800307c(7, 0, (void *)0x08006241);
    Func_0800307c(6, 0, (void *)0x08006241);
    *interrupt_master = 0;

    *interrupt_enable &= 0xff3f;
    if (*interrupt_flags & 0x80)
        *interrupt_flags = 0x80;
    if (*interrupt_flags & 0x40)
        *interrupt_flags = 0x40;

    *rcnt = 0x8000;
    *rcnt = 0;
    *(volatile u32 *)serial_control = 0x1000;
    *rcnt = 0;
    *(volatile u32 *)serial_control = 0x2000;
    *serial_control |= 0x4003;
    *interrupt_master = 1;

    dma->source = &zero;
    dma->destination = work;
    dma->control = 0x85000058;

    *(s32 *)(work + 0x14) = -1;
    *(void **)(work + 0x28) = work + 0x60;
    *(void **)(work + 0x2c) = work + 0x80;
    for (index = 0; index < 2; index++) {
        *(void **)(work + 0x30 + index * 4) =
            work + 0xa0 + index * 0x60;
        *(void **)(work + 0x40 + index * 4) =
            work + 0xc0 + index * 0x60;
        *(void **)(work + 0x50 + index * 4) =
            work + 0xe0 + index * 0x60;
    }

    *interrupt_master = 0;
    *interrupt_enable |= 0x80;
    *interrupt_master = 1;
    *(volatile u16 *)0x03001cb0 = 1;
    *(u8 *)0x020023a0 = 0;
    *(u32 *)0x02002080 = 0;
    *(u16 *)0x02002008 = 0;
    *(u32 *)0x020023ac = 0;
    *(u16 *)0x02002238 = 0;
    Func_0800651c();
    *interrupt_master = saved_master;
}
