#include "DMA.H"

/*
 * Toreto house: upload the room's palette bank from the source pointed at by
 * the loader's palette pointer. The DMA runs from the VRAM pattern bank into
 * the palette RAM and waits on the DMA status register.
 */

#define ToretoPalette_CaptureBank Func_02001224

void ToretoPalette_CaptureBank(void)
{
    Dma_Set((const void *)0x05000000, *(void **)0x03001ed0, 0x84000070,
            (volatile u32 *)0x040000d4);
}
