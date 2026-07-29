#include "types.h"

extern u8 Data_03001388[];

u16 *Func_08002f40(s32);
void Func_080072f0(void *, const void *, s32, void *);

void Func_080e46f0(s32 index)
{
    u16 colors[64];
    u16 *palette = (u16 *)0x05000000;
    u16 *palette_cursor;
    s32 i;

    Func_080072f0(colors, Func_08002f40(index), sizeof(colors),
                  Data_03001388);
    colors[0] = 0;

    palette_cursor = palette;
    i = 0;
    do {
        u32 current = *palette_cursor;
        u32 target = colors[i];
        s32 red = current & 0x1f;
        s32 green = (current >> 5) & 0x1f;
        s32 blue = (current >> 10) & 0x1f;
        s32 target_red = target & 0x1f;
        s32 target_green = (target >> 5) & 0x1f;
        s32 target_blue = (target >> 10) & 0x1f;

        if (red < target_red)
            red++;
        else if (red > target_red)
            red--;
        if (green < target_green)
            green++;
        else if (green > target_green)
            green--;
        if (blue < target_blue)
            blue++;
        else if (blue > target_blue)
            blue--;

        colors[i] = red | (green << 5) | (blue << 10);
        i++;
        palette_cursor++;
    } while (i != 64);

    Func_080072f0(palette, colors, sizeof(colors), Data_03001388);
}
