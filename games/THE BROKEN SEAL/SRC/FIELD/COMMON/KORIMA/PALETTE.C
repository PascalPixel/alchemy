#include "GLOBAL.H"
#include "DMA.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"

extern u8 *Data_03001ed0;

void KorimaPalette_SaveFirst(void)
{
    Dma_Set(Data_03001ed0, KorimaPalette_First, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
}

void KorimaPalette_SaveSecond(void)
{
    Dma_Set(Data_03001ed0, KorimaPalette_Second, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
}

void KorimaPalette_Capture(void)
{
    u8 *buf = Data_03001ed0;

    Dma_Set((const void *)0x05000000, buf, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Dma_Set((const void *)0x05000200, buf + 0x1c0, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Engine_ColorBufferApplySource(0x10000, 0);
}

void KorimaPalette_Restore(s32 second)
{
    u8 *buf = Data_03001ed0;

    if (second)
        Dma_Set(KorimaPalette_Second, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
    else
        Dma_Set(KorimaPalette_First, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    KorimaPalette_Capture();
}
