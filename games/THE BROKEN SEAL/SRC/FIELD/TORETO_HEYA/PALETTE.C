#include "DMA.H"
extern struct BattleEffectBuffers *Data_03001ed0;

/*
 * Toreto house: upload the room's palette bank from the source pointed at by
 * the loader's palette pointer. The DMA runs from the VRAM pattern bank into
 * the palette RAM and waits on the DMA status register.
 */

void ToretoPalette_CaptureBank(void)
{
    Dma_Set((const void *)0x05000000, *(void **)&Data_03001ed0, 0x84000070,
            (volatile u32 *)0x040000d4);
}
