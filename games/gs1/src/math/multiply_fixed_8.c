#include "types.h"
#include "scene.h"

/* math/multiply_fixed_8.c */
/* math/multiply_fixed_8_8.c */
s32 FixedPoint_Multiply8_8(s32 arg0, s32 arg1)
{
    s16 left = arg0;
    s16 right = arg1;
    s32 product;
    s32 adjusted;
    s32 multiplier;

    multiplier = right;
    product = left;
    product *= multiplier;
    if (product >= 0) {
        adjusted = product;
    } else {
        adjusted = product + 255;
    }
    return (s16)(adjusted >> 8);
}

/* math/scale_by_ratio_fixed.c */
s32 FixedPoint_Ratio(s32, s32);

s16 FixedPoint_ScaleByRatio(s16 arg0, s16 arg1)
{
    return FixedPoint_Ratio(arg0 << 8, arg1);
}

/* math/reciprocal_fixed.c */
s32 FixedPoint_Ratio(s32, s16);

s16 FixedPoint_Reciprocal(s16 arg0)
{
    return FixedPoint_Ratio(0x10000, arg0);
}

/* graphics/color/scale_rgb555_buffer.c */
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale, s32 count)
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
