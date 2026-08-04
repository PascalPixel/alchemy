#include "types.h"

s32 Func_080022fc(s32, s32);
s16 Func_0801cc50(const s16 *, s32, s32, s32);

/*
 * Derive three clamped five-bit color channels from the two configuration
 * values, then generate the seven-color UI palette ramp.
 */
void Func_0801ccc0(s32 color, s32 brightness)
{
    const u8 *curve = (const u8 *)0x08036750;
    volatile u16 *palette = (volatile u16 *)0x050001E8;
    s16 channels[3];
    s16 base;
    s16 adjustment;
    s16 red;
    s16 green;
    s16 blue;

    base = (s16)(Func_080022fc(color + 0xC, 0x18) * 4);
    adjustment = (s16)(brightness - 7);

    red = (s16)(curve[(s16)Func_080022fc(base, 0x60)] + adjustment);
    green = (s16)(
        curve[Func_080022fc(base + 0x20, 0x60)] + adjustment);
    blue = (s16)(
        curve[Func_080022fc(base + 0x40, 0x60)] + adjustment);

    if (red < 0) {
        red = 0;
    }
    if (red > 0x1F) {
        red = 0x1F;
    }
    if (green < 0) {
        green = 0;
    }
    if (green > 0x1F) {
        green = 0x1F;
    }
    if (blue < 0) {
        blue = 0;
    }
    if (blue > 0x1F) {
        blue = 0x1F;
    }

    channels[0] = red;
    channels[1] = green;
    channels[2] = blue;

    palette[0] = Func_0801cc50(channels, 0xEEEE, 0xCCCC, 0x11110);
    palette[1] = Func_0801cc50(channels, 0xD555, 0xBBBB, 0xEEEE);
    palette[2] = Func_0801cc50(channels, 0xBBBB, 0xAAAA, 0xCCCC);
    palette[3] = Func_0801cc50(channels, 0xA221, 0x9999, 0xAAAA);
    palette[4] = Func_0801cc50(channels, 0x10888, 0xDDDD, 0x13333);
    palette[5] = Func_0801cc50(channels, 0x12221, 0xEEEE, 0x15555);
    palette[6] = Func_0801cc50(channels, 0x13BBB, 0x10000, 0x17777);
}
