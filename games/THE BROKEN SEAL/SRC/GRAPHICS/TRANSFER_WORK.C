#include "DMA.H"
#include "TRANSFORM.H"
void Graphics_SaveTransferWorkOnce(void)
{
    if (gTransformStackDepth <= 0) {
        Dma_Set(gTransform, gTransformStackTop, 0x8400000c, (volatile u32 *)0x040000d4);
        gTransformStackDepth++;
        gTransformStackTop += 48;
    }
}

#include "DMA.H"
void Graphics_SaveTransferWork(void *destination)
{
    Dma_Set(gTransform, destination, 0x8400000c, (volatile u32 *)0x040000d4);
}

#include "DMA.H"
void Graphics_LoadTransferWork(const void *source)
{
    Dma_Set(source, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
}

#include "DMA.H"
void Graphics_RestoreTransferWork(void)
{
    if (gTransformStackDepth > 0) {
        --gTransformStackDepth;
        gTransformStackTop -= 48;
        Dma_Set(gTransformStackTop, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
    }
}
