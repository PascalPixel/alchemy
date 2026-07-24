typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080b98b4(s32 arg0)
{
    s32 iter;
    s32 bank;
    s32 col;
    s32 mask;
    s32 idx;

    bank = 15;
    iter = 0;
    mask = 31;
    do
    {
        for (col = 0; col <= 15; col++)
        {
            u32 color;
            s32 blue;
            s32 green;
            s32 red;
            s32 value;

            idx = bank * 16 + col;
            color = ((u16 *)0x05000000)[idx];
            blue = (color >> 10) & mask;
            green = (color >> 5) & mask;
            red = color & mask;
            blue += arg0;
            green += arg0;
            red += arg0;
            if (blue > 31)
                blue = 31;
            if (green > 31)
                green = 31;
            if (red > 31)
                red = 31;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            value = blue << 10;
            value |= green << 5;
            value |= red;
            ((s16 *)0x04FFFFE0)[idx] = (s16)value;
        }
        iter++;
        bank = 5;
    } while (iter <= 1);
}
