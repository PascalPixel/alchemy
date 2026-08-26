#include "types.h"

struct DisplayScrollState {
    u8 padding000[0xf00];
    u8 page;
};

extern struct DisplayScrollState *Data_03001ed8;

#define DisplayScroll_ArmHBlankDma Func_080944ec

void DisplayScroll_ArmHBlankDma(void)
{
    volatile u32 *dma;
    struct DisplayScrollState *state;
    u32 *source;
    u32 control;
    u16 status;
    volatile u32 *destination;

    state = Data_03001ed8;
    source = (u32 *)((u8 *)state + state->page * 0x780);

    dma = (volatile u32 *)0x040000b0;
    control = 0xc5ff;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control &= status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    control = 0x7fff;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control &= status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    (void)*(volatile u16 *)((u8 *)dma + 10);

    destination = (volatile u32 *)0x04000014;
    *destination = *source++;
    *destination = *source++;
    *destination = *source++;

    dma[0] = (u32)source;
    dma[1] = (u32)destination;
    control = 0xa6600003;
    dma[2] = control;
}
