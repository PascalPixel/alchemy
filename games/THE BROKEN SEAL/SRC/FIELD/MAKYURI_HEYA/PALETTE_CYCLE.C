#include "PROBE.H"

/* The phase of the lighthouse glow's red channel and the green and blue it
 * glows over. */
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;

/* Every fifth frame, advance the red phase and shift the red channel of
 * palette colours 105..110 up one slot, dimming the first three by 40%, over
 * the shared green and blue. */
void Makyuri_CyclePalette(void)
{
    volatile u16 cur;
    u32 i;
    s32 v;
    s32 bits;
    u16 *dst;

    cur = 0;
    if (gFrameCount % 5 != 0)
        return;
    gPaletteCycleRed = (gPaletteCycleRed + 4) & 31;
    for (i = 0; i < 6; i++) {
        cur = ((u16 *)0x05000000)[110 - i] & 31;
        v = cur;
        if (i <= 2)
            v -= v * 4 / 10;
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

    for (i = 0; i <= 6; i++)
        ((u16 *)0x05000000)[111 - i] = 0;
}
