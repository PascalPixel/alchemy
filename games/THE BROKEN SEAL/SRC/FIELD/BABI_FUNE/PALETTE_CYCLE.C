#include "DMA.H"
#include "TYPES.H"

#define BabiFune_CyclePalette Func_020000b0

extern u8 Data_00000000[];
s32 Func_020013ce();

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

    frame = (u16 *)0x0200981c;
    v = Func_020013ce(*frame, 6);
    Dma_Set((const void *)(((u32)(v << 16) >> 15) + 0x020094ac), (void *)0x050000e8, 0x80000006, (volatile u32 *)0x040000d4);
    {
        s32 n = *frame + 1;

        *frame = n;
        if ((u32)(n << 16) > 0x230000) {
            /* FAKEMATCH: the zero is a HImode pool constant, placed first
             * in the pool; n is a fresh scope so the store precedes the test. */
            s32 z = (u16)(u32)Data_00000000;

            *frame = z;
        }
    }
}
