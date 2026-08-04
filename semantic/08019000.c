#include "types.h"

struct TileWindow_08019000 {
    u8 padding_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

extern u16 *Data_03001e8c;

void Func_08019000(
    struct TileWindow_08019000 *window,
    s32 value,
    u32 offsetX,
    u32 offsetY,
    u32 mode)
{
    u16 *tiles;
    u32 prefix;
    u32 cell;
    u32 x;
    u32 y;

    y = offsetY;
    y++;
    tiles = Data_03001e8c;
    x = offsetX;
    x++;
    if (y > window->height - 1 || x > window->width - 1)
        return;

    prefix = 0xf000;
    switch (mode) {
    case 2:
        prefix = 0xe000;
        break;
    case 3:
        break;
    case 4:
        prefix = 0x1000;
        break;
    default:
        prefix = 0;
        break;
    }

    if (mode == 1)
        return;

    if (mode >= 1) {
        if (mode <= 4) {
            cell = (window->y + y) * 32 + window->x + x;
            if (cell < 640)
                tiles[cell] = prefix | value;
            return;
        }
    }

    cell = (window->y + y) * 32 + window->x + x;
    if (cell < 640)
        tiles[cell] = value;
}
