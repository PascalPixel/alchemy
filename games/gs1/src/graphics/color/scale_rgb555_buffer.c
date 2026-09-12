#include "types.h"

s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count)
{
    s32 remaining;
    u32 mask_red;
    u32 mask_green;
    u32 mask_blue;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (count > 0) {
        mask_red = 0x1f;
        mask_green = 0x3e0;
        mask_blue = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & mask_red;
            green = pixel & mask_green;
            blue = mask_blue & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
            pixel |= (blue >> 16) & mask_blue;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}
