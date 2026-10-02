/* Unlinked TLA near miss. Complete extent 56 bytes, literal pool included;
   14 differing byte positions in each of JA/EN/DE/ES/FR/IT.
   Native pop begins the DMA control prefix before the top-pointer load/update. Three ordinary local/pointer forms retain14 differing positions in English. Three tagged empty-boundary trials worsened the whole280 bank from26 to40/40/32 differing positions and were not retained. The reviewed Dma_Set body is unchanged; this canonical body uses ordinary C. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"
#include "DMA.H"

void Graphics_RestoreTransferWork(void)
{
    if (gTransformStackDepth > 0) {
        --gTransformStackDepth;
        gTransformStackTop -= 48;
        Dma_Set(gTransformStackTop, gTransform, 0x8400000c, (volatile u32 *)0x040000d4);
    }
}
