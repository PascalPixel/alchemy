#include "TYPES.H"

extern u8 Makyuri_ZeroWord[];
extern s32 gFrameCount;
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
s32 Engine_MathModulo(s32 dividend, s32 divisor);
s32 Engine_MathDivide(s32 dividend, s32 divisor);

/* Mercury Lighthouse: every fifth frame, advance the red phase and shift the
 * red channel of palette colours 105..110 up one slot, dimming the first
 * three by 40%, over a shared green and blue. */
void Makyuri_CyclePalette(void)
{
    volatile u16 cur;
    u32 i;
    s32 v;
    s32 bits;
    u16 *dst;

    {
        /* FAKEMATCH: the zero comes from a HImode pool constant; its short
         * pool range dumps the literal pool before the loop. */
        s32 z = (u16)(u32)Makyuri_ZeroWord;

        cur = z;
    }
    if (Engine_MathModulo(gFrameCount, 5) != 0)
        return;
    gPaletteCycleRed = (gPaletteCycleRed + 4) & 31;
    for (i = 0; i < 6; i++) {
        cur = ((u16 *)0x05000000)[110 - i] & 31;
        v = cur;
        if (i <= 2)
            v -= Engine_MathDivide(v * 4, 10);
        dst = &((u16 *)0x05000000)[111 - i];
        bits = (gPaletteCycleBlue << 10) | (gPaletteCycleGreen << 5);
        *dst = v | bits;
    }
    *(u16 *)0x050000d2 = gPaletteCycleRed | bits;
}

/* Clear palette colours 105..111. */
void Makyuri_ClearPalette(void)
{
    u32 i;
    u16 *p;
    s32 z;

    /* FAKEMATCH: the zero comes from a HImode pool constant, which dumps
     * the literal pool before the loop. */
    z = (u16)(u32)Makyuri_ZeroWord;
    i = 0;
    p = (u16 *)0x050000de;
    do {
        i++;
        *p = z;
        p--;
    } while (i <= 6);
}
