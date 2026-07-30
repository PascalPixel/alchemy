#include "types.h"

s32 Func_080f6038(u16 *source, u16 *destination, s32 scale, s32 count)
{
    s32 remaining;
    u32 maskRed;
    u32 maskGreen;
    u32 maskBlue;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (count > 0) {
        maskRed = 0x1f;
        maskGreen = 0x3e0;
        maskBlue = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & maskRed;
            green = pixel & maskGreen;
            blue = maskBlue & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & maskRed) | ((green >> 16) & maskGreen);
            pixel |= (blue >> 16) & maskBlue;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}
