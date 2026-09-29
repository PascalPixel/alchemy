#include "TYPES.H"

extern s32 gFrameCount;
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;
s32 Engine_MathModulo(s32 dividend, s32 divisor);
s32 Engine_MathDivide(s32 dividend, s32 divisor);

/* Mercury Lighthouse: every fifth frame, advance the red phase and shift the
 * red channel of palette colours 105..110 up one slot, dimming the first
 * three by 40%, over a shared green and blue. The same functions sit in both
 * lighthouse overlays that cycle the light. */
void Makyuri_CyclePalette(void)
{
    volatile u16 cur;
    u32 i;
    s32 v;
    s32 bits;
    u16 *dst;

    cur = 0;
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

    for (i = 0; i < 7; i++)
        ((u16 *)0x05000000)[111 - i] = 0;
}
