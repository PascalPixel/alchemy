#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "FIELD_EVENT.H"
#include "IO_REG.H"
#include "BATTLE_EFFECT_RUNTIME.H"


void KorimaPalette_SaveFirst(void)
{
    Dma_Set(Data_03001ed0->palette, KorimaPalette_First, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
}

void KorimaPalette_SaveSecond(void)
{
    Dma_Set(Data_03001ed0->palette, KorimaPalette_Second, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
}

void KorimaPalette_Capture(void)
{
    u16 *buf = Data_03001ed0->palette;

    Dma_Set((const void *)BG_PLTT, buf, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Dma_Set((const void *)OBJ_PLTT, buf + 224, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Engine_ColorBufferApplySource(0x10000, 0);
}

void KorimaPalette_Restore(s32 second)
{
    u16 *buf = Data_03001ed0->palette;

    if (second)
        Dma_Set(KorimaPalette_Second, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
    else
        Dma_Set(KorimaPalette_First, buf, DMA_ENABLE32 | DMA_32BIT | 0xe0, REG_DMA3);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    KorimaPalette_Capture();
}
