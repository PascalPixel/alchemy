#include "DMA.H"
struct UiWindowWork { u8 data[0xa70]; };
void UiWindow_InitializeWork(void)
{
    struct UiWindowWork *work = *(struct UiWindowWork **)0x03001f2c;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    work->data[28] = 255;
    work->data[30] = 1;
    work->data[31] = 1;
    work->data[274] = 1;
    work->data[275] = 1;
}
