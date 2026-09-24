#include "DMA.H"

struct DisplayScrollState {
    u8 padding000[0xf00];
    u8 page;
};

extern struct DisplayScrollState *Data_03001ed8;


void DisplayScroll_ArmHBlankDma(void)
{
    volatile u32 *dma;
    struct DisplayScrollState *state;
    u32 *source;
    u16 status;
    u32 control;
    volatile u32 *destination;

    state = Data_03001ed8;
    source = (u32 *)((u8 *)state + state->page * 0x780);

    dma = (volatile u32 *)0x040000b0;
    destination = (volatile u32 *)0x04000014;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0xc5ff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    status = *(volatile u16 *)((u8 *)dma + 10);
    control = 0x7fff & status;
    *(volatile u16 *)((u8 *)dma + 10) = control;
    (void)*(volatile u16 *)((u8 *)dma + 10);

    *destination = *source++;
    *destination = *source++;
    *destination = *source++;

    control = 0xa6600003;
    Dma_Set(source, destination, control, dma);
}
