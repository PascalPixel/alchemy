#include "IWRAM_CALL.H"

/* Scales the three tint channels kept at work + 0x576 by Q16 factors and
   packs them, clamped to 0-31, into a BGR555 colour. */
u16 Palette_ScaleTintChannels(u8 *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
    s32 r;
    s32 g;
    s32 b;

    r = Iwram_MulQ16(*(u16 *)(work + 0x576) << 16, scale_r) >> 16;
    g = Iwram_MulQ16(*(u16 *)(work + 0x578) << 16, scale_g) >> 16;
    b = Iwram_MulQ16(*(u16 *)(work + 0x57a) << 16, scale_b) >> 16;
    if (r < 0)
        r = 0;
    if (g < 0)
        g = 0;
    if (b < 0)
        b = 0;
    if (r > 31)
        r = 31;
    if (g > 31)
        g = 31;
    if (b > 31)
        b = 31;
    return r + ((b << 10) + (g << 5));
}
