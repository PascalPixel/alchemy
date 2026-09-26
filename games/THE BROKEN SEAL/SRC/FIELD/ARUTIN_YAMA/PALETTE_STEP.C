#include "TYPES.H"
#include "DMA.H"

struct Half {
    u16 v;
};

/* The field palette buffer. */
extern u16 *Data_03001ed0;
/* Frames left before the next step of the animation. */
extern s16 Data_0200d25c;
/* The position in the script below. */
extern s16 Data_0200d260;
/* Pairs of (first colour, frames to hold), ended by -1. */
extern s8 Data_0200bd28[];

/* Steps a palette animation: when the hold runs out, reads the next pair of
 * the script (restarting it at -1) and copies nine colours from that point of
 * the palette buffer to background palette colours 3-11. */
void ArutinYama_StepPaletteAnim(void)
{
    u16 *palette = Data_03001ed0;

    if (Data_0200d25c <= 0) {
        s32 frame;
        struct Half zero;

    next:
        frame = Data_0200bd28[Data_0200d260++];
        if (frame == -1) {
            zero.v = 0;
            Data_0200d260 = zero.v;
            goto next;
        }
        Data_0200d25c = Data_0200bd28[Data_0200d260++];
        palette += frame;
        Dma_Set(palette, (void *)0x05000006, 0x80000009, (volatile u32 *)0x040000d4);
    }
    Data_0200d25c--;
}
