#include "types.h"

struct State_080944ec {
    u8 padding000[0xf00];
    u8 page;
};

extern struct State_080944ec *Data_03001ed8;

void Func_080944ec(void)
{
    volatile u32 *dma;
    struct State_080944ec *state;
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
