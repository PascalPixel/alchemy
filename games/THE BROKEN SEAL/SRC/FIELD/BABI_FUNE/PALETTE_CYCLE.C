#include "DMA.H"
#include "TYPES.H"

s32 IwramUnsignedDivide();
extern u16 BabiFune_PaletteStep;
extern const u16 BabiFune_PaletteFrames[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* Babi Fune: copy six colours of the cycling palette for the current step
 * into palette colours 116..121 and advance the step, wrapping after 35. */
void BabiFune_CyclePalette(void)
{
    u16 *frame;
    s32 v;

    frame = &BabiFune_PaletteStep;
    v = IwramUnsignedDivide(*frame, 6);
    Dma_Set((const void *)(((u32)(v << 16) >> 15) + (u32)BabiFune_PaletteFrames), (void *)0x050000e8, 0x80000006, (volatile u32 *)0x040000d4);
    {
        /* FAKEMATCH: n is a fresh scope so the store precedes the test. */
        s32 n = *frame + 1;

        *frame = n;
        if ((u32)(n << 16) > 0x230000)
            *frame = 0;
    }
}
