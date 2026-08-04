#include "types.h"

struct Window_080164d4 {
    u8 padding000[12];
    u16 tileX;
    u16 tileY;
};

extern u8 *Data_03001e8c;
void Func_0801e260(s32 x, s32 y, u32 width, u32 height);

void Func_080164d4(
    struct Window_080164d4 *window,
    u32 leftPixel,
    u32 topPixel,
    u32 rightPixel,
    u32 bottomPixel)
{
    u32 left = (leftPixel >> 3) + window->tileX;
    u32 top = (topPixel >> 3) + window->tileY;
    u32 right = ((rightPixel + 7) >> 3) + window->tileX;
    u32 bottom = ((bottomPixel + 7) >> 3) + window->tileY;
    u32 width = right - left;
    u32 height = bottom - top;
    u16 *row;
    u32 y;

    left++;
    top++;
    Func_0801e260(left, top, width, height);

    row = (u16 *)Data_03001e8c + top * 32 + left;
    for (y = 0; y < height; y++) {
        u32 x;

        for (x = 0; x < width; x++) {
            row[x] = 0xf020;
        }
        row += 32;
    }
    Data_03001e8c[0xea3] = 1;
}
