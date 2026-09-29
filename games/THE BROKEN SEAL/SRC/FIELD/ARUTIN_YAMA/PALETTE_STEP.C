#include "TYPES.H"
#include "DMA.H"

struct Half {
    u16 v;
};

/* The field palette buffer. */
extern u16 *Data_03001ed0;
/* Frames left before the next step of the animation. */
extern s16 ArutinYama_PaletteHold;
/* The position in the script below. */
extern s16 ArutinYama_PaletteStep;
/* Pairs of (first colour, frames to hold), ended by -1. */
extern s8 ArutinYama_PaletteScript[];

/* Steps a palette animation: when the hold runs out, reads the next pair of
 * the script (restarting it at -1) and copies nine colours from that point of
 * the palette buffer to background palette colours 3-11. */
void ArutinYama_StepPaletteAnim(void)
{
    u16 *palette = Data_03001ed0;

    if (ArutinYama_PaletteHold <= 0) {
        s32 frame;
        struct Half zero;

    next:
        frame = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
        if (frame == -1) {
            zero.v = 0;
            ArutinYama_PaletteStep = zero.v;
            goto next;
        }
        ArutinYama_PaletteHold = ArutinYama_PaletteScript[ArutinYama_PaletteStep++];
        palette += frame;
        Dma_Set(palette, (void *)0x05000006, 0x80000009, (volatile u32 *)0x040000d4);
    }
    ArutinYama_PaletteHold--;
}
