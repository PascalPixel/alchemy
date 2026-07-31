typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_393 owner at 0x02000ddc, 64 bytes: restore one of the two banked
 * palette snapshots into the live work buffer and publish it.
 *
 * The exact inverse of the 0x02000d5c / 0x02000d7c leaf pair, which bank the
 * work buffer OUT to 0x020090e0 and 0x020097e0 (file offsets 0x10e0 and 0x17e0
 * under the proven 0x02008000 link base).  Here those two addresses are the
 * DMA source and the work buffer is the destination; the selector picks which
 * bank comes back.  The overlay image is writable EWRAM, which is what makes a
 * round trip through it possible at all.
 *
 * Complete owner: 'push {lr}' and the single interworking epilogue at
 * 0x02000e04.  Control-flow walk: a two-arm diamond that rejoins at 0x02000df0
 * and runs straight to the 'bx r0', so the 20-byte five-word pool at
 * 0x02000e08 is code-unreachable.  44 code + 20 pool = 64, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so the owner is void.
 *
 * Calls: 2 sites, from 'bun tools/overlay_call_targets.ts resource_393 0ddc'.
 * Both arms of the diamond differ ONLY in which pool word they load - they do
 * not each carry a copy of the transfer or of the two calls, and writing them
 * that way would inflate the per-target multiset by two.
 */

struct DmaTransfer {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_0808a330();               /* publish the completed work buffer */
void Func_02000d9c();               /* snapshot both palette banks */

void Func_02000ddc(s32 selector)
{
    volatile struct DmaTransfer *const dma3 =
        (volatile struct DmaTransfer *)0x040000d4;
    u32 workBuffer = *(volatile u32 *)0x03001ed0;

    dma3->source = selector != 0 ? 0x020097e0 : 0x020090e0;
    dma3->destination = workBuffer;
    dma3->control = 0x840000e0;

    Func_0808a330(0x10000, 0);
    Func_02000d9c();
}
