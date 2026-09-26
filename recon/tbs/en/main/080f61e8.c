/*
 * DRAFT main:080f61e8, 208 bytes including pools. Hypothesis 2: resource-load
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
 * Stop after three hypotheses or 30 minutes.
 */

#include "TYPES.H"
#include "DMA.H"

void *Resource_GetTableEntry(s32 id);
extern u8 Value_0000001f[];

static __inline__ void Palette_LoadResource(s32 id, u16 *buf)
{
    const void *source = Resource_GetTableEntry(id);
    Dma_Set(source, buf, 0x84000020, (volatile u32 *)0x040000d4);
}

static __inline__ void Palette_WriteBank(const u16 *buf)
{
    Dma_Set(buf + 1, (void *)0x05000002, 0x8000003f, (volatile u32 *)0x040000d4);
}

static __inline__ s32 Palette_StepChannel(s32 color, s32 goal)
{
    if (color < goal)
        color++;
    else if (color > goal)
        color--;
    return color;
}

void Unnamed_080f61e8(s32 id)
{
    u16 *palette = (u16 *)0x05000000;
    u16 buf[64];
    s32 i;
    s32 mask;

    Palette_LoadResource(id, buf);
    /* FAKEMATCH: linker mask keeps green/blue distinct from immediate red. */
    mask = (s32)Value_0000001f;
    for (i = 0; i != 64; i++) {
        u32 color = *palette;
        s32 r = color & 31;
        u32 shifted = color << 16;
        s32 g = (shifted >> 21) & mask;
        s32 b = (shifted >> 26) & mask;
        u32 goal = buf[i];
        s32 gr = goal & 31;
        u32 goal_shifted = goal << 16;
        s32 gg = (goal_shifted >> 21) & mask;
        s32 gb = (goal_shifted >> 26) & mask;

        r = Palette_StepChannel(r, gr);
        g = Palette_StepChannel(g, gg);
        b = Palette_StepChannel(b, gb);
        buf[i] = (b << 10) | (g << 5) | r;
        palette++;
    }
    Palette_WriteBank(buf);
}
