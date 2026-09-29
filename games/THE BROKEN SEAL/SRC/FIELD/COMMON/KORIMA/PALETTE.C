/* Korima: the village's palette snapshots. The colour work buffer mirrors
 * palette RAM; each overlay keeps two saved copies of it in its variables.
 * The same four functions sit in each of the four Korima overlays. */
#include "DMA.H"

/* The colour work buffer, as the battle effects' colour buffers name it. */
extern u8 *Data_03001ed0;
extern u8 KorimaPalette_First[];
extern u8 KorimaPalette_Second[];
void Engine_ColorBufferApplySource(s32 value, s32 mode);
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);

void KorimaPalette_SaveFirst(void)
{
    Dma_Set(Data_03001ed0, KorimaPalette_First, 0x840000e0, (volatile u32 *)0x040000d4);
}

void KorimaPalette_SaveSecond(void)
{
    Dma_Set(Data_03001ed0, KorimaPalette_Second, 0x840000e0, (volatile u32 *)0x040000d4);
}

/* Copy both halves of palette RAM into the work buffer and take it as the
 * colour source. */
void KorimaPalette_Capture(void)
{
    u8 *buf = Data_03001ed0;

    Dma_Set((const void *)0x05000000, buf, 0x84000070, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x05000200, buf + 0x1c0, 0x84000070, (volatile u32 *)0x040000d4);
    Engine_ColorBufferApplySource(0x10000, 0);
}

/* Put a saved copy back into the work buffer, take it as the colour target
 * and capture palette RAM again as the source. */
void KorimaPalette_Restore(s32 second)
{
    u8 *buf = Data_03001ed0;

    if (second)
        Dma_Set(KorimaPalette_Second, buf, 0x840000e0, (volatile u32 *)0x040000d4);
    else
        Dma_Set(KorimaPalette_First, buf, 0x840000e0, (volatile u32 *)0x040000d4);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    KorimaPalette_Capture();
}
