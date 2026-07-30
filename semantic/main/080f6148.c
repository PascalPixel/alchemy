typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 Value_0000001f;

void Func_080f6148(void)
{
    s32 red;
    s32 green;
    s32 blue;
    s32 count;
    s32 mask;
    u16 *palette;
    u16 color;
    u32 shifted;

    palette = (u16 *)0x05000140;
    mask = (s32)&Value_0000001f;
    count = 0;
    do {
        color = *palette;
        blue = color & 31;
        shifted = color << 16;
        red = shifted >> 26;
        red &= mask;
        green = shifted >> 21;
        green &= mask;
        red--;
        green--;
        blue--;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        count++;
        *palette = (red << 10) | (green << 5) | blue;
        palette++;
    } while (count != 16);

    palette = (u16 *)0x05000202;
    mask = (s32)&Value_0000001f;
    count = 0;
    do {
        color = *palette;
        blue = color & 31;
        shifted = color << 16;
        red = shifted >> 26;
        red &= mask;
        green = shifted >> 21;
        green &= mask;
        red--;
        green--;
        blue--;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        count++;
        *palette = (red << 10) | (green << 5) | blue;
        palette++;
    } while (count != 239);
}
