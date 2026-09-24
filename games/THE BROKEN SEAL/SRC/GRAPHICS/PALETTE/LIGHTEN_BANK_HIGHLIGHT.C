#include "DMA.H"

#define Palette_LightenBankHighlight Func_080a2144

/* Copies palette bank 15 over the given object bank, then lightens its
   colour 4 by 9 in each channel, saturating at 31. */
void Palette_LightenBankHighlight(s32 bank)
{
    u16 *palette = (u16 *)(0x05000000 + (bank << 5));
    u32 color;
    u32 red;
    u32 green;
    u32 blue;

    Dma_Set((const void *)0x050001e0, palette, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e0, palette, 0x84000008, (volatile u32 *)0x040000d4);
    color = palette[4];
    /* FAKEMATCH: the (u16) casts on an already 16-bit colour produce the
       reference's shift pair. */
    blue = (u16)color >> 10;
    green = ((u16)color >> 5) & 31;
    red = color & 31;
    blue += 9;
    if (blue > 31)
        blue = 31;
    green += 9;
    if (green > 31)
        green = 31;
    red += 9;
    if (red > 31)
        red = 31;
    palette[4] = (blue << 10) | (green << 5) | red;
}
