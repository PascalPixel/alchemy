#include "GLOBAL.H"
#include "TYPES.H"

extern s32 gFrameCount;

void Makyuri_CyclePalette(void)
{
    volatile u16 cur;
    u32 i;
    s32 v;
    s32 bits;
    u16 *dst;

    cur = 0;
    if ((u32)gFrameCount % 5 != 0)
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

void Makyuri_ClearPalette(void)
{
    u32 i;

    for (i = 0; i < 7; i++)
        ((u16 *)0x05000000)[111 - i] = 0;
}
