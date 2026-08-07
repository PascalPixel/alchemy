#include "types.h"

/*
 * resource_392 owner at 0x02000bcc, 64 bytes: capture the two hardware-palette
 * banks into their non-contiguous work-buffer slots, then publish the buffer.
 *
 * TRANSPOSED from semantic/overlays/resource_394_c_02000f74.c.  The two owners
 * are the same routine shared verbatim: over all 32 halfwords they differ in
 * exactly 2 places, both halves of the single BL pair.  No pool word differs -
 * in particular the work-buffer pointer 0x03001ed0 and the DMA control words are
 * byte-identical.
 *
 * What was changed: the entry symbol, and the one call, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_392 0bcc' to the veneer publishing
 * Func_0808a338 (the 394 source predates the corrected 'bl' rule and named it
 * Func_020020aa; resource_394's own site resolves to the same import).
 */

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_0808a338();   /* publish the completed work buffer */

/*
 * Capture the two hardware-palette banks into their non-contiguous work-buffer
 * slots, then publish the completed buffer.
 *
 */
void Func_02000bcc(void)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;
    u32 destination = *(volatile u32 *)0x03001ed0;

    dma3->source = 0x05000000;
    dma3->destination = destination;
    dma3->control = 0x84000070;

    dma3->source = 0x05000200;
    dma3->destination = destination + 0x1c0;
    dma3->control = 0x84000070;

    Func_0808a338(0x10000, 0);
}
