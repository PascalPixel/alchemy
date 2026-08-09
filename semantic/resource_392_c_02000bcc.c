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
 * 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_392 0bcc' to the veneer publishing
 * Func_0808a338 (the 394 source predates the corrected 'bl' rule and named it
 * Func_020020aa; resource_394's own site resolves to the same import).
 */

void Func_02001a62();
                        /* publish the completed work buffer */

/*
 * Capture the two hardware-palette banks into their non-contiguous work-buffer
 * slots, then publish the completed buffer.
 *
 */
void Func_02000bcc(void)
{
    u32 *destination_cell = (u32 *)0x03001ed0;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u32 destination = *destination_cell;
    u32 source = 0x05000000;
    u32 control = 0x84000070;

    dma3[0] = source;
    dma3[1] = destination;
    dma3[2] = control;

    source = 0x05000200;
    destination += 0x1c0;
    dma3[0] = source;
    dma3[1] = destination;
    dma3[2] = control;

    Func_02001a62(0x10000, 0);
}
