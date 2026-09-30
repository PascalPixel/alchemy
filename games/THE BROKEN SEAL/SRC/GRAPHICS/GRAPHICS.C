#include "TYPES.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"
#include "DMA.H"

/* Allocates the transform stack's 48-byte slots, empties it and loads the
   identity into the current transform. */
void Render_ResetTransformState(void)
{
    gTransformStackTop = Runtime_AllocateBlock(2, sizeof(gTransform));
    gTransformStackDepth = 0;
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}

void Graphics_SaveTransferWorkOnce(void)
{
    if (gTransformStackDepth <= 0) {
        Dma_Set(gTransform, gTransformStackTop, 0x8400000c, (volatile u32 *)0x040000d4);
        gTransformStackDepth++;
        gTransformStackTop += 48;
    }
}

void Graphics_SaveTransferWork(void *destination)
{
    Dma_Set(gTransform, destination, 0x8400000c, (volatile u32 *)0x040000d4);
}

void Graphics_LoadTransferWork(const void *source)
{
    Dma_Set(source, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
}

void Graphics_RestoreTransferWork(void)
{
    if (gTransformStackDepth > 0) {
        --gTransformStackDepth;
        gTransformStackTop -= 48;
        Dma_Set(gTransformStackTop, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
    }
}
