#include "DMA.H"

extern u8 *Palette_WorkBuffer;
extern u8 KorimaPalette_First[];
extern u8 KorimaPalette_Second[];
void Engine_ApplyColorToSourceBuffer(s32 flags, s32 color);
void Engine_ApplyColorToTargetBuffer(s32 flags, s32 color);

void KorimaPalette_SaveFirst(void)
{
    Dma_Set(Palette_WorkBuffer, KorimaPalette_First, 0x840000e0, (volatile u32 *)0x040000d4);
}

void KorimaPalette_SaveSecond(void)
{
    Dma_Set(Palette_WorkBuffer, KorimaPalette_Second, 0x840000e0, (volatile u32 *)0x040000d4);
}

void KorimaPalette_Capture(void)
{
    u8 *buf = Palette_WorkBuffer;
    Dma_Set((const void *)0x05000000, buf, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x05000200, buf + 0x1c0, 0x84000070, (volatile u32 *)0x040000d4);
    Engine_ApplyColorToSourceBuffer(0x10000, 0);
}

void KorimaPalette_Restore(s32 second)
{
    u8 *buf = Palette_WorkBuffer;
    if (second)
        Dma_Set(KorimaPalette_Second, buf, 0x840000e0, (volatile u32 *)0x040000d4);
    else
        Dma_Set(KorimaPalette_First, buf, 0x840000e0, (volatile u32 *)0x040000d4);
    Engine_ApplyColorToTargetBuffer(0x10000, 0);
    KorimaPalette_Capture();
}
