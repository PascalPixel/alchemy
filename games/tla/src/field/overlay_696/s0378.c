#include "ov696.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s16 Data_02006350[];

s32 Math_Sine(s32 angle);
u32 Random16Far(void);

static inline s32 LowestLineOffset(void)
{
    /* FAKEMATCH: the bound returned by an inline is built inside the loop as the listing builds it; as a constant loop.c hoists -64 into r8 (differ 21) */
    return -64;
}

// Fills BG0's scanline scroll table with a jittered sine wave and streams it
// to BG0HOFS by HBlank DMA 0.
void Func_02000378(void)
{
    s16 *line = Data_02006350;
    s32 i;
    s16 *source;
    volatile u16 *scroll;
    volatile u16 *dma;

    for (i = 0; i != 160; i++, line++) {
        s32 range = (160 - i) >> 2;

        *line = (Iwram_MulQ16(Math_Sine((i + *(u8 *)&gFrameCount) << 8), (144 - i) << 10) >> 11) - 4;
        *line += Random16Far() % (range * 2) - range;
        if (*line > 56)
            *line -= 56;
        else if (*line < LowestLineOffset())
            *line += 64;
    }
    line = Data_02006350;
    scroll = (volatile u16 *)0x04000010;
    *scroll = *line;
    dma = REG_DMA0;
    /* FAKEMATCH: the one-pass block reads the control before its mask is built, as in overlay 674's Func_02001cac (differ 4 without it) */
    do {
        dma[5] &= 0xc5ff; // stop the start timing and the repeat
        dma[5] &= 0x7fff; // stop the channel
    } while (0);
    source = line + 1;
    (void)dma[5];
    /* FAKEMATCH: line, reused for the destination, keeps the source step before the last control read; plain, combine folds the step into Dma_Set's r0 copy after the read (differ 2), and an asm barrier on the source also matches */
    line = (s16 *)scroll;
    Dma_Set(source, line, 0xa2600001, (volatile u32 *)dma);
}
