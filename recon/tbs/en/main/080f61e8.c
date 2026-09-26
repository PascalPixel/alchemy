/*
 * DRAFT main:080f61e8, canonical baseline best, 208-byte target. H2: resource-load
 * and bank-write DMA regions each retain their own inline C boundary; test
 * whether that separates the first pool from the final transfer's constants.
 * Hypothesis 1: explicit
 * widened RGB555 values and inline channel steps preserve the reference's
 * channel ancestry before any allocation tuning. The first mask load is
 * WORD ldr r7,[pc,#12], not a halfword load; reject halfword-mask reach.
 * Admission: first pool at +0x30 contains word 31, DMA3, 0x84000020 in that
 * order; first DMA loads at +0x14/+0x18 and mask load at +0x22 must address it.
 * Baseline: 204/208 bytes, 84 differing halfwords, topology equal, 62 wrong
 * instructions, aligned distance 35 edits. It already emits LSL16/LSR21/26.
 * Its machine dump has only SI fixups, all range (0,1020), with one final
 * pool; no 64-byte word-load reach exists. Loop dump hoists one shared mask
 * and strength-reduces buf indexing to a separate two-byte induction.
 * Residual H1: 224/208 bytes, 108 differing halfwords, topology equal,
 * 134 wrong instructions, aligned distance 76 edits. Runs 23: 0 register-only,
 * 0 order-only, 2 pool/immediate, 9 copy/rematerialise, 0 pool-placement,
 * 12 other. Admission fails: all constants remain in one final pool; SI
 * fixups still have range 1020. Explicit shifting plus inline channel steps
 * increases high-register saves without fixing the pool. No allocator tuning.
 * Residual H2: byte-identical to H1, 224/208 bytes and 108 differing halfwords;
 * topology equal, 134 wrong instructions, aligned distance 76 edits, same
 * 23 residual runs. DMA wrappers do not create a machine barrier or split
 * the pool. SI fixups still have range 1020, and the final barrier is within
 * all ranges, so arm_reorg retains one final pool. Both proposed source
 * families fail first-pool admission. Close this pool axis before register
 * tuning; no supported new structural hypothesis remains in this brief.
 * Canonical residual: 204/208 bytes; 84 differing halfwords; topology equal;
 * 62 wrong instructions, aligned distance 35 edits. Eight runs: 0 register-only,
 * 0 order-only, 0 pool/immediate, 2 copy/rematerialise, 0 pool-placement,
 * 6 other. First pool admission still fails; retain baseline as diagnostic
 * evidence only. Earlier forms are committed, not discarded. No adoption.
 * The .08.loop dump is the approved -dL evidence (generated through -da).
 * Stop: both briefed axes failed admission; no allocator tuning performed.
 */

#include "TYPES.H"
#include "DMA.H"

void *Resource_GetTableEntry(s32 id);
/* FAKEMATCH: linker mask keeps green/blue distinct from immediate red. */
extern u8 Value_0000001f[];

void Unnamed_080f61e8(s32 id)
{
    u16 *palette = (u16 *)0x05000000;
    u16 buf[64];
    s32 i;

    Dma_Set(Resource_GetTableEntry(id), buf, 0x84000020, (volatile u32 *)0x040000d4);
    for (i = 0; i != 64; i++) {
        s32 color = *palette;
        s32 r = color & 31;
        s32 g = ((u16)color >> 5) & (s32)Value_0000001f;
        s32 b = ((u16)color >> 10) & (s32)Value_0000001f;
        s32 goal = buf[i];
        s32 gr = goal & 31;
        s32 gg = ((u16)goal >> 5) & (s32)Value_0000001f;
        s32 gb = ((u16)goal >> 10) & (s32)Value_0000001f;

        if (r < gr)
            r++;
        else if (r > gr)
            r--;
        if (g < gg)
            g++;
        else if (g > gg)
            g--;
        if (b < gb)
            b++;
        else if (b > gb)
            b--;
        buf[i] = (b << 10) | (g << 5) | r;
        palette++;
    }
    Dma_Set(buf + 1, (void *)0x05000002, 0x8000003f, (volatile u32 *)0x040000d4);
}
