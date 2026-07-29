typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_080f4100(u16 *source, u16 *destination, u32 scale, s32 count)
{
    u32 maskRed;
    u32 maskGreen;
    u32 maskBlue;
    u32 colour;
    u32 red;
    u32 green;
    u32 blue;
    s32 remaining;

    if (count > 0) {
        maskRed = 0x1F;
        maskGreen = 0x3E0;
        maskBlue = 0x7C00;
        remaining = count;
        do {
            colour = *source;
            red = (colour & maskRed) * scale;
            green = (colour & maskGreen) * scale;
            blue = (colour & maskBlue) * scale;
            remaining--;
            *destination = ((red >> 16) & maskRed) | ((green >> 16) & maskGreen)
                | ((blue >> 16) & maskBlue);
            source++;
            destination++;
        } while (remaining != 0);
    }
    return 0;
}
