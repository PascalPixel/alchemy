#include "TYPES.H"

/* Scales each 5-bit component by its own Q16 factor and packs BGR555. */
u16 Color_ScaleComponents(const s16 *rgb, s32 red, s32 green, s32 blue);

extern const u8 PaletteGlow_WaveTable[];

#define GLOW_PALETTE ((u16 *)0x050001e8)

/* Cycles a base color around the wave table and writes seven shades of it
   to OBJ palette 15, entries 4 to 10. */
void PaletteGlow_Update(s32 phase, s32 brightness)
{
    s16 rgb[3];
    s16 step;
    s16 red;
    s16 green;
    s16 blue;
    s16 offset;

    step = ((phase + 12) % 24) * 4;
    offset = brightness - 7;
    red = PaletteGlow_WaveTable[(s16)(step % 96)] + offset;
    green = PaletteGlow_WaveTable[(step + 32) % 96] + offset;
    blue = PaletteGlow_WaveTable[(step + 64) % 96] + offset;
    if (red < 0)
        red = 0;
    if (red > 31)
        red = 31;
    if (green < 0)
        green = 0;
    if (green > 31)
        green = 31;
    if (blue < 0)
        blue = 0;
    if (blue > 31)
        blue = 31;
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
    GLOW_PALETTE[0] = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
    GLOW_PALETTE[1] = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
    GLOW_PALETTE[2] = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
    GLOW_PALETTE[3] = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
    GLOW_PALETTE[4] = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
    GLOW_PALETTE[5] = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
    GLOW_PALETTE[6] = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}
